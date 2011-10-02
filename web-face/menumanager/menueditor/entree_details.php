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

$pageTitle = $id ? getLM("Entree in menu folder details") : getLM("Adding a new entree to menu folder");

$basePage = "index.php";

$table = "ksi.menu_dish";

//$GLOBALS["__debuginfo"] = 1;




if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("price", (float)$_POST["price"]),
		array("pos", (int)$_POST["pos"] ? $_POST["pos"] : null)
	);


	if ($id) {

		logTranBegin(63);

		$strSQL = "UPDATE " . $table . " SET price = " . (float)$_POST["price"] . ", pos = " . ((int)$_POST["pos"] ? $_POST["pos"] : "NULL") . " WHERE menu_item_id = " . (int)$_GET["f"] . " AND dish_id = " . (int)$_GET["e"];

		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		if (!ociexecute($stmt, OCI_DEFAULT)) die("error updating record");
		ocicommit($GLOBALS["objConn"]);

		logTranEnd();


	}
	else {
		$arrFV[] = array("menu_item_id", (int)$_GET["f"]);
		$arrFV[] = array("dish_id", (int)$_POST["dish_id"]);
		/*$id = */insertItem($table, $arrFV, 62);
	}
	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));

}









$strSQL = "SELECT " . $table . ".*, ksi.menu_item.name AS folder, ksi.menu.name AS menu 
	FROM ksi.menu_item 
	INNER JOIN ksi.menu ON ksi.menu_item.menu_id = ksi.menu.menu_id AND  ksi.menu_item.menu_item_id = " . (int)$_GET["f"] . "
	LEFT JOIN " . $table . " ON " . $table . ".menu_item_id = ksi.menu_item.menu_item_id AND " . $table . ".dish_id = " . $id;


$stmt = ociparse($GLOBALS["objConn"], $strSQL);
ociexecute($stmt, OCI_DEFAULT);

ocifetchinto($stmt, $result, OCI_ASSOC);

if (!$result) {
	echo "error: record not found";
	return;
}




function writeMainContent() {




	writeFormTableHeader();

	writeValRow(getLM("menu"), $GLOBALS["result"]["MENU"]);
	writeValRow(getLM("folder"), $GLOBALS["result"]["FOLDER"]);
	writeDishWithGroupSelect("dish_id", $GLOBALS["result"]["DISH_ID"], $GLOBALS["id"] ? array(0, 0) : array(1, (int)$_GET["f"]));

	writeTextInputRow(getLM("Price") . ", " . getLM("$"), "price", $GLOBALS["result"]["PRICE"]);
	writeTextInputRow(getLM("Position in list"), "pos", $GLOBALS["result"]["POS"]);

	echo "<tr><td colspan=2 align=center>" . getLM("Note: Entrees already added to selected menu folder wont appear in entree dropdown") . "</td></tr>";

	writeFormTableFooter();

?>
<script>
<?
	if ($GLOBALS["id"]) {
?>
document.itemform.dish_id.disabled = true;
document.itemform.dish_id_g.disabled = true;
<?
	}
?>
</script>
<?
}





require($pathToSiteRoot . "common/template_dtl.php");
?>