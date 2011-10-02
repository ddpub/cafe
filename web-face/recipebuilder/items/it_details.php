<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle =  getLM("Editing tare list, assigned to item");

$basePage = "index.php";

$table = "ksi.tare_product";

$strSQL = "SELECT product.name
		, tare_product.product_id

        , tare_product.weight_full
        , tare.title
		, tare.tare_id
        , tare.group_id
        , tare.weight
        , tare.capacity
        , wm.shortname AS wmname
        , cm.shortname AS cmname
    FROM ksi.tare
        LEFT JOIN ksi.tare_product ON tare.tare_id = ksi.tare_product.tare_id AND tare_product.product_id = " . (int)$_GET["p"] . "
        LEFT JOIN ksi.product ON product.product_id = " . (int)$_GET["p"] . "
        LEFT JOIN ksi.measure wm ON wm.measure_id = tare.weight_measure_id
        LEFT JOIN ksi.measure cm ON cm.measure_id = tare.capacity_measure_id
    WHERE cm.measure_type_id = product.measure_type_id
    ORDER BY tare.title
";


$stmt = ociparse($GLOBALS["objConn"], $strSQL);
ociexecute($stmt, OCI_DEFAULT);

$arrData = array();
$arrDataIdx = array();
while (ocifetchinto($stmt, $row, OCI_ASSOC)) {
	$itemName = $row["NAME"];
	$arrData[$row["GROUP_ID"]] []= $row;
	$arrDataIdx[$row["TARE_ID"]] = array("WEIGHT_FULL" => $row["WEIGHT_FULL"]);
}
ocifreestatement($stmt);

$pageTitle .= " " . $itemName;





if ($_POST["submit"] == getLM("save")) {

	$arrSQL = array();


	for ($i = 0; $i < count($_POST["tareid"]); $i++) {

		if (!(float)$_POST["weight_full"][$i] && $arrDataIdx[$_POST["tareid"][$i]]["WEIGHT_FULL"]) {
			$arrSQL []= "DELETE FROM ksi.tare_product WHERE product_id = " . (int)$_GET["p"] . " AND tare_id = " . $_POST["tareid"][$i];
		}
		elseif ((float)$_POST["weight_full"][$i] && !$arrDataIdx[$_POST["tareid"][$i]]["WEIGHT_FULL"]) {
			$arrSQL []= "INSERT INTO ksi.tare_product (tare_id, product_id, weight_full) VALUES(" . $_POST["tareid"][$i] . ", " . (int)$_GET["p"] . ", " . (float)$_POST["weight_full"][$i] . ")";

		}
		elseif ((float)$_POST["weight_full"][$i] != $arrDataIdx[$_POST["tareid"][$i]]["WEIGHT_FULL"]) {
			$arrSQL []= "UPDATE ksi.tare_product SET weight_full = " . (float)$_POST["weight_full"][$i] . " WHERE product_id = " . (int)$_GET["p"] . " AND tare_id = " . $_POST["tareid"][$i];
		}
	}


	foreach ($arrSQL as $val) {
		switch (substr($val, 0, 6)) {
			case "INSERT":
				$trnId = 175;
				break;

			case "UPDATE":
				$trnId = 176;
				break;

			case "DELETE":
				$trnId = 177;
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



	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));

}








if ($id) {
	$strSQL = "SELECT * FROM " . $table . " WHERE " . $idField . " = " . $id;
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);
	
	ocifetchinto($stmt, $result, OCI_ASSOC);

	if (!$result) {
		echo "error: record not found";
		return;
	}
}




require_once($GLOBALS["pathToSiteRoot"] . "common/class_rtree.php");

class rTree2 extends rTree {

	function passTree($parentID, $level) {
		for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
			echo "<tr><td colspan=4 style=\"padding-left:" . $level * 20 . "px\"><b>" .  $this->arrTree[$this->arrIdx[$parentID][$i]][2] . "</b></td></tr>";
			writeTareG($this->arrTree[$this->arrIdx[$parentID][$i]][0], $level * 20 + 5);
			$this->passTree($this->arrIdx[$parentID][$i], $level+1);
			
		}
	}
}



function writeTareG($group_id, $padding) {
	global $arrData;

	if (count($arrData[$group_id])) foreach ($arrData[$group_id] as $val) {
		echo "<tr><input type=hidden name=\"tareid[]\" value=\"" . $val["TARE_ID"] . "\">";
		echo "<td>" . $val["TITLE"] . "</td>";
		echo "<td align=right>" . $val["CAPACITY"] . " " . $val["CMNAME"] ."</td>";
		echo "<td align=right>" . $val["WEIGHT"] . " " . $val["WMNAME"] ."</td>";
		echo "<td align=right><input type=text name=\"weight_full[]\" value=\"" . $val["WEIGHT_FULL"] . "\" style=\"width: 50px; text-align: right\"> " . $val["WMNAME"] ."</td>";

		echo "</tr>";

	}
}





function writeMainContent() {
	writeFormTableHeader();


	echo "<tr><th>" . getLM("tare") . "</th><th>" . getLM("capacity") . "</th><th>" . getLM("empty tare weight") . "</th><th>" . getLM("full tare weight") . "</th></tr>";


	$tree = new rTree2("SELECT group_id, parent_id, title FROM ksi.tare_group ORDER BY title", $GLOBALS["objConn"]);

	$tree->passTree(0, 0, $str);



	writeFormTableFooter();

}





require($pathToSiteRoot . "common/template_dtl.php");
?>