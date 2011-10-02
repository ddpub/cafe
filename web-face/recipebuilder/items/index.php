<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);

if ($_GET["act"] == "delg") {

	logTranBegin(3);
	$strSQL = "DELETE FROM ksi.product_group WHERE group_id = " . (int)$_GET["id"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = getLM("Cannot delete item group: its not empty");
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=delg&/", "", $_SERVER["QUERY_STRING"]));
	}

}
elseif ($_GET["act"] == "deli") {
	logTranBegin(6);
	$strSQL = "DELETE FROM ksi.product WHERE product_id = " . (int)$_GET["id"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = getLM("Cannot delete item: it is included in a recipe");
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=deli&/", "", $_SERVER["QUERY_STRING"]));
	}
}
if ($_GET["act"] == "delit") {
	logTranBegin(177);
	$strSQL = "DELETE FROM ksi.tare_product WHERE product_id = " . (int)$_GET["id"] . " AND tare_id = " . (int)$_GET["t"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = getLM("Unknown error");
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=delit&/", "", $_SERVER["QUERY_STRING"]));
	}

}

function writeMainContent() {

?>
<table cellspacing=0 cellpadding=0 cellspacing=5 width="100%">
<td valign=top>
<?


	class rTree2 extends rTree {
	
		function writeTreeExpanded($parentID, $level) {
	
			for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
				if ($GLOBALS["curGroup"] == $this->arrIdx[$parentID][$i]) {
					echo "<tr class=\"selectedRow\"><td style=\"padding-left: " . (10 + 20 * $level) . "px\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle>" . $this->arrTree[$this->arrIdx[$parentID][$i]][2];
				}
				else {
					echo "<tr><td style=\"padding-left:" . (10+ 20 * $level) . "px\"><a href=\"" . $_SERVER["SCRIPT_NAME"] . "?g=" . $this->arrIdx[$parentID][$i] . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_c.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle>" . $this->arrTree[$this->arrIdx[$parentID][$i]][2] . "</a>";
				}
				echo "</td><td align=right style=\"padding-right:" . (40 - $level * 12 > 0 ? 40 - $level * 12 : 0) . "px\">" . $this->arrTree[$this->arrIdx[$parentID][$i]][3] . "</td><td>" . str_repeat("&nbsp;", $level) . $this->arrTree[$this->arrIdx[$parentID][$i]][4] . "</td>";
	
	
				echo "<td class=tiny nowrap><a href=\"group_details.php?id=" . $this->arrIdx[$parentID][$i] . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delg&id=" . $this->arrIdx[$parentID][$i] . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">" . getLM("delete") . "</a></td></tr>";
	
				$this->writeTreeExpanded($this->arrIdx[$parentID][$i], $level+1);
			}
		}
	
	
	
		function passTreeCustom($parentID, $level) {
	
			for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
				if (!$GLOBALS["curGroup"]) {
					$GLOBALS["curGroup"] = $this->arrIdx[$parentID][$i];
					$GLOBALS["curGroupName"] = $this->arrTree[$this->arrIdx[$parentID][$i]][2];
				}
	
				if ($_GET["g"] == $this->arrIdx[$parentID][$i]) {
					$GLOBALS["curGroup"] = $this->arrIdx[$parentID][$i];
					$GLOBALS["curGroupName"] = $this->arrTree[$this->arrIdx[$parentID][$i]][2];
				}
	
				$this->passTreeCustom($this->arrIdx[$parentID][$i], $level+1);
			}
		}
	
	}




	$groupTree = new rTree2("SELECT group_id, parent_id, name, pos FROM ksi.product_group ORDER BY pos, name", $GLOBALS["objConn"]);


	$groupTree->passTreeCustom(0, 0);

	$tmpArr = array(array(getLM("Add new"), "group_details.php", "images/icons/addfolder.gif"));

	if (count($groupTree->arrTree)) {
		array_push($tmpArr, 		
		array(getLM("Edit group"), "group_details.php?id=" . $GLOBALS["curGroup"], "images/icons/edit.gif"),
		array(getLM("Delete group"), $_SERVER["SCRIPT_NAME"] . "?act=delg&id=" . $GLOBALS["curGroup"], "images/icons/delete.gif", "onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\""));
	}






	echo "<h2>" . getLM("Item groups") . "</h2>";

	writeActionsBar($tmpArr);

	if (count($groupTree->arrTree)) {
		echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr><th colspan=2>" . getLM("item group") . "</th><th>" . getLM("position") . "</th><th>" . getLM("action") . "</th></tr>";
		$groupTree->writeTreeExpanded(0, 0);
		echo "</table>";
	}
?>
</td>
<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<?















	$strSQL = "SELECT ksi.product.product_id, ksi.product.name, ksi.measure_type.name AS measure_type, ksi.measure.name AS default_measure, product.pos 
	FROM ksi.product 
		INNER JOIN ksi.measure_type ON ksi.product.measure_type_id = ksi.measure_type.measure_type_id
		INNER JOIN ksi.measure ON ksi.product.default_measure_id = ksi.measure.measure_id
	WHERE ksi.product.group_id = " . $GLOBALS["curGroup"] . "
	ORDER BY product.pos, ksi.product.name";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);


	echo "<h2>" . ucfirst($GLOBALS["curGroupName"]) . " " . getLM("items") . "</h2>";

	writeActionsBar(array(
		array(getLM("Add new"), "item_details.php?g=" . $GLOBALS["curGroup"], "images/icons/add.gif"),
/*		array("Edit", "group_details.php?id=" . $GLOBALS["curGroup"], "images/icons/edit.gif"),
		array("Delete", $_SERVER["SCRIPT_NAME"] . "?act=delg&id=" . $GLOBALS["curGroup"], "images/icons/delete.gif"),*/
	));


	echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
	echo "<tr><th>" . getLM("item") . "</th><th>" . getLM("measure type") . "</th><th>" . getLM("default measure") . "</th><th>" . getLM("position") . "</th><th>" . getLM("action") . "</th></tr>";

	oci_fetch_all($stmt, $arrRes, 0, -1, OCI_FETCHSTATEMENT_BY_ROW + OCI_ASSOC );
	ocifreestatement($stmt);

	foreach ($arrRes as $val) {
		if (!$GLOBALS["curItem"]) {
			$GLOBALS["curItem"] = $val["PRODUCT_ID"];
			$GLOBALS["curItemName"] = $val["NAME"];
		}

		if ($_GET["p"] == $val["PRODUCT_ID"]) {
			$GLOBALS["curItem"] = $val["PRODUCT_ID"];
			$GLOBALS["curItemName"] = $val["NAME"];
		}
	}

	reset($arrRes);
	foreach ($arrRes as $val) {
		echo "<tr" . ($val["PRODUCT_ID"] == $GLOBALS["curItem"] ? " class=\"selectedRow\"" : "") . "><td>";
//		echo "<a href=\"item_details.php?id=" . ociresult($stmt, "PRODUCT_ID") . "\">" . ociresult($stmt, "NAME") . "</a>";
		if ($val["PRODUCT_ID"] == $GLOBALS["curItem"]) {
			echo $val["NAME"];
		} else {
			echo "<a href=\"" . $_SERVER["SCRIPT_NAME"] . "?g=" . $GLOBALS["curGroup"] . "&p=" . $val["PRODUCT_ID"] . "\">" . $val["NAME"] . "</a>";
		}
		echo "</td>
		<td>" . $val["MEASURE_TYPE"] . "</td>
		<td>" . $val["DEFAULT_MEASURE"] . "</td>
		<td>" . $val["POS"] . "</td>
		<td class=tiny><a href=\"item_details.php?id=" . $val["PRODUCT_ID"] . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=deli&id=" . $val["PRODUCT_ID"] . "&g=" . $GLOBALS["curGroup"] . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">" . getLM("delete") . "</a></td></tr>";
	}


	echo "</table>";

?>
</td>
<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<?
	echo "<h2>" . getLM("Assigned tares") . "</h2>";

	if ($GLOBALS["curItem"]) {


		writeActionsBar(array(
			array(getLM("Edit list"), "it_details.php?g=" . $GLOBALS["curGroup"] . "&p=" . $GLOBALS["curItem"], "images/icons/edit.gif"),
	/*		array("Edit", "group_details.php?id=" . $GLOBALS["curGroup"], "images/icons/edit.gif"),
			array("Delete", $_SERVER["SCRIPT_NAME"] . "?act=delg&id=" . $GLOBALS["curGroup"], "images/icons/delete.gif"),*/
		));
	
		$strSQL = "SELECT ksi.tare_product.*
			, ksi.tare.title
			, ksi.tare.capacity
			, cm.shortname AS cm_name
			, wm.shortname AS wm_name
		FROM ksi.tare_product 
			INNER JOIN ksi.tare ON ksi.tare_product.tare_id = ksi.tare.tare_id AND ksi.tare_product.product_id = " . $GLOBALS["curItem"] . "
			INNER JOIN ksi.measure cm ON ksi.tare.capacity_measure_id = cm.measure_id 
			INNER JOIN ksi.measure wm ON ksi.tare.weight_measure_id = wm.measure_id 
			";
	
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
		$arrRes = array();
		oci_fetch_all($stmt, $arrRes, 0, -1, OCI_FETCHSTATEMENT_BY_ROW + OCI_ASSOC);
		ocifreestatement($stmt);
	
	
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" . getLM("tare") . "</th><th >" . getLM("tare capacity") . "</th><th>" . getLM("full tare weight") . "</th><th>" . getLM("action") . "</th></tr>";
	
		foreach ($arrRes as $val) {
			echo "<tr>";
			echo "<td>" . $val["TITLE"] . "</td>";
			echo "<td align=right>" . $val["CAPACITY"] . " " . $val["CM_NAME"] . "</td>";
			echo "<td align=right>" . $val["WEIGHT_FULL"] . " " . $val["WM_NAME"] . "</td>";
			echo "<td><a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delit&id=" . $val["PRODUCT_ID"] . "&t=" . $val["TARE_ID"] . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">" . getLM("delete") . "</a></td>";
			echo "</tr>";
		}
		echo "</table>";
	} else {
		echo getLM("Add an item first");
	}
	

?>
</td>
</table>
<?
}

require($pathToSiteRoot . "common/template.php");
?>