<?
$pathToSiteRoot = "../../";
$appName = "administrator";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = $id ? getLM("Token details") : getLM("Adding a new token");

$basePage = "index.php";

$table = "ksi.token";
$idField = "token_id";
$idNewVal = "ksi.token_seq.nextval";


//$__debuginfo = 1;

if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("name", $_POST["name"])
//		array("cafe_id", $_POST["cafe_id"])
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 100);
	}
	else {
		$arrFV[] = array("cafe_id", $_POST["cafe_id"]);
		$arrFV[] = array($idField, $idNewVal);
		/*$id = */insertItem($table, $arrFV, 99);
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
		writeSelectSQLRow(getLM("Cafe"), "cafe_id", "SELECT cafe_id, name FROM ksi.cafe " . ($GLOBALS["_settings"]["NETWORK_MODE"] == "NETWORK" ? "WHERE type_id IN (2) AND deleted IS NULL" : " WHERE deleted IS NULL") . " ORDER BY LOWER(name)", $GLOBALS["result"]["CAFE_ID"] ? $GLOBALS["result"]["CAFE_ID"] : $_GET["c"], "", ($GLOBALS["result"] ? array("addTagData" => " disabled"): ""));
	writeTextInputRow(getLM("Token"), "name", $GLOBALS["result"]["NAME"]);
	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>