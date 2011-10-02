<?
$pathToSiteRoot = "../../";
$appName = "administrator";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = $id ? getLM("Table details") : getLM("Adding a new table");

$basePage = "index.php";

$table = "ksi.tables";
$idField = "table_id";
$idNewVal = "ksi.tables_seq.nextval";


//$__debuginfo = 1;

if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("name", $_POST["name"]),
		array("table_group_id", $_POST["table_group_id"])
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 135);
	}
	else {
//		$arrFV[] = array("cafe_id", $_POST["cafe_id"]);
		$arrFV[] = array($idField, $idNewVal);
		/*$id = */insertItem($table, $arrFV, 134);
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

	writeSelectSQLRow(getLM("Cafe"), "cafe_id", "SELECT cafe_id, name FROM ksi.cafe WHERE type_id IN (1, 2) ORDER BY LOWER(name)", $GLOBALS["result"]["CAFE_ID"] ? $GLOBALS["result"]["CAFE_ID"] : $_GET["c"], "", ($GLOBALS["result"] ? array("addTagData" => " disabled"): ""));
	writeSelectSQLRow(getLM("Table group"), "table_group_id", "SELECT table_group_id, name FROM ksi.table_group WHERE cafe_id = " .  $_GET["c"] . " ORDER BY LOWER(name)", ($GLOBALS["result"]["TABLE_GROUP_ID"] ? $GLOBALS["result"]["TABLE_GROUP"] : $_GET["g"]), "");
	writeTextInputRow(getLM("Table"), "name", $GLOBALS["result"]["NAME"]);
	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>