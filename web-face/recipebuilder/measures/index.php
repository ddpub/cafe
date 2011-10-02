<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);

if ($_GET["act"] == "deltg") {
	$strSQL = "DELETE FROM ksi.tare_group WHERE group_id = " . (int)$_GET["id"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);

	logTranBegin(174);

	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "Cannot delete tare group: its not empty";
	}
	logTranEnd();
}

if ($_GET["act"] == "delt") {
	$strSQL = "DELETE FROM ksi.tare WHERE tare_id = " . (int)$_GET["id"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);

	logTranBegin(171);

	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "Cannot delete tare: its not empty";
	}
	logTranEnd();
}


elseif ($_GET["act"] == "delm") {

	logTranBegin(43);

	$strSQL = "DELETE FROM ksi.measure WHERE measure_id = " . (int)$_GET["id"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "Cannot delete measure: it is being used";
	}
	logTranEnd();


}

function writeMainContent() {

?>
<table cellspacing=0 cellpadding=0 cellspacing=5 width="100%">
<td valign=top>
<?

	$strSQL = "SELECT measure_type_id AS group_id, name FROM ksi.measure_type ORDER BY name";
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);



	$curGroup = 0;
	$arrGroup = array();


	while (ocifetch($stmt)) {
		if (!$curGroup) {
			$curGroup = ociresult($stmt, "GROUP_ID");
			$curGroupName = ociresult($stmt, "NAME");
		}
		if ((int)$_GET["g"] == ociresult($stmt, "GROUP_ID")) {
			$curGroup = ociresult($stmt, "GROUP_ID");
			$curGroupName = ociresult($stmt, "NAME");
		}
		$arrGroup[] = array(ociresult($stmt, "GROUP_ID"), ociresult($stmt, "NAME"));
	}
	ocifreestatement($stmt);


	echo "<h2>" . getLM("Measure types") . "</h2>";
/*
	writeActionsBar(array(
		array("Add new group", "group_details.php", "images/icons/addfolder.gif"),
		array("Rename group", "group_details.php?id=" . $curGroup, "images/icons/rename.gif"),
		array("Delete group", $_SERVER["SCRIPT_NAME"] . "?act=delg&id=" . $curGroup, "images/icons/delete.gif", "onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\""),
	));
*/
	echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\" style=\"margin-top: 44px;\"><tr><th>" . getLM("measure type") . "</th><!--th>" . getLM("action") . "</th--></tr>";

	foreach ($arrGroup as $val) {
		echo "<tr" . ($val[0] == $curGroup ? " class=\"selectedRow\"" : "") . "><td>";
		if ($val[0] == $curGroup) {
			echo "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle>" . $val[1];
		}
		else {
			echo "<a href=\"" . $_SERVER["SCRIPT_NAME"] . "?g=" . $val[0] . ((int)$_GET["tg"] ? "&tg=" . $_GET["tg"] : "") . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "/images/icons/fld_c.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle>" . $val[1] . "</a>";
		}

		echo "</td>" /*"<td class=tiny><a href=\"group_details.php?id=" . $val[0] . "\">rename</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delg&id=" . $val[0] . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">delete</a></td>"*/ . "</tr>";
	}
	echo "</table>";
?>
</td>
<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<?















	$strSQL = "SELECT *
	FROM ksi.measure
	WHERE ksi.measure.measure_type_id = " . $curGroup . "
	ORDER BY ksi.measure.base_units";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);


	echo "<h2>" . ucfirst($curGroupName) . " " . getLM("measures") . "</h2>";

	writeActionsBar(array(
		array(getLM("Add new measure"), "measure_details.php?g=" . $curGroup, "images/icons/add.gif"),
/*		array("Edit", "group_details.php?id=" . $curGroup, "images/icons/edit.gif"),
		array("Delete", $_SERVER["SCRIPT_NAME"] . "?act=delg&id=" . $curGroup, "images/icons/delete.gif"),*/
	));


	echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
	echo "<tr><th>" . getLM("measure") . "</th><th>" . getLM("short name") . "</th><th colspan=2>" . getLM("in base units") . "</th><th>" . getLM("action") . "</th></tr>";
	while (ocifetch($stmt)) {
		echo "<tr><td>";
//		echo "<a href=\"item_details.php?id=" . ociresult($stmt, "PRODUCT_ID") . "\">" . ociresult($stmt, "NAME") . "</a>";
		echo ociresult($stmt, "NAME");
		echo "</td>
		<td>" . ociresult($stmt, "SHORTNAME") . "</td>
		<td align=right style=\"padding-right:0px\">" . (int)ociresult($stmt, "BASE_UNITS") . "</td>
		<td align=left style=\"padding-left:0px\">" . substr((int)ociresult($stmt, "BASE_UNITS") - (float)ociresult($stmt, "BASE_UNITS"), 2) . "</td>
		<td class=tiny><a href=\"measure_details.php?id=" . ociresult($stmt, "MEASURE_ID") . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delm&id=" . ociresult($stmt, "MEASURE_ID") . "&g=" . $curGroup . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">" . getLM("delete") . "</a></td></tr>";
	}
	ocifreestatement($stmt);

	echo "</table>";

?>
</td>
<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<?




// tare groups




	class rTree2 extends rTree {
	
		function writeTreeExpanded($parentID, $level) {
	
			for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
				if ($GLOBALS["curTareGroup"] == $this->arrIdx[$parentID][$i]) {
					echo "<tr class=\"selectedRow\"><td style=\"padding-left: " . (10 + 20 * $level) . "px\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle>" . $this->arrTree[$this->arrIdx[$parentID][$i]][2];
				}
				else {
					echo "<tr><td style=\"padding-left:" . (10+ 20 * $level) . "px\"><a href=\"" . $_SERVER["SCRIPT_NAME"] . "?tg=" . $this->arrIdx[$parentID][$i] . ((int)$_GET["g"] ? "&g=" . $_GET["g"] : "") . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_c.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle>" . $this->arrTree[$this->arrIdx[$parentID][$i]][2] . "</a>";
				}
				echo "</td><td align=right style=\"padding-right:" . (40 - $level * 12 > 0 ? 40 - $level * 12 : 0) . "px\">" . $this->arrTree[$this->arrIdx[$parentID][$i]][3] . "</td>";
	
	
				echo "<td class=tiny nowrap><a href=\"tare_group_details.php?id=" . $this->arrIdx[$parentID][$i] . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=deltg&id=" . $this->arrIdx[$parentID][$i] . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">" . getLM("delete") . "</a></td></tr>";
	
				$this->writeTreeExpanded($this->arrIdx[$parentID][$i], $level+1);
			}
		}
	
	
	
		function passTreeCustom($parentID, $level) {
	
			for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
				if (!$GLOBALS["curTareGroup"]) {
					$GLOBALS["curTareGroup"] = $this->arrIdx[$parentID][$i];
					$GLOBALS["curTareGroupName"] = $this->arrTree[$this->arrIdx[$parentID][$i]][2];
				}
	
				if ($_GET["tg"] == $this->arrIdx[$parentID][$i]) {
					$GLOBALS["curTareGroup"] = $this->arrIdx[$parentID][$i];
					$GLOBALS["curTareGroupName"] = $this->arrTree[$this->arrIdx[$parentID][$i]][2];
				}
	
				$this->passTreeCustom($this->arrIdx[$parentID][$i], $level+1);
			}
		}
	
	}




	$groupTree = new rTree2("SELECT group_id, parent_id, title AS name FROM ksi.tare_group ORDER BY name", $GLOBALS["objConn"]);


	$groupTree->passTreeCustom(0, 0);

	$tmpArr = array(array(getLM("Add new"), "tare_group_details.php", "images/icons/addfolder.gif"));

	if (count($groupTree->arrTree)) {
		array_push($tmpArr, 		
		array(getLM("Edit group"), "tare_group_details.php?id=" . $GLOBALS["curGroup"], "images/icons/edit.gif"),
		array(getLM("Delete group"), $_SERVER["SCRIPT_NAME"] . "?act=deltg&id=" . $GLOBALS["curGroup"], "images/icons/delete.gif", "onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\""));
	}






	echo "<h2>" . getLM("Tare groups") . "</h2>";

	writeActionsBar($tmpArr);

	if (count($groupTree->arrTree)) {
		echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr><th colspan=2>" . getLM("tare group") . "</th><th>" . getLM("action") . "</th></tr>";
		$groupTree->writeTreeExpanded(0, 0);
		echo "</table>";
	}
?>
</td>
<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<?
	echo "<h2>" . ucfirst($curTareGroupName) . " " . getLM("tare") . "</h2>";

	if (count($groupTree->arrTree)) {

		$strSQL = "SELECT tare.*
			, mw.shortname AS mwname
			, mc.shortname AS mcname
		FROM ksi.tare
			INNER JOIN ksi.measure mw ON mw.measure_id = tare.weight_measure_id
			INNER JOIN ksi.measure mc ON mc.measure_id = tare.capacity_measure_id
		WHERE ksi.tare.group_id = " . $GLOBALS["curTareGroup"] . "
		ORDER BY ksi.tare.title";
	
	
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
	
	
	
	
		writeActionsBar(array(
			array(getLM("Add new tare"), "tare_details.php?tg=" . $GLOBALS["curTareGroup"], "images/icons/add.gif"),
	/*		array("Edit", "group_details.php?id=" . $curGroup, "images/icons/edit.gif"),
			array("Delete", $_SERVER["SCRIPT_NAME"] . "?act=delg&id=" . $curGroup, "images/icons/delete.gif"),*/
		));
	
	
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" . getLM("tare") . "</th><th colspan=2>" . getLM("empty tare weight") . "</th><th colspan=2>" . getLM("capacity") . "</th><th>" . getLM("action") . "</th></tr>";
		while (ocifetch($stmt)) {
			echo "<tr><td>";
	//		echo "<a href=\"item_details.php?id=" . ociresult($stmt, "PRODUCT_ID") . "\">" . ociresult($stmt, "NAME") . "</a>";
			echo ociresult($stmt, "TITLE");
			echo "</td>
			<td align=right style=\"padding-right:0px\">" . (int)ociresult($stmt, "WEIGHT") . "</td>
			<td align=left style=\"padding-left:0px\">" . substr((int)ociresult($stmt, "WEIGHT") - (float)ociresult($stmt, "WEIGHT"), 2) . "&nbsp;" . ociresult($stmt, "MWNAME") . "</td>
			<td align=right style=\"padding-right:0px\">" . (int)ociresult($stmt, "CAPACITY") . "</td>
			<td align=left style=\"padding-left:0px\">" . substr((int)ociresult($stmt, "CAPACITY") - (float)ociresult($stmt, "CAPACITY"), 2) . "&nbsp;" . ociresult($stmt, "MCNAME") . "</td>
	
			<td class=tiny><a href=\"tare_details.php?id=" . ociresult($stmt, "TARE_ID") . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delt&id=" . ociresult($stmt, "TARE_ID") . "&g=" . $curGroup . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">" . getLM("delete") . "</a></td></tr>";
		}
		ocifreestatement($stmt);
	
		echo "</table>";
	}
	else {
		echo getLM("Please create a tare group first");
	}
?>
</td>
</table>
<?
}

require($pathToSiteRoot . "common/template.php");
?>