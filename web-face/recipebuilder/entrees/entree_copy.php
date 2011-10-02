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

$pageTitle = getLM("Creating a new entree via copy");

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
		array("recipe", $_POST["recipe"]),
		array("employee_meal", (int)($_POST["employee_meal"] != ""))
	);


	$arrFV[] = array($idField, $idNewVal);

	logTranBegin(29);
	$newid = insertItem($table, $arrFV);

	$arrSQL = array();

	$arrSQL []= "INSERT INTO ksi.dish_product (dish_id, product_id, amount, measure_id, loss_h_percent, loss_c_percent) (SELECT " . $newid .", product_id, amount, measure_id, loss_h_percent, loss_c_percent FROM ksi.dish_product WHERE dish_id = " . $id . ")";

	$arrSQL []= "INSERT INTO ksi.dish_ingredient (dish_id, ingredient_id, amount, measure_id, loss_h_percent, loss_c_percent) (SELECT " . $newid .", ingredient_id, amount, measure_id, loss_h_percent, loss_c_percent FROM ksi.dish_ingredient WHERE dish_id = " . $id . ")";

	$arrSQL []= "INSERT INTO ksi.subdish (base_dish_id, dish_id, count) (SELECT " . $newid .", dish_id, count FROM ksi.subdish WHERE base_dish_id = " . $id . ")";

	$arrSQL []= "INSERT INTO ksi.dish_mod_group (dish_id, group_id) (SELECT " . $newid .", group_id FROM ksi.dish_mod_group WHERE dish_id = " . $id . ")";

	foreach( $arrSQL as $val ) {
		$stmt = ociparse($GLOBALS["objConn"], $val);

		if (!@ociexecute($stmt, OCI_DEFAULT)) {
			oci_rollback;
			$e = oci_error($stmt);
			pr($e);
			die("error updating record");
		}

	}

	logTranEnd();


	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));

}









$strSQL = "SELECT * FROM " . $table . " WHERE " . $idField . " = " . $id;
$stmt = ociparse($GLOBALS["objConn"], $strSQL);
ociexecute($stmt, OCI_DEFAULT);

ocifetchinto($stmt, $result, OCI_ASSOC);

if (!$result) {
	echo "error: record not found";
	return;
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
	global $arrHotkeyColor, $id;


	$tree = new rTree2("SELECT group_id, parent_id, name FROM ksi.dish_group ORDER BY NAME", $GLOBALS["objConn"]);

	$tree->passTree(0, 0, $str);



	writeFormTableHeader();
	if ($GLOBALS["id"]) 
		writeValRow("id", $GLOBALS["id"]);
	writeTextInputRow(getLM("Entree"), "name", $GLOBALS["result"]["NAME"] . " " . getLM("copy1"));

	writeValRow(getLM("entree group"), "<select name=\"group_id\">" . $str . "</select>");
	
	writeSelectSQLRow(getLM("Where is prepared"), "is_beverage", "SELECT kt_id, name FROM ksi.kitchen_type ORDER BY name", $GLOBALS["result"]["IS_BEVERAGE"] ? $GLOBALS["result"]["IS_BEVERAGE"] : 0);

	writeTextInputRow(getLM("Entree code"), "code", $GLOBALS["result"]["CODE"]);
	writeTextInputRow(getLM("Suggested price") . ", " . getLM("$"), "def_price", $GLOBALS["result"]["DEF_PRICE"]);
	writeTextInputRow(getLM("Recipe number"), "recipe_number", $GLOBALS["result"]["RECIPE_NUMBER"]);
	writeTextareaRow(getLM("Description"), "description", $GLOBALS["result"]["DESCRIPTION"]);
	writeCheckboxRow(getLM("This entree can be in employee meal"), "employee_meal", $GLOBALS["result"]["EMPLOYEE_MEAL"]);

	echo "<input type=hidden name=\"recipe\" value=\"" . nl2br(htmlentities($GLOBALS["result"]["RECIPE"], ENT_QUOTES, cp1251)) . "\">";

	$str = "<select name=\"hotkey color\">";
	foreach ($arrHotkeyColor as $val) {
		$str .= "<option value=\"" . $val[0] . "\"" . ($GLOBALS["result"]["HOTKEY_COLOR"] == $val[0] ? " selected" : "") . " style=\"background-color:#" . $val[0] . "; color: #" . ((hexdec(substr($val[0] . "000000", 0, 2)) + hexdec(substr($val[0] . "000000", 2, 2)) + hexdec(substr($val[0] . "000000", 4, 2))) / 3 < 128 ? "FFFFFF" : "000000") .  "\">" . $val[1] . "</option>";
	}
	$str .= "</select>";
	writeValRow(getLM("Hotkey color"), $str);


	echo "<tr><td colspan=2 valign=top><table width=100% cellspacing=8>";



// items

	$strSQL = "SELECT ksi.product.product_id
			, ksi.product.name
			, ksi.dish_product.amount
			, ksi.measure.shortname
			, ksi.dish_product.loss_c_percent
			, ksi.dish_product.loss_h_percent
			FROM ksi.dish_product 
				INNER JOIN ksi.product ON ksi.dish_product.product_id = ksi.product.product_id
				INNER JOIN ksi.measure ON ksi.dish_product.measure_id = ksi.measure.measure_id
			WHERE dish_id = " . $id . "
			ORDER BY ksi.product.name";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);


	ociexecute($stmt, OCI_DEFAULT);
	oci_fetch_all($stmt, $res, 0, -1, OCI_FETCHSTATEMENT_BY_ROW);
	oci_free_statement($stmt);


	if (count($res)) {
		echo "<td valign=top><h3>" . getLM("Items") . "</h3><table cellspacing=0 cellpadding=1 border=0 class=\"grid\"><th>" . getLM("item") . "</th><th colspan=2>" . getLM("qty") . "</th>";
		foreach ($res as $val) {
			echo "<tr><td>" . $val["NAME"] . "</td><td align=right  style=\"padding-right: 2px\">" . $val["AMOUNT"] . "</td><td style=\"padding-left: 0px\">" . $val["SHORTNAME"] . "</td></tr>";
		}
		echo "</table></td>";
	}





// ingredients

	$strSQL = "SELECT ksi.ingredient.ingredient_id, ksi.ingredient.name, ksi.dish_ingredient.amount, ksi.measure.shortname 
			, ksi.dish_ingredient.loss_c_percent
			, ksi.dish_ingredient.loss_h_percent

			FROM ksi.dish_ingredient 
				INNER JOIN ksi.ingredient ON ksi.dish_ingredient.ingredient_id = ksi.ingredient.ingredient_id
				INNER JOIN ksi.measure ON ksi.dish_ingredient.measure_id = ksi.measure.measure_id
			WHERE dish_id = " . $id . "
			ORDER BY ksi.ingredient.name";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);


	ociexecute($stmt, OCI_DEFAULT);
	oci_fetch_all($stmt, $res, 0, -1, OCI_FETCHSTATEMENT_BY_ROW);
	oci_free_statement($stmt);


	if (count($res)) {
		echo "<td valign=top><h3>" . getLM("Ingredients") . "</h3><table cellspacing=0 cellpadding=1 border=0 class=\"grid\"><th>" . getLM("ingredient") . "</th><th colspan=2>" . getLM("qty") . "</th>";
		foreach ($res as $val) {
			echo "<tr><td>" . $val["NAME"] . "</td><td align=right  style=\"padding-right: 2px\">" . $val["AMOUNT"] . "</td><td style=\"padding-left: 0px\">" . $val["SHORTNAME"] . "</td></tr>";
		}
		echo "</table></td>";
	}



// entrees

	$strSQL = "SELECT ksi.dish.dish_id, ksi.dish.name, ksi.subdish.count
			FROM ksi.subdish 
				INNER JOIN ksi.dish ON ksi.subdish.dish_id = ksi.dish.dish_id
			WHERE base_dish_id = " . $id . "
			ORDER BY ksi.dish.name";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);

	ociexecute($stmt, OCI_DEFAULT);
	oci_fetch_all($stmt, $res, 0, -1, OCI_FETCHSTATEMENT_BY_ROW);
	oci_free_statement($stmt);


	if (count($res)) {
		echo "<td valign=top><h3>" . getLM("Entrees") . "</h3><table cellspacing=0 cellpadding=1 border=0 class=\"grid\"><th>" . getLM("entree") . "</th><th>" . getLM("qty") . "</th>";
		foreach ($res as $val) {
			echo "<tr><td>" . $val["NAME"] . "</td><td align=right>" . $val["AMOUNT"] . "</td></tr>";
		}
		echo "</table></td>";
	}



// assigned modifiers


	$strSQL = "SELECT ksi.mod_group.group_id, ksi.mod_group.name
		FROM ksi.mod_group
			INNER JOIN ksi.dish_mod_group ON ksi.dish_mod_group.group_id = ksi.mod_group.group_id
		WHERE dish_id = " . $id . "
		ORDER BY ksi.mod_group.pos, ksi.mod_group.name";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);

	ociexecute($stmt, OCI_DEFAULT);
	oci_fetch_all($stmt, $res, 0, -1, OCI_FETCHSTATEMENT_BY_ROW);
	oci_free_statement($stmt);


	if (count($res)) {
		echo "<td valign=top><h3>" . getLM("Assigned modifiers") . "</h3><table cellspacing=0 cellpadding=1 border=0 class=\"grid\"><th>" . getLM("modifier group") . "</th>";
		foreach ($res as $val) {
			echo "<tr><td>" . $val["NAME"] . "</td></tr>";
		}
		echo "</table></td>";
	}



// recipe

	if (strlen($GLOBALS["result"]["RECIPE"])) {
		echo "<td valign=top><h3>" . getLM("Recipe") . "</h3>" . $GLOBALS["result"]["RECIPE"] . "</td>";

	}


	echo "</table></td></tr>";


	writeFormTableFooter();

}





require($pathToSiteRoot . "common/template_dtl.php");
?>