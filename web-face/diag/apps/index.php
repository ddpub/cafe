<?
$pathToSiteRoot = "../../";
$appName = "diag";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);





function writeMainContent() {


?>
<table cellspacing=0 cellpadding=0 cellspacing=5 width="100%">
<td valign=top>
<?

	$strSQL = "SELECT application.*, role.name AS rolename
       FROM ksi.application 
         INNER JOIN ksi.application_role ON application.application_id = application_role.application_id
         INNER JOIN ksi.role ON application_role.role_id = role.role_id
       ORDER BY application.name, role.role_id";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	$appArr = array();
	while (ocifetch($stmt)) {
		$appArr[ociresult($stmt, "APPLICATION_ID")]["appname"] = ociresult($stmt, "NAME");
		$appArr[ociresult($stmt, "APPLICATION_ID")]["server_type"] = ociresult($stmt, "SERVER_TYPE");
		$appArr[ociresult($stmt, "APPLICATION_ID")]["roles"][] = ociresult($stmt, "ROLENAME");
	}

	ocifreestatement($stmt);

	echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr>
	<th>" . getLM("application") . "</th>
	<th>" . getLM("id") . "</th>
	<th>" . getLM("can connect to") . "</th>
	<th>" . getLM("available to roles") . "</th>";
	echo "</tr>";

	foreach ($appArr as $key => $val) {
		$i = $i ? 0 : 1;

		echo "<tr style=\"background-color: " .($err ? "#ff8080" : ($i ? "#ffffff" : "#f6f6f6")) . "\">";

		echo "<td>" . $val["appname"] . "</td>";
		echo "<td align=right>" . $key . "</td>";

		echo "<td>" . ($val["server_type"] & 1 ? "central server<br>" : "") . ($val["server_type"] & 2 ? "cafe server" : "") . "</td>";
		
		echo "<td>";

		foreach ($val["roles"] as $val2) {
			echo strtolower($val2) . "<br>";
		}

		echo "</td>";
		echo "</tr>";
	}


?>
</td>
</table>
<?
}





require($pathToSiteRoot . "common/template.php");
?>