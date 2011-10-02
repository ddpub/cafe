<?
$pathToSiteRoot = "../../";
$appName = "menumanager";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = $id ? getLM("Menu folder details") : getLM("Adding a new menu folder");

$basePage = "index.php";

$table = "ksi.menu_item";
$idField = "menu_item_id";
$idNewVal = "ksi.menu_item_seq.nextval";




if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("name", $_POST["name"]),
		array("pos", (float)$_POST["pos"] ? $_POST["pos"] : null),
		array("parent_id", (int)$_POST["parent_id"] ? $_POST["parent_id"] : null),
		array("description", $_POST["description"]),
		array("color", $_POST["color"])
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 60);
	}
	else {
		$arrFV[] = array("menu_id", (int)$_GET["m"]);
		$arrFV[] = array($idField, $idNewVal);
		/*$id = */insertItem($table, $arrFV, 59);
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
	global $arrHotkeyColor;



	writeFormTableHeader();
	if ($GLOBALS["id"]) 
		writeValRow("id", $GLOBALS["id"]);

	$str = "";
	$tree = new rTree2("SELECT menu_item_id, parent_id, name FROM ksi.menu_item WHERE menu_id = " . ($GLOBALS["id"] ? $GLOBALS["result"]["MENU_ID"] : (int)$_GET["m"]) . " ORDER BY pos", $GLOBALS["objConn"]);

	$tree->passTree(0, 0, $str);

	writeValRow(getLM("Parent folder"), "<select name=\"parent_id\"><option value=\"0\">&lt;root&gt;</option>" . $str . "</select>");


	writeTextInputRow(getLM("Folder title"), "name", $GLOBALS["result"]["NAME"]);
	writeTextInputRow(getLM("Position in list"), "pos", $GLOBALS["result"]["POS"]);
	writeTextareaRow(getLM("Description"), "description", $GLOBALS["result"]["DESCRIPTION"]);

	$str = "<select name=\"color\">";
	foreach ($arrHotkeyColor as $val) {
		$str .= "<option value=\"" . $val[0] . "\"" . ($GLOBALS["result"]["COLOR"] == $val[0] ? " selected" : "") . " style=\"background-color:#" . $val[0] . "; color: #" . ((hexdec(substr($val[0] . "000000", 0, 2)) + hexdec(substr($val[0] . "000000", 2, 2)) + hexdec(substr($val[0] . "000000", 4, 2))) / 3 < 128 ? "FFFFFF" : "000000") .  "\">" . $val[1] . "</option>";
	}
	$str .= "</select>";
//	writeValRow("Hotkey color", $str);
	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>