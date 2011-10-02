<?
$pathToSiteRoot = "../../";
$appName = "administrator";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = $id ? getLM("Kitchen details") : getLM("Adding a new kitchen");

$basePage = "index.php";

$table = "ksi.kitchen";
$idField = "kitchen_id";
$idNewVal = "ksi.kitchen_seq.nextval";


//$__debuginfo = 1;

if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("name", $_POST["name"])
//		array("cafe_id", $_POST["cafe_id"])
	);
	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 146);
	}
	else {
		$arrFV[] = array("cafe_id", $_POST["cafe_id"]);
		$arrFV[] = array($idField, $idNewVal);
		$id = insertItem($table, $arrFV, 145);
	}

	logTranBegin(149);
		$tmpStr = "";
		if (count($_POST["whs"])) {
			foreach ($_POST["whs"] as $val) {
				$tmpStr .= ", " . $val;
	
				$strSQL = "BEGIN INSERT INTO ksi.kitchen_warehouse (kitchen_id, warehouse_id) VALUES (" . $id . ", " . $val . "); 	EXCEPTION WHEN OTHERS THEN NULL; END; ";
	
				$stmt = ociparse($GLOBALS["objConn"], $strSQL);
				ociexecute($stmt, OCI_DEFAULT);
			}
		}

		$tmpStr = substr($tmpStr, 2);
		
		$strSQL = "DELETE FROM ksi.kitchen_warehouse WHERE kitchen_id = " . $id . ($tmpStr ? " AND warehouse_id NOT IN(" . $tmpStr . ")" : "");
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);

	logTranEnd();


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






function writeMainContent() {

	writeFormTableHeader();
	if ($GLOBALS["id"]) 
		writeValRow("id", $GLOBALS["id"]);
		writeSelectSQLRow(getLM("Cafe"), "cafe_id", "SELECT cafe_id, name FROM ksi.cafe " . ($GLOBALS["_settings"]["NETWORK_MODE"] == "NETWORK" ? "WHERE type_id IN (2) AND deleted IS NULL" : " WHERE deleted IS NULL") . " ORDER BY LOWER(name)", $GLOBALS["result"]["CAFE_ID"] ? $GLOBALS["result"]["CAFE_ID"] : $_GET["c"], "", ($GLOBALS["result"] ? array("addTagData" => " disabled"): ""));
	writeTextInputRow(getLM("Kitchen"), "name", $GLOBALS["result"]["NAME"]);

	echo "<tr><td colspan=2 align=center><br><b>" . getLM("Warehouse assignment") . "</b></td></tr>";

	$strSQL = "SELECT warehouse.warehouse_id, warehouse.name, kitchen_warehouse.kitchen_id
					FROM ksi.warehouse
						LEFT JOIN ksi.kitchen_warehouse ON ksi.kitchen_warehouse.warehouse_id = ksi.warehouse.warehouse_id 
							AND kitchen_warehouse.kitchen_id = " . (int)$GLOBALS["id"] . "
					WHERE warehouse.cafe_id = "  . ($GLOBALS["result"]["CAFE_ID"] ? $GLOBALS["result"]["CAFE_ID"] : $_GET["c"]) .  " AND warehouse.deleted IS NULL
	";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	while ($row = oci_fetch_assoc($stmt)) {
		writeValRow($row["NAME"], "<input type=checkbox name=\"whs[]\" value=\"" . $row["WAREHOUSE_ID"] . "\"" . ($row["KITCHEN_ID"] ? " checked" : "") . ">");
		
	}
	ocifreestatement($stmt);

	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>