<?
$pathToSiteRoot = "../../";
$appName = "menumanager";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["e"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$basePage = "index.php";

//$GLOBALS["__debuginfo"] = 1;




if ($_POST["submit"] == getLM("save")) {


	logTranBegin(71);

	$strSQL = "UPDATE ksi.menu_dish_modifier SET price = " . (float)$_POST["price"] . ", active = " . (int)($_POST["active"] != "") . " WHERE dish_id = " . (int)$_GET["e"] . " AND menu_item_id = " . (int)$_GET["f"] . " AND modifier_id = " . (int)$_GET["md"];

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!ociexecute($stmt, OCI_DEFAULT)) die("error updating record");
	ocicommit($GLOBALS["objConn"]);

	logTranEnd();

	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));

}








$strSQL = "SELECT ksi.modifier.modifier_id
		, ksi.modifier.name
		, ksi.mod_group.name AS modgroup
		, ksi.menu_dish_modifier.price
		, ksi.menu_dish_modifier.active
	FROM ksi.modifier 
		INNER JOIN ksi.mod_group ON ksi.mod_group.group_id = ksi.modifier.group_id
		INNER JOIN ksi.menu_dish_modifier ON ksi.menu_dish_modifier.modifier_id = ksi.modifier.modifier_id 
			AND ksi.menu_dish_modifier.dish_id = " . (int)$_GET["e"] . " 
			AND ksi.menu_dish_modifier.menu_item_id = " . (int)$_GET["f"] . "
	WHERE modifier_id = " . (int)$_GET["md"] . "
	ORDER BY ksi.mod_group.name, ksi.modifier.pos";



$stmt = ociparse($GLOBALS["objConn"], $strSQL);
ociexecute($stmt, OCI_DEFAULT);

ocifetchinto($stmt, $result, OCI_ASSOC);

if (!$result) {
	echo "error: record not found";
	return;
}



$pageTitle =  ucfirst($result["MODGROUP"] . " / " . $result["NAME"]) . " " . getLM("assigned modifier details");









function writeMainContent() {




	writeFormTableHeader();


	writeValRow(getLM("Modifier"), ucfirst($GLOBALS["result"]["MODGROUP"] . " / " . $GLOBALS["result"]["NAME"]));
	writeCheckboxRow(getLM("Active"), "active", $GLOBALS["result"]["ACTIVE"], $arrAddParams = array());
	writeTextInputRow(getLM("Price") . ", " . getLM("$"), "price", $GLOBALS["result"]["PRICE"]);

	writeFormTableFooter();

}





require($pathToSiteRoot . "common/template_dtl.php");
?>