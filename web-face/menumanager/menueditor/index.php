<?
$pathToSiteRoot = "../../";
$appName = "menumanager";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);


if ($_GET["act"] == "delm") {

	logTranBegin(58);

//	$strSQL = "DELETE FROM ksi.menu WHERE menu_id = " . (int)$_GET["id"];
	$strSQL = "UPDATE ksi.menu SET deleted = sysdate WHERE menu_id = " . (int)$_GET["id"];

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$e = oci_error($stmt);
		$alrtMsg = "Unexpected error. " . str_replace("\n", "  " , $e["message"]);
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=delm&/", "", $_SERVER["QUERY_STRING"]));
	}

}
elseif ($_GET["act"] == "delf") {

	logTranBegin(61);

	$strSQL = "DELETE FROM ksi.menu_item WHERE menu_item_id = " . (int)$_GET["id"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$e = oci_error($stmt);
		$alrtMsg = "unexpected error. " . str_replace("\n", "  " , $e["message"]);
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=delf&/", "", $_SERVER["QUERY_STRING"]));
	}


}
elseif ($_GET["act"] == "dele") {

	logTranBegin(64);

	$strSQL = "DELETE FROM ksi.menu_dish WHERE menu_item_id = " . (int)$_GET["f"] . " AND dish_id = " . (int)$_GET["e"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "unexpected error";
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=dele&/", "", $_SERVER["QUERY_STRING"]));
	}


}




function writeMainContent() {

?>
<table cellspacing=0 cellpadding=0 cellspacing=5 width="100%">
<td valign=top>
<h2><? echo getLM("Menus"); ?></h2>
<?

	$strSQL = "SELECT ksi.cafe.cafe_id, ksi.cafe.name AS cafe, ksi.cafe.type_id, ksi.menu.menu_id, ksi.menu.name AS menu, ksi.menu.pos
		FROM ksi.menu LEFT JOIN ksi.cafe ON ksi.menu.cafe_id = ksi.cafe.cafe_id 
		WHERE ksi.menu.deleted IS NULL AND ksi.cafe.deleted IS NULL
		ORDER BY LOWER(ksi.cafe.name), pos, LOWER(ksi.menu.name)";



	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	$GLOBALS["curMenu"] = 0;
	while (ocifetch($stmt)) {
		if (!$GLOBALS["curMenu"]) {
			$GLOBALS["curMenu"] = ociresult($stmt, "MENU_ID");
			$curMenuName = ociresult($stmt, "MENU");
			$curCafe = (int)ociresult($stmt, "CAFE_ID");
		}
		if ((int)$_GET["m"] == ociresult($stmt, "MENU_ID")) {
			$GLOBALS["curMenu"] = ociresult($stmt, "MENU_ID");
			$curMenuName = ociresult($stmt, "MENU");
			$curCafe = (int)ociresult($stmt, "CAFE_ID");
		}
		$arrCafe[] = array(ociresult($stmt, "CAFE_ID"), ociresult($stmt, "CAFE"), ociresult($stmt, "MENU_ID"), ociresult($stmt, "MENU"), ociresult($stmt, "TYPE_ID"), ociresult($stmt, "POS"));
	}
	ocifreestatement($stmt);

	writeActionsBar(array(
		array(getLM("Add new"), "menu_details.php?c=" . $curCafe, "images/icons/addfolder.gif"),
		array(getLM("Copy"), "menu_copy.php?id=" . $GLOBALS["curMenu"], "images/icons/copy.gif"),
		array(getLM("Edit/Move"), "menu_details.php?id=" . $GLOBALS["curMenu"], "images/icons/move.gif"),
		array(getLM("Delete"), $_SERVER["SCRIPT_NAME"] . "?act=delm&id=" . $GLOBALS["curMenu"], "images/icons/delete.gif", "onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\""),
	));




	if (!count($arrCafe)) {
		echo "There are no menus";
	}
	else {
		echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr><th>" .  getLM("menu") . "</th><th>" . getLM("position") . "</th><th>" . getLM("action") . "</th></tr>";
		$curCafe1 = -1;
		foreach ($arrCafe as $val) {
			if ($curCafe1 != $val[0]) {
				echo "<tr><td colspan=2><h3 style=\"margin-top: 10px; margin-left: -10px; margin-bottom: 0px\">" . ($val[1] ? "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/" . ($val[4] == 2 ? "cafe" : "laptop") . ".gif\" width=16 height=16 alt=\"\" align=absmiddle> " . $val[1] : "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/0.gif\" width=16 height=16 alt=\"\"> <font color=\"#808080\">menu templates</font>") . "</h3></td></tr>";
				$curCafe1 = $val[0];
			}
	
	
			echo "<tr" . ($val[2] == $GLOBALS["curMenu"] ? " class=\"selectedRow\"" : "") . "><td nowrap>";
			if ($val[2] == $GLOBALS["curMenu"]) {
				echo "<!--img src=\"" . $GLOBALS["pathToSiteRoot"] . "/images/icons/menu.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle-->" . $val[3];
			}
			else {
				echo "<a href=\"" . $_SERVER["SCRIPT_NAME"] . "?m=" . $val[2] . "\"><!--img src=\"" . $GLOBALS["pathToSiteRoot"] . "/images/icons/menu.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle-->" . $val[3] . "</a>";
			}
			echo "</td><td align=right>" . $val[5] . "</td>";
			echo "<td class=tiny nowrap><a href=\"menu_copy.php?id=" . $val[2] . "\">" . getLM("copy") . "</a> | <a href=\"menu_details.php?id=" . $val[2] . "\">" . getLM("edit/move") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delm&id=" . $val[2] . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">" . getLM("delete") . "</a></td></tr>";
		}
	
		echo "</table>";
	}
?>
</td>











<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<h2><? echo ucfirst($curMenuName) . ", " . (stristr($curMenuName, getLM("menu")) !== false ? "" : " " . getLM("menu")) . " " . getLM("folders"); ?></h2>
<?




class rTree2 extends rTree {

	function writeTreeExpanded($parentID, $level) {

		for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
			if ($GLOBALS["curMenuItem"] == $this->arrIdx[$parentID][$i]) {
				echo "<tr class=\"selectedRow\"><td style=\"padding-left: " . (10 + 20 * $level) . "px\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle>" . $this->arrTree[$this->arrIdx[$parentID][$i]][2];
			}
			else {
				echo "<tr><td style=\"padding-left:" . (10+ 20 * $level) . "px\"><a href=\"" . $_SERVER["SCRIPT_NAME"] . "?m=" . $GLOBALS["curMenu"] . "&f=" . $this->arrIdx[$parentID][$i] . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_c.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle>" . $this->arrTree[$this->arrIdx[$parentID][$i]][2] . "</a>";
			}
			echo "</td><td align=right style=\"padding-right:" . (40 - $level * 12 > 0 ? 40 - $level * 12 : 0) . "px\">" . $this->arrTree[$this->arrIdx[$parentID][$i]][3] . "</td>";

/*
			if ($this->arrTree[$this->arrIdx[$parentID][$i]][4]) {
				echo "<td align=center><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/0.gif\" width=12 height=12 alt=\"#" . $this->arrTree[$this->arrIdx[$parentID][$i]][4] . "\" style=\"border: 1px solid black; background-color: #" . $this->arrTree[$this->arrIdx[$parentID][$i]][4] . "\"></td>";
			}
			else {
				echo "<td >&nbsp;</td>";
			}
*/

			echo "<td class=tiny nowrap><a href=\"folder_copy.php?id=" . $this->arrIdx[$parentID][$i] . "\">" . getLM("copy") . "</a> | <a href=\"folder_details.php?id=" . $this->arrIdx[$parentID][$i] . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delf&id=" . $this->arrIdx[$parentID][$i] . "&m=" . $GLOBALS["curMenu"] . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">" . getLM("delete") . "</a></td></tr>";

			$this->writeTreeExpanded($this->arrIdx[$parentID][$i], $level+1);
		}
	}



	function passTreeCustom($parentID, $level) {

		for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
			if (!$GLOBALS["curMenuItem"]) {
				$GLOBALS["curMenuItem"] = $this->arrIdx[$parentID][$i];
				$GLOBALS["curMenuItemName"] = $this->arrTree[$this->arrIdx[$parentID][$i]][2];
			}

			if ($_GET["f"] == $this->arrIdx[$parentID][$i]) {
				$GLOBALS["curMenuItem"] = $this->arrIdx[$parentID][$i];
				$GLOBALS["curMenuItemName"] = $this->arrTree[$this->arrIdx[$parentID][$i]][2];
			}

			$this->passTreeCustom($this->arrIdx[$parentID][$i], $level+1);
		}
	}

}




	$menuTree = new rTree2("SELECT menu_item_id, parent_id, name, pos, color FROM ksi.menu_item WHERE ksi.menu_item.menu_id = " . (int)$GLOBALS["curMenu"] . " ORDER BY pos, name", $GLOBALS["objConn"]);


	$menuTree->passTreeCustom(0, 0);

	$tmpArr = array(array(getLM("Add new"), "folder_details.php?m=" . $GLOBALS["curMenu"], "images/icons/addfolder.gif"));

	if (count($menuTree->arrTree)) {
		array_push($tmpArr, 		
			array(getLM("Copy"), "folder_copy.php?id=" . $GLOBALS["curMenuItem"], "images/icons/copy.gif"),
			array(getLM("Edit"), "folder_details.php?id=" . $GLOBALS["curMenuItem"], "images/icons/edit.gif"),
			array(getLM("Delete"), $_SERVER["SCRIPT_NAME"] . "?act=delf&id=" . $GLOBALS["curMenuItem"] . "&m=" . $GLOBALS["curMenu"], "images/icons/delete.gif", "onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\""));
	}




	if (!count($arrCafe)) {
		echo "Create menu first";
	}
	else {
		writeActionsBar($tmpArr);

		echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr><th>" . getLM("menu folder") . "</th><th>" . getLM("position") . "</th><!-- th>color</th--><th>" . getLM("action") . "</th></tr>";
		$menuTree->writeTreeExpanded(0, 0);
		echo "</table>";
	}
?>
</td>








<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<h2><? echo ucfirst($GLOBALS["curMenuItemName"]) . ", " . getLM("folder entrees") ; ?></h2>
<?

	if (!$GLOBALS["curMenuItem"]) {
		echo getLM("Create menu folder first");
	}
	else {

		$strSQL = "SELECT ksi.dish.name AS dish, menu_dish.dish_id, menu_dish.price, ksi.menu_dish.pos 
			FROM ksi.menu_dish INNER JOIN ksi.dish ON ksi.menu_dish.dish_id = ksi.dish.dish_id
			WHERE ksi.menu_dish.menu_item_id = " . (int)$GLOBALS["curMenuItem"] . "
			ORDER BY pos, dish
		";
	
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
	
		$curDish = 0;
		while (ocifetch($stmt)) {
			if (!$curDish) {
				$curDish = ociresult($stmt, "DISH_ID");
				$curDishName = ociresult($stmt, "DISH");
			}
			if ((int)$_GET["e"] == ociresult($stmt, "DISH_ID")) {
				$curDish = ociresult($stmt, "DISH_ID");
				$curDishName = ociresult($stmt, "DISH");
			}
			$arrDish[] = array(ociresult($stmt, "DISH_ID"), ociresult($stmt, "DISH"), ociresult($stmt, "POS"), ociresult($stmt, "PRICE"));
		}
		ocifreestatement($stmt);
	
		$tmpStr = "";
		$tmpArr = array(array(getLM("Add new"), "entree_details.php?f=" . $GLOBALS["curMenuItem"], "images/icons/additem.gif"));
	
		if (count($arrDish)) {
			foreach ($arrDish as $val) {
				$tmpStr .= "<tr" . ($val[0] == $curDish ? " class=\"selectedRow\"" : "") . "><td>";
				if ($val[0] == $curDish) {
					$tmpStr .= $val[1];
				}
				else {
					$tmpStr .= "<a href=\"" . $_SERVER["SCRIPT_NAME"] . "?m=" . $GLOBALS["curMenu"] . "&f=" . $GLOBALS["curMenuItem"] . "&e=" . $val[0] . "\">" . $val[1] . "</a>";
				}
				$tmpStr .= "</td><td align=right>" . $val[2] . "</td><td align=right nowrap>" . ($GLOBALS["_langId"] == 0 ? "$" : "") . number_format($val[3], 2, ".", ",") . ($GLOBALS["_langId"] == 1 ? " руб." : "") . "</td>
				<td class=tiny nowrap><a href=\"entree_details.php?e=" . $val[0] . "&f=" . $GLOBALS["curMenuItem"] . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=dele&f=" . $GLOBALS["curMenuItem"] . "&e=" . $val[0] . "&m=" . $GLOBALS["curMenu"] . "\" onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\">" . getLM("delete") . "</a></td></tr>";
			}

			array_push($tmpArr, 		
				array(getLM("Edit"), "entree_details.php?f=" . $GLOBALS["curMenuItem"] . "&e=" . $curDish, "images/icons/edit.gif"),
				array(getLM("Delete"), $_SERVER["SCRIPT_NAME"] . "?act=dele&f=" . $GLOBALS["curMenuItem"] . "&e=" . $curDish . "&m=" . $GLOBALS["curMenu"], "images/icons/delete.gif", "onclick=\"return confirm(" . getLM("'Are you sure you want to delete this record?'") . ")\"")
			);
	 
		}
	
		writeActionsBar($tmpArr);
	
		echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr><th>" . getLM("entree") . "</th><th>" . getLM("position") . "</th><th>" . getLM("price") . "</th><th>" . getLM("action") . "</th></tr>";
		echo $tmpStr;
		echo "</table>";
	}
?>
</td>








<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<? 
	if (!$curDish) {
		echo getLM("Add an entree to this menu folder first");
	}
	else {
?>
<h2><? echo ucfirst($curDishName) . " " . getLM("assigned modifiers") ; ?></h2>
<?
		$strSQL = "SELECT ksi.modifier.modifier_id
				, ksi.modifier.name
				, ksi.mod_group.name AS modgroup
				, ksi.menu_dish_modifier.price
				, ksi.menu_dish_modifier.active
			FROM ksi.modifier 
				INNER JOIN ksi.mod_group ON ksi.mod_group.group_id = ksi.modifier.group_id
				INNER JOIN ksi.menu_dish_modifier ON ksi.menu_dish_modifier.modifier_id = ksi.modifier.modifier_id 
					AND ksi.menu_dish_modifier.dish_id = " . $curDish . " 
					AND ksi.menu_dish_modifier.menu_item_id = " . $GLOBALS["curMenuItem"] . "
			ORDER BY ksi.mod_group.name, ksi.modifier.pos";


		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
		
		$nrows = OCIFetchStatement($stmt, $result);
		ocifreestatement($stmt);

		if (!$nrows) {
			echo "<p>There are no assigned modifiers to " . $curDishName . " entree.</p><p>Use recipe builder application to assign it.</p>";
		}
		else {
			echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"  style=\"margin-top: 44px;\">
<tr><th>" . getLM("modifier") . "</th><th>" . getLM("price") . "</th><th>" . getLM("action") . "</th></tr>";
			$curG = "";
			for ( $i = 0; $i < $nrows; $i++ ) {
				if ($curG != $result["MODGROUP"][$i]) {
					echo "<tr><td colspan=4><h3 style=\"margin-top: 10px; margin-bottom: 0px\">" . $result["MODGROUP"][$i] . "</h3></td></tr>";
					$curG = $result["MODGROUP"][$i];
				}
				echo "<tr" . ($result["ACTIVE"][$i] ? "" : " style=\"color: #808080\"") . ">";
				echo "<td><input type=checkbox onclick=\"return false;\"" . ($result["ACTIVE"][$i] ? " checked" : "") . ">&nbsp;&nbsp;";
				echo "" . $result["NAME"][$i] . "</td>";
				echo "<td align=right nowrap>" . ($result["PRICE"][$i] ? ($GLOBALS["_langId"] == 0 ? " $" : "") . number_format($result["PRICE"][$i], 2, ".", ",") . ($GLOBALS["_langId"] == 1 ? " руб." : "") : "-") . "</td>";
				echo "<td class=tiny><a href=\"mod_details.php?e=" . $curDish . "&f=" . $GLOBALS["curMenuItem"] . "&md=" . $result["MODIFIER_ID"][$i] . "\">" . getLM("edit") . "</a></td>";
				echo "</tr>";
			}
	
	
			echo "</table>";
		}
	}
	
?>
</td>
</table>
<?
}

require($pathToSiteRoot . "common/template.php");
