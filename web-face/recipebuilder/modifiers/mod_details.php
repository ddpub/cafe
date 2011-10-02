<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = $id ? getLM("Modifier details") : getLM("Adding a new modifier");

$basePage = "index.php";

$table = "ksi.modifier";
$idField = "modifier_id";
$idNewVal = "ksi.modifier_seq.nextval";



if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("name", $_POST["name"]),
		array("dish_id", (int)$_POST["dish_id"] ? $_POST["dish_id"] : null),
		array("group_id", $_POST["group_id"]),
		array("def_price", (float)$_POST["def_price"]),
		array("pos", (float)$_POST["pos"]),
		array("weight", (float)$_POST["weight"]),
		array("hotkey_color", $_POST["hotkey_color"])
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 39);
	}
	else {
		$arrFV[] = array($idField, $idNewVal);
		/*$id = */insertItem($table, $arrFV, 38);
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
	writeSelectSQLRow(getLM("Modifier group"), "group_id", "SELECT group_id, name FROM ksi.mod_group WHERE deleted IS NULL ORDER BY name", $GLOBALS["result"]["GROUP_ID"] ? $GLOBALS["result"]["GROUP_ID"] : $_GET["g"]);


	writeValRow(getLM("Modifier is an entree"), "<input type=checkbox name=\"ch1\" onclick=\"ch1click(this.checked)\"" . ($GLOBALS["result"]["DISH_ID"] ? " checked" : "") . "><span class=tiny>(" . getLM("Means whether an entree selected below will be writed off the kitchen when this modifier is ordered") . ")</span>");
	writeDishWithGroupSelect("dish_id", $GLOBALS["result"]["DISH_ID"], array(), "ch1");

	writeTextInputRow(getLM("Suggested price") . ", " . getLM("$"), "def_price", $GLOBALS["result"]["DEF_PRICE"]);
	writeTextInputRow(getLM("Position in list"), "pos", $GLOBALS["result"]["POS"]);
	writeTextInputRow(getLM("Abstract weight"), "weight", $GLOBALS["result"]["WEIGHT"]);

	$str = "<select name=\"hotkey color\">";
	foreach ($GLOBALS["arrHotkeyColor"] as $val) {
		$str .= "<option value=\"" . $val[0] . "\"" . ($GLOBALS["result"]["HOTKEY_COLOR"] == $val[0] ? " selected" : "") . " style=\"background-color:#" . $val[0] . "; color: #" . ((hexdec(substr($val[0] . "000000", 0, 2)) + hexdec(substr($val[0] . "000000", 2, 2)) + hexdec(substr($val[0] . "000000", 4, 2))) / 3 < 128 ? "FFFFFF" : "000000") .  "\">" . $val[1] . "</option>";
	}
	$str .= "</select>";
	writeValRow(getLM("Hotkey color"), $str);

	writeFormTableFooter();

?>
<script>
function ch1click(val) {
	var frm = document.itemform;
	
	frm.dish_id_g.disabled = !val;
	frm.dish_id.disabled   = !val;
	document.getElementById('dish_id_g_tr').style.display = val ? 'block' : 'none';
	document.getElementById('dish_id_tr').style.display = val ? 'block' : 'none';
}

ch1click(document.itemform.ch1.checked);
</script>
<?

}





require($pathToSiteRoot . "common/template_dtl.php");
?>