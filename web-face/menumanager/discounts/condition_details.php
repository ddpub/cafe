<?
$pathToSiteRoot = "../../";
$appName = "menumanager";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["c"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);
$table = "ksi.coupon_condition";

$basePage = "index.php";

$strSQL = "SELECT dish.dish_id, dish.name, dish.group_id, coupon_condition.quantity
	FROM ksi.dish
		LEFT JOIN ksi.coupon_condition ON coupon_condition.dish_id = dish.dish_id AND coupon_condition.coupon_id = " . (int)$_GET["dc"] . "
	WHERE dish.deleted IS NULL ";

$stmt = ociparse($GLOBALS["objConn"], $strSQL);
ociexecute($stmt, OCI_DEFAULT);

$arrDish = array();
$arrDishIdx = array();
while (ocifetchinto($stmt, $row, OCI_ASSOC)) {
	$arrDish[$row["GROUP_ID"]] []= $row;
	$arrDishIdx[$row["DISH_ID"]] = $row["QUANTITY"];
}
ocifreestatement($stmt);
















if ($_POST["submit"] == getLM("save")) {

	$arrSQL = array();


	for ($i = 0; $i < count($_POST["dshid"]); $i++) {
		if ((int)$_POST["dshq"][$i] == 0 && (int)$arrDishIdx[$_POST["dshid"][$i]] != 0) {
			$arrSQL []= "DELETE FROM ksi.coupon_condition WHERE coupon_id = " . (int)$_GET["dc"] . " AND dish_id = " . $_POST["dshid"][$i];
		}
		elseif ((int)$_POST["dshq"][$i] != 0 && (int)$arrDishIdx[$_POST["dshid"][$i]] == 0) {
			$arrSQL []= "INSERT INTO ksi.coupon_condition (coupon_id, dish_id, quantity) VALUES(" . (int)$_GET["dc"] . ", " . $_POST["dshid"][$i] . ", " . (int)$_POST["dshq"][$i] . ")";
		}
		elseif ((int)$_POST["dshq"][$i] != (int)$arrDishIdx[$_POST["dshid"][$i]]) {
			$arrSQL []= "UPDATE ksi.coupon_condition SET quantity = " . (int)$_POST["dshq"][$i] . " WHERE coupon_id = " . (int)$_GET["dc"] . " AND dish_id = " . $_POST["dshid"][$i];
		}
	}

//pr ($arrSQL);


	foreach ($arrSQL as $val) {
		switch (substr($val, 0, 6)) {
			case "INSERT":
				$trnId = 113;
				break;

			case "UPDATE":
				$trnId = 114;
				break;

			case "DELETE":
				$trnId = 121;
				break;

			default:
				die("error: unknown sql command");
		}


		logTranBegin($trnId);

		$stmt = ociparse($GLOBALS["objConn"], $val);
		if (!@ociexecute($stmt, OCI_DEFAULT)) {
			if ($haltOnError) {
				$e = oci_error($stmt);
				pr($e);
				die("error performing sql query");
			}
			else {
				ocirollback($GLOBALS["objConn"]);
				return 0; // error
			}
		}

		logTranEnd();
	}


/*





	$arrFV = array(
		array("coupon_id", (int)$_GET["dc"]),
		array("dish_id", (int)$_POST["dish_id"]),
		array("quantity", (int)$_POST["quantity"])
	);


	if ($id) {

		logTranBegin(114);

		$strSQL = "UPDATE " . $table . " 
			SET quantity = " . (int)$_POST["quantity"] . "
			WHERE coupon_id = " . (int)$_GET["dc"] . " AND dish_id = " . (int)$_GET["c"];
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);

		logTranEnd();

	}
	else {
		insertItem($table, $arrFV, 113);
	}
*/
	header("Location: " . $basePage . "?dc=" . (int)$_GET["dc"]/*($qstr ? "?" . $qstr  : "")*/);

}

if ($_GET["c"]) {
	$strSQL = "SELECT ksi.coupon_condition.* 
				FROM ksi.coupon_condition WHERE coupon_id = " . (int)$_GET["dc"] . " AND dish_id = " . (int)$_GET["c"];
	
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);
	
	ocifetchinto($stmt, $result, OCI_ASSOC);
	
	if (!$result) {
		echo "error: record not found";
		return;
	}
}

$pageTitle = $id ? getLM("Coupon/discount condition details") : getLM("Adding a condition entree to the coupon/discount");











function writeMainContent() {
	global $arrDish;

	writeFormTableHeader();



	function writeDishG($group_id, $padding) {
		global $arrDish;

		if (count($arrDish[$group_id])) foreach ($arrDish[$group_id] as $val) {
			echo "<tr><td style=\"padding-left: " . $padding . "px\">" . $val["NAME"] . "</td><td align=right class=tiny>кол-во: <input type=text name=\"dshq[]\" value=\"" . $val["QUANTITY"] . "\" style=\"width: 50px; text-align: right\"><input type=hidden name=\"dshid[]\" value=\"" . $val["DISH_ID"] . "\"></td></tr>";
		}
	}





	require_once($GLOBALS["pathToSiteRoot"] . "common/class_rtree.php");
	
	class rTree2 extends rTree {
	
		function passTree($parentID, $level) {
			for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
				echo "<tr><td colspan=2 style=\"padding-left:" . $level * 20 . "px\"><b>" .  $this->arrTree[$this->arrIdx[$parentID][$i]][2] . "</b></td></tr>";
				writeDishG($this->arrTree[$this->arrIdx[$parentID][$i]][0], $level * 20 + 5);
				$this->passTree($this->arrIdx[$parentID][$i], $level+1);
				
			}
		}
	}
	
	$tree = new rTree2("SELECT dish_group.group_id, dish_group.parent_id, dish_group.name 
							FROM ksi.dish_group 
						ORDER BY dish_group.name", $GLOBALS["objConn"]);
	
	$tree->passTree(0, 0);

	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>