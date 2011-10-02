<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = $id ? getLM("Measure details") : getLM("Adding a new measure");

$basePage = "index.php";

$table = "ksi.measure";
$idField = "measure_id";
$idNewVal = "ksi.measure_seq.nextval";




if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("name", $_POST["name"]),
		array("measure_type_id", $_POST["measure_type_id"]),
		array("shortname", $_POST["shortname"]),
		array("base_units", (float)$_POST["base_units"])
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 42);
	}
	else {
		$arrFV[] = array($idField, $idNewVal);
		/*$id = */insertItem($table, $arrFV, 41);
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
	writeTextInputRow(getLM("Title"), "name", $GLOBALS["result"]["NAME"]);
	writeSelectSQLRow(getLM("Measure type"), "measure_type_id", "SELECT measure_type_id, name FROM ksi.measure_type ORDER BY name", $GLOBALS["result"]["MEASURE_TYPE_ID"] ? $GLOBALS["result"]["MEASURE_TYPE_ID"] : $_GET["g"]);
	writeTextInputRow(getLM("Short name"), "shortname", $GLOBALS["result"]["SHORTNAME"]);
	writeTextInputRow(getLM("Rate to base units"), "base_units", $GLOBALS["result"]["BASE_UNITS"]);
	writeFormTableFooter();



}





require($pathToSiteRoot . "common/template_dtl.php");
?>