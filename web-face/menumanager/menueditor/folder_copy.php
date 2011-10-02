<?
$pathToSiteRoot = "../../";
$appName = "menumanager";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = "Menu folder copy";
$basePage = "index.php";


class rTree2 extends rTree {

	function writeMenuTree($parentID, $level) {

		if ($parentID /*&& (count($this->arrIdx[$parentID]) || $this->arrTree[$parentID][3])*/) {
			echo "<tr><td colspan=2  style=\"padding-left:" . (20 * $level) . "px\"><b style=\"margin: 10 0 0 0;\">" . $this->arrTree[$parentID][2] . "</b></td></tr>";
	
			if ($GLOBALS["arrDish"][$parentID]) {
				foreach ($GLOBALS["arrDish"][$parentID] as $val) {
					echo "<tr><td style=\"padding-left:" . (20+ 20 * $level) . "px;\" valign=top>" . $val["NAME"];

/*					if ($GLOBALS["arrModf"][$parentID][$val["DISH_ID"]]) {
						echo "<a href=\"" . $_SERVER["SCRIPT_NAME"] . "\" onclick=\"tglMod('" . $parentID . "_" . $val["DISH_ID"] . "');return false;\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/buldn.gif\" width=6 height=6 alt=\"\" border=0 name=\"i" . $parentID . "_" . $val["DISH_ID"] . "\" hspace=5></a>";

						echo "<div class=\"modftable\" id=\"" . $parentID . "_" . $val["DISH_ID"] . "\">";
						foreach ($GLOBALS["arrModf"][$parentID][$val["DISH_ID"]] as $key => $val1) {
							echo "<table class=\"modftable\"><tr><th colspan=2>" . $key . "</td></tr>";
							foreach ($val1 as $val2) {
								echo "<tr><td>" . $val2[0] . "</td><td align=right>" . ($val2[1] ? getPrice($val2[1], 1) : "&nbsp;") . "</td></tr>";
							}
							echo "</table>";
						}
						echo "</div>";
						if ($val["DESCRIPTION"]) {
							echo "<div class=tiny style=\"margin-top: 4px\">" . $val["DESCRIPTION"] . "</div>";
						}

					}*/

					echo "</td><td align=right valign=top style=\"padding-left: 20px\">$" . number_format($val["PRICE"], 2) . "</td></tr>";
				}
			}
		}

		for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {

			$this->writeMenuTree($this->arrIdx[$parentID][$i], $level+1);

		}
	}



	function writeFolders($parentID, $level, $stopID) {

//		if ($parentID == $stopID)

		if ($parentID) {
			echo "<input type=radio name=\"mi\" value=\"" . $parentID . "\"" . ($stopID == $parentID || $stopID == -1 ? " disabled" : "") . ">";
			echo "<span style=\"padding-left:" . (20 * $level) . "px;" . ($stopID == $parentID || $stopID == -1 ? "color: gray" : "") . "\">" . $this->arrTree[$parentID][2] . "</span><br>";
	
		}

		if ($stopID == $parentID) {
			$stopID = -1;
		}

		for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {

			$this->writeFolders($this->arrIdx[$parentID][$i], $level+1, $stopID);

		}
	}



	function passTreeCustom($parentID, $newParentID, $menuID) {



		if ($parentID) {
			$strSQL = "INSERT INTO ksi.menu_item (menu_item_id, menu_id, parent_id, name, pos) VALUES (ksi.menu_item_seq.nextval, " . (int)$menuID . ", " . ((int)$newParentID ? $newParentID : "NULL") . ", '" . sqlFixUp($this->arrTree[$parentID][2]) . "', " . (strlen($this->arrTree[$parentID][3]) ? (int)$this->arrTree[$parentID][3] : "NULL") . ") RETURNING menu_item_id INTO :menu_item_id";
			$stmt = ociparse($GLOBALS["objConn"], $strSQL);
			OCIBindByName($stmt, ":menu_item_id", &$newMenuItemId, 32);

			if (!ociexecute($stmt, OCI_DEFAULT)) {
				$GLOBALS["strError"] = getLM("Could not create a menu folder");
				return;
			}


			$strSQL = "INSERT INTO ksi.menu_dish (dish_id, menu_item_id, price, pos, hotkey_id, hotkey_color) (SELECT dish_id, " . $newMenuItemId . ", price, pos, hotkey_id, hotkey_color FROM ksi.menu_dish WHERE menu_item_id = " . $parentID . ")";

			$stmt = ociparse($GLOBALS["objConn"], $strSQL);

			if (!ociexecute($stmt, OCI_DEFAULT)) {
				$GLOBALS["strError"] = getLM("Could not copy menu folder contents");
				return;
			}
		}


		for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
			$this->passTreeCustom($this->arrIdx[$parentID][$i], $newMenuItemId, $menuID);
		}


/*

		for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
			$strSQL = "INSERT INTO ksi.menu_item (menu_item_id, menu_id, parent_id, name, pos) VALUES (ksi.menu_item_seq.nextval, " . (int)$menuID . ", " . ((int)$newParentID ? $newParentID : "NULL") . ", '" . sqlFixUp($this->arrTree[$this->arrIdx[$parentID][$i]][2]) . "', " . (strlen($this->arrTree[$this->arrIdx[$parentID][$i]][3]) ? (int)$this->arrTree[$this->arrIdx[$parentID][$i]][3] : "NULL") . ") RETURNING menu_item_id INTO :menu_item_id";
			$stmt = ociparse($GLOBALS["objConn"], $strSQL);
			OCIBindByName($stmt, ":menu_item_id", &$newMenuItemId, 32);

			if (!ociexecute($stmt, OCI_DEFAULT)) {
				$GLOBALS["strError"] = "Could not create a menu folder.";
				return;
			}


			$strSQL = "INSERT INTO ksi.menu_dish (dish_id, menu_item_id, price, pos, hotkey_id, hotkey_color) (SELECT dish_id, " . $newMenuItemId . ", price, pos, hotkey_id, hotkey_color FROM ksi.menu_dish WHERE menu_item_id = " . $this->arrIdx[$parentID][$i] . ")";
			$stmt = ociparse($GLOBALS["objConn"], $strSQL);

			if (!ociexecute($stmt, OCI_DEFAULT)) {
				$GLOBALS["strError"] = "Could not copy menu folder contents";
				return;
			}

			$this->passTreeCustom($this->arrIdx[$parentID][$i], $newMenuItemId, $menuID);
		}
*/	}

}











if ($_POST["submit"] == getLM("save")) {

	$strSQL = "SET TRANSACTION NAME 'Copy_menu_folder_" . $id . "'";
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	logTranBegin(112);


	$tree = new rTree2("SELECT menu_item_id, parent_id, name, pos FROM ksi.menu_item WHERE menu_id = (SELECT menu_id FROM ksi.menu_item WHERE menu_item_id = " . $GLOBALS["id"] . ") ORDER BY pos", $GLOBALS["objConn"]);

	if ((int)$_POST["mi"] > 0) {
		$strSQL = "SELECT menu_id FROM ksi.menu_item WHERE menu_item_id = " . (int)$_POST["mi"];
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
		ocifetchinto($stmt, $row, OCI_ASSOC);
		ocifreestatement($stmt);
		$menuId = $row["MENU_ID"];
		$menuItemId = (int)$_POST["mi"];
	}
	else {
		$menuId = abs((int)$_POST["mi"]);
		$menuItemId = 0;
	}

	$tree->passTreeCustom($GLOBALS["id"], $menuItemId, $menuId);

	if ($strError) {
		$strSQL = "ROLLBACK";
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
		ocifreestatement($stmt);
	}
	else {
		$strError = getLM("Menu folder copied successfully");

		$strSQL = "COMMIT";
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
		ocifreestatement($stmt);

		logTranEnd();
	}
}








if ($id) {
	$strSQL = "SELECT * FROM ksi.menu_item WHERE menu_item_id = " . $id;
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);
	
	ocifetchinto($stmt, $result, OCI_ASSOC);

	if (!$result) {
		echo "error: record not found";
		return;
	}
}












function writeMainContent() {


	if ($GLOBALS["strError"]) {
		echo "<center><h2>" . ucfirst($GLOBALS["result"]["NAME"]) . " menu folder copy status</h2><p>" . $GLOBALS["strError"] . "</p>";
		echo "<br><input type=\"button\" value=\"" . getLM("return to menus") . "\" onclick=\"location.href='" . $GLOBALS["basePage"] . ($GLOBALS["qstr"] ? "?" . $GLOBALS["qstr"]  : "") . "'\"></center>";
		return;
	}







	writeFormTableHeader();
	if ($GLOBALS["id"]) 

		echo "<tr><td colspan=2><p>" . getLM("Menu folder you are going to copy") . ":</p>";

		$tree = new rTree2("SELECT menu_item_id, parent_id, name FROM ksi.menu_item WHERE menu_id = (SELECT menu_id FROM ksi.menu_item WHERE menu_item_id = " . $GLOBALS["id"] . ") ORDER BY pos", $GLOBALS["objConn"]);


		$strSQL = "SELECT DISH.DISH_ID
				, DISH.NAME 
				, DISH.DESCRIPTION
				, MENU_DISH.PRICE
				, MENU_ITEM.MENU_ITEM_ID
			FROM ksi.dish 
			INNER JOIN ksi.menu_dish ON ksi.menu_dish.dish_id = ksi.dish.dish_id
			INNER JOIN ksi.menu_item ON ksi.menu_item.menu_item_id = ksi.menu_dish.menu_item_id
		WHERE ksi.dish.deleted IS NULL 
			AND ksi.menu_item.menu_id = (SELECT menu_id FROM ksi.menu_item WHERE menu_item_id = " . $GLOBALS["id"] . ")
		ORDER BY menu_item.pos, menu_dish.pos";

		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);

		$tmpStr = 0;
		$GLOBALS["arrDish"] = array();
		while (ocifetchinto($stmt, $row, OCI_ASSOC)) {
			$GLOBALS["arrDish"][$row["MENU_ITEM_ID"]][] = array("NAME" => $row["NAME"], "DESCRIPTION" => $row["DESCRIPTION"], "PRICE" => $row["PRICE"]);
		}
		ocifreestatement($stmt);

		echo "<table border=0>";
		$tree->writeMenuTree($GLOBALS["result"]["MENU_ITEM_ID"], 0, $str);
		echo "</table>";
/*
		echo "<p>Menu folder to be created:</p></td></tr>";

		writeTextInputRow("Folder title", "name", $GLOBALS["result"]["NAME"]);
		writeTextInputRow("Position in list", "pos", $GLOBALS["result"]["POS"]);
		writeTextareaRow("description", "description", $GLOBALS["result"]["DESCRIPTION"]);
*/

		echo "<tr><td colspan=2>&nbsp;<p>" . getLM("Please select the destination") . ":</p>";

		$strSQL = "SELECT ksi.cafe.name AS cafe, ksi.cafe.cafe_id AS cafe_id, ksi.menu.name, ksi.menu.menu_id
			FROM ksi.menu 
				LEFT JOIN ksi.cafe ON ksi.cafe.cafe_id = ksi.menu.cafe_id
			WHERE ksi.menu.deleted IS NULL AND ksi.cafe.deleted IS NULL
			ORDER BY LOWER(ksi.cafe.name), LOWER(ksi.menu.name)";

		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);

		$curCafe = -1;
		while (ocifetchinto($stmt, $row, OCI_ASSOC)) {
			if ($curCafe != $row["CAFE_ID"]) {
				echo "<h2>" . $row["CAFE"] . "</h2>";
				$curCafe = $row["CAFE_ID"];
			}
			echo "<h3 style=\"margin-left: 37px\">" . $row["NAME"] . "</h3>";
			echo "<input type=radio name=\"mi\" value=\"-" . $row["MENU_ID"] . "\"> &lt;menu root&gt;<br>";
			$tree = new rTree2("SELECT menu_item_id, parent_id, name FROM ksi.menu_item WHERE menu_id = " . $row["MENU_ID"] . " ORDER BY pos", $GLOBALS["objConn"]);

			$tree->writeFolders(0, 0, $GLOBALS["result"]["MENU_ITEM_ID"]);


		}
		ocifreestatement($stmt);



		writeFormTableFooter();





?>
<script>
document.itemform.onsubmit = checkForm;

function checkForm() {
	flag1 = false;

	if (document.itemform.mi.length) {
		for (i = 0; i < document.itemform.mi.length; i++) {
			if (document.itemform.mi[i].checked) {
				flag1 = true;
				break;
			}
		}
	}
	else if (document.itemform.mi) {
		if (document.itemform.checked) {
			flag1 = true;
		}
	}
	
	if (!flag1) {
		alert('<? echo getLM("Please select the destination menu folder")?>');
		return false;
	}
	else {
		return true;
	}
}
</script>
<?
}





require($pathToSiteRoot . "common/template_dtl.php");
?>