<?
$pathToSiteRoot = "../../";
$appName = "administrator";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = $id ? getLM("Terminal details") : getLM("Adding a new terminal");

$basePage = "index.php";

$table = "ksi.terminal";
$idField = "terminal_id";
$idNewVal = "ksi.terminal_seq.nextval";




if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("name", $_POST["name"]),
		array("mac", $_POST["mac"]),
		array("description", $_POST["description"])
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 80);
	}
	else {
		$arrFV[] = array("cafe_id", $_POST["cafe_id"]);
		$arrFV[] = array($idField, $idNewVal);
		/*$id = */insertItem($table, $arrFV, 78);
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

	writeSelectSQLRow(getLM("Cafe"), "cafe_id", "SELECT cafe_id, name FROM ksi.cafe " . 
	($GLOBALS["_settings"]["NETWORK_MODE"] == "NETWORK" 
		? "WHERE type_id IN (2) AND deleted IS NULL" 
		: " WHERE deleted IS NULL"
	) . " ORDER BY LOWER(name)", 
	($GLOBALS["result"]["CAFE_ID"] 
		? $GLOBALS["result"]["CAFE_ID"] 
		: $_GET["c"]
	), "", ($GLOBALS["result"] ? array("addTagData" => " disabled"): ""));

	writeTextInputRow(getLM("Terminal"), "name", $GLOBALS["result"]["NAME"]);
	writeTextInputRow(getLM("MAC address"), "mac", $GLOBALS["result"]["MAC"]);
	writeTextareaRow(getLM("Memo"), "description", $GLOBALS["result"]["DESCRIPTION"]);
	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>