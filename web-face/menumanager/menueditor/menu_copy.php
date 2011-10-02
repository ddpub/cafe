<?
$pathToSiteRoot = "../../";
$appName = "menumanager";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);

$pageTitle = getLM("Copying menu to other cafes");

$basePage = "index.php";

$id = (int)$_GET["id"];





if ($_POST["submit"] == getLM("save")) {

	class rTree2 extends rTree {
		function passTreeCustom($parentID, $level, $newParentID, $menuID) {

			for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
				$strSQL = "INSERT INTO ksi.menu_item (menu_item_id, menu_id, parent_id, name, pos) VALUES (ksi.menu_item_seq.nextval, " . (int)$menuID . ", " . ((int)$newParentID ? $newParentID : "NULL") . ", '" . sqlFixUp($this->arrTree[$this->arrIdx[$parentID][$i]][2]) . "', " . (strlen($this->arrTree[$this->arrIdx[$parentID][$i]][3]) ? (int)$this->arrTree[$this->arrIdx[$parentID][$i]][3] : "NULL") . ") RETURNING menu_item_id INTO :menu_item_id";
				$stmt = ociparse($GLOBALS["objConn"], $strSQL);
				OCIBindByName($stmt, ":menu_item_id", &$newMenuItemId, 32);

				if (!ociexecute($stmt, OCI_DEFAULT)) {
					$arrErrors[$GLOBALS["val"]] = array(1, getLM("Could not create a menu folder."));
					return;
				}


				$strSQL = "INSERT INTO ksi.menu_dish (dish_id, menu_item_id, price, pos, hotkey_id, hotkey_color) (SELECT dish_id, " . $newMenuItemId . ", price, pos, hotkey_id, hotkey_color FROM ksi.menu_dish WHERE menu_item_id = " . $this->arrIdx[$parentID][$i] . ")";
				$stmt = ociparse($GLOBALS["objConn"], $strSQL);

				if (!ociexecute($stmt, OCI_DEFAULT)) {
					$GLOBALS["arrErrors"][$GLOBALS["val"]] = array(1, getLM("Could not copy menu folder contents"));
					return;
				}

				$this->passTreeCustom($this->arrIdx[$parentID][$i], $level+1, $newMenuItemId, $menuID);
			}
		}
	}

	$tree = new rTree2("SELECT menu_item_id, parent_id, name, pos FROM ksi.menu_item WHERE menu_id = " . $id, $GLOBALS["objConn"]);
	








	if (!count($_POST["dst"])) {
		echo getLM("You have selected nothing of destination cafes");
	}
	else {
		foreach ($_POST["dst"] as $val) {

			$strSQL = "SET TRANSACTION NAME 'Copy_menu_" . $id . "_to_cafe_" . $val . "'";
			$stmt = ociparse($GLOBALS["objConn"], $strSQL);
			ociexecute($stmt, OCI_DEFAULT);

			logTranBegin(57);


			$strSQL = "SELECT COUNT(*) AS num FROM ksi.menu WHERE name = '" . sqlFixUp($_POST["name"]) . "' AND deleted is null AND cafe_id = " . $val;

			$stmt = ociparse($GLOBALS["objConn"], $strSQL);
			ociexecute($stmt, OCI_DEFAULT);
			ocifetch($stmt);
			if (ociresult($stmt, "NUM") != 0) {
				$arrErrors[$val] = array(1, getLM("Menu with the same name already exists"));

				logTranEnd();

				$strSQL = "ROLLBACK";
				$stmt = ociparse($GLOBALS["objConn"], $strSQL);
				ociexecute($stmt, OCI_DEFAULT);

				continue;

			}

			$strSQL = "INSERT INTO ksi.menu (menu_id, cafe_id, name) VALUES (ksi.menu_seq.nextval, " . ((int)$val ? $val : "NULL") . ", '" . sqlFixUp($_POST["name"]) . "') RETURNING menu_id INTO :menu_id";
			$stmt = ociparse($GLOBALS["objConn"], $strSQL);
			OCIBindByName($stmt, ":menu_id", &$newMenuId, 32);

			if (!@ociexecute($stmt, OCI_DEFAULT)) {
				$arrErrors[$val] = array(1, getLM("Could not create a menu instance. Probably menu with the same name already exists"));

				logTranEnd();

				$strSQL = "ROLLBACK";
				$stmt = ociparse($GLOBALS["objConn"], $strSQL);
				ociexecute($stmt, OCI_DEFAULT);

				continue;
			}



			$tree->passTreeCustom(0, 0, 0, $newMenuId);

			if ($arrErrors[$val][0]) {

				logTranEnd();

				$strSQL = "ROLLBACK";
				$stmt = ociparse($GLOBALS["objConn"], $strSQL);
				ociexecute($stmt, OCI_DEFAULT);

				continue;
			}

			$arrErrors[$val] = array(0, getLM("Copied OK"));

			logTranEnd();

			$strSQL = "COMMIT";
			$stmt = ociparse($GLOBALS["objConn"], $strSQL);
			ociexecute($stmt, OCI_DEFAULT);
		}
	}


}










$strSQL = "SELECT * FROM ksi.menu WHERE menu_id = " . $id;
$stmt = ociparse($GLOBALS["objConn"], $strSQL);
ociexecute($stmt, OCI_DEFAULT);

ocifetchinto($stmt, $result, OCI_ASSOC);

if (!$result) {
	die("error: record not found");

}





function writeMainContent() {


	$strSQL = "SELECT * FROM ksi.cafe WHERE cafe_id <> " . (int)$GLOBALS["result"]["CAFE_ID"] . " AND cafe.type_id IN (2) AND deleted IS NULL ORDER BY LOWER(name)";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	if ($GLOBALS["arrErrors"]) {
		echo "<h2>" . ucfirst($GLOBALS["result"]["NAME"]) . " " . getLM("copy status") . "</h2><table align=center border=0 align=center>";
		while (ocifetch($stmt)) {
			if ($GLOBALS["arrErrors"][ociresult($stmt, "CAFE_ID")][1]) {
				writeValRow(ociresult($stmt, "NAME"), $GLOBALS["arrErrors"][ociresult($stmt, "CAFE_ID")][1]);
			}
		}
		ocifreestatement($stmt);
		if ((int)$GLOBALS["result"]["CAFE_ID"] && $GLOBALS["arrErrors"][0][1]) {
			writeValRow("<font color=\"#808080\">" . getLM("menu templates") . "</font>", $GLOBALS["arrErrors"][0][1]);
		}
		echo "<tr><td colspan=2 align=center><br><input type=\"button\" value=\"" . getLM("return to menu editor") . "\" onclick=\"location.href='" . $GLOBALS["basePage"] . ($GLOBALS["qstr"] ? "?" . $GLOBALS["qstr"]  : "") . "'\"></td></tr>";
		echo "</table>";
		return;
	}





	writeFormTableHeader();
	writeTextInputRow(getLM("Copy menu as"), "name", $GLOBALS["result"]["NAME"]);


	$tmpStr = "";

	while (ocifetch($stmt)) {
		$tmpStr .= "<input type=checkbox name=\"dst[]\" value=\"" . ociresult($stmt, "CAFE_ID") . "\"> " . ociresult($stmt, "NAME") . "<br>";
	}
	ocifreestatement($stmt);
	if ((int)$GLOBALS["result"]["CAFE_ID"]) {
		$tmpStr .= "<input type=checkbox name=\"dst[]\" value=\"0\"> <font color=\"#808080\">" . getLM("menu templates") . "</font><br>";
	}

	writeValRow(getLM("Destination cafes"), $tmpStr);

	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>