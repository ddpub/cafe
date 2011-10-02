<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["dp"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);
$table = "ksi.dish_product";
$idField = "dish_product_id";
$idNewVal = "ksi.dish_product_seq.nextval";

$basePage = "index.php";



if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("measure_id", (int)$_POST["measure_id"]),
		array("loss_c_percent", (float)$_POST["loss_c_percent"]),
		array("loss_h_percent", (float)$_POST["loss_h_percent"]),
		array("amount", (float)$_POST["amount"])
	);

/*
	if ($id) {

		logTranBegin(48);

		$strSQL = "UPDATE " . $table . " 
			SET amount = " . (float)$_POST["amount"] . ", measure_id = " . (int)$_POST["measure_id"] . ", loss_c_percent = " . (float)$_POST["loss_c_percent"] . ", loss_h_percent = " . (float)$_POST["loss_h_percent"] . "

			WHERE dish_id = " . (int)$_GET["e"] . " AND product_id = " . (int)$_GET["it"];
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
		ocicommit($GLOBALS["objConn"]);

		logTranEnd();

	}
	else {
		insertItem($table, $arrFV, 47);
	}
*/
	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 48);
	}
	else {
		$arrFV[] = array($idField, $idNewVal);
		$arrFV[] = array("dish_id", (int)$_GET["e"]);
		$arrFV[] = array("product_id", (int)$_POST["product_id"]);
		/*$id = */insertItem($table, $arrFV, 47);
	}


	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));

}


if ($id) {
	$strSQL = "SELECT ksi.dish.name AS dish_name
			,  ksi.product.name AS product_name
			, ksi.dish_product.*
		FROM ksi.dish 
			LEFT JOIN ksi.dish_product ON ksi.dish.dish_id = ksi.dish_product.dish_id
			LEFT JOIN ksi.product ON ksi.product.product_id = ksi.dish_product.product_id
		WHERE ksi.dish_product.dish_product_id = " . (int)$_GET["dp"];
	
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);
	
	ocifetchinto($stmt, $result, OCI_ASSOC);
	
	if (!$result) {
		echo "error: record not found";
		return;
	}
} 
else {
	$strSQL = "SELECT dish_id, name AS dish_name FROM ksi.dish WHERE dish_id = " . (int)$_GET["e"];
	
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);
	
	ocifetchinto($stmt, $result, OCI_ASSOC);
	
	if (!$result) {
		echo "error: entree not found";
		return;
	}
}



$pageTitle = $id ? "Details of " . $result["PRODUCT_NAME"] . " item, included in the " . $result["DISH_NAME"] . " item" : "Adding an item to the " . $result["DISH_NAME"] . " entree";




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




	$strSQL = "SELECT ksi.product_group.group_id
			, ksi.product_group.name AS product_group
			, ksi.product.product_id
			, ksi.product.name AS product
			, ksi.product.measure_type_id
		FROM ksi.product_group 
			INNER JOIN ksi.product ON ksi.product.group_id = ksi.product_group.group_id
		ORDER BY ksi.product_group.name, ksi.product.name";


	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	$curGroup = 0;
	$str1 = "";
	$str2 = "";
	while (ocifetchinto($stmt, $row, OCI_ASSOC)) {
/*		if ($curGroup != $row["GROUP_ID"]) {
			$str1 .= "<option value=" . $row["GROUP_ID"] . ">" . $row["PRODUCT_GROUP"] . "</option>";
			$curGroup = $row["GROUP_ID"];
		}*/
		if ($row["PRODUCT_ID"] == $GLOBALS["result"]["PRODUCT_ID"]) {
			$initGroup = $row["GROUP_ID"];
		}
		$str2 .= "arrItm[arrItm.length] = new Array(" . $row["GROUP_ID"] . ", " . $row["PRODUCT_ID"] . ", '" . addslashes($row["PRODUCT"]) . "', " . $row["MEASURE_TYPE_ID"] . ");\n";
	}
	ocifreestatement($stmt);

	$str = "";
	$tree = new rTree2("SELECT group_id, parent_id, name FROM ksi.product_group ORDER BY NAME", $GLOBALS["objConn"]);

	$tree->passTree(0, 0, $str);

	writeValRow(getLM("Item group"), "<select name=\"product_id_g\"  onchange=\"fillItemC()\">" . $str . "</select>");
?>
<tr>
	<td align=right><? echo getLM("Item"); ?>: </td>
	<td><select name="product_id"></select></td>
</tr>
<script>
var arrItm = new Array();
<? echo $str2; ?>

function fillItemC() {
	var val = document.itemform.product_id_g.options[document.itemform.product_id_g.selectedIndex].value;
	var obj = document.itemform.product_id.options;

	for (i = obj.length; i > 0; i--) {
		obj[i - 1] = null;
	}

	for (i = 0; i < arrItm.length; i++) {
		if (arrItm[i][0] == val) {
			obj[obj.length] = new Option(arrItm[i][2], arrItm[i][1]);
			if (arrItm[i][1] == <? echo (int)$GLOBALS["result"]["PRODUCT_ID"]?>) {
				obj[obj.length - 1].selected = true;
			}
		}
	}
	if (document.itemform.product_id.onchange) {
		document.itemform.product_id.onchange();
	}
}


for (i = 0; i < document.itemform.product_id_g.options.length; i++) {
	if (document.itemform.product_id_g.options[i].value == <? echo (int)$initGroup; ?>) {
		document.itemform.product_id_g.options[i].selected = true;
		break;
	}
}

document.itemform.onsubmit = checkForm;

function checkForm() {
	if (document.itemform.product_id.options.length) {
		return true;
	}
	else {
		alert("You must specify the item.");
		return false;
	}
}

fillItemC();
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
document.itemform.product_id.onchange = fillMsr;

function fillMsr() {
	var itemval = document.itemform.product_id.options.length ? document.itemform.product_id[document.itemform.product_id.selectedIndex].value : null;

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
document.itemform.product_id.disabled = true;
document.itemform.product_id_g.disabled = true;
<?
	}

	echo "</script>";
	echo "</td></tr>";

	writeTextInputRow(getLM("weight loss, cold processing, %"), "loss_c_percent", $GLOBALS["result"]["LOSS_C_PERCENT"]);
	writeTextInputRow(getLM("weight loss, hot processing, %"), "loss_h_percent", $GLOBALS["result"]["LOSS_H_PERCENT"]);

?>
<!--tr>
	<td align=right valign=top><? echo getLM("Note"); ?>: </td>
	<td><? echo getLM("If you don't see an item in the item list,<br>it means that the missing item is already added."); ?></td>
</tr-->
<?
	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>