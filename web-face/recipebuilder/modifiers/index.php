<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

if ($_GET["act"] == "delg") {

	logTranBegin(37);

	$strSQL = "UPDATE ksi.mod_group SET deleted = sysdate WHERE group_id = " . (int)$_GET["id"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = getLM("Cannot delete modifier group: its not empty");
	}
	else {
		logTranEnd();
	}
//	ocicommit($GLOBALS["objConn"]);

//	_showCurTransaction();




}
elseif ($_GET["act"] == "delm") {

	logTranBegin(40);

	$strSQL = "UPDATE ksi.modifier SET deleted = sysdate WHERE modifier_id = " . (int)$_GET["id"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = getLM("Cannot delete modifier: it is being used");
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=delm&/", "", $_SERVER["QUERY_STRING"]));
	}

//	ocicommit($GLOBALS["objConn"]);

//	logTranEnd();

}

function writeMainContent() {

?>
<table cellspacing=0 cellpadding=0 cellspacing=5 width="100%">
<td valign=top>
<?

	$strSQL = "SELECT * FROM ksi.mod_group WHERE deleted IS NULL ORDER BY pos, name";

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
		$arrGroup[] = array(ociresult($stmt, "GROUP_ID"), ociresult($stmt, "NAME"), ociresult($stmt, "MAX_WEIGHT"), ociresult($stmt, "POS"));
	}
	ocifreestatement($stmt);


	echo "<h2>" . getLM("Modifier groups") . "</h2>";

	writeActionsBar(array(
		array(getLM("Add new group"), "group_details.php", "images/icons/addfolder.gif"),
		array(getLM("Copy group"), "group_copy.php?id=" . $curGroup, "images/icons/copy.gif"),
		array(getLM("Edit group"), "group_details.php?id=" . $curGroup, "images/icons/edit.gif"),
		array(getLM("Delete group"), $_SERVER["SCRIPT_NAME"] . "?act=delg&id=" . $curGroup, "images/icons/delete.gif", "onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\""),
	));

	echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr><th>" . getLM("modifier group") . "</th><th>" . getLM("position in list") . "</th><th>" . getLM("max weight") . "</th><th>" . getLM("action") . "</th></tr>";

	foreach ($arrGroup as $val) {
		echo "<tr" . ($val[0] == $curGroup ? " class=\"selectedRow\"" : "") . "><td>";
		if ($val[0] == $curGroup) {
			echo "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle>" . $val[1];
		}
		else {
			echo "<a href=\"" . $_SERVER["SCRIPT_NAME"] . "?g=" . $val[0] . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "/images/icons/fld_c.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle>" . $val[1] . "</a>";
		}
		echo "</td><td align=right>" . $val[3] . "</td>";
		echo "</td><td align=right>" . $val[2] . "</td>";

		echo "<td class=tiny nowrap><a href=\"group_copy.php?id=" . $val[0] . "\">" . getLM("copy") . "</a> | <a href=\"group_details.php?id=" . $val[0] . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delg&id=" . $val[0] . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">" . getLM("delete") . "</a></td></tr>";
	}
	echo "</table>";
?>
</td>
<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<?















	$strSQL = "SELECT ksi.modifier.modifier_id, ksi.modifier.name, ksi.modifier.def_price, ksi.modifier.pos, ksi.modifier.weight, dish.name AS dish, ksi.modifier.hotkey_color 
	FROM ksi.modifier 
		LEFT JOIN ksi.dish ON ksi.dish.dish_id = ksi.modifier.dish_id
	WHERE ksi.modifier.group_id = " . $curGroup . " AND ksi.modifier.deleted IS NULL
	ORDER BY ksi.modifier.pos";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);


	echo "<h2>" . ucfirst($curGroupName) . " modifiers</h2>";

	writeActionsBar(array(
		array(getLM("Add new"), "mod_details.php?g=" . $curGroup, "images/icons/add.gif"),
/*		array("Edit", "group_details.php?id=" . $curGroup, "images/icons/edit.gif"),
		array("Delete", $_SERVER["SCRIPT_NAME"] . "?act=delg&id=" . $curGroup, "images/icons/delete.gif"),*/
	));


	echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
	echo "<tr><th>" . getLM("modifier") . "</th><th>" . getLM("entree (whether)") . "</th><th>" . getLM("suggested price") . "</th><th>" . getLM("position in list") . "</th><th>" . getLM("weight") . "</th><th>" . getLM("color") . "</th><th>" . getLM("action") . "</th></tr>";
	while (ocifetch($stmt)) {
		echo "<tr><td>";
//		echo "<a href=\"item_details.php?id=" . ociresult($stmt, "PRODUCT_ID") . "\">" . ociresult($stmt, "NAME") . "</a>";
		echo ociresult($stmt, "NAME");
		echo "</td>
		<td>" . ociresult($stmt, "DISH") . "</td>
		<td align=right>" . (ociresult($stmt, "DEF_PRICE") ? ($GLOBALS["_langId"] == 0 ? "$" : "") . number_format(ociresult($stmt, "DEF_PRICE"),  2, ".", ",") . ($GLOBALS["_langId"] == 1 ? " руб." : "") : "-") . "</td>
		<td align=right>" . ociresult($stmt, "POS") . "</td><td align=right>" . ociresult($stmt, "WEIGHT") . "</td>";

		if (ociresult($stmt, "HOTKEY_COLOR")) {
			echo "<td align=center><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/0.gif\" width=10 height=10 alt=\"#" . ociresult($stmt, "HOTKEY_COLOR") . "\" style=\"border: 1px solid black; background-color: #" . ociresult($stmt, "HOTKEY_COLOR") . "\"></td>";
		}
		else {
			echo "<td >&nbsp;</td>";
		}

		echo "<td class=tiny><a href=\"mod_details.php?id=" . ociresult($stmt, "MODIFIER_ID") . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delm&id=" . ociresult($stmt, "MODIFIER_ID") . "&g=" . $curGroup . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">" . getLM("delete") . "</a></td></tr>";
	}
	ocifreestatement($stmt);

	echo "</table>";

?>
</td>
</table>
<?
}

require($pathToSiteRoot . "common/template.php");
?>