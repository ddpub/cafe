<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["e2"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);
$table = "ksi.subdish";

$basePage = "index.php";



if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("base_dish_id", (int)$_GET["e"]),
		array("dish_id", (int)$_POST["dish_id"]),
		array("count", (int)$_POST["count"])
	);


	if ($id) {

		logTranBegin(30);

		$strSQL = "UPDATE " . $table . " 
			SET count = " . (int)$_POST["count"] . "
			WHERE base_dish_id = " . (int)$_GET["e"] . " AND dish_id = " . (int)$_GET["e2"];
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
//		ocicommit($GLOBALS["objConn"]);

		logTranEnd();

	}
	else {
		insertItem($table, $arrFV, 29);
	}
	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));

}


$strSQL = "SELECT ksi.dish.name AS base_dish_name
		,  d2.name AS dish_name
		, ksi.subdish.*
	FROM ksi.dish 
		LEFT JOIN ksi.subdish ON ksi.subdish.base_dish_id = ksi.dish.dish_id
			AND subdish.dish_id = " . (int)$_GET["e2"] .  "
		LEFT JOIN ksi.dish d2  ON d2.dish_id = ksi.subdish.dish_id
	WHERE ksi.dish.dish_id = " . (int)$_GET["e"];

$stmt = ociparse($GLOBALS["objConn"], $strSQL);
ociexecute($stmt, OCI_DEFAULT);

ocifetchinto($stmt, $result, OCI_ASSOC);

if (!$result) {
	echo "error: record not found";
	return;
}

$pageTitle = $id ? "Details of " . $result["DISH_NAME"] . " entree, included in the " . $result["BASE_DISH_NAME"] . " entree" : "Adding an entree to the " . $result["BASE_DISH_NAME"] . " entree";




class rTree2 extends rTree {

	function passTree($parentID, $level, &$str) {
		if ($GLOBALS["id"] && $GLOBALS["id"] == $parentID) {
			return;
		}
		for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
			$str .= "<option value=\"" . $this->arrIdx[$parentID][$i] . "\">" . str_repeat("&nbsp;&nbsp;&nbsp;&nbsp;", $level) . $this->arrTree[$this->arrIdx[$parentID][$i]][2] . "</option>";

			$this->passTree($this->arrIdx[$parentID][$i], $level+1, &$str);
		}
	}
}




function writeMainContent() {

	writeFormTableHeader();
	writeValRow(getLM("Entree"), $GLOBALS["result"]["BASE_DISH_NAME"]);




	$strSQL = "SELECT ksi.dish_group.group_id
			, ksi.dish_group.name AS dish_group
			, ksi.dish.dish_id
			, ksi.dish.name AS dish

		FROM ksi.dish_group 
			INNER JOIN ksi.dish ON ksi.dish.group_id = ksi.dish_group.group_id
" . ((int)$_GET["e2"] ? " WHERE /*ksi.dish.dish_id <> " . (int)$_GET["e"] . "*/" : "
			LEFT JOIN ksi.subdish ON ksi.subdish.dish_id = ksi.dish.dish_id AND ksi.subdish.base_dish_id = " . (int)$_GET["e"] . " 
		WHERE ksi.subdish.dish_id IS NULL AND ksi.dish.dish_id <> " . (int)$_GET["e"] . " AND "  ) . " dish.deleted IS NULL
		ORDER BY ksi.dish_group.name, ksi.dish.name";


	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	$curGroup = 0;
	$str1 = "";
	$str2 = "";
	while (ocifetchinto($stmt, $row, OCI_ASSOC)) {
/*		if ($curGroup != $row["GROUP_ID"]) {
			$str1 .= "<option value=" . $row["GROUP_ID"] . ">" . $row["DISH_GROUP"] . "</option>";
			$curGroup = $row["GROUP_ID"];
		}*/
		if ($row["DISH_ID"] == $GLOBALS["result"]["DISH_ID"]) {
			$initGroup = $row["GROUP_ID"];
		}
		$str2 .= "arrDsh[arrDsh.length] = new Array(" . $row["GROUP_ID"] . ", " . $row["DISH_ID"] . ", '" . addslashes($row["DISH"]) . "');\n";
	}
	ocifreestatement($stmt);

	$str = "";
	$tree = new rTree2("SELECT group_id, parent_id, name FROM ksi.dish_group ORDER BY NAME", $GLOBALS["objConn"]);

	$tree->passTree(0, 0, $str);

	writeValRow(getLM("Entree group"), "<select name=\"dish_id_g\" onchange=\"fillDishC()\">" . $str . "</select>");

?>
<tr>
	<td align=right><? echo getLM("Entree");?>: </td>
	<td><select name="dish_id"></select></td>
</tr>
<script>
var arrDsh = new Array();
<? echo $str2; ?>

function fillDishC() {
	var val = document.itemform.dish_id_g.options[document.itemform.dish_id_g.selectedIndex].value;
	var obj = document.itemform.dish_id.options;

	for (i = obj.length; i > 0; i--) {
		obj[i - 1] = null;
	}

	for (i = 0; i < arrDsh.length; i++) {
		if (arrDsh[i][0] == val) {
			obj[obj.length] = new Option(arrDsh[i][2], arrDsh[i][1]);
			if (arrDsh[i][1] == <? echo (int)$GLOBALS["result"]["DISH_ID"]?>) {
				obj[obj.length - 1].selected = true;
			}
		}
	}
	if (document.itemform.dish_id.onchange) {
		document.itemform.dish_id.onchange();
	}
}


for (i = 0; i < document.itemform.dish_id_g.options.length; i++) {
	if (document.itemform.dish_id_g.options[i].value == <? echo (int)$initGroup; ?>) {
		document.itemform.dish_id_g.options[i].selected = true;
		break;
	}
}



fillDishC();


document.itemform.onsubmit = checkForm;

function checkForm() {
	if (document.itemform.dish_id.options.length) {
		return true;
	}
	else {
		alert("You must specify the entree.");
		return false;
	}
}
<?
	if ($GLOBALS["id"]) {
?>
document.itemform.dish_id.disabled = true;
document.itemform.dish_id_g.disabled = true;
<?
	}
?>
</script>
<?












/*	echo "<tr>";
	echo "<td align=right>Count: </td><td>";
	writeTextInput("count", $GLOBALS["result"]["COUNT"], $arrAddParams = array("addTagData" => " style=\"text-align: right; \""));

	echo "</td></tr>";*/
	writeTextInputRow(getLM("quantity"), "count", $GLOBALS["result"]["COUNT"], $arrAddParams = array("addTagData" => " style=\"text-align: right; \""));

?>
<tr>
	<td align=right valign=top><? echo getLM("Note")?>: </td>
	<td><? echo getLM("If you don't see an entree in the list,<br>it means that the missing entree is already added.");?></td>
</tr>
<?
	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>