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

$pageTitle = $id ? getLM("Tare details") : getLM("Adding a new tare");

$basePage = "index.php";

$table = "ksi.tare";
$idField = "tare_id";
$idNewVal = "ksi.tare_seq.nextval";




if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("title", $_POST["title"]),
		array("group_id", $_POST["group_id"]),
		array("weight", (float)$_POST["weight"]),
		array("weight_measure_id", $_POST["weight_measure_id"]),
		array("capacity", (float)$_POST["capacity"]),
		array("capacity_measure_id", $_POST["capacity_measure_id"])
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 170);
	}
	else {
		$arrFV[] = array($idField, $idNewVal);
		/*$id = */insertItem($table, $arrFV, 169);
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
			$str .= "<option value=\"" . $this->arrIdx[$parentID][$i] . "\"" . (($GLOBALS["id"] ? $GLOBALS["result"]["GROUP_ID"] : $_GET["tg"]) == $this->arrIdx[$parentID][$i] ? " selected" : "") . ">" . str_repeat("&nbsp;&nbsp;&nbsp;&nbsp;", $level) . $this->arrTree[$this->arrIdx[$parentID][$i]][2] . "</option>";

			$this->passTree($this->arrIdx[$parentID][$i], $level+1, &$str);
		}
	}
}




function writeMainContent() {


	$tree = new rTree2("SELECT group_id, parent_id, title FROM ksi.tare_group ORDER BY title", $GLOBALS["objConn"]);

	$tree->passTree(0, 0, $str);


	writeFormTableHeader();
	if ($GLOBALS["id"]) 
		writeValRow("id", $GLOBALS["id"]);
	writeTextInputRow(getLM("name"), "title", $GLOBALS["result"]["TITLE"]);

	writeValRow(getLM("tare group"), "<select name=\"group_id\">" . $str . "</select>");

	echo "<tr>";
	echo "<td align=right>" . getLM("empty tare weight") . ": </td><td>";
	writeTextInput("weight", $GLOBALS["result"]["WEIGHT"], $arrAddParams = array("addTagData" => " style=\"text-align: right; width: 100px\""));
	echo " <select name=\"weight_measure_id\" style=\"width: 50px\">";

	$strSQL = "SELECT ksi.measure.measure_id
			, ksi.measure.shortname
		FROM ksi.measure 
			WHERE measure_type_id = 4
		ORDER BY ksi.measure.base_units";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	while (ocifetchinto($stmt, $row, OCI_ASSOC)) {
		echo "<option value=\"" . $row["MEASURE_ID"] . "\"" . ($row["MEASURE_ID"] == $GLOBALS["result"]["WEIGHT_MEASURE_ID"] ? " selected" : "") . ">" . $row["SHORTNAME"] . "</option>";
	}
	ocifreestatement($stmt);
	echo "</select>";


	writeSelectSQLRow(getLM("tare capacity measure type"), "measure_type_id", "SELECT measure_type_id, name FROM ksi.measure_type ORDER BY name", $GLOBALS["result"]["MEASURE_TYPE_ID"], "", array("addTagData" => "onchange=\"fill_dm();\""));
	writeValRow(getLM("capacity"), "<input type=text name=\"capacity\" value=\"" . $GLOBALS["result"]["CAPACITY"] . "\" style=\"text-align: right; width: 100px\"> <select name=\"capacity_measure_id\" style=\"width: 50px\"></select>");

	writeFormTableFooter();


?>
<script>
arrM = new Array();
<?
	$strSQL = "SELECT * FROM ksi.measure ORDER BY measure_type_id, base_units";
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);
	while (ocifetch($stmt)) {
		echo "arrM[arrM.length] = new Array(" . ociresult($stmt, "MEASURE_TYPE_ID") . ", " . ociresult($stmt, "MEASURE_ID") . ", '" . addslashes(ociresult($stmt, "SHORTNAME")) . "');\n";
	}
	ocifreestatement($stmt);

	echo "initval = " . (int)$GLOBALS["result"]["CAPACITY_MEASURE_ID"] .  ";";
?>

function fill_dm() {
	for (i = document.itemform.capacity_measure_id.length; i > 0; i--) {
		document.itemform.capacity_measure_id[i - 1] = null;
	}

	for (i = 0; i < arrM.length; i++) {
		if (arrM[i][0] == document.itemform.measure_type_id[document.itemform.measure_type_id.selectedIndex].value) {
			document.itemform.capacity_measure_id[document.itemform.capacity_measure_id.length] = new Option(arrM[i][2], arrM[i][1]);
			if (initval == arrM[i][1]) {
				document.itemform.capacity_measure_id[document.itemform.capacity_measure_id.length - 1].selected = true;
			}
		}
	}
}

document.body.onload = fill_dm;
</script>
<?

}





require($pathToSiteRoot . "common/template_dtl.php");
?>