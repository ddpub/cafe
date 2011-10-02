<?
$pathToSiteRoot = "../../";
$appName = "menumanager";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : "m=" . $_GET["m"] /*substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1)*/;

$pageTitle = getLM("Cashier hotkey details");

$basePage = "index.php";



$strSQL = "SELECT ksi.menu_dish.* 
FROM ksi.menu_dish 
	INNER JOIN ksi.menu_item ON ksi.menu_dish.menu_item_id = ksi.menu_item.menu_item_id
WHERE ksi.menu_item.menu_id = " . (int)$_GET["m"] . " AND ksi.menu_dish.hotkey_id =  " . (int)$_GET["k"];


$stmt = ociparse($GLOBALS["objConn"], $strSQL);
ociexecute($stmt, OCI_DEFAULT);
ocifetchinto($stmt, $result, OCI_ASSOC);

if ($_POST["submit"] == getLM("save")) {

	list($dish_id, $menu_item_id) = split("\|", $_POST["d"]);

	if ($_POST["clearhk"]) {
		logTranBegin(90);
		$strSQL = "UPDATE ksi.menu_dish SET hotkey_id = NULL, hotkey_color = NULL WHERE menu_item_id = " . $result["MENU_ITEM_ID"] . " AND dish_id = " . $result["DISH_ID"];
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
		ocicommit($objConn);
//pr($strSQL);
		logTranEnd();
	} 

	else {
		logTranBegin(89);
		if ($result) {
			if ($result["MENU_ITEM_ID"] != $menu_item_id || $result["DISH_ID"] != $dish_id) {

				$strSQL = "UPDATE ksi.menu_dish SET hotkey_id = NULL /*, hotkey_color = NULL*/ WHERE menu_item_id = " . $result["MENU_ITEM_ID"] . " AND dish_id = " . $result["DISH_ID"];
				$stmt = ociparse($GLOBALS["objConn"], $strSQL);
				ociexecute($stmt, OCI_DEFAULT);
die("ddd");
			}
		} 
		$strSQL = "UPDATE ksi.menu_dish SET hotkey_id = " . (int)$_GET["k"] . ", hotkey_color='" . sqlFixUp($_POST["hotkey_color"]) . "' WHERE menu_item_id = " . $menu_item_id . " AND dish_id = " . $dish_id;


		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
		ocicommit($objConn);


		logTranEnd();
	}

	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));


}



class rTree2 extends rTree {

	function writeTreeExpanded($parentID, $level) {

		for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {

			echo "<tr><td colspan=2 style=\"padding-left:" . (20 * $level) . "px\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_c.gif\" width=16 height=16 alt=\"\" border=0 style=\"margin-right: 5px\" align=absmiddle>" . $this->arrTree[$this->arrIdx[$parentID][$i]][2];
			writeDishes($this->arrIdx[$parentID][$i], $level);
			echo "</td></tr>";

			$this->writeTreeExpanded($this->arrIdx[$parentID][$i], $level+1);
		}
	}
}

function writeDishes($folder, $level) {
	global $arrDish;


	if (count($arrDish[$folder])) {
		foreach ($arrDish[$folder] as $val) {
			echo "<tr><td" . /*($val[2] && strlen($val[3]) ? " bgcolor=\"#" . substr($val[3] . "000000", 0, 6) . "\"" : "") .*/ "><input type=radio name=\"d\" value=\"" . $val[0] . "|" . $folder . "\"";
	
			if ($val[2]) {
				if ($val[2] == (int)$_GET["k"]) {
					echo " checked";
				}
				else {
					echo " disabled";
				}
			}
	
			echo "></td><td style=\"padding-left:" . (20 * $level) . "px\">" . $val[1] . "</td></tr>";
		}
	}
}

function writeMainContent() {
	global $arrDish, $arrHotkeyColor;


	$strSQL = "SELECT ksi.menu_item.menu_item_id
		, ksi.menu_item.parent_id
		, ksi.menu_item.name AS menu_item
	FROM ksi.menu_item 
	WHERE ksi.menu_item.menu_id = " . (int)$_GET["m"] . "
	ORDER BY menu_item.pos
	";

	$tree = new rTree2($strSQL, $GLOBALS["objConn"]);

	$strSQL = "SELECT ksi.dish.dish_id
			, ksi.dish.name
			, ksi.menu_dish.menu_item_id 
			, ksi.menu_dish.hotkey_color
			, ksi.menu_dish.hotkey_id
			, ksi.dish.hotkey_color AS dish_color

		FROM ksi.menu_item 
			INNER JOIN ksi.menu_dish ON ksi.menu_dish.menu_item_id = ksi.menu_item.menu_item_id
			INNER JOIN ksi.dish ON ksi.menu_dish.dish_id = ksi.dish.dish_id

		WHERE ksi.menu_item.menu_id = " . (int)$_GET["m"] . " AND ksi.dish.deleted IS NULL
		ORDER BY ksi.menu_item.pos, ksi.menu_dish.pos";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	$arrDish = array();
	while (ocifetch($stmt)) {
		$arrDish[ociresult($stmt, "MENU_ITEM_ID")][] = array(ociresult($stmt, "DISH_ID"), ociresult($stmt, "NAME"), ociresult($stmt, "HOTKEY_ID"), ociresult($stmt, "HOTKEY_COLOR"), ociresult($stmt, "DISH_COLOR"));
	}
	ocifreestatement($stmt);


	writeFormTableHeader();

	echo "<tr><td valign=top>" .  getLM("Select an entree from the menu"). ":</td><td valign=top><table>";
	$tree->writeTreeExpanded(0, 0);
	echo "</table></td></tr>";


//	writeSelectRow("Hotkey color in hex", "hotkey_color", $arrHotkeyColor, ($GLOBALS["result"]["HOTKEY_COLOR"] ? substr($GLOBALS["result"]["HOTKEY_COLOR"] . "000000", 0, 6) : "FFFFFF"));
/*
	$str = "<select name=\"hotkey_color\">";
	foreach ($arrHotkeyColor as $val) {
		$str .= "<option value=\"" . $val[0] . "\"" . ($GLOBALS["result"]["HOTKEY_COLOR"] == $val[0] ? " selected" : "") . " style=\"background-color:#" . $val[0] . "; color: #" . ((hexdec(substr($val[0] . "000000", 0, 2)) + hexdec(substr($val[0] . "000000", 2, 2)) + hexdec(substr($val[0] . "000000", 4, 2))) / 3 < 128 ? "FFFFFF" : "000000") .  "\">" . $val[1] . "</option>";
	}
	$str .= "</select>";
	writeValRow("Hotkey color", $str);
*/
	writeCheckboxRow(getLM("Clear hotkey"), "clearhk", 0);
	writeFormTableFooter();
?>
<script>
document.itemform.onsubmit = checkForm;

function checkForm() {
	var obj = document.itemform;
	
	for (i = 0; i < obj.d.length; i++) {
		if (obj.d[i].checked)
			return true;
		
	}

	alert('You must assign an entree to the hotkey');
	return false;
}
</script>
<?
}





require($pathToSiteRoot . "common/template_dtl.php");
?>