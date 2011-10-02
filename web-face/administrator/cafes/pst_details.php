<?
$pathToSiteRoot = "../../";
$appName = "administrator";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = $id ? getLM("Personnel schedule type details") : getLM("Adding a new personnel schedule type");

$basePage = "index.php";

$table = "ksi.cafe_schedule_type";
$idField = "type_id";
$idNewVal = "ksi.cafe_sch_type_seq.nextval";




if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("title", $_POST["title"])
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 161);
	}
	else {
		$arrFV[] = array($idField, $idNewVal);
		/*$id = */insertItem($table, $arrFV, 160);
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
	writeTextInputRow(getLM("Personnel schedule type"), "title", $GLOBALS["result"]["TITLE"]);
	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>