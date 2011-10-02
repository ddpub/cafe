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

$pageTitle = $id ? getLM("Entree details") : getLM("Adding a new entree");

$basePage = "index.php";

$table = "ksi.dish";
$idField = "dish_id";
$idNewVal = "ksi.dish_seq.nextval";




if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("name", $_POST["name"]),
		array("group_id", $_POST["group_id"]),
		array("description", $_POST["description"]),
		array("def_price", $_POST["def_price"]),
		array("is_beverage", $_POST["is_beverage"]),
		array("hotkey_color", $_POST["hotkey_color"]),
		array("code", $_POST["code"]),
		array("recipe_number", $_POST["recipe_number"]),
		array("employee_meal", (int)($_POST["employee_meal"] != ""))
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 24);
	}
	else {
		$arrFV[] = array($idField, $idNewVal);
		/*$id = */insertItem($table, $arrFV, 23);
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
			$str .= "<option value=\"" . $this->arrIdx[$parentID][$i] . "\"" . (($GLOBALS["id"] ? $GLOBALS["result"]["GROUP_ID"] : $_GET["g"]) == $this->arrIdx[$parentID][$i] ? " selected" : "") . ">" . str_repeat("&nbsp;&nbsp;&nbsp;&nbsp;", $level) . $this->arrTree[$this->arrIdx[$parentID][$i]][2] . "</option>";

			$this->passTree($this->arrIdx[$parentID][$i], $level+1, &$str);
		}
	}
}







function writeMainContent() {
	global $arrHotkeyColor;


	$tree = new rTree2("SELECT group_id, parent_id, name FROM ksi.dish_group ORDER BY NAME", $GLOBALS["objConn"]);

	$tree->passTree(0, 0, $str);



	writeFormTableHeader();
	if ($GLOBALS["id"]) 
		writeValRow("id", $GLOBALS["id"]);
	writeTextInputRow(getLM("Entree"), "name", $GLOBALS["result"]["NAME"]);

//	writeSelectSQLRow(getLM("Entree group"), "group_id", "SELECT group_id, name FROM ksi.dish_group ORDER BY name", $GLOBALS["result"]["GROUP_ID"] ? $GLOBALS["result"]["GROUP_ID"] : $_GET["g"]);

	writeValRow(getLM("entree group"), "<select name=\"group_id\">" . $str . "</select>");
	
//	writeValRow("Entree type", "<select name=\"is_beverage\"><option value=0>food</option><option value=1" . ($GLOBALS["result"]["IS_BEVERAGE"] ? " selected" : "") . ">beverage</option></select>");

	writeSelectSQLRow(getLM("Where is prepared"), "is_beverage", "SELECT kt_id, name FROM ksi.kitchen_type ORDER BY name", $GLOBALS["result"]["IS_BEVERAGE"] ? $GLOBALS["result"]["IS_BEVERAGE"] : 0);

	writeTextInputRow(getLM("Entree code"), "code", $GLOBALS["result"]["CODE"]);
	writeTextInputRow(getLM("Suggested price") . ", " . getLM("$"), "def_price", $GLOBALS["result"]["DEF_PRICE"]);
	writeTextInputRow(getLM("Recipe number"), "recipe_number", $GLOBALS["result"]["RECIPE_NUMBER"]);
	writeTextareaRow(getLM("Description"), "description", $GLOBALS["result"]["DESCRIPTION"]);
	writeCheckboxRow(getLM("This entree can be in employee meal"), "employee_meal", $GLOBALS["result"]["EMPLOYEE_MEAL"]);



	$str = "<select name=\"hotkey color\">";
	foreach ($arrHotkeyColor as $val) {
		$str .= "<option value=\"" . $val[0] . "\"" . ($GLOBALS["result"]["HOTKEY_COLOR"] == $val[0] ? " selected" : "") . " style=\"background-color:#" . $val[0] . "; color: #" . ((hexdec(substr($val[0] . "000000", 0, 2)) + hexdec(substr($val[0] . "000000", 2, 2)) + hexdec(substr($val[0] . "000000", 4, 2))) / 3 < 128 ? "FFFFFF" : "000000") .  "\">" . $val[1] . "</option>";
	}
	$str .= "</select>";
	writeValRow(getLM("Hotkey color"), $str);

	writeFormTableFooter();

}





require($pathToSiteRoot . "common/template_dtl.php");
?>