<?
$pathToSiteRoot = "../../";
$appName = "administrator";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = $id ? getLM("Supplier details") : getLM("Adding a new supplier");

$basePage = "index.php";

$table = "ksi.supplier";
$idField = "supplier_id";
$idNewVal = "ksi.supplier_seq.nextval";




if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("name", $_POST["name"]),
		array("phone", $_POST["phone"]),
		array("email", $_POST["email"]),
		array("description", $_POST["description"])
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 45);
	}
	else {
		$arrFV[] = array($idField, $idNewVal);
		/*$id = */insertItem($table, $arrFV, 44);
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
	writeTextInputRow(getLM("Supplier"), "name", $GLOBALS["result"]["NAME"]);
	writeTextInputRow(getLM("Phone"), "phone", $GLOBALS["result"]["PHONE"]);
	writeTextInputRow("Email", "email", $GLOBALS["result"]["EMAIL"]);
	writeTextareaRow(getLM("Memo"), "description", $GLOBALS["result"]["DESCRIPTION"]);
	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>