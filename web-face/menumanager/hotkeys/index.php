<?
$pathToSiteRoot = "../../";
$appName = "menumanager";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);







function writeMainContent() {

?>
<table cellspacing=0 cellpadding=0 cellspacing=5 width="100%">
<td valign=top>
<h2>Menus</h2>
<?

	$strSQL = "SELECT ksi.cafe.cafe_id, ksi.cafe.name AS cafe, ksi.cafe.type_id, ksi.menu.menu_id, ksi.menu.name AS menu
		FROM ksi.menu LEFT JOIN ksi.cafe ON ksi.menu.cafe_id = ksi.cafe.cafe_id 
		WHERE ksi.menu.deleted IS NULL AND ksi.cafe.deleted IS NULL
		ORDER BY LOWER(ksi.cafe.name), LOWER(ksi.menu.name)";



	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	$curMenu = 0;
	while (ocifetch($stmt)) {
		if (!$curMenu) {
			$curMenu = ociresult($stmt, "MENU_ID");
			$curMenuName = ociresult($stmt, "MENU");
			$curCafe = (int)ociresult($stmt, "CAFE_ID");
		}
		if ((int)$_GET["m"] == ociresult($stmt, "MENU_ID")) {
			$curMenu = ociresult($stmt, "MENU_ID");
			$curMenuName = ociresult($stmt, "MENU");
			$curCafe = (int)ociresult($stmt, "CAFE_ID");
		}
		$arrCafe[] = array(ociresult($stmt, "CAFE_ID"), ociresult($stmt, "CAFE"), ociresult($stmt, "MENU_ID"), ociresult($stmt, "MENU"), ociresult($stmt, "TYPE_ID"));
	}
	ocifreestatement($stmt);



	echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr><th>" . getLM("menu") . "</th></tr>";
	$curCafe1 = -1;
	if (count($arrCafe)) foreach ($arrCafe as $val) {
		if ($curCafe1 != $val[0]) {
			echo "<tr><td colspan=2><h3 style=\"margin-top: 10px; margin-left: -10px; margin-bottom: 0px\">" . ($val[1] ? "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/" . ($val[4] == 2 ? "cafe" : "laptop") . ".gif\" width=16 height=16 alt=\"\" align=absmiddle> " . $val[1] : "<img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/0.gif\" width=16 height=16 alt=\"\"> <font color=\"#808080\">" . getLM("menu templates") . "</font>") . "</h3></td></tr>";
			$curCafe1 = $val[0];
		}


		echo "<tr" . ($val[2] == $curMenu ? " class=\"selectedRow\"" : "") . "><td nowrap>";
		if ($val[2] == $curMenu) {
			echo "<!--img src=\"" . $GLOBALS["pathToSiteRoot"] . "/images/icons/menu.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle-->" . $val[3];
		}
		else {
			echo "<a href=\"" . $_SERVER["SCRIPT_NAME"] . "?m=" . $val[2] . "\"><!--img src=\"" . $GLOBALS["pathToSiteRoot"] . "/images/icons/menu.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 10px\" align=absmiddle-->" . $val[3] . "</a>";
		}

		echo "</td></tr>";
	}

	echo "</table>";
?>
</td>











<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<h2><? echo ucfirst($curMenuName) . (stristr($curMenuName, getLM("menu")) !== false ? "" : " " . getLM("menu")); ?>, <? echo getLM("cashier hotkeys"); ?></h2>
<?

	if ($curMenu) {

		$strSQL = "SELECT ksi.dish.hotkey_color, ksi.menu_dish.hotkey_id, ksi.dish.name, ksi.menu_dish.menu_item_id
			FROM ksi.menu_dish 
				INNER JOIN ksi.menu_item ON ksi.menu_dish.menu_item_id = ksi.menu_item.menu_item_id
				INNER JOIN ksi.dish ON ksi.dish.dish_id = ksi.menu_dish.dish_id
			WHERE  ksi.menu_item.menu_id = " . (int)$curMenu . " AND ksi.menu_dish.hotkey_id IS NOT NULL
			ORDER BY hotkey_id";
	
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
	
		$arrHK = array();
		while (ocifetch($stmt)) {
				$arrHK[ociresult($stmt, "HOTKEY_ID") - 1] = array(ociresult($stmt, "NAME"), ociresult($stmt, "HOTKEY_COLOR"), ociresult($stmt, "MENU_ITEM_ID"));
		}
		ocifreestatement($stmt);
	
	
		echo "<table>";
		for ($i = 0; $i < 40; $i++) {
			if (!($i % 8)) {
				echo ($i ? "</tr>" : "") . "<tr>";
			}
			if ($arrHK[$i]) {
				echo "<td align=center style=\"border: 1px solid black; width: 80px; height: 80px; cursor: hand;" . (strlen($arrHK[$i][1]) ? " background-color:#" . substr($arrHK[$i][1] . "000000", 0, 6) . ";" . ((hexdec(substr($arrHK[$i][1] . "000000", 0, 2)) + hexdec(substr($arrHK[$i][1] . "000000", 2, 2)) + hexdec(substr($arrHK[$i][1] . "000000", 4, 2))) / 3 < 128 ? "color: white;" : "") : "") . "\" onclick=\"location.href='hotkey_details.php?k=" . ($i + 1) . "&m=" . $curMenu . "'\">" . $arrHK[$i][0] . "</td>";
	//			echo "<td align=center style=\"border: 1px solid black; width: 80px; height: 80px; cursor: hand;" . (strlen($arrHK[$i][1]) ? " background-color:#" . $arrHK[$i][1] . ";" . ((hexdec(substr($arrHK[$i][1], 0, 2)) + hexdec(substr($arrHK[$i][1], 2, 2)) + hexdec(substr($arrHK[$i][1], 4, 2))) / 3 < 128 ? "color: white;" : "") : "") . "\" onclick=\"location.href='hotkey_details.php?k=" . ($i + 1) . "&m=" . $curMenu . "'\">" . $arrHK[$i][0] . "</td>";
			}
			else {
				echo "<td align=center style=\"border: 1px solid silver; width: 80px; height: 80px; color: gray; cursor: hand;\" onclick=\"location.href='hotkey_details.php?k=" . ($i + 1) . "&m=" . $curMenu . "'\">empty</td>";
			}
	
		}
		echo "</tr></table>";
	}
	else {
		echo "create a menu first";
	}
?>
</td>
</table>
<?
}

require($pathToSiteRoot . "common/template.php");
