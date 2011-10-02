<?
$pathToSiteRoot = "../../";
$appName = "administrator";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

if ($_GET["act"] == "delc" && $GLOBALS["_settings"]["NETWORK_MODE"] == "NETWORK") {

	logTranBegin(46);
	$strSQL = "UPDATE ksi.cafe SET deleted = SYSDATE WHERE cafe_id = " . (int)$_GET["id"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "unexpected error";
	}
	else {
		logTranEnd();
	}

}
elseif ($_GET["act"] == "delw") {
	logTranBegin(52);
//	$strSQL = "DELETE FROM ksi.warehouse WHERE warehouse_id = " . (int)$_GET["id"];
	$strSQL = "UPDATE ksi.warehouse SET deleted = SYSDATE WHERE warehouse_id = " . (int)$_GET["id"];

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "unexpected error";
	}
	else {
		logTranEnd();
	}
}
elseif ($_GET["act"] == "delt") {
	logTranBegin(79);
//	$strSQL = "DELETE FROM ksi.terminal WHERE terminal_id = " . (int)$_GET["id"];
	$strSQL = "UPDATE ksi.terminal SET deleted = SYSDATE WHERE terminal_id = " . (int)$_GET["id"];

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "unexpected error";
	}
	else {
		logTranEnd();
	}

}
elseif ($_GET["act"] == "deltk") {
	logTranBegin(101);
	$strSQL = "DELETE FROM ksi.token WHERE token_id = " . (int)$_GET["id"];

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "unexpected error";
	}
	else {
		logTranEnd();
	}

}
elseif ($_GET["act"] == "delsch") {
	logTranBegin(108);
	$strSQL = "DELETE FROM ksi.delivery_schedule WHERE schedule_id = " . (int)$_GET["id"];

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "unexpected error";
	}
	else {
		logTranEnd();
	}
}
elseif ($_GET["act"] == "delg") {
	logTranBegin(136);
//	$strSQL = "DELETE FROM ksi.table_group WHERE table_group_id = " . (int)$_GET["id"];

	$strSQL = "UPDATE ksi.table_group SET DELETED = sysdate WHERE table_group_id = " . (int)$_GET["id"];

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "unexpected error";
	}
	else {
		logTranEnd();
	}
}
elseif ($_GET["act"] == "deltb") {
	logTranBegin(133);
	$strSQL = "DELETE FROM ksi.tables WHERE table_id = " . (int)$_GET["id"];

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "unexpected error";
	}
	else {
		logTranEnd();
	}
}
elseif ($_GET["act"] == "delk") {
	logTranBegin(147);
	$strSQL = "DELETE FROM ksi.kitchen WHERE kitchen_id = " . (int)$_GET["id"];

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "unexpected error";
	}
	else {
		logTranEnd();
	}
}
elseif ($_GET["act"] == "delpst") {
	logTranBegin(162);
	$strSQL = "DELETE FROM ksi.cafe_schedule_type WHERE type_id = " . (int)$_GET["id"];

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "this schedule type is being used";
	}
	else {
		logTranEnd();
	}
}
elseif ($_GET["act"] == "delkt") {
	logTranBegin(165);
	$strSQL = "DELETE FROM ksi.kitchen_type WHERE kt_id = " . (int)$_GET["id"];

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "this kitchen type is being used";
	}
	else {
		logTranEnd();
	}
}






function writeMainContent() {

?>
<table cellspacing=0 cellpadding=0 cellspacing=5 width="100%">
<td valign=top>
<h2><? echo getLM("Cafes"); ?></h2>
<?
	$strSQL = "SELECT ksi.cafe.cafe_id, ksi.cafe.name, ksi.cafe.type_id, ksi.cafe_type.name AS cafetype 
		FROM ksi.cafe 
			INNER JOIN ksi.cafe_type ON ksi.cafe.type_id = ksi.cafe_type.type_id
		WHERE ksi.cafe.deleted IS NULL
		ORDER BY ksi.cafe.type_id, LOWER(ksi.cafe.name)";



	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	$curCafe = 0;
	while (ocifetch($stmt)) {
		if (!$curCafe) {
			$curCafe = ociresult($stmt, "CAFE_ID");
			$curCafeName = ociresult($stmt, "NAME");
			$curCafeType = ociresult($stmt, "TYPE_ID");
		}
		if ((int)$_GET["c"] == ociresult($stmt, "CAFE_ID")) {
			$curCafe = ociresult($stmt, "CAFE_ID");
			$curCafeName = ociresult($stmt, "NAME");
			$curCafeType = ociresult($stmt, "TYPE_ID");
		}
		$arrCafe[] = array(ociresult($stmt, "CAFE_ID"), ociresult($stmt, "NAME"), ociresult($stmt, "CAFETYPE"), ociresult($stmt, "TYPE_ID"));
	}
	ocifreestatement($stmt);

	$tmpArr = array(
		($GLOBALS["_settings"]["NETWORK_MODE"] == "NETWORK" ? array(getLM("Add new"), "cafe_details.php", "images/icons/add.gif") : null),
		array(getLM("Edit"), "cafe_details.php?id=" . $curCafe, "images/icons/edit.gif")
	);

	if ($curCafeType != 1) {
		array_push($tmpArr, array(getLM("Delete"), $_SERVER["SCRIPT_NAME"] . "?act=delm&id=" . $curCafe, "images/icons/delete.gif", "onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\""));
	}

	writeActionsBar($tmpArr);

	echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr><th>" . getLM("cafe") . "</th><th>" . getLM("type") . "</th><th>" . getLM("action") . "</th></tr>";

	foreach ($arrCafe as $val) {
		echo "<tr" . ($val[0] == $curCafe ? " class=\"selectedRow\"" : "") . "><td>";

		switch ($val[3]) {
			case 1:
				$tmpStr = "server.gif";
				break;
			case 2:
				$tmpStr = "cafe.gif";
				break;
			case 3:
				$tmpStr = "laptop.gif";
				break;
		}

		$imgStr = "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/" . $tmpStr . "\" width=16 height=16 alt=\"" . $val[2] . "\" border=0 align=absmiddle style=\"margin-right: 10px\">";

		if ($val[0] == $curCafe) {
			echo $imgStr . $val[1];
		}
		else {
			echo "<a href=\"" . $_SERVER["SCRIPT_NAME"] . "?c=" . $val[0] . "\">" . $imgStr . $val[1] . "</a>";
		}

		echo "<td>" . $val[2] . "</td><td class=tiny nowrap><a href=\"cafe_details.php?id=" . $val[0] . "\">" . getLM("edit") . "</a>" .
			($val[3] != 1 ? " | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delc&id=" . $val[0] . "\" onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a>" : "") .  
			"</td></tr>";
	}
	echo "</table>";


?>
</td>
<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<h2><? echo ($curCafeType == 1 && $GLOBALS["_settings"]["NETWORK_MODE"] == "NETWORK" ? getLM("Central") . " " : ($GLOBALS["_settings"]["NETWORK_MODE"] == "NETWORK" ? $curCafeName . " ": "" )) . getLM("warehouses") ; ?></h2>
<?



	$strSQL = "SELECT * FROM ksi.warehouse WHERE cafe_id = " . $curCafe . " AND ksi.warehouse.deleted IS NULL ORDER BY name";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);


	writeActionsBar(array(
		array(getLM("Add new"), "warehouse_details.php?c=" . $curCafe, "images/icons/add.gif"),
	));


	echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
	echo "<tr><th>" . getLM("warehouse") . "</th><th>" . getLM("action") . "</th></tr>";
	while (ocifetch($stmt)) {
		echo "<tr><td>" . ociresult($stmt, "NAME") . "</td>
		<td class=tiny nowrap><a href=\"warehouse_details.php?id=" . ociresult($stmt, "WAREHOUSE_ID") . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delw&id=" . ociresult($stmt, "WAREHOUSE_ID") . "&c=" . $curCafe . "\" onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
	}
	ocifreestatement($stmt);

	echo "</table>";








 	if ($curCafeType != 1 || $GLOBALS["_settings"]["NETWORK_MODE"] == "STANDALONE") {?>

<h2><? echo ($GLOBALS["_settings"]["NETWORK_MODE"] == "NETWORK" ? $curCafeName . " ": "" ) . getLM("kitchen instances"); ?></h2>
<?

		$strSQL = "SELECT * FROM ksi.kitchen WHERE cafe_id = " . $curCafe . " ORDER BY name";
	
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
	
	
		writeActionsBar(array(
			array(getLM("Add new"), "kitchen_details.php?c=" . $curCafe, "images/icons/add.gif"),
		));
	
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" . getLM("kitchen") . "</th><th>" . getLM("action") . "</th></tr>";
		while (ocifetch($stmt)) {
			echo "<tr><td>" . ociresult($stmt, "NAME") . "</td>
			<td class=tiny nowrap><a href=\"kitchen_details.php?id=" . ociresult($stmt, "KITCHEN_ID") . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delk&id=" . ociresult($stmt, "KITCHEN_ID") . "&c=" . $curCafe . "\" onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
		}
		ocifreestatement($stmt);
	
		echo "</table>";
	
	
	
	
	
	?>
	<h2><? echo ($GLOBALS["_settings"]["NETWORK_MODE"] == "NETWORK" ? $curCafeName . " ": "" ) . getLM("POS terminals"); ?></h2>
	<?
	
	
	
		$strSQL = "SELECT * FROM ksi.terminal WHERE cafe_id = " . $curCafe . " AND ksi.terminal.deleted IS NULL ORDER BY name";
	
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
	
	
		writeActionsBar(array(
			array(getLM("Add new"), "terminal_details.php?c=" . $curCafe, "images/icons/add.gif"),
		));
	
	
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" . getLM("terminal") . "</th><th>" . getLM("mac address") . "</th><th>" . getLM("action") . "</th></tr>";
		while (ocifetch($stmt)) {
			echo "<tr><td>" . ociresult($stmt, "NAME") . "</td>
			<td>" . ociresult($stmt, "MAC") . "</td>
			<td class=tiny nowrap><a href=\"terminal_details.php?id=" . ociresult($stmt, "TERMINAL_ID") . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delt&id=" . ociresult($stmt, "TERMINAL_ID") . "&c=" . $curCafe . "\" onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
		}
		ocifreestatement($stmt);
	
		echo "</table>";
	?>
	
	<h2><? echo ($GLOBALS["_settings"]["NETWORK_MODE"] == "NETWORK" ? $curCafeName . " ": "" ) . getLM("tokens"); ?></h2>
	<?
	
	
	
		$strSQL = "SELECT * FROM ksi.token WHERE cafe_id = " . $curCafe . " ORDER BY name";
	
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
	
	
		writeActionsBar(array(
			array(getLM("Add new"), "token_details.php?c=" . $curCafe, "images/icons/add.gif"),
		));
	
	
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" . getLM("token") . "</th><th>" . getLM("action") . "</th></tr>";
		while (ocifetch($stmt)) {
			echo "<tr><td>" . ociresult($stmt, "NAME") . "</td>
			<td class=tiny nowrap><a href=\"token_details.php?id=" . ociresult($stmt, "TOKEN_ID") . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=deltk&id=" . ociresult($stmt, "TOKEN_ID") . "&c=" . $curCafe . "\" onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
		}
		ocifreestatement($stmt);
	
		echo "</table>";
	}


?>
</td>

<?




	if ($curCafeType == 2 || $GLOBALS["_settings"]["NETWORK_MODE"] == "STANDALONE") {?>
<td valign=top>
<h2><? echo ($GLOBALS["_settings"]["NETWORK_MODE"] == "NETWORK" ? $curCafeName . " ": "" ) . getLM("delivery schedule"); ?></h2>
<?
		$strSQL = "SELECT schedule_id
		, zip
		, CASE WHEN active = 1 THEN '" . getLM("active") . "' ELSE '" . getLM("inactive") . "' END AS active
		, TO_CHAR(open_time, 'hh12:miam') || ' &ndash; ' || TO_CHAR(close_time, 'hh12:miam') AS whours
		, CASE WHEN weekday_mask = 127 
			THEN 'all days'
		ELSE
			
				CASE WHEN BITAND(weekday_mask, 1) <> 0 THEN 'Su ' END ||
				CASE WHEN BITAND(weekday_mask, 2) <> 0 THEN 'Mo ' END ||
				CASE WHEN BITAND(weekday_mask, 4) <> 0 THEN 'Tu ' END ||
				CASE WHEN BITAND(weekday_mask, 8) <> 0 THEN 'We ' END ||
				CASE WHEN BITAND(weekday_mask, 16) <> 0 THEN 'Th ' END ||
				CASE WHEN BITAND(weekday_mask, 32) <> 0 THEN 'Fr ' END ||
				CASE WHEN BITAND(weekday_mask, 64) <> 0 THEN 'Sa ' END
	
		END AS wdays
		FROM ksi.delivery_schedule WHERE cafe_id = " . $curCafe . " ORDER BY zip";
	
	
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
	
	
		writeActionsBar(array(
			array(getLM("Add new"), "schedule_details.php?c=" . $curCafe, "images/icons/add.gif"),
		));
	
	
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" . getLM("zip") . "</th><th colspan=2>" . getLM("delivery days and hours") . "</th><th>" . getLM("active") . "</th><th>" . getLM("action") . "</th></tr>";
		while (ocifetch($stmt)) {
			echo "<tr><td>" . ociresult($stmt, "ZIP") . "</td>
			<td>" . ociresult($stmt, "WDAYS") . "</td>
			<td>" . ociresult($stmt, "WHOURS") . "</td>
			<td>" . ociresult($stmt, "ACTIVE") . "</td>
			<td class=tiny nowrap><a href=\"schedule_details.php?id=" . ociresult($stmt, "SCHEDULE_ID") . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delsch&id=" . ociresult($stmt, "SCHEDULE_ID") . "&c=" . $curCafe . "\" onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
		}
		ocifreestatement($stmt);
	
		echo "</table>";
	?><br>
	<table><td valign=top>
	<h2><? echo ($GLOBALS["_settings"]["NETWORK_MODE"] == "NETWORK" ? $curCafeName . " ": "" ) . getLM("table groups"); ?></h2>
	<?
		$strSQL = "SELECT * FROM ksi.table_group WHERE cafe_id = " . $curCafe . " AND DELETED IS NULL ORDER BY pos, name";
	
	
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
	
		$curGroup = 0;
		$arrGroup = array();
	
		while (ocifetch($stmt)) {
			if (!$curGroup) {
				$curGroup = ociresult($stmt, "TABLE_GROUP_ID");
				$curGroupName = ociresult($stmt, "NAME");
			}
			if ((int)$_GET["g"] == ociresult($stmt, "TABLE_GROUP_ID")) {
				$curGroup = ociresult($stmt, "TABLE_GROUP_ID");
				$curGroupName = ociresult($stmt, "NAME");
			}
			$arrGroup[] = array(ociresult($stmt, "TABLE_GROUP_ID"), ociresult($stmt, "NAME"), ociresult($stmt, "IS_DEFAULT"), ociresult($stmt, "POS"));
		}
		ocifreestatement($stmt);
	
	
		$arrTmp = array(array(getLM("Add new group"), "tgroup_details.php?c=" . $curCafe, "images/icons/addfolder.gif"));
	
		if (count($arrGroup)) {
			array_push($arrTmp, 
				array(getLM("Edit group"), "tgroup_details.php?id=" . $curGroup, "images/icons/edit.gif"),
				array(getLM("Delete group"), $_SERVER["SCRIPT_NAME"] . "?act=delg&id=" . $curGroup . ($showDeleted ? "&showdeleted" : ""), "images/icons/delete.gif", "onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\"")
			);
		}
		writeActionsBar($arrTmp);
	
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" . getLM("table group") . "</th><th>" . getLM("default") . "</th><th>" . getLM("position") . "</th><th>" . getLM("action") . "</th></tr>";
		foreach ($arrGroup as $val) {
			echo "<tr" . ($val[0] == $curGroup ? " class=\"selectedRow\"" : "") . "><td nowrap>";
			if ($val[0] == $curGroup) {
				echo "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle>" . $val[1];
			}
			else {
				echo "<a href=\"" . $_SERVER["SCRIPT_NAME"] . "?c=" . $curCafe . "&g=" . $val[0] . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "/images/icons/fld_c.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle>" . $val[1] . "</a>";
			}
	
			echo "</td><td nowrap>" . ($val[2] ? "yes" : "") . "</td><td nowrap>" . $val[3] . "</td><td class=tiny nowrap><a href=\"tgroup_details.php?c=" . $curCafe . "&id=" . $val[0] . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delg&c=" . $curCafe . "&id=" . $val[0] .  "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
		}
		echo "</table>";
	?>
	</td><td valign=top>
	<h2><? echo ($GLOBALS["_settings"]["NETWORK_MODE"] == "NETWORK" ? $curCafeName . " ": "" ) . getLM("tables"); ?></h2>
	<?
		if (!count($arrGroup)) {
			echo "<p>" . getLM("Create table group first") . "</p>";
		}
		else {
	
			$strSQL = "SELECT * FROM ksi.tables WHERE table_group_id = " . $curGroup . " ORDER BY name";
		
			$stmt = ociparse($GLOBALS["objConn"], $strSQL);
			ociexecute($stmt, OCI_DEFAULT);
		
		
			writeActionsBar(array(
				array(getLM("Add new"), "table_details.php?c=" . $curCafe . "&g=" . $curGroup . "", "images/icons/add.gif"),
			));
		
		
			echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
			echo "<tr><th>" . getLM("table") . "</th><th>" . getLM("action") . "</th></tr>";
			while (ocifetch($stmt)) {
				echo "<tr><td>" . ociresult($stmt, "NAME") . "</td>
				<td class=tiny nowrap><a href=\"table_details.php?id=" . ociresult($stmt, "TABLE_ID") . "&c=" . $curCafe . "&g=" . $curGroup . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=deltb&id=" . ociresult($stmt, "TABLE_ID") . "&c=" . $curCafe . "&g=" . $curGroup . "\" onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
			}
			ocifreestatement($stmt);
		
			echo "</table>";
		}
	
	?>
	</td></table>
	
	</td>
	<? 
	}








	if ($curCafeType == 1 || $GLOBALS["_settings"]["NETWORK_MODE"] == "STANDALONE") {?>
<td valign=top>
<h2><? echo getLM("Personnel schedule types"); ?></h2>
<?
		$strSQL = "SELECT * FROM ksi.cafe_schedule_type ORDER BY title";
	
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
	
	
		writeActionsBar(array(
			array(getLM("Add new"), "pst_details.php?c=" . $curCafe, "images/icons/add.gif"),
		));
	
	
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" . getLM("type") . "</th><th>" . getLM("action") . "</th></tr>";
		while (ocifetch($stmt)) {
			echo "<tr><td>" . ociresult($stmt, "TITLE") . "</td>
			<td class=tiny nowrap><a href=\"pst_details.php?id=" . ociresult($stmt, "TYPE_ID") . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delpst&id=" . ociresult($stmt, "TYPE_ID") . "&c=" . $curCafe . "\" onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
		}
		ocifreestatement($stmt);
	
		echo "</table>";
?>


<h2><? echo getLM("Kitchen types"); ?></h2>
<?
		$strSQL = "SELECT * FROM ksi.kitchen_type ORDER BY name";
	
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
	
	
		writeActionsBar(array(
			array(getLM("Add new"), "kt_details.php?c=" . $curCafe, "images/icons/add.gif"),
		));
	
	
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" . getLM("type") . "</th><th>" . getLM("action") . "</th></tr>";
		while (ocifetch($stmt)) {
			echo "<tr><td>" . ociresult($stmt, "NAME") . "</td>
			<td class=tiny nowrap><a href=\"kt_details.php?id=" . ociresult($stmt, "KT_ID") . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delkt&id=" . ociresult($stmt, "KT_ID") . "&c=" . $curCafe . "\" onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
		}
		ocifreestatement($stmt);
	
		echo "</table>";
?>
</td>
<?
	}
?>
</table>
<?
}

require($pathToSiteRoot . "common/template.php");
?>