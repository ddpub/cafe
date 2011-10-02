<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);

if ($_GET["act"] == "delg") {

	logTranBegin(22);

	$strSQL = "DELETE FROM ksi.dish_group WHERE group_id = " . (int)$_GET["id"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "Cannot delete entree group: its not empty";
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=delg&/", "", $_SERVER["QUERY_STRING"]));
	}


}
elseif ($_GET["act"] == "dele") {

	logTranBegin(25);

/*	$strSQL = "DELETE FROM ksi.dish WHERE dish_id = " . (int)$_GET["id"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {*/

		$strSQL = "UPDATE ksi.dish SET deleted = SYSDATE WHERE dish_id = " . (int)$_GET["id"];
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		if (!(int)ociexecute($stmt, OCI_DEFAULT)) {
	
			$alrtMsg = "Cannot mark entree deleted: unexpected error";
		}
		else {
			logTranEnd();
			header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=dele&/", "", $_SERVER["QUERY_STRING"]));
		}


/*	}*/

}
elseif ($_GET["act"] == "delit") {

	logTranBegin(49);

	$strSQL = "DELETE FROM ksi.dish_product WHERE dish_product_id = " . (int)$_GET["dp"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "Undexpected error, contact the developer";
	}
	else {
		logTranEnd();
//		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=delit&/", "", $_SERVER["QUERY_STRING"]));
	}


}
elseif ($_GET["act"] == "delei") {

	logTranBegin(28);

	$strSQL = "DELETE FROM ksi.dish_ingredient WHERE dish_ingredient_id = " . (int)$_GET["di"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "Undexpected error, contact the developer";
	}
	else {
		logTranEnd();
//		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=delei&/", "", $_SERVER["QUERY_STRING"]));
	}


}
elseif ($_GET["act"] == "delee") {

	logTranBegin(31);

	$strSQL = "DELETE FROM ksi.subdish WHERE base_dish_id = " . (int)$_GET["e"] . " AND dish_id = " . (int)$_GET["e2"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "Undexpected error, contact the developer";
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=delee&/", "", $_SERVER["QUERY_STRING"]));
	}


}
elseif ($_GET["act"] == "delem") {

	logTranBegin(34);

	$strSQL = "DELETE FROM ksi.dish_mod_group WHERE dish_id = " . (int)$_GET["e"] . " AND group_id = " . (int)$_GET["m"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "Undexpected error, contact the developer";
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=delem&/", "", $_SERVER["QUERY_STRING"]));
	}

}

elseif ($_GET["act"] == "undel") {

	logTranBegin(74);

	$strSQL = "UPDATE ksi.dish SET DELETED = NULL WHERE dish_id = " . (int)$_GET["id"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "Undexpected error, contact the developer";
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=undel&/", "", $_SERVER["QUERY_STRING"]));
	}

}
elseif ($_GET["act"] == "purge") {

	logTranBegin(75);

	$strSQL = "DELETE FROM ksi.dish WHERE dish_id = " . (int)$_GET["id"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "Could not purge the entree: it is being used";
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=purge&/", "", $_SERVER["QUERY_STRING"]));
	}

}








class rTree2 extends rTree {

	function writeTreeExpanded($parentID, $level) {

		for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
			if ($GLOBALS["curGroup"] == $this->arrIdx[$parentID][$i]) {
				echo "<tr class=\"selectedRow\"><td style=\"padding-left: " . (10 + 20 * $level) . "px\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle>" . $this->arrTree[$this->arrIdx[$parentID][$i]][2];
			}
			else {
				echo "<tr><td style=\"padding-left:" . (10+ 20 * $level) . "px\"><a href=\"" . $_SERVER["SCRIPT_NAME"] . "?g=" . $this->arrIdx[$parentID][$i] . ($showDeleted ? "&showdeleted" : "") . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_c.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle>" . $this->arrTree[$this->arrIdx[$parentID][$i]][2] . "</a>";
			}
			echo "</td><td align=right style=\"padding-right:" . (40 - $level * 12 > 0 ? 40 - $level * 12 : 0) . "px\">" . $this->arrTree[$this->arrIdx[$parentID][$i]][3] . "</td>";


			echo "<td class=tiny nowrap><a href=\"group_details.php?id=" . $this->arrIdx[$parentID][$i] . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delg&id=" . $this->arrIdx[$parentID][$i] . ($showDeleted ? "&showdeleted" : "") . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">" . getLM("delete") . "</a></td></tr>";

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

	if (/*in_array(1, $GLOBALS["_person_roles"]) &&*/ isset($_GET["showdeleted"])) {
		$showDeleted = true;
	}
	else {
		$showDeleted = false;
	}
	$GLOBALS["showDeleted"] = $showDeleted;

?>
<table cellspacing=0 cellpadding=0 cellspacing=5 width="100%">
<td valign=top>
<?
/*
	$strSQL = "SELECT group_id, name FROM ksi.dish_group ORDER BY name";
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);



	$GLOBALS["curGroup"] = 0;
	$arrGroup = array();


	while (ocifetch($stmt)) {
		if (!$GLOBALS["curGroup"]) {
			$GLOBALS["curGroup"] = ociresult($stmt, "GROUP_ID");
			$GLOBALS["curGroupName"] = ociresult($stmt, "NAME");
		}
		if ((int)$_GET["g"] == ociresult($stmt, "GROUP_ID")) {
			$GLOBALS["curGroup"] = ociresult($stmt, "GROUP_ID");
			$GLOBALS["curGroupName"] = ociresult($stmt, "NAME");
		}
		$arrGroup[] = array(ociresult($stmt, "GROUP_ID"), ociresult($stmt, "NAME"));
	}
	ocifreestatement($stmt);
*/


	$groupTree = new rTree2("SELECT group_id, parent_id, name FROM ksi.dish_group ORDER BY name", $GLOBALS["objConn"]);

	$groupTree->passTreeCustom(0, 0);


	echo "<h2>" . getLM("Entree groups") . "</h2>";

	$arrTmp = array(array(getLM("Add new group"), "group_details.php", "images/icons/addfolder.gif"));
	
	if (count($groupTree->arrTree)) {
		array_push($arrTmp, 
			array(getLM("Edit group"), "group_details.php?id=" . $GLOBALS["curGroup"], "images/icons/edit.gif"),
			array(getLM("Delete group"), $_SERVER["SCRIPT_NAME"] . "?act=delg&id=" . $GLOBALS["curGroup"] . ($showDeleted ? "&showdeleted" : ""), "images/icons/delete.gif", "onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\"")
		);
	}
	writeActionsBar($arrTmp);

	echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr><th colspan=2>" . getLM("entree group") . "</th><th>" . getLM("action") . "</th></tr>";

	$groupTree->writeTreeExpanded(0, 0);


	echo "</table>";
?>
</td>
<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<?






	$strSQL = "SELECT ksi.dish.dish_id, ksi.dish.name, ksi.dish.def_price, ksi.kitchen_type.name AS type, ksi.dish.recipe, ksi.dish.deleted, dish.hotkey_color, dish.code
	FROM ksi.dish
		LEFT JOIN ksi.kitchen_type ON ksi.kitchen_type.kt_id = ksi.dish.is_beverage

	WHERE ksi.dish.group_id = " . $GLOBALS["curGroup"] . ($showDeleted ? "" : " AND ksi.dish.deleted IS NULL " ) . "
	ORDER BY ksi.dish.name";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);


	$curDish = 0;
	$arrDish = array();


	while (ocifetch($stmt)) {
		if (!$curDish) {
			$curDish = ociresult($stmt, "DISH_ID");
			$curDishName = ociresult($stmt, "NAME");
			$curDishDel = ociresult($stmt, "DELETED");
			$recipe = ociresult($stmt, "RECIPE");
		}
		if ((int)$_GET["e"] == ociresult($stmt, "DISH_ID")) {
			$curDish = ociresult($stmt, "DISH_ID");
			$curDishName = ociresult($stmt, "NAME");
			$curDishDel = ociresult($stmt, "DELETED");
			$recipe = ociresult($stmt, "RECIPE");
		}
		$arrDish[] = array(ociresult($stmt, "DISH_ID"), ociresult($stmt, "NAME"), ociresult($stmt, "TYPE"), ociresult($stmt, "DEF_PRICE"), ociresult($stmt, "DELETED"), ociresult($stmt, "HOTKEY_COLOR"), ociresult($stmt, "CODE"));
	}
	ocifreestatement($stmt);

	echo "<h2>" . ucfirst($GLOBALS["curGroupName"]) . " " . (substr($GLOBALS["curGroupName"], strlen($GLOBALS["curGroupName"]) - strlen(getLM("entrees")), strlen(getLM("entrees"))) == getLM("entrees") ? "" : getLM("entrees")) . "</h2>";

	$arrTmp = array(array(getLM("Add new"), "entree_details.php?g=" . $GLOBALS["curGroup"], "images/icons/add.gif"));
	if (count($arrDish)) {
		if ($curDishDel) {
			array_push($arrTmp, 
				array(getLM("Undelete"), $_SERVER["SCRIPT_NAME"] . "?act=undel&id=" . $curDish . "&g=" . $GLOBALS["curGroup"] . ($showDeleted ? "&showdeleted" : ""), "images/icons/restore.gif"), 
				array(getLM("Purge"), $_SERVER["SCRIPT_NAME"] . "?act=purge&id=" . $curDish . "&g=" . $GLOBALS["curGroup"] . ($showDeleted ? "&showdeleted" : ""), "images/icons/purge.gif")
			);
		}
		else {
			array_push($arrTmp, 
				array(getLM("Add via copy"), "entree_copy.php?id=" . $curDish, "images/icons/copy.gif"), 
				array(getLM("Edit"), "entree_details.php?id=" . $curDish, "images/icons/edit.gif"), 
				array(getLM("Delete"), $_SERVER["SCRIPT_NAME"] . "?act=dele&id=" . $curDish . "&g=" . $GLOBALS["curGroup"] . ($showDeleted ? "&showdeleted" : ""), "images/icons/delete.gif", " onclick=\"return confirm('" . getLM("This will also remove the entree from all the menues. Are you sure you want to delete this record?") . "')\"")
			);
		}
	}

	if (in_array(1, $GLOBALS["_person_roles"])) {
		if ($showDeleted) {
			array_push($arrTmp, 
				array(getLM("Hide deleted entrees"), $_SERVER["SCRIPT_NAME"] . "?e=" . $curDish . "&g=" . $GLOBALS["curGroup"], "images/icons/trash_hide.gif")
			);
		}
		else {
			array_push($arrTmp, 
				array(getLM("Show deleted entrees"), $_SERVER["SCRIPT_NAME"] . "?showdeleted&e=" . $curDish . "&g=" . $GLOBALS["curGroup"], "images/icons/trashbin.gif")
			);

		}
	}

	writeActionsBar($arrTmp);

	echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
	echo "<tr><th>" . getLM("entree") . "</th><th>" . getLM("code") . "</th><th>" . getLM("where is prepared") . "</th><th>" . getLM("suggested price") . "</th><th>" . getLM("color") . "</th>" . ($showDeleted ? "<th>deleted</th>" : "") . "<th>" . getLM("action") . "</th></tr>";
	foreach ($arrDish as $val) {
		echo "<tr" . ($val[0] == $curDish ? " class=\"selectedRow\"" : "") . "><td>";
		if ($val[0] == $curDish) {
			echo ($val[4] ? "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/trashbin.gif\" width=16 height=16 border=0 align=absmiddle alt=\"this entree is deleted\">&nbsp;&nbsp;" : "") . $val[1];
		}
		else {
			echo ($val[4] ? "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/trashbin.gif\" width=16 height=16 border=0 align=absmiddle alt=\"this entree is deleted\">&nbsp;&nbsp;" : "") . "<a href=\"" . $_SERVER["SCRIPT_NAME"] . "?g=" . $GLOBALS["curGroup"] . "&e=" . $val[0] . ($showDeleted ? "&showdeleted" : "") . "\">" . $val[1] . "</a>";
		}
		echo "</td><td>" . $val[6] . "</td><td>" . $val[2] . "</td><td align=right>" . ($GLOBALS["_langId"] == 0 ? "$" : "") . number_format($val[3], 2, ".", ",") . ($GLOBALS["_langId"] == 1 ? " руб." : "") . "</td>";

		if ($val[5]) {
			echo "<td align=center><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/0.gif\" width=10 height=10 alt=\"#" . $val[5] . "\" style=\"border: 1px solid black; background-color: #" . $val[5] . "\"></td>";
		}
		else {
			echo "<td >&nbsp;</td>";
		}


		if ($showDeleted) {
			echo "<td nowrap class=tiny>" . $val[4] . "</td>";
		}

		echo "<td class=tiny nowrap>";

		if ($val[4]) {
			echo "<a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=undel&id=" . $val[0] . "&g=" . $GLOBALS["curGroup"] . ($showDeleted ? "&showdeleted" : "") . "\">" . getLM("undelete") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=purge&id=" . $val[0] . "&g=" . $GLOBALS["curGroup"] . ($showDeleted ? "&showdeleted" : "") . "\">" . getLM("purge") . "</a>";
		}
		else {
			echo "<a href=\"entree_details.php?id=" . $val[0] . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=dele&id=" . $val[0] . "&g=" . $GLOBALS["curGroup"] . ($showDeleted ? "&showdeleted" : "") . "\" onclick=\"return confirm('" . getLM("This will also remove the entree from all the menues. Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a>";
		}

		echo "</td></tr>";
	}

	echo "</table>";

?>

</td>
<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<?
	echo "<h2>" . ucfirst($curDishName) . " " . getLM("entree contents") . "</h2>";
	
	if (!$curDish) {
		echo getLM("Please create an entree first");
	}
	else {
		echo "<h3>" . getLM("Items") . "</h3>";

		$strSQL = "SELECT ksi.product.product_id
			, ksi.product.name
			, ksi.dish_product.amount
			, ksi.measure.shortname
			, ksi.dish_product.loss_c_percent
			, ksi.dish_product.loss_h_percent
			, ksi.dish_product.dish_product_id
			FROM ksi.dish_product 
				INNER JOIN ksi.product ON ksi.dish_product.product_id = ksi.product.product_id
				INNER JOIN ksi.measure ON ksi.dish_product.measure_id = ksi.measure.measure_id
			WHERE dish_id = " . $curDish . "
			ORDER BY ksi.product.name";

		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);

		writeActionsBar(array(
			array(getLM("Add new item"), "e_item_details.php?e=" . $curDish, "images/icons/additem.gif"),
		));
	
	
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" . getLM("item") . "</th><th colspan=2>" . getLM("qty") . "</th><th>" . getLM("weight loss, cold processing, %") . "</th><th>" . getLM("weight loss, hot processing, %") . "</th><th>" . getLM("action") . "</th></tr>";
		while (ocifetch($stmt)) {
			echo "<tr><td>";
//			echo "<a href=\"i_item_details.php?e=" . $curDish . "&it=" . ociresult($stmt, "PRODUCT_ID") . "\">" . ociresult($stmt, "NAME") . "</a>";
			echo ociresult($stmt, "NAME");
			echo "</td><td align=right  style=\"padding-right: 2px\">" . ociresult($stmt, "AMOUNT") . "</td>
			<td style=\"padding-left: 0px\">" . ociresult($stmt, "SHORTNAME") . "</td>
			<td align=right>" . (ociresult($stmt, "LOSS_C_PERCENT") ? ociresult($stmt, "LOSS_C_PERCENT") . "%" : "-") . "</td>
			<td align=right>" . (ociresult($stmt, "LOSS_H_PERCENT") ? ociresult($stmt, "LOSS_H_PERCENT") . "%" : "-") . "</td>
			<td class=tiny nowrap><a href=\"e_item_details.php?dp=" . ociresult($stmt, "DISH_PRODUCT_ID") . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delit&dp=" . ociresult($stmt, "DISH_PRODUCT_ID") . "&g=" . $GLOBALS["curGroup"] . "&e=" . $curDish . ($showDeleted ? "&showdeleted" : "") . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
		}
		ocifreestatement($stmt);
	
		echo "</table>";


		echo "<h3>" . getLM("Ingredients") . "</h3>";

		$strSQL = "SELECT ksi.ingredient.ingredient_id, ksi.ingredient.name, ksi.dish_ingredient.amount, ksi.measure.shortname 
			, ksi.dish_ingredient.loss_c_percent
			, ksi.dish_ingredient.loss_h_percent
			, ksi.dish_ingredient.dish_ingredient_id

			FROM ksi.dish_ingredient 
				INNER JOIN ksi.ingredient ON ksi.dish_ingredient.ingredient_id = ksi.ingredient.ingredient_id
				INNER JOIN ksi.measure ON ksi.dish_ingredient.measure_id = ksi.measure.measure_id
			WHERE dish_id = " . $curDish . "
			ORDER BY ksi.ingredient.name";

		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);

		writeActionsBar(array(
			array(getLM("Add new ingredient"), "e_ingredient_details.php?e=" . $curDish, "images/icons/additem.gif"),
		));
	
	
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" . getLM("ingredient") . "</th><th colspan=2>" . getLM("qty") . "</th><th>" . getLM("weight loss, cold processing, %") . "</th><th>" . getLM("weight loss, hot processing, %") . "</th><th>" . getLM("action") . "</th></tr>";
		while (ocifetch($stmt)) {
			echo "<tr><td>";
//			echo "<a href=\"e_ingredient_details.php?e=" . $curDish . "&i=" . ociresult($stmt, "INGREDIENT_ID") . "\">" . ociresult($stmt, "NAME") . "</a>";
			echo ociresult($stmt, "NAME");
			echo "</td><td align=right  style=\"padding-right: 2px\">" . ociresult($stmt, "AMOUNT") . "</td>
			<td style=\"padding-left: 0px\">" . ociresult($stmt, "SHORTNAME") . "</td>
			<td align=right>" . (ociresult($stmt, "LOSS_C_PERCENT") ? ociresult($stmt, "LOSS_C_PERCENT") . "%" : "-") . "</td>
			<td align=right>" . (ociresult($stmt, "LOSS_H_PERCENT") ? ociresult($stmt, "LOSS_H_PERCENT") . "%" : "-") . "</td>
			<td class=tiny nowrap><a href=\"e_ingredient_details.php?di=" . ociresult($stmt, "DISH_INGREDIENT_ID") . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delei&di=" . ociresult($stmt, "DISH_INGREDIENT_ID") . "&g=" . $GLOBALS["curGroup"] . "&e=" . $curDish . ($showDeleted ? "&showdeleted" : "") . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
		}
		ocifreestatement($stmt);
	
		echo "</table>";




		echo "<h3>" . getLM("Entrees") . "</h3>";

		$strSQL = "SELECT ksi.dish.dish_id, ksi.dish.name, ksi.subdish.count
			FROM ksi.subdish 
				INNER JOIN ksi.dish ON ksi.subdish.dish_id = ksi.dish.dish_id
			WHERE base_dish_id = " . $curDish . "
			ORDER BY ksi.dish.name";

		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);

		writeActionsBar(array(
			array(getLM("Add new entree"), "e_entree_details.php?e=" . $curDish, "images/icons/additem.gif"),
		));
	
	
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" . getLM("entree") . "</th><th>" . getLM("qty") . "</th><th>" . getLM("action") . "</th></tr>";
		while (ocifetch($stmt)) {
			echo "<tr><td>";
//			echo "<a href=\"e_entree_details.php?e=" . $curDish . "&e2=" . ociresult($stmt, "INGREDIENT_ID") . "\">" . ociresult($stmt, "NAME") . "</a>";
			echo ociresult($stmt, "NAME");
			echo "</td>
			<td align=right>" . ociresult($stmt, "COUNT") . "</td>
			<td class=tiny nowrap><a href=\"e_entree_details.php?e=" . $curDish . "&e2=" . ociresult($stmt, "DISH_ID") . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delee&e=" . $curDish . "&e2=" . ociresult($stmt, "DISH_ID") . "&g=" . $GLOBALS["curGroup"] . ($showDeleted ? "&showdeleted" : "") . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
		}
		ocifreestatement($stmt);
	
		echo "</table>";






		echo "<h3>" . getLM("Recipe") . "</h3>";

		writeActionsBar(array(
			array(getLM("Edit recipe"), "e_recipe.php?id=" . $curDish, "images/icons/edit.gif"),
		));
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" . getLM("recipe") . "</th></tr><tr><td>" . nl2br($recipe) . "</td></tr>";
		echo "</table>";


		echo "<h3>" . getLM("Assigned modifiers") . "</h3>";

		$strSQL = "SELECT ksi.mod_group.group_id, ksi.mod_group.name
			FROM ksi.mod_group
				INNER JOIN ksi.dish_mod_group ON ksi.dish_mod_group.group_id = ksi.mod_group.group_id
			WHERE dish_id = " . $curDish . "
			ORDER BY ksi.mod_group.pos, ksi.mod_group.name";

		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);

		writeActionsBar(array(
			array(getLM("Assign new modifier group"), "e_mod_details.php?e=" . $curDish, "images/icons/additem.gif"),
		));
	
	
		echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
		echo "<tr><th>" . getLM("modifier group") . "</th><th>" . getLM("action") . "</th></tr>";
		while (ocifetch($stmt)) {
			echo "<tr><td>";
//			echo "<a href=\"e_entree_details.php?e=" . $curDish . "&e2=" . ociresult($stmt, "INGREDIENT_ID") . "\">" . ociresult($stmt, "NAME") . "</a>";
			echo ociresult($stmt, "NAME");
			echo "</td>
			<td class=tiny nowrap><a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delem&e=" . $curDish . "&m=" . ociresult($stmt, "GROUP_ID") . "&g=" . $GLOBALS["curGroup"] . ($showDeleted ? "&showdeleted" : "") . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
		}
		ocifreestatement($stmt);
	
		echo "</table>";

	}




?>
</td>



</table>
<?
}

require($pathToSiteRoot . "common/template.php");
?>