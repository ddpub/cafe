<?
$pathToSiteRoot = "../../";
$appName = "menumanager";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["a"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);
$table = "ksi.coupon_application";

$basePage = "index.php";

$strSQL = "SELECT dish.dish_id, dish.name, dish.group_id, coupon_application.quantity, coupon_application.price, coupon_application.coupon_id
	FROM ksi.dish
		LEFT JOIN ksi.coupon_application ON coupon_application.dish_id = dish.dish_id AND coupon_application.coupon_id = " . (int)$_GET["dc"] . "
	WHERE dish.deleted IS NULL ";
	

$stmt = ociparse($GLOBALS["objConn"], $strSQL);
ociexecute($stmt, OCI_DEFAULT);

$arrDish = array();
$arrDishIdx = array();
while (ocifetchinto($stmt, $row, OCI_ASSOC)) {
	$arrDish[$row["GROUP_ID"]] []= $row;
	$arrDishIdx[$row["DISH_ID"]] = array("QUANTITY" => $row["QUANTITY"], "PRICE" => $row["PRICE"], "COUPON_ID" => $row["COUPON_ID"]);
}
ocifreestatement($stmt);












if ($_POST["submit"] == getLM("save")) {

	$arrSQL = array();


	for ($i = 0; $i < count($_POST["dshid"]); $i++) {



		if (!strlen($_POST["dshch" . $_POST["dshid"][$i]]) && $arrDishIdx[$_POST["dshid"][$i]]["COUPON_ID"]) {
			$arrSQL []= "DELETE FROM ksi.coupon_application WHERE coupon_id = " . (int)$_GET["dc"] . " AND dish_id = " . $_POST["dshid"][$i];
		}
		elseif (strlen($_POST["dshch" . $_POST["dshid"][$i]]) && !$arrDishIdx[$_POST["dshid"][$i]]["COUPON_ID"]) {
			$arrSQL []= "INSERT INTO ksi.coupon_application (coupon_id, dish_id, quantity, price) VALUES(" . (int)$_GET["dc"] . ", " . $_POST["dshid"][$i] . ", " . (int)$_POST["dshq"][$i] . ", " . (float)$_POST["dshp"][$i] . ")";

		}
		elseif (strlen($_POST["dshch" . $_POST["dshid"][$i]]) && $arrDishIdx[$_POST["dshid"][$i]]["COUPON_ID"] && ($arrDishIdx[$_POST["dshid"][$i]]["QUANTITY"] != (float)$_POST["dshq"][$i] || $arrDishIdx[$_POST["dshid"][$i]]["PRICE"] != (float)$_POST["dshp"][$i])) {
			$arrSQL []= "UPDATE ksi.coupon_application SET quantity = " . (int)$_POST["dshq"][$i] . ", price = " . (float)$_POST["dshp"][$i] . " WHERE coupon_id = " . (int)$_GET["dc"] . " AND dish_id = " . $_POST["dshid"][$i];
		}
	}



	foreach ($arrSQL as $val) {
		switch (substr($val, 0, 6)) {
			case "INSERT":
				$trnId = 115;
				break;

			case "UPDATE":
				$trnId = 116;
				break;

			case "DELETE":
				$trnId = 122;
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



	header("Location: " . $basePage . "?dc=" . (int)$_GET["dc"]/*($qstr ? "?" . $qstr  : "")*/);

}

if ($_GET["a"]) {
	$strSQL = "SELECT ksi.coupon_application.* 
				FROM ksi.coupon_application WHERE coupon_id = " . (int)$_GET["dc"] . " AND dish_id = " . (int)$_GET["a"];
	
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);
	
	ocifetchinto($stmt, $result, OCI_ASSOC);
	
	if (!$result) {
		echo "error: record not found";
		return;
	}
}

$pageTitle = $id ? getLM("Coupon/discount application details") : getLM("Adding an application entree to the coupon/discount");




function writeMainContent() {

	writeFormTableHeader();



	function writeDishG($group_id, $padding) {
		global $arrDish;

		if (count($arrDish[$group_id])) foreach ($arrDish[$group_id] as $val) {
			echo "<tr><td style=\"padding-left: " . $padding . "px\">" . $val["NAME"] . "</td><td align=right class=tiny>выбран: <input type=checkbox name=\"dshch" . $val["DISH_ID"] . "\"" . ((int)$val["COUPON_ID"] ? " checked" : "") . ">кол-во: <input type=text name=\"dshq[]\" value=\"" . $val["QUANTITY"] . "\" style=\"width: 50px; text-align: right\">цена: <input type=text name=\"dshp[]\" value=\"" . $val["PRICE"] . "\" style=\"width: 50px; text-align: right\"><input type=hidden name=\"dshid[]\" value=\"" . $val["DISH_ID"] . "\"></td></tr>";
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