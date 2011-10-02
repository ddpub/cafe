<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">

<html>
<head>
	<title><? echo getLM($arrApps[$appName]["appTitle"]) . " / " . ucwords($pageTitle) . " &bull; " . $_person_name; ?></title>
	<link rel="stylesheet" href="<? echo $pathToSiteRoot; ?>common/styles/common.css" type="text/css">
	<style>
		 textarea {
			width: 200px;
			height: 200px;
		}
	</style>
</head>

<body>
<table cellspacing=5 cellpadding=0 border=0 width="100%">
	<td><h1 style="display: inline"><? echo ucwords($pageTitle);?></h1></td>
	<td align=right class="tiny"><strong><? echo getLM($arrApps[$appName]["appTitle"]); ?></strong><br><br> <? echo getLM("logged as")?>: <? echo $_person_name . " (" . $_person_roles_str . ")"?> | <a href="<? echo $_SERVER["SCRIPT_NAME"]?>?logout"><? echo getLM("logout")?></a></td>
</table>
<p>

<?
writeMainContent();
?>

</body>
</html>
<?























// basic functions

function writeFormHeader() {

	if (isset($GLOBALS["upload"]) && is_object($GLOBALS["upload"])) {
		echo "<tr id=\"uploader\"><td colspan=2>"; 
		$GLOBALS["upload"]->writeUploadForms(); 
		echo "</td></tr><tr><td colspan=2><br></td></tr>";
	}

	echo "<form name=\"itemform\" action=\"" . $_SERVER["PHP_SELF"] . ($_SERVER["QUERY_STRING"] ? "?" . $_SERVER["QUERY_STRING"] : "") . "\" method=post>";
	echo "<input type=\"hidden\" name=\"qstr\" value=\"" . $GLOBALS["qstr"] . "\">";

	if (isset($GLOBALS["upload"]) && is_object($GLOBALS["upload"])) {
		$GLOBALS["upload"] -> writeUploadHiddenInputs();
	}

}



function writeFormFooter() {
	echo "</form>";
}



function writeTextInput($controlName, $value, $arrAddParams = array()) {
	echo "<input type=\"text\" name=\"" . $controlName . "\" value=\"" . htmlspecialchars($value) . "\"" . (is_array($arrAddParams) ? " " . $arrAddParams["addTagData"] : "" ) . ">";
}



function writeSelect($controlName, $arrOptions, $val, $nullCaption, $arrAddParams) {
	echo "<select name=\"" . $controlName . "\"" . (is_array($arrAddParams) ? " " . $arrAddParams["addTagData"] : "" ) . ">";
	if ($nullCaption) {
		echo "<option value=\"\">" . htmlspecialchars($nullCaption) . "</option>";
	}
	foreach($arrOptions as $option) {
		echo "<option value=\"" . $option[0] . "\"" . ((int)$option[0] == (int)$val ? " selected" : "") . ">" . htmlspecialchars($option[1]) . "</option>";
	}
	echo "</select>";
}

function writeTextarea($controlName, $value, $arrAddParams = array()) {
	echo "<textarea name=\"" . $controlName . "\"" . $arrAddParams["addTagData"]  . ">" . htmlspecialchars($value) . "</textarea>";
}


function writeCheckbox($controlName, $value, $arrAddParams) {
	echo "<input type=\"checkbox\" name=\"" . $controlName . "\" " . ($value ? " checked" : "") . ($arrAddParams["addTagData"] ? " " . $arrAddParams["addTagData"] : "") . ">";
}







// featured functions

function writeValRow($caption, $val, $id="") {
?>
	<tr<? echo $id ? " id=\"" . $id . "\"" : "";?>>
		<td align=right><? echo $caption ?>: </td>
		<td><? echo $val; ?></td>
	</tr>
<?
}

function writeTextInputRow($caption, $controlName, $value, $arrAddParams = array()) {
?>
	<tr>
		<td align=right><? echo $caption ?>: </td>
		<td><? writeTextInput($controlName, $value, $arrAddParams); ?></td>
	</tr>
<?
}


function writeSelectRow($caption, $controlName, $arr, $val, $nullCaption, $arrAddParams = array()) {
?>
	<tr>
		<td align=right><? echo $caption ?>: </td>
		<td><? writeSelect($controlName, $arr, $val, $nullCaption, $arrAddParams); ?></td>
	</tr>
<?
}




function writeSelectSQLRow($caption, $controlName, $strSQL, $val, $nullCaption = "", $arrAddParams = array()) {
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);

	ociexecute($stmt, OCI_DEFAULT);

	while (ocifetchinto($stmt, $row)) {
		$arr[] = $row;
	}
	ocifreestatement($stmt);
?>
	<tr>
		<td align=right><? echo $caption ?>: </td>
		<td><? writeSelect($controlName, $arr, $val, $nullCaption, $arrAddParams); ?></td>
	</tr>
<?
}


function writeTextareaRow($caption, $controlName, $value, $arrAddParams = array()) {
?>
	<tr>
		<td align=right valign=top><? echo $caption ?>: </td>
		<td><? writeTextarea($controlName, $value, $arrAddParams); ?></td>
	</tr>
<?
}

function writeCheckboxRow($caption, $controlName, $value, $arrAddParams = array()) {
?>
	<tr>
		<td align=right><? echo $caption ?>: </td>
		<td><? writeCheckbox($controlName, $value, $arrAddParams); ?></td>
	</tr>
<?
}





function writeFormTableHeader() {
	writeFormHeader();
	echo "<table border=0 cellspacing=5 cellpadding=0 align=center>";
}





function writeFormTableFooter() {
?>
	<tr>
		<td colspan=2 align=center><br><br><input type="submit" name="submit" value="<? echo getLM("save")?>"> &nbsp;&nbsp;<input type="button" name="cancel" value="<? echo getLM("cancel")?>" onclick="location.href='<? echo $GLOBALS["basePage"] . ($GLOBALS["qstr"] ? "?" . $GLOBALS["qstr"]  : ""); ?>'"></td>
	</tr>
</table>
<?
	writeFormFooter();
}







// ---------------------- even more featured functions

function writeMeasureWithTypeSelect($controlName, $val) {

	$strSQL = "SELECT ksi.measure_type.measure_type_id
			, ksi.measure_type.name AS measure_type
			, ksi.measure.measure_id
			, ksi.measure.name AS measure
		FROM ksi.measure_type 
			INNER JOIN ksi.measure ON ksi.measure.measure_type_id = ksi.measure_type.measure_type_id
		ORDER BY ksi.measure_type.name, ksi.measure.base_units";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	$curType = 0;
	$str1 = "";
	$str2 = "";
	while (ocifetchinto($stmt, $row, OCI_ASSOC)) {
		if ($curType != $row["MEASURE_TYPE_ID"]) {
			$str1 .= "<option value=" . $row["MEASURE_TYPE_ID"] . ">" . $row["MEASURE_TYPE"] . "</option>";
			$curType = $row["MEASURE_TYPE_ID"];
		}
		if ($row["MEASURE_ID"] == $val) {
			$initType = $row["MEASURE_TYPE_ID"];
		}
		$str2 .= "arrMsr[arrMsr.length] = new Array(" . $row["MEASURE_TYPE_ID"] . ", " . $row["MEASURE_ID"] . ", '" . addslashes($row["MEASURE"]) . "');\n";
	}
	ocifreestatement($stmt);

?>
<tr>
	<td align=right><? echo getLM("Measure type")?>: </td>
	<td><select name="<? echo $controlName?>_t" onchange="fillMeasureC()"><? echo $str1; ?></select></td>
</tr>
<tr>
	<td align=right><? echo getLM("Measure")?>: </td>
	<td><select name="<? echo $controlName?>"></select></td>
</tr>
<script>
var arrMsr = new Array();
<? echo $str2; ?>

function fillMeasureC() {
	var val = document.itemform.<? echo $controlName?>_t.options[document.itemform.<? echo $controlName?>_t.selectedIndex].value;
	var obj = document.itemform.<? echo $controlName?>.options;

	for (i = obj.length; i > 0; i--) {
		obj[i - 1] = null;
	}

	for (i = 0; i < arrMsr.length; i++) {
		if (arrMsr[i][0] == val) {
			obj[obj.length] = new Option(arrMsr[i][2], arrMsr[i][1]);
			if (arrMsr[i][1] == <? echo (int)$val?>) {
				obj[obj.length - 1].selected = true;
			}
		}
	}
}


for (i = 0; i < document.itemform.<? echo $controlName?>_t.options.length; i++) {
	if (document.itemform.<? echo $controlName?>_t.options[i].value == <? echo (int)$initType; ?>) {
		document.itemform.<? echo $controlName?>_t.options[i].selected = true;
		break;
	}
}



fillMeasureC();
</script>
<?
}


function writeProductWithGroupSelect($controlName, $val) {

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
			if ($curGroup != $row["GROUP_ID"]) {
			$str1 .= "<option value=" . $row["GROUP_ID"] . ">" . $row["PRODUCT_GROUP"] . "</option>";
			$curGroup = $row["GROUP_ID"];
		}
		if ($row["PRODUCT_ID"] == $val) {
			$initGroup = $row["GROUP_ID"];
		}
		$str2 .= "arrItm[arrItm.length] = new Array(" . $row["GROUP_ID"] . ", " . $row["PRODUCT_ID"] . ", '" . addslashes($row["PRODUCT"]) . "', " . $row["MEASURE_TYPE_ID"] . ");\n";
	}
	ocifreestatement($stmt);

?>
<tr>
	<td align=right><? echo getLM("Item group")?>: </td>
	<td><select name="<? echo $controlName?>_g" onchange="fillItemC()"><? echo $str1; ?></select></td>
</tr>
<tr>
	<td align=right><? echo getLM("Item")?>: </td>
	<td><select name="<? echo $controlName?>"></select></td>
</tr>
<script>
var arrItm = new Array();
<? echo $str2; ?>

function fillItemC() {
	var val = document.itemform.<? echo $controlName?>_g.options[document.itemform.<? echo $controlName?>_g.selectedIndex].value;
	var obj = document.itemform.<? echo $controlName?>.options;

	for (i = obj.length; i > 0; i--) {
		obj[i - 1] = null;
	}

	for (i = 0; i < arrItm.length; i++) {
		if (arrItm[i][0] == val) {
			obj[obj.length] = new Option(arrItm[i][2], arrItm[i][1]);
			if (arrItm[i][1] == <? echo (int)$val?>) {
				obj[obj.length - 1].selected = true;
			}
		}
	}
	if (document.itemform.<? echo $controlName?>.onchange) {
		document.itemform.<? echo $controlName?>.onchange();
	}
}


for (i = 0; i < document.itemform.<? echo $controlName?>_g.options.length; i++) {
	if (document.itemform.<? echo $controlName?>_g.options[i].value == <? echo (int)$initGroup; ?>) {
		document.itemform.<? echo $controlName?>_g.options[i].selected = true;
		break;
	}
}



fillItemC();
</script>
<?
}









function writeDishWithGroupSelect($controlName, $val, $mode = array(0, 0), $checkbox = "") {

require_once($GLOBALS["pathToSiteRoot"] . "common/class_rtree.php");

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

	if ($mode[0] == 0) {

		$strSQL = "SELECT ksi.dish_group.group_id
				, ksi.dish_group.name AS dish_group
				, ksi.dish.dish_id
				, ksi.dish.name AS dish
			FROM ksi.dish_group 
				INNER JOIN ksi.dish ON ksi.dish.group_id = ksi.dish_group.group_id
			WHERE ksi.dish.deleted IS NULL
			ORDER BY ksi.dish_group.name, ksi.dish.name";
	}
	elseif ($mode[0] == 1) {// adding entree to menu_folder, filtering already added
		$strSQL = "SELECT ksi.dish_group.group_id
				, ksi.dish_group.name AS dish_group
				, ksi.dish.dish_id
				, ksi.dish.name AS dish
			FROM ksi.dish_group 
				INNER JOIN ksi.dish ON ksi.dish.group_id = ksi.dish_group.group_id
				LEFT JOIN ksi.menu_dish ON ksi.menu_dish.dish_id = ksi.dish.dish_id AND ksi.menu_dish.menu_item_id = " . (int)$mode[1] . "
			WHERE ksi.dish.deleted IS NULL AND ksi.menu_dish.dish_id IS NULL
			ORDER BY ksi.dish_group.name, ksi.dish.name";

	}
//	echo $strSQL;

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
		if ($row["DISH_ID"] == $val) {
			$initGroup = $row["GROUP_ID"];
		}
		$str2 .= "arrDsh[arrDsh.length] = new Array(" . $row["GROUP_ID"] . ", " . $row["DISH_ID"] . ", '" . addslashes($row["DISH"]) . "');\n";
	}
	ocifreestatement($stmt);

	$str = "";
	$tree = new rTree2("SELECT group_id, parent_id, name FROM ksi.dish_group ORDER BY name", $GLOBALS["objConn"]);

	$tree->passTree(0, 0, $str);

	writeValRow(getLM("Entree group"), "<select name=\"" . $controlName . "_g\"  onchange=\"fillDishC()\">" . $str . "</select>", $controlName . "_g_tr");


?>
<!--tr id="<? echo $controlName?>_g_tr">
	<td align=right><? echo getLM("Entree group")?>: </td>
	<td><select name="<? echo $controlName?>_g" onchange="fillDishC()"><? echo $str1; ?></select></td>
</tr-->
<tr id="<? echo $controlName?>_tr">
	<td align=right><? echo getLM("Entree")?>: </td>
	<td><select name="<? echo $controlName?>"></select></td>
</tr>
<script>
var arrDsh = new Array();
<? echo $str2; ?>

function fillDishC() {
	var val = document.itemform.<? echo $controlName?>_g.options[document.itemform.<? echo $controlName?>_g.selectedIndex].value;
	var obj = document.itemform.<? echo $controlName?>.options;

	for (i = obj.length; i > 0; i--) {
		obj[i - 1] = null;
	}

	for (i = 0; i < arrDsh.length; i++) {
		if (arrDsh[i][0] == val) {
			obj[obj.length] = new Option(arrDsh[i][2], arrDsh[i][1]);
			if (arrDsh[i][1] == <? echo (int)$val?>) {
				obj[obj.length - 1].selected = true;
			}
		}
	}
	if (document.itemform.<? echo $controlName?>.onchange) {
		document.itemform.<? echo $controlName?>.onchange();
	}
}


for (i = 0; i < document.itemform.<? echo $controlName?>_g.options.length; i++) {
	if (document.itemform.<? echo $controlName?>_g.options[i].value == <? echo (int)$initGroup; ?>) {
		document.itemform.<? echo $controlName?>_g.options[i].selected = true;
		break;
	}
}


fillDishC();


document.itemform.onsubmit = checkForm;


function checkForm() {
	if (document.itemform.<? echo $controlName;?>.options.length<? echo $checkbox ? " || !document.itemform." . $checkbox . ".checked" : "";?>) {
		return true;
	}
	else {
		alert('<? getLM("You must specify an entree");?>');
		return false;
	}
}
</script>
<?
}

?>

