<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);
$table = "ksi.ingredient";
$idField = "ingredient_id";

$basePage = "index.php";



if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("recipe", $_POST["recipe"])
	);



	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 19);
	}
	else {
		die("unexpected error");
	}
	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));

}


$strSQL = "SELECT * FROM " . $table . " WHERE " . $idField . " = " . $id;

$stmt = ociparse($GLOBALS["objConn"], $strSQL);
ociexecute($stmt, OCI_DEFAULT);

ocifetchinto($stmt, $result, OCI_ASSOC);

if (!$result) {
	echo "error: record not found";
	return;
}

$pageTitle = "Edit " . $result["NAME"] . " ingredient recipe";




function writeMainContent() {

	writeFormTableHeader();
	writeTextareaRow(getLM("recipe"), "recipe", $GLOBALS["result"]["RECIPE"]);
	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>