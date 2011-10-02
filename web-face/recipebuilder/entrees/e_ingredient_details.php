<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["di"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);
$table = "ksi.dish_ingredient";
$idField = "dish_ingredient_id";
$idNewVal = "ksi.dish_ingredient_seq.nextval";

$basePage = "index.php";



if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("measure_id", (int)$_POST["measure_id"]),
		array("loss_c_percent", (float)$_POST["loss_c_percent"]),
		array("loss_h_percent", (float)$_POST["loss_h_percent"]),
		array("amount", (float)$_POST["amount"])
	);


	if ($id) {
/*
		logTranBegin(27);

		$strSQL = "UPDATE " . $table . " 
			SET amount = " . (float)$_POST["amount"] . ", measure_id = " . (int)$_POST["measure_id"] . ", loss_c_percent = " . (float)$_POST["loss_c_percent"] . ", loss_h_percent = " . (float)$_POST["loss_h_percent"] . "
			WHERE dish_id = " . (int)$_GET["e"] . " AND ingredient_id = " . (int)$_GET["i"];
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
		ocicommit($GLOBALS["objConn"]);

		logTranEnd();
*/
		updateItem($table, $arrFV, $idField, $id, 27);

	}
	else {
		$arrFV[] = array($idField, $idNewVal);
		$arrFV[] = array("dish_id", (int)$_GET["e"]);
		$arrFV[] = array("ingredient_id", (int)$_POST["ingredient_id"]);

		insertItem($table, $arrFV, 26);
	}
	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));

}


if ($id) {
	$strSQL = "SELECT ksi.dish.name AS dish_name
			,  ingredient.name AS ingredient_name
			, ksi.dish_ingredient.*
		FROM ksi.dish 
			LEFT JOIN ksi.dish_ingredient ON ksi.dish_ingredient.dish_id = ksi.dish.dish_id
			LEFT JOIN ksi.ingredient  ON ksi.ingredient.ingredient_id = ksi.dish_ingredient.ingredient_id
		WHERE ksi.dish_ingredient.dish_ingredient_id = " . (int)$_GET["di"];
	
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);
	
	ocifetchinto($stmt, $result, OCI_ASSOC);
	
	if (!$result) {
		echo "error: record not found";
		return;
	}
}
else {
		$strSQL = "SELECT ksi.dish.name AS dish_name
		FROM ksi.dish 
		WHERE ksi.dish.dish_id = " . (int)$_GET["e"];
	
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);
	
	ocifetchinto($stmt, $result, OCI_ASSOC);
	
	if (!$result) {
		echo "error: entree not found";
		return;
	}

}

$pageTitle = $id ? "Details of " . $result["INGREDIENT_NAME"] . " ingredient, included in the " . $result["DISH_NAME"] . " entree" : "Adding an ingredient to the " . $result["DISH_NAME"] . " entree";






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
	writeValRow(getLM("Entree"), $GLOBALS["result"]["DISH_NAME"]);




	$strSQL = "SELECT ksi.ingredient_group.group_id
			, ksi.ingredient_group.name AS ingredient_group
			, ksi.ingredient.ingredient_id
			, ksi.ingredient.name AS ingredient
			, ksi.measure.measure_type_id
		FROM ksi.ingredient_group 
			INNER JOIN ksi.ingredient ON ksi.ingredient.group_id = ksi.ingredient_group.group_id
			INNER JOIN ksi.measure ON ksi.measure.measure_id = ksi.ingredient.measure_id
		ORDER BY ksi.ingredient_group.name, ksi.ingredient.name";


	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	$curGroup = 0;
	$str1 = "";
	$str2 = "";
	while (ocifetchinto($stmt, $row, OCI_ASSOC)) {
/*		if ($curGroup != $row["GROUP_ID"]) {
			$str1 .= "<option value=" . $row["GROUP_ID"] . ">" . $row["INGREDIENT_GROUP"] . "</option>";
			$curGroup = $row["GROUP_ID"];
		}*/
		if ($row["INGREDIENT_ID"] == $GLOBALS["result"]["INGREDIENT_ID"]) {
			$initGroup = $row["GROUP_ID"];
		}
		$str2 .= "arrItm[arrItm.length] = new Array(" . $row["GROUP_ID"] . ", " . $row["INGREDIENT_ID"] . ", '" . addslashes($row["INGREDIENT"]) . "', " . $row["MEASURE_TYPE_ID"] . ");\n";
	}
	ocifreestatement($stmt);


	$str = "";
	$tree = new rTree2("SELECT group_id, parent_id, name FROM ksi.ingredient_group ORDER BY NAME", $GLOBALS["objConn"]);

	$tree->passTree(0, 0, $str);

	writeValRow(getLM("Ingredient group"), "<select name=\"ingredient_id_g\"  onchange=\"fillItemC()\">" . $str . "</select>");

?>
<tr>
	<td align=right><? echo getLM("Ingredient"); ?>: </td>
	<td><select name="ingredient_id"></select></td>
</tr>
<script>
var arrItm = new Array();
<? echo $str2; ?>

function fillItemC() {
	var val = document.itemform.ingredient_id_g.options[document.itemform.ingredient_id_g.selectedIndex].value;
	var obj = document.itemform.ingredient_id.options;

	for (i = obj.length; i > 0; i--) {
		obj[i - 1] = null;
	}

	for (i = 0; i < arrItm.length; i++) {
		if (arrItm[i][0] == val) {
			obj[obj.length] = new Option(arrItm[i][2], arrItm[i][1]);
			if (arrItm[i][1] == <? echo (int)$GLOBALS["result"]["INGREDIENT_ID"]?>) {
				obj[obj.length - 1].selected = true;
			}
		}
	}
	if (document.itemform.ingredient_id.onchange) {
		document.itemform.ingredient_id.onchange();
	}
}


for (i = 0; i < document.itemform.ingredient_id_g.options.length; i++) {
	if (document.itemform.ingredient_id_g.options[i].value == <? echo (int)$initGroup; ?>) {
		document.itemform.ingredient_id_g.options[i].selected = true;
		break;
	}
}


fillItemC();


document.itemform.onsubmit = checkForm;

function checkForm() {
	if (document.itemform.ingredient_id.options.length) {
		return true;
	}
	else {
		alert("You must specify the ingredient.");
		return false;
	}
}
</script>
<?












	echo "<tr>";
	echo "<td align=right>" . getLM("Amount") . ": </td><td>";
	writeTextInput("amount", $GLOBALS["result"]["AMOUNT"], $arrAddParams = array("addTagData" => " style=\"text-align: right; width: 100px\""));
	echo " <select name=\"measure_id\" style=\"width: 50px\"></select>";


	$strSQL = "SELECT ksi.measure_type.measure_type_id
			, ksi.measure_type.name AS measure_type
			, ksi.measure.measure_id
			, ksi.measure.shortname
		FROM ksi.measure_type 
			INNER JOIN ksi.measure ON ksi.measure.measure_type_id = ksi.measure_type.measure_type_id
		ORDER BY ksi.measure_type.name, ksi.measure.base_units";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	echo "<script>arrMsr = new Array();\n";
	while (ocifetchinto($stmt, $row, OCI_ASSOC)) {
		echo "arrMsr[arrMsr.length] = new Array(" . $row["MEASURE_TYPE_ID"] . ", " . $row["MEASURE_ID"] . ", '" . addslashes($row["SHORTNAME"]) . "');\n";
	}
	ocifreestatement($stmt);

?>
fillMsr();	
document.itemform.ingredient_id.onchange = fillMsr;

function fillMsr() {
	var itemval = document.itemform.ingredient_id.options.length ? document.itemform.ingredient_id[document.itemform.ingredient_id.selectedIndex].value : null;
	for (i = 0; i < arrItm.length; i++) {
		if (arrItm[i][1] == itemval) {
			mt = arrItm[i][3];
			break;
		}
	}

	var obj = document.itemform.measure_id.options;
	for (i = obj.length; i > 0; i--) {
		obj[i-1] = null;
	}

	if (itemval == null) return;
	
	for (i = 0; i < arrMsr.length; i++) {
		if (arrMsr[i][0] == mt) {
			obj[obj.length] = new Option(arrMsr[i][2], arrMsr[i][1]);
			if (arrMsr[i][1] == <? echo (int)$GLOBALS["result"]["MEASURE_ID"]; ?>) {
				obj[obj.length - 1].selected = true;
			}
		}
	}
}


<?
	if ($GLOBALS["id"]) {
?>
document.itemform.ingredient_id.disabled = true;
document.itemform.ingredient_id_g.disabled = true;
<?
	}

	echo "</script>";
	echo "</td></tr>";

	writeTextInputRow(getLM("weight loss, cold processing, %"), "loss_c_percent", $GLOBALS["result"]["LOSS_C_PERCENT"]);
	writeTextInputRow(getLM("weight loss, hot processing, %"), "loss_h_percent", $GLOBALS["result"]["LOSS_H_PERCENT"]);

?>
<!--tr>
	<td align=right valign=top><? echo getLM("Note");?>: </td>
	<td><? echo getLM("If you don't see an ingredient in the ingredient list,<br>it means that the missing ingredient is already added."); ?></td>
</tr-->
<?
	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>