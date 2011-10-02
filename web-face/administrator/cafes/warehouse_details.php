<?
$pathToSiteRoot = "../../";
$appName = "administrator";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = $id ? getLM("Warehouse details") : getLM("Adding a new warehouse");

$basePage = "index.php";

$table = "ksi.warehouse";
$idField = "warehouse_id";
$idNewVal = "ksi.warehouse_seq.nextval";




if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("name", $_POST["name"])
//		array("cafe_id", $_POST["cafe_id"])
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 51);
	}
	else {
		$arrFV[] = array("cafe_id", $_POST["cafe_id"]);
		$arrFV[] = array($idField, $idNewVal);
		/*$id = */insertItem($table, $arrFV, 50);
	}
	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));

}








if ($id) {
	$strSQL = "SELECT * FROM " . $table . " WHERE " . $idField . " = " . $id;
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);
	
	ocifetchinto($stmt, $result, OCI_ASSOC);

	if (!$result) {
		echo "error: record not found";
		return;
	}
}






function writeMainContent() {

	writeFormTableHeader();
	if ($GLOBALS["id"]) 
		writeValRow("id", $GLOBALS["id"]);
		writeSelectSQLRow(getLM("Cafe"), "cafe_id", "SELECT cafe_id, name FROM ksi.cafe WHERE deleted IS NULL ORDER BY LOWER(name)", $GLOBALS["result"]["CAFE_ID"] ? $GLOBALS["result"]["CAFE_ID"] : $_GET["c"], "", ($GLOBALS["result"] ? array("addTagData" => " disabled"): ""));
	writeTextInputRow(getLM("Warehouse"), "name", $GLOBALS["result"]["NAME"]);
	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>