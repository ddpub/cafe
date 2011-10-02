<?
$pathToSiteRoot = "../../";
$appName = "menumanager";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = $id ? getLM("Menu details") : getLM("Adding a new menu");

$basePage = "index.php";

$table = "ksi.menu";
$idField = "menu_id";
$idNewVal = "ksi.menu_seq.nextval";

//$GLOBALS["__debuginfo"] = true;



if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("name", $_POST["name"]),
		array("description", $_POST["description"]),
		array("cafe_id", (int)$_POST["cafe_id"] ? $_POST["cafe_id"] : null),
		array("pos", (float)$_POST["pos"] ? $_POST["pos"] : null)
	);


	if ($id) {
		if (!updateItem($table, $arrFV, $idField, $id, 56, 0)) {
			$errMsg = "Error updating item, menu with name &ldquo;" . $_POST["name"] . "&rdquo; exists.";
		}
	}
	else {
		$arrFV[] = array($idField, $idNewVal);
		if (!insertItem($table, $arrFV, 55, 0)) {
			$errMsg = "Error inserting item, menu with name &ldquo;" . $_POST["name"] . "&rdquo; exists.";
		}
	}

	if (!$errMsg) header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));

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

	if ($GLOBALS["errMsg"]) {
		echo "<tr><td colspan=2 align=center style=\"color: red; font-weight: bold\">" . $GLOBALS["errMsg"] . "</td></tr>";
	}

	if ($GLOBALS["id"]) 
		writeValRow("id", $GLOBALS["id"]);
	writeTextInputRow(getLM("Menu"), "name", $GLOBALS["result"]["NAME"]);
	writeSelectSQLRow(getLM("Cafe"), "cafe_id", "SELECT cafe_id, name FROM ksi.cafe WHERE " . ($GLOBALS["_settings"]["NETWORK_MODE"] == "NETWORK" ? " type_id IN (2) AND deleted IS NULL" : " deleted IS NULL") . " ORDER BY LOWER(name)", $GLOBALS["id"] ? $GLOBALS["result"]["CAFE_ID"] : (int)$_GET["c"], $nullCaption = "<menu templates>");
	writeTextInputRow(getLM("Position in list"), "pos", $GLOBALS["result"]["POS"]);
	writeTextareaRow(getLM("Description"), "description", $GLOBALS["result"]["DESCRIPTION"]);
	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>