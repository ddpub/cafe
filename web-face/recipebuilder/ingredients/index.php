<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);

if ($_GET["act"] == "delg") {

	logTranBegin(9);

	$strSQL = "DELETE FROM ksi.ingredient_group WHERE group_id = " . (int)$_GET["id"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "Cannot delete ingredient group: its not empty";
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=delg&/", "", $_SERVER["QUERY_STRING"]));
	}


}
elseif ($_GET["act"] == "deli") {

	logTranBegin(12);

	$strSQL = "DELETE FROM ksi.ingredient WHERE ingredient_id = " . (int)$_GET["id"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "Cannot delete ingredient: it is included in a recipe";
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=deli&/", "", $_SERVER["QUERY_STRING"]));
	}


}
elseif ($_GET["act"] == "delit") {

	logTranBegin(15);

	$strSQL = "DELETE FROM ksi.ingredient_product WHERE ingredient_product_id = " . (int)$_GET["ip"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "Undexpected error, contact the developer";
	}
	else {
		logTranEnd();
//		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=delit&/", "", $_SERVER["QUERY_STRING"]));
	}


}
elseif ($_GET["act"] == "delii") {

	logTranBegin(18);

	$strSQL = "DELETE FROM ksi.ingredients WHERE ingredients_id = " . (int)$_GET["ii"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "Undexpected error, contact the developer";
	}
	else {
		logTranEnd();
//		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=delii&/", "", $_SERVER["QUERY_STRING"]));
	}


}












class rTree2 extends rTree {

	function writeTreeExpanded($parentID, $level) {

		for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
			if ($GLOBALS["curGroup"] == $this->arrIdx[$parentID][$i]) {
				echo "<tr class=\"selectedRow\"><td style=\"padding-left: " . (10 + 20 * $level) . "px\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle>" . $this->arrTree[$this->arrIdx[$parentID][$i]][2];
			}
			else {
				echo "<tr><td style=\"padding-left:" . (10+ 20 * $level) . "px\"><a href=\"" . $_SERVER["SCRIPT_NAME"] . "?g=" . $this->arrIdx[$parentID][$i] . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_c.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle>" . $this->arrTree[$this->arrIdx[$parentID][$i]][2] . "</a>";
			}
			echo "</td><td align=right style=\"padding-right:" . (40 - $level * 12 > 0 ? 40 - $level * 12 : 0) . "px\">" . $this->arrTree[$this->arrIdx[$parentID][$i]][3] . "</td><td>" . str_repeat("&nbsp;", $level) . $this->arrTree[$this->arrIdx[$parentID][$i]][4] . "</td>";


			echo "<td class=tiny nowrap><a href=\"group_details.php?id=" . $this->arrIdx[$parentID][$i] . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delg&id=" . $this->arrIdx[$parentID][$i] . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">" . getLM("delete") . "</a></td></tr>";

			$this->writeTreeExpanded($this->arrIdx[$parentID][$i], $level+1);
		}
	}



	function passTreeCustom($parentID, $level) {

		for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
			if (!$GLOBALS["curGroup"]) {
				$GLOBALS["curGroup"] = $this->arrIdx[$parentID][$i];
				$GLOBALS["curGroupName"] = $this->arrTree[$this->arrIdx[$parentID][$i]][2];
			}

			if ($_GET["g"] == $this->arrIdx[$parentID][$i]) {
				$GLOBALS["curGroup"] = $this->arrIdx[$parentID][$i];
				$GLOBALS["curGroupName"] = $this->arrTree[$this->arrIdx[$parentID][$i]][2];
			}

			$this->passTreeCustom($this->arrIdx[$parentID][$i], $level+1);
		}
	}

}






function writeMainContent() {

?>
<table cellspacing=0 cellpadding=0 cellspacing=5 width="100%">
<td valign=top>
<?



	$groupTree = new rTree2("SELECT group_id, parent_id, name, pos FROM ksi.ingredient_group ORDER BY pos, name", $GLOBALS["objConn"]);
	$groupTree->passTreeCustom(0, 0);


	$tmpArr = array(array(getLM("Add new"), "group_details.php", "images/icons/addfolder.gif"));

	if (count($groupTree->arrTree)) {
		array_push($tmpArr, 		
		array(getLM("Edit group"), "group_details.php?id=" . $GLOBALS["curGroup"], "images/icons/edit.gif"),
		array(getLM("Delete group"), $_SERVER["SCRIPT_NAME"] . "?act=delg&id=" . $GLOBALS["curGroup"], "images/icons/delete.gif", "onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\""));
	}


	echo "<h2>" . getLM("Ingredient groups") . "</h2>";

	writeActionsBar($tmpArr);





	echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr><th colspan=2>" . getLM("ingredient group") . "</th><th>" . getLM("position") . "</th><th>" . getLM("action") . "</th></tr>";

	$groupTree->writeTreeExpanded(0, 0);

	echo "</table>";
?>
</td>
<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top width="30%">
<?















	$strSQL = "SELECT ksi.ingredient.ingredient_id, ksi.ingredient.name, ksi.measure.name AS measure, ksi.ingredient.recipe, ingredient.pos
	FROM ksi.ingredient 
		INNER JOIN ksi.measure ON ksi.ingredient.measure_id = ksi.measure.measure_id
	WHERE ksi.ingredient.group_id = " . $GLOBALS["curGroup"] . "
	ORDER BY ingredient.pos, ksi.ingredient.name";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);


	$curIngredient = 0;
	$arrIngredient = array();


	while (ocifetch($stmt)) {
		if (!$curIngredient) {
			$curIngredient = ociresult($stmt, "INGREDIENT_ID");
			$curIngredientName = ociresult($stmt, "NAME");
			$recipe = ociresult($stmt, "RECIPE");
			$curMeasure = ociresult($stmt, "MEASURE");
		}
		if ((int)$_GET["i"] == ociresult($stmt, "INGREDIENT_ID")) {
			$curIngredient = ociresult($stmt, "INGREDIENT_ID");
			$curIngredientName = ociresult($stmt, "NAME");
			$recipe = ociresult($stmt, "RECIPE");
			$curMeasure = ociresult($stmt, "MEASURE");
		}
		$arrIngredient[] = array(ociresult($stmt, "INGREDIENT_ID"), ociresult($stmt, "NAME"), ociresult($stmt, "MEASURE"));
	}
	ocifreestatement($stmt);

	echo "<h2>" . ucfirst($GLOBALS["curGroupName"]) . " " . getLM("ingredients") . "</h2>";

	$arrTmp = array(array(getLM("Add new"), "ingredient_details.php?g=" . $GLOBALS["curGroup"], "images/icons/add.gif"));
	if (count($arrIngredient)) {
		array_push($arrTmp, 
			array(getLM("Edit"), "ingredient_details.php?id=" . $curIngredient, "images/icons/edit.gif"), 
			array(getLM("Delete"), $_SERVER["SCRIPT_NAME"] . "?act=deli&id=" . $curIngredient . "&g=" . $GLOBALS["curGroup"], "images/icons/delete.gif")
		);
	}

	writeActionsBar($arrTmp);

	echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
	echo "<tr><th>" . getLM("ingredient") . "</th><th>" . getLM("measures in") . "</th><th>" . getLM("position") . "</th><th>" . getLM("action") . "</th></tr>";
	foreach ($arrIngredient as $val) {
		echo "<tr" . ($val[0] == $curIngredient ? " class=\"selectedRow\"" : "") . "><td>";
		if ($val[0] == $curIngredient) {
			echo $val[1];
		}
		else {
			echo "<a href=\"" . $_SERVER["SCRIPT_NAME"] . "?g=" . $GLOBALS["curGroup"] . "&i=" . $val[0] . "\">" . $val[1] . "</a>";
		}
		echo "</td><td>" . $val[2] . "</td><td>" . $val[3] . "</td>
		<td class=tiny nowrap><a href=\"ingredient_details.php?id=" . $val[0] . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=deli&id=" . $val[0] . "&g=" . $GLOBALS["curGroup"] . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">" . getLM("delete") . "</a></td></tr>";
	}

	echo "</table>";

?>

</td>
<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top width="40%">
<?
	echo "<h2>" . $curIngredientName . " " . getLM("ingredient contents") . "</h2><p>" . getLM("Items and ingredients list, needed to make one") . " " . $curMeasure . " " . getLM("of1") . " " . $curIngredientName . "</p>";
	
	if (!$curIngredient) {
		echo getLM("Please create an ingredient first");
	}
	else {
		echo "<h3>" . getLM("Items") . "</h3>";

		$strSQL = "SELECT ksi.product.product_id, ksi.product.name, ksi.ingredient_product.amount, ksi.measure.shortname 
			, ksi.ingredient_product.loss_c_percent
			, ksi.ingredient_product.loss_h_percent
			, ksi.ingredient_product.ingredient_product_id

			FROM ksi.ingredient_product 
				INNER JOIN ksi.product ON ksi.ingredient_product.product_id = ksi.product.product_id
				INNER JOIN ksi.measure ON ksi.ingredient_product.measure_id = ksi.measure.measure_id
			WHERE ingredient_id = " . $curIngredient . "
			ORDER BY ksi.product.name";

		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);

		writeActionsBar(array(
			array(getLM("Add new item"), "i_item_details.php?i=" . $curIngredient, "images/icons/additem.gif"),
		));
	
	
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" .getLM("item") . "</th><th colspan=2>" . getLM("qty") . "</th><th>" . getLM("weight loss, cold processing, %") . "</th><th>" . getLM("weight loss, hot processing, %") . "</th><th>" . getLM("action") . "</th></tr>";
		while (ocifetch($stmt)) {
			echo "<tr><td>";
//			echo "<a href=\"i_item_details.php?i=" . $curIngredient . "&it=" . ociresult($stmt, "PRODUCT_ID") . "\">" . ociresult($stmt, "NAME") . "</a>";
			echo ociresult($stmt, "NAME");
			echo "</td>
			<td align=right  style=\"padding-right: 2px\">" . ociresult($stmt, "AMOUNT") . "</td>
			<td style=\"padding-left: 0px\">" . ociresult($stmt, "SHORTNAME") . "</td>
			<td align=right>" . (ociresult($stmt, "LOSS_C_PERCENT") ? ociresult($stmt, "LOSS_C_PERCENT") . "%" : "-") . "</td>
			<td align=right>" . (ociresult($stmt, "LOSS_H_PERCENT") ? ociresult($stmt, "LOSS_H_PERCENT") . "%" : "-") . "</td>

			<td class=tiny><a href=\"i_item_details.php?ip=" . ociresult($stmt, "INGREDIENT_PRODUCT_ID") . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delit&ip=" . ociresult($stmt, "INGREDIENT_PRODUCT_ID") . "&g=" . $GLOBALS["curGroup"] . "&i=" . $curIngredient . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">" . getLM("delete") . "</a></td></tr>";
		}
		ocifreestatement($stmt);
	
		echo "</table>";


		echo "<h3>" . getLM("Ingredients") . "</h3>";

		$strSQL = "SELECT ksi.ingredient.ingredient_id, ksi.ingredient.name, ksi.ingredients.amount, ksi.measure.shortname 
			, ksi.ingredients.loss_c_percent
			, ksi.ingredients.loss_h_percent
			, ksi.ingredients.ingredients_id

			FROM ksi.ingredients 
				INNER JOIN ksi.ingredient ON ksi.ingredients.ingredient_id = ksi.ingredient.ingredient_id
				INNER JOIN ksi.measure ON ksi.ingredients.measure_id = ksi.measure.measure_id
			WHERE base_ingredient_id = " . $curIngredient . "
			ORDER BY ksi.ingredient.name";

		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);

		writeActionsBar(array(
			array(getLM("Add new ingredient"), "i_ingredient_details.php?i=" . $curIngredient, "images/icons/additem.gif"),
		));
	
	
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" . getLM("ingredient") . "</th><th colspan=2>" . getLM("qty") . "</th><th>" . getLM("weight loss, cold processing, %") . "</th><th>" . getLM("weight loss, hot processing, %") . "</th><th>" . getLM("action") . "</th></tr>";
		while (ocifetch($stmt)) {
			echo "<tr><td>";
			echo ociresult($stmt, "NAME");
			echo "</td>
			<td align=right  style=\"padding-right: 2px\">" . ociresult($stmt, "AMOUNT") . "</td>
			<td style=\"padding-left: 0px\">" . ociresult($stmt, "SHORTNAME") . "</td>
			<td align=right>" . (ociresult($stmt, "LOSS_C_PERCENT") ? ociresult($stmt, "LOSS_C_PERCENT") . "%" : "-") . "</td>
			<td align=right>" . (ociresult($stmt, "LOSS_H_PERCENT") ? ociresult($stmt, "LOSS_H_PERCENT") . "%" : "-") . "</td>

			<td class=tiny nowrap><a href=\"i_ingredient_details.php?ii=" . ociresult($stmt, "INGREDIENTS_ID") . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delii&ii=" . ociresult($stmt, "INGREDIENTS_ID") . "&g=" . $GLOBALS["curGroup"] . "&i=" . $curIngredient . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">" . getLM("delete") . "</a></td></tr>";
		}
		ocifreestatement($stmt);
	
		echo "</table>";

		echo "<h3>" . getLM("Recipe") . "</h3>";

		writeActionsBar(array(
			array(getLM("Edit recipe"), "i_recipe.php?id=" . $curIngredient, "images/icons/edit.gif"),
		));
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" . getLM("recipe") . "</th></tr><tr><td>" . nl2br($recipe) . "</td></tr>";
		echo "</table>";

	}
?>
</td>



</table>
<?
}

require($pathToSiteRoot . "common/template.php");
?>