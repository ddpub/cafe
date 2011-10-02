<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = $id ? getLM("Ingredient group details") : getLM("Adding a new ingredient group");

$basePage = "index.php";

$table = "ksi.ingredient_group";
$idField = "group_id";
$idNewVal = "ksi.ingredient_group_seq.nextval";




if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("name", $_POST["name"]),
		array("pos", (float)$_POST["pos"] ? $_POST["pos"]: null),
		array("parent_id", (int)$_POST["parent_id"] ? $_POST["parent_id"] : null)
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 8);
	}
	else {
		$arrFV[] = array($idField, $idNewVal);
		/*$id = */insertItem($table, $arrFV, 7);
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







class rTree2 extends rTree {

	function passTree($parentID, $level, &$str) {
		if ($GLOBALS["id"] && $GLOBALS["id"] == $parentID) {
			return;
		}
		for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
			$str .= "<option value=\"" . $this->arrIdx[$parentID][$i] . "\"" . (($GLOBALS["id"] ? $GLOBALS["result"]["PARENT_ID"] : $_GET["p"]) == $this->arrIdx[$parentID][$i] ? " selected" : "") . ">" . str_repeat("&nbsp;&nbsp;&nbsp;&nbsp;", $level) . $this->arrTree[$this->arrIdx[$parentID][$i]][2] . "</option>";

			$this->passTree($this->arrIdx[$parentID][$i], $level+1, &$str);
		}
	}
}




function writeMainContent() {

	writeFormTableHeader();
	if ($GLOBALS["id"]) 
		writeValRow("id", $GLOBALS["id"]);
	writeTextInputRow(getLM("title"), "name", $GLOBALS["result"]["NAME"]);
	writeTextInputRow(getLM("position"), "pos", $GLOBALS["result"]["POS"]);
	
	$str = "";
	$tree = new rTree2("SELECT group_id, parent_id, name FROM ksi.ingredient_group WHERE group_id <> " . (int)$GLOBALS["id"] . " ORDER BY pos, name", $GLOBALS["objConn"]);

	$tree->passTree(0, 0, $str);

	writeValRow(getLM("parent group"), "<select name=\"parent_id\"><option value=\"0\">&lt;root&gt;</option>" . $str . "</select>");



	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>