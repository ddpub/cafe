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

$pageTitle = $id ? getLM("Item details") : getLM("Adding a new item");

$basePage = "index.php";

$table = "ksi.product";
$idField = "product_id";
$idNewVal = "ksi.product_seq.nextval";




if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("name", $_POST["name"]),
		array("group_id", $_POST["group_id"]),
		array("measure_type_id", $_POST["measure_type_id"]),
		array("default_measure_id", $_POST["default_measure_id"]),
		array("pos", (float)$_POST["pos"] ? $_POST["pos"]: null),
		array("default_price", (float)$_POST["default_price"])
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 5);
	}
	else {
		$arrFV[] = array($idField, $idNewVal);
		/*$id = */insertItem($table, $arrFV, 4);
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


	$tree = new rTree2("SELECT group_id, parent_id, name FROM ksi.product_group ORDER BY pos, name", $GLOBALS["objConn"]);

	$tree->passTree(0, 0, $str);


	writeFormTableHeader();
	if ($GLOBALS["id"]) 
		writeValRow("id", $GLOBALS["id"]);
	writeTextInputRow(getLM("title"), "name", $GLOBALS["result"]["NAME"]);

	writeValRow(getLM("item group"), "<select name=\"group_id\">" . $str . "</select>");

	writeSelectSQLRow(getLM("Measure type"), "measure_type_id", "SELECT measure_type_id, name FROM ksi.measure_type ORDER BY name", $GLOBALS["result"]["MEASURE_TYPE_ID"], "", array("addTagData" => "onchange=\"fill_dm();\""));
	writeValRow(getLM("default measure"), "<select name=\"default_measure_id\"></select>");
	writeTextInputRow(getLM("default price"), "default_price", $GLOBALS["result"]["DEFAULT_PRICE"]);
	writeTextInputRow(getLM("position"), "pos", $GLOBALS["result"]["POS"]);
	writeFormTableFooter();


?>
<script>
arrM = new Array();
<?
	$strSQL = "SELECT * FROM ksi.measure ORDER BY measure_type_id, base_units";
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);
	while (ocifetch($stmt)) {
		echo "arrM[arrM.length] = new Array(" . ociresult($stmt, "MEASURE_TYPE_ID") . ", " . ociresult($stmt, "MEASURE_ID") . ", '" . addslashes(ociresult($stmt, "NAME")) . "');\n";
	}
	ocifreestatement($stmt);

	echo "initval = " . (int)$GLOBALS["result"]["DEFAULT_MEASURE_ID"] .  ";";
?>

function fill_dm() {
	for (i = document.itemform.default_measure_id.length; i > 0; i--) {
		document.itemform.default_measure_id[i - 1] = null;
	}

	for (i = 0; i < arrM.length; i++) {
		if (arrM[i][0] == document.itemform.measure_type_id[document.itemform.measure_type_id.selectedIndex].value) {
			document.itemform.default_measure_id[document.itemform.default_measure_id.length] = new Option(arrM[i][2], arrM[i][1]);
			if (initval == arrM[i][1]) {
				document.itemform.default_measure_id[document.itemform.default_measure_id.length - 1].selected = true;
			}
		}
	}
}

document.body.onload = fill_dm;
fill_dm();
</script>
<?

}





require($pathToSiteRoot . "common/template_dtl.php");
?>