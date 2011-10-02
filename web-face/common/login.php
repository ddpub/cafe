<?
require("lang.php");

session_start();
if ($_SERVER["HTTPS"] != "on" && $_useHTTPS) {
	header("Location: https://" . $_SERVER["SERVER_NAME"] . $_SERVER["REQUEST_URI"]);
}

if ((int)isset($_GET["logout"])) {
	logout();
	writeAuthProblem(1);
}


if (strlen($_POST["_user_login"]) > 0) {
	$_login = $_POST["_user_login"];
	$_passwd = $_POST["_user_password"];
}
else {
	 $_login = $_SESSION["login"];
	 $_passwd = $_SESSION["passwd"];
}



if ($objConn = @ocinlogon($_login, $_passwd, "(DESCRIPTION =
 (ADDRESS_LIST =
 (ADDRESS = (PROTOCOL = TCP)(HOST = host)(PORT = 1521))
 )
 (CONNECT_DATA =
 (SERVICE_NAME = xe)
 )
 )")) {

	$_SESSION["login"] = $_login;
	$_SESSION["passwd"] = $_passwd;

	$strSQL = "SELECT * FROM ksi.person WHERE LOWER(login) = LOWER(user) AND fire IS NULL";

	$stmt = ociparse($objConn, $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	if (!ocifetch($stmt)) {
		session_destroy();
		setcookie("logged", 0, 0, "/");
		die("critial error: logged user not found in db.");
	}
	$GLOBALS["_person_id"] = ociresult($stmt, "PERSON_ID");
	$GLOBALS["_person_name"] = ociresult($stmt, "FIRSTNAME") . " " . ociresult($stmt, "LASTNAME");
	ocifreestatement($stmt);

	fillSystemVars();


	setcookie("logged", 1, 0, "/");
	$justLogged = true;


	if ($_login && $_COOKIE["login"] != $_login) {
		setcookie("login", $_login, time() + 31536000, "/"); // set cookie for one year
	}
}
else {
	logout();
	if ($_login || $_passwd) {
		$errMsg = getLM("wrong login / password");
	}
}


if ((int)$_COOKIE["logged"] && !(int)$GLOBALS["_person_id"]) {
	$errMsg = getLM("Your session is expired, please login once again");
}



if (!(int)$GLOBALS["_person_id"] || !((int)$_COOKIE["logged"]  || $justLogged)) {
	?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">

<html>
<head>
	<title><? echo getLM("Authorization"); ?></title>
 	<link rel="stylesheet" href="<? echo $pathToSiteRoot; ?>common/styles/common.css" type="text/css">
</head>

<body>
<table cellspacing=0 cellpadding=0 border=0 width="100%" height="100%">
<td align=center>
	<table cellspacing=5 cellpadding=0 border=0>
	<form name="authform" action="<? echo $_SERVER["PHP_SELF"] . ((int)strlen($_SERVER["QUERY_STRING"]) ? "?" . $_SERVER["QUERY_STRING"] : "");  ?>" method=post>
		<tr>
			<td align=center colspan=2><h1><? echo getLM("Please authorize");?>:</h1><? echo (strlen($errMsg) ? "<font color=red><b>" . $errMsg . "</b></font><br>&nbsp;<br>": "") ; ?></td>
		</tr>
		<tr>
			<td align=right><? echo getLM("login");?>:</td>
			<td><input name="_user_login" type=text value="<? echo $_COOKIE["login"]; ?>"></td>
		</tr>
		<tr>
			<td align=right><? echo getLM("password");?>:</td>
			<td><input name="_user_password" type=password></td>
		</tr>
		<tr>
			<td align=center colspan=2><br>
			<input type=submit name="submit" value="<? echo getLM("submit");?>">&nbsp;&nbsp;&nbsp;&nbsp;<input type=button name="cancel" value="<? echo getLM("cancel");?>" onclick="window.location.href='<? echo $pathToSiteRoot; ?>'"></td>
		</tr>
	<?
	if ($_POST) {
		foreach ($_POST as $key => $value) {
			if (is_array($value)) {
				foreach ($value as $value2) {
					if ($key != "_user_login" && $key != "_user_password") {
						echo "<input type=\"hidden\" name=\"" . $key . "[]\" value=\"" . $value2 . "\">";
					}
				}
			}
			else {
				if ($key != "_user_login" && $key != "_user_password") {
					echo "<input type=\"hidden\" name=\"" . $key . "\" value=\"" . $value . "\">";
				}
			}
		}
	}
	?>
	</form>
	</table>
</td>
</table>
<script>
	if (!document.authform._user_login.value.length) {
		document.authform._user_login.focus();
	} else {
		document.authform._user_password.focus();
	}
</script>
</body>
</html>
<?
	exit;
}
else {
/*
	if ($GLOBALS["justLogged"]) {
		$strSQL = "UPDATE logins SET last_login_date = NOW(), last_logout_date = NOW(), last_login_ip = " . ip2int($_SERVER["REMOTE_ADDR"]) . ", last_session_reqs = 0 WHERE logins.person_id = " . $GLOBALS["_person_id"];
	}
	else {
		$strSQL = "UPDATE logins SET last_logout_date = NOW() WHERE logins.person_id = " . (int)$GLOBALS["_person_id"];
	}

	mysql_query($strSQL, $GLOBALS["objConn"]) or die("Ошибка сохранения статистики<br>" . mysql_error());
*/
}


function logout() {
	global $justLogged;

	setcookie("logged", 0, 0, "/");
	$justLogged = false;
	$GLOBALS["_person_id"] = 0;
	$_SESSION["login"] = "";
	$_SESSION["passwd"] = "";
}



function writeAuthProblem($val) {
	$arrErr = array(
		0 => getLM("There are no pages for your security role"),
		1 => getLM("You have successfully logged out")
	);
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">

<html>
<head>
	<title><? echo $arrErr[$val]; ?></title>
	<link rel="stylesheet" href="/common/styles/common.css" type="text/css">
</head>
<body>
<table cellspacing=0 cellpadding=0 border=0 width="100%" height="100%">
<tr>
<td valign=middle align=center><h1><? echo $arrErr[$val]; ?></h1><p><a href="<? echo $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . ($GLOBALS["_person_id"] ? "?logout" : "") ?>"><? echo getLM("log in as another user");?></a></p></td>
</tr>
</table>
</body>
</html>
<?
	exit;
}





function fillSystemVars() {

	$strSQL = "SELECT ksi.person_role.role_id, ksi.role.name 
	FROM ksi.person_role 
		INNER JOIN ksi.role ON ksi.role.role_id = ksi.person_role.role_id
	WHERE ksi.person_role.person_id = " . (int)$GLOBALS["_person_id"];

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	while (ocifetch($stmt)) {
		$GLOBALS["_person_roles"][] = ociresult($stmt, "ROLE_ID");
		$GLOBALS["_person_roles_str"] .= ", " . ociresult($stmt, "NAME");
	}
	$GLOBALS["_person_roles_str"] = substr($GLOBALS["_person_roles_str"], 2);
	ocifreestatement($stmt);

	$strSQL = "SELECT key, value FROM ksi.settings";
	
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		die("could not get system parameters");
	}
	$GLOBALS["_settings"] = array();
	while ($row = oci_fetch_assoc($stmt)) {
		$GLOBALS["_settings"][$row["KEY"]] = $row["VALUE"];
	}
	ocifreestatement($stmt);


}
?>
