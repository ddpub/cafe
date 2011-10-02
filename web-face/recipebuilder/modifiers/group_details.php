<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = $id ? getLM("Modifier group details") : getLM("Adding a new modifier group");

$basePage = "index.php";

$table = "ksi.mod_group";
$idField = "group_id";
$idNewVal = "ksi.mod_group_seq.nextval";


//$__debuginfo = true;

if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("name", $_POST["name"]),
		array("pos", (float)$_POST["pos"]),
		array("max_weight", (int)$_POST["max_weight"])
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 36);
	}
	else {
		$arrFV[] = array($idField, $idNewVal);
		/*$id = */insertItem($table, $arrFV, 35);
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
	writeTextInputRow(getLM("Modifier group title"), "name", $GLOBALS["result"]["NAME"]);
	writeTextInputRow(getLM("Position in list"), "pos", $GLOBALS["result"]["POS"]);
	writeTextInputRow(getLM("Max. abstract weight of modifiers to be selected"), "max_weight", $GLOBALS["result"]["MAX_WEIGHT"]);
	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>