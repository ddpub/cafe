<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);







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


}




function getTreePath($tree, $el) {

	$arr = array();

	$curEl = $el;

	while ((int)$curEl) {
		$arr []= array($curEl, $tree->arrTree[$curEl]["2"]);
		$curEl = $tree->arrTree[$curEl]["1"];
	} 

	return $arr;
}




function hiLite($string, $lite) {
	$lite = substr(stristr($string, $lite), 0, strlen($lite));

	return str_ireplace($lite, "<font style=\"background-color:#ffffc0\">" . $lite . "</font>", $string);
}



function writeMainContent() {


	$areaArr = array(
		array("Items (Products)", "SELECT product_id AS id, name, group_id, 0 AS deleted FROM ksi.product WHERE LOWER(name) LIKE '%' || LOWER('" . sqlFixUp($_GET["srchstring"]) . "') || '%' ORDER BY name", "product"), 
		array("Cooked Ingredents", "SELECT ingredient_id AS id, name, group_id, 0 AS deleted FROM ksi.ingredient WHERE LOWER(name) LIKE '%' || LOWER('" . sqlFixUp($_GET["srchstring"]) . "') || '%' ORDER BY name", "cooked ingredient"), 
		array("Entrees", "SELECT dish_id AS id, name, group_id, deleted FROM ksi.dish WHERE LOWER(name) LIKE '%' || LOWER('" . sqlFixUp($_GET["srchstring"]) . "') || '%' ORDER BY name", "entree"),
		array("Modifiers", "SELECT modifier_id AS id, modifier.name, modifier.group_id, 0 AS deleted, ksi.mod_group.name AS group_name FROM ksi.modifier INNER JOIN ksi.mod_group ON modifier.group_id = mod_group.group_id WHERE LOWER(modifier.name) LIKE '%' || LOWER('" . sqlFixUp($_GET["srchstring"]) . "') || '%' AND modifier.deleted is NULL ORDER BY name", "modifier")
		 )

?>
<table cellspacing=0 cellpadding=0 cellspacing=5 width="100%">
<tr>
<td valign=top >
<form name="srch" action="<? echo $_SERVER["SCRIPT_NAME"]?>" method="get" onsubmit="return checkform();">
<input type=text name="srchstring" value="<? echo $_GET["srchstring"];?>"> <input type=submit name=submit value="<? echo getLM("Search") ?>"><br>

<h3><? echo getLM("Where to search"); ?>:</h3>
<table><td>
<input type=checkbox name="area[]" value=0<? echo @in_array(0, $_GET["area"]) ? " checked" : "";?>> <? echo getLM("Items"); ?><br>
<input type=checkbox name="area[]" value=1<? echo @in_array(1, $_GET["area"]) ? " checked" : "";?>> <? echo getLM("Ingredients"); ?><br>
<input type=checkbox name="area[]" value=2<? echo @(in_array(2, $_GET["area"]) || !isset($_GET["submit"])) ? " checked" : "";?>> <? echo getLM("Entrees"); ?><br>
<input type=checkbox name="area[]" value=3<? echo @in_array(3, $_GET["area"]) ? " checked" : "";?>> <? echo getLM("Modifiers"); ?>
</td><td valign=top>&nbsp;&nbsp;&nbsp;</td><td valign=top>
<input type=checkbox name="inside" value=1<? echo $_GET["inside"] ? " checked" : "";?>> <? echo getLM("Search in item details"); ?>
</td>
</table>
<?



if (($_GET["srchstring"])) {
?>
<h2><? echo getLM("Search results"); ?>:</h2>
<?
	$tree0 = new rTree2("SELECT group_id, parent_id, name FROM ksi.product_group ORDER BY name", $GLOBALS["objConn"]);
	$tree1 = new rTree2("SELECT group_id, parent_id, name FROM ksi.ingredient_group ORDER BY name", $GLOBALS["objConn"]);
	$tree2 = new rTree2("SELECT group_id, parent_id, name FROM ksi.dish_group ORDER BY name", $GLOBALS["objConn"]);

	foreach ($_GET["area"] as $val) {

		$strSQL = $areaArr[$val][1];
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);

		$str = "<table class=\"grid\" cellpadding=1 cellspacing=0 border=0><tr><th>" . getLM("group") . "</th><th>" . getLM($areaArr[$val][2]) . "</th></tr>";

		$num = 0;
		while (ocifetch($stmt)) {

			switch ($val) {


				case 0:
					$num++;

					$str .= "<tr><td>";
					$arr = getTreePath($tree0, ociresult($stmt, "GROUP_ID"));


					$str2 = "";
					for ($i = count($arr) - 1; $i >= 0; $i--) {
						$str2 .= "<a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/items/index.php?g=" . $arr[$i][0] . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle>" . $arr[$i][1] . "</a> " . ($i ? " &nbsp;&nbsp;\&nbsp;&nbsp; " : "");
					}

					$str .= $str2 . "</td><td>" . (ociresult($stmt, "DELETED") ? "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/trashbin.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle title=\"deleted on " . ociresult($stmt, "DELETED") . "\">" : "") . hiLite(ociresult($stmt, "NAME"), $_GET["srchstring"]) . "</td></tr>";

					break;




				case 1:
					$num++;
					$str .= "<tr><td>";
					$arr = getTreePath($tree1, ociresult($stmt, "GROUP_ID"));

					$str2 = "";
					for ($i = count($arr) - 1; $i >= 0; $i--) {
						$str2 .= "<a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/ingredients/index.php?g=" . $arr[$i][0] . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle>" . $arr[$i][1] . "</a> " . ($i ? " &nbsp;&nbsp;\&nbsp;&nbsp; " : "");
					}

					$str .= $str2 . "</td><td><a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/ingredients/index.php?g=" . ociresult($stmt, "GROUP_ID") . "&i=" . ociresult($stmt, "ID") . (ociresult($stmt, "DELETED") ? "&showdeleted" : "") . "\">" . (ociresult($stmt, "DELETED") ? "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/trashbin.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle title=\"deleted on " . ociresult($stmt, "DELETED") . "\">" : "") . hiLite(ociresult($stmt, "NAME"), $_GET["srchstring"]) . "</a></td></tr>";

					break;




				case 2:
					$num++;
					$str .= "<tr><td>";
					$arr = getTreePath($tree2, ociresult($stmt, "GROUP_ID"));

					$str2 = "";
					for ($i = count($arr) - 1; $i>=0; $i--) {
						$str2 .= "<a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/entrees/index.php?g=" . $arr[$i][0] . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle>" . $arr[$i][1] . "</a> " . ($i ? " &nbsp;&nbsp;\&nbsp;&nbsp; " : "");
					}

					$str .= $str2 . "</td><td><a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/entrees/index.php?g=" . ociresult($stmt, "GROUP_ID") . "&e=" . ociresult($stmt, "ID") . (ociresult($stmt, "DELETED") ? "&showdeleted" : "") . "\">" . (ociresult($stmt, "DELETED") ? "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/trashbin.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle title=\"Deleted on " . ociresult($stmt, "DELETED") . "\">" : "") . hiLite(ociresult($stmt, "NAME"), $_GET["srchstring"]) . "</a></td></tr>";
					break;




				case 3:
					$num++;
					$str .= "<tr><td>";
					$str2 = "<a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/modifiers/index.php?g=" . ociresult($stmt, "GROUP_ID") . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle>" . ociresult($stmt, "GROUP_NAME") . "</a> ";

					$str .= $str2 . "</td><td>" . (ociresult($stmt, "DELETED") ? "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/trashbin.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle title=\"" . getLM("deleted on") . " " . ociresult($stmt, "DELETED") . "\">" : "") . hiLite(ociresult($stmt, "NAME"), $_GET["srchstring"]) . "</td></tr>";
					break;

			}
		}




// looking inside
		if ($_GET["inside"]) {

//die("here");

			switch ($val) {
	

				case 1:
// looking in products
					$strSQL2 = "SELECT ingredient.ingredient_id, ingredient.name AS ingredient_name
							, ingredient.group_id, product.product_id AS id, product.name, product.group_id AS product_group_id
						FROM ksi.product 
							INNER JOIN ksi.ingredient_product ON ingredient_product.product_id = product.product_id
							INNER JOIN ksi.ingredient ON ingredient.ingredient_id = ingredient_product.ingredient_id
						WHERE LOWER(product.name) LIKE '%' || LOWER('" . sqlFixUp($_GET["srchstring"]) . "') || '%' 
						ORDER BY ingredient.name";

					$stmt2 = ociparse($GLOBALS["objConn"], $strSQL2);
					ociexecute($stmt2, OCI_DEFAULT);

					$num2 = 0;
					$str .= "<tr><td colspan=2>searching in details:</td></tr>";
					while (ocifetch($stmt2)) {
						$num++;
						$num2 ++;
						$arr = getTreePath($tree1, ociresult($stmt2, "GROUP_ID"));
						$str .= "<tr><td>";

						$str2 = "";
						for ($i = count($arr) - 1; $i>=0; $i--) {
							$str2 .= "<a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/ingredients/index.php?g=" . $arr[$i][0] . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle>" . $arr[$i][1] . "</a> " . ($i ? " &nbsp;&nbsp;\&nbsp;&nbsp; " : "");
						}
	
						$str .= $str2 . "</td><td><a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/ingredients/index.php?g=" . ociresult($stmt2, "GROUP_ID") . "&i=" . ociresult($stmt2, "ID") . "\">" . hiLite(ociresult($stmt2, "INGREDIENT_NAME"), $_GET["srchstring"]) . "</a> <span class=tiny>(" . getLM("item") . " &ldquo;" . hiLite(ociresult($stmt2, "NAME"), $_GET["srchstring"]) . "&rdquo; " . getLM("is the part of it") . ")</span></td></tr>";
					}
					ocifreestatement($stmt2);



// looking in ingredients
					$strSQL2 = "SELECT ingredient.ingredient_id AS id
							, ingredient.name
							, ingredient.group_id
							, base_ingredient.ingredient_id AS base_ingredient_id
							, base_ingredient.name AS base_ingredient_name
							, base_ingredient.group_id AS base_group_id
						FROM ksi.ingredients 
							INNER JOIN ksi.ingredient ON ingredient.ingredient_id = ingredients.ingredient_id
							INNER JOIN ksi.ingredient base_ingredient ON ingredients.base_ingredient_id = base_ingredient.ingredient_id
						WHERE LOWER(ingredient.name) LIKE '%' || LOWER('" . sqlFixUp($_GET["srchstring"]) . "') || '%' 
						ORDER BY ingredient.name";

					$stmt2 = ociparse($GLOBALS["objConn"], $strSQL2);
					ociexecute($stmt2, OCI_DEFAULT);

					while (ocifetch($stmt2)) {
						$num++;
						$num2 ++;
						$arr = getTreePath($tree1, ociresult($stmt2, "BASE_GROUP_ID"));
						$str .= "<tr><td>";

						$str2 = "";
						for ($i = count($arr) - 1; $i>=0; $i--) {
							$str2 .= "<a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/ingredients/index.php?g=" . $arr[$i][0] . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle>" . $arr[$i][1] . "</a> " . ($i ? " &nbsp;&nbsp;\&nbsp;&nbsp; " : "");
						}
	
						$str .= $str2 . "</td><td><a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/ingredients/index.php?g=" . ociresult($stmt2, "BASE_GROUP_ID") . "&i=" . ociresult($stmt2, "BASE_INGREDIENT_ID") . "\">" . hiLite(ociresult($stmt2, "BASE_INGREDIENT_NAME"), $_GET["srchstring"]) . "</a> <span class=tiny>(" . getLM("ingredient") . " &ldquo;" . hiLite(ociresult($stmt2, "NAME"), $_GET["srchstring"]) . "&rdquo; " . getLM("is the part of it") . ")</span></td></tr>";
					}
					ocifreestatement($stmt2);




					break;



				case 2:


// looking in products
					$strSQL2 = "SELECT dish.dish_id, dish.name AS dish_name, dish.deleted
							, dish.group_id, product.product_id AS id, product.name, product.group_id AS product_group_id
						FROM ksi.product 
							INNER JOIN ksi.dish_product ON dish_product.product_id = product.product_id
							INNER JOIN ksi.dish ON dish.dish_id = dish_product.dish_id
						WHERE LOWER(product.name) LIKE '%' || LOWER('" . sqlFixUp($_GET["srchstring"]) . "') || '%' 
						ORDER BY dish.name";

					$stmt2 = ociparse($GLOBALS["objConn"], $strSQL2);
					ociexecute($stmt2, OCI_DEFAULT);

					$num2 = 0;
					$str .= "<tr><td colspan=2>searching in details:</td></tr>";
					while (ocifetch($stmt2)) {
						$num++;
						$num2 ++;
						$arr = getTreePath($tree2, ociresult($stmt2, "GROUP_ID"));
						$str .= "<tr><td>";

						$str2 = "";
						for ($i = count($arr) - 1; $i>=0; $i--) {
							$str2 .= "<a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/entrees/index.php?g=" . $arr[$i][0] . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle>" . $arr[$i][1] . "</a> " . ($i ? " &nbsp;&nbsp;\&nbsp;&nbsp; " : "");
						}
	
						$str .= $str2 . "</td><td><a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/entrees/index.php?g=" . ociresult($stmt2, "GROUP_ID") . "&e=" . ociresult($stmt2, "ID") . (ociresult($stmt2, "DELETED") ? "&showdeleted" : "") . "\">" . (ociresult($stmt2, "DELETED") ? "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/trashbin.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle title=\"Deleted on " . ociresult($stmt2, "DELETED") . "\">" : "") . hiLite(ociresult($stmt2, "DISH_NAME"), $_GET["srchstring"]) . "</a> <span class=tiny>(" . getLM("item") . " &ldquo;" . hiLite(ociresult($stmt2, "NAME"), $_GET["srchstring"]) . "&rdquo; " . getLM("is the part of it") . ")</span></td></tr>";
					}
					ocifreestatement($stmt2);





// looking in ingredients
					$strSQL2 = "SELECT dish.dish_id, dish.name AS dish_name, dish.deleted
							, dish.group_id, ingredient.ingredient_id AS id, ingredient.name, ingredient.group_id AS ingredient_group_id
						FROM ksi.ingredient 
							INNER JOIN ksi.dish_ingredient ON dish_ingredient.ingredient_id = ingredient.ingredient_id
							INNER JOIN ksi.dish ON dish.dish_id = dish_ingredient.dish_id
						WHERE LOWER(ingredient.name) LIKE '%' || LOWER('" . sqlFixUp($_GET["srchstring"]) . "') || '%' 
						ORDER BY dish.name";

					$stmt2 = ociparse($GLOBALS["objConn"], $strSQL2);
					ociexecute($stmt2, OCI_DEFAULT);

					$num2 = 0;
					while (ocifetch($stmt2)) {
						$num++;
						$num2++;
						$arr = getTreePath($tree2, ociresult($stmt2, "GROUP_ID"));
						$str .= "<tr><td>";

						$str2 = "";
						for ($i = count($arr) - 1; $i>=0; $i--) {
							$str2 .= "<a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/entrees/index.php?g=" . $arr[$i][0] . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle>" . $arr[$i][1] . "</a> " . ($i ? " &nbsp;&nbsp;\&nbsp;&nbsp; " : "");
						}
	
						$str .= $str2 . "</td><td><a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/entrees/index.php?g=" . ociresult($stmt2, "GROUP_ID") . "&e=" . ociresult($stmt2, "ID") . (ociresult($stmt2, "DELETED") ? "&showdeleted" : "") . "\">" . (ociresult($stmt2, "DELETED") ? "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/trashbin.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle title=\"Deleted on " . ociresult($stmt2, "DELETED") . "\">" : "") . hiLite(ociresult($stmt2, "DISH_NAME"), $_GET["srchstring"]) . "</a> <span class=tiny>(" . getLM("ingredient") . " &ldquo;" . hiLite(ociresult($stmt2, "NAME"), $_GET["srchstring"]) . "&rdquo; " . getLM("is the part of it") . ")</span></td></tr>";
					}
					ocifreestatement($stmt2);



// looking in entrees
					$strSQL2 = "SELECT dish2.*
							, dish.dish_id AS base_dish_id
							, dish.name AS base_dish_title
							, deleted AS base_dish_deleted 
							, group_id AS base_group_id
						FROM ksi.dish 
							INNER JOIN ksi.subdish ON dish.dish_id = subdish.base_dish_id 
							INNER JOIN ksi.dish dish2 ON dish2.dish_id = subdish.dish_id
						WHERE LOWER(dish2.name) LIKE '%' || LOWER('" . sqlFixUp($_GET["srchstring"]) . "') || '%' 
						ORDER BY dish.name";

					$stmt2 = ociparse($GLOBALS["objConn"], $strSQL2);
					ociexecute($stmt2, OCI_DEFAULT);

					$num2 = 0;
					while (ocifetch($stmt2)) {
						$num++;
						$num2 ++;
						$arr = getTreePath($tree2, ociresult($stmt2, "BASE_GROUP_ID"));
						$str .= "<tr><td>";


						$str2 = "";
						for ($i = count($arr) - 1; $i>=0; $i--) {
							$str2 .= "<a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/entrees/index.php?g=" . $arr[$i][0] . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle>" . $arr[$i][1] . "</a> " . ($i ? " &nbsp;&nbsp;\&nbsp;&nbsp; " : "");
						}
	
						$str .= $str2 . "</td><td><a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/entrees/index.php?g=" . ociresult($stmt2, "GROUP_ID") . "&e=" . ociresult($stmt2, "ID") . (ociresult($stmt2, "DELETED") ? "&showdeleted" : "") . "\">" . (ociresult($stmt2, "DELETED") ? "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/trashbin.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle title=\"Deleted on " . ociresult($stmt2, "DELETED") . "\">" : "") . hiLite(ociresult($stmt2, "NAME"), $_GET["srchstring"]) . "</a> <span class=tiny>(" . getLM("entree") . " &ldquo;" . hiLite(ociresult($stmt2, "NAME"), $_GET["srchstring"]) . "&rdquo; " . getLM("is the part of it") . ")</span></td></tr>";



					}

					ocifreestatement($stmt2);
					break;


					case 3:
// searching in entrees
						$strSQL2 = "SELECT modifier.modifier_id, modifier.name AS modifier_name, modifier.deleted
								, mod_group.group_id, mod_group.name AS group_name
								, dish.name AS dish_name, dish.dish_id
							FROM ksi.modifier 
								INNER JOIN ksi.dish ON modifier.dish_id = dish.dish_id
								INNER JOIN ksi.mod_group ON mod_group.group_id = modifier.group_id
							WHERE LOWER(dish.name) LIKE '%' || LOWER('" . sqlFixUp($_GET["srchstring"]) . "') || '%' 
						ORDER BY dish.name";

					$stmt2 = ociparse($GLOBALS["objConn"], $strSQL2);
					ociexecute($stmt2, OCI_DEFAULT);

					$num2 = 0;
					$str .= "<tr><td colspan=2>searching in details:</td></tr>";

					while (ocifetch($stmt2)) {
						$num++;
						$num2 ++;

						$str .= "<tr><td>";
						$str2 = "<a href=\"" . $GLOBALS["pathToSiteRoot"] . $GLOBALS["appName"] . "/modifiers/index.php?g=" . ociresult($stmt2, "GROUP_ID") . "\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_o.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle>" . ociresult($stmt2, "GROUP_NAME") . "</a> ";
	
						$str .= $str2 . "</td><td>" . (ociresult($stmt2, "DELETED") ? "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/trashbin.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 3px\" align=absmiddle title=\"" . getLM("deleted on") . " " . ociresult($stmt2, "DELETED") . "\">" : "") . hiLite(ociresult($stmt2, "MODIFIER_NAME"), $_GET["srchstring"]) . " <span class=tiny>(" . getLM("entree") . " &ldquo;" . hiLite(ociresult($stmt2, "DISH_NAME"), $_GET["srchstring"]) . "&rdquo; " . getLM("is the part of it") . ")</span></td></tr>";


					}


					break;

			}
		}







		$str .= "</table>";
		echo "<p><h3 style=\"display: inline;\">" . getLM($areaArr[$val][0]) . "</h3> &nbsp;&nbsp; " . getLM("found") . " " . $num . " item" . ($num == 1 ? "" : "s") . "</p>";
		if ($num) echo $str;
	}
	ocifreestatement($stmt);
}


?>
</form>
<script>
function checkform() {

	if (document.srch.srchstring.value == '') {
		alert('Specify the search string');
		document.srch.srchstring.focus();
		return false;
	}

	flag1 = 0;
	areaobj = document.srch('area[]');
	for (i = 0; i < areaobj.length; i++) {
		if (areaobj[i].checked) {
			flag1 = 1;
			break;
		}
	}
	
	if (!flag1) {
		alert('Select the search area');
		return false;
	}
	return true;

}
</script>
</td>
</tr>

</table>
<?
}

require($pathToSiteRoot . "common/template.php");
?>