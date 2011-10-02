<?
$pathToSiteRoot = "../../";
$appName = "administrator";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = $id ? getLM("Table group details") : getLM("Adding a new table group");

$basePage = "index.php";

$table = "ksi.table_group";
$idField = "table_group_id";
$idNewVal = "ksi.table_group_seq.nextval";




if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("name", $_POST["name"]),
		array("is_default", (int)($_POST["is_default"] != "")),
		array("pos", (float)$_POST["pos"] ? $_POST["pos"] : null)
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 135);
	}
	else {
		$arrFV[] = array("cafe_id", $_POST["cafe_id"]);
		$arrFV[] = array($idField, $idNewVal);
		$id = insertItem($table, $arrFV, 134);

	}

	logTranBegin(148);
	for ($i = 0; $i < count($_POST["kt_id"]); $i++) {

		if ((int)$_POST["kitchen_id"][$i]) {
			$strSQL = "
				MERGE INTO ksi.table_group_kitchen t
				USING (SELECT " . $id . " AS tgid, " . $_POST["kt_id"][$i] . " AS ktid FROM dual) s
				ON (t.table_group_id = tgid AND t.kitchen_type_id = s.ktid)
				WHEN MATCHED THEN UPDATE SET t.kitchen_id = " . $_POST["kitchen_id"][$i] . "
				WHEN NOT MATCHED THEN INSERT (t.table_group_id, t.kitchen_type_id, t.kitchen_id)
				VALUES (" . $id . ", " . $_POST["kt_id"][$i] . ", " . $_POST["kitchen_id"][$i] . ")
			";
	
			$stmt = ociparse($GLOBALS["objConn"], $strSQL);
			ociexecute($stmt, OCI_DEFAULT);
		}

	}
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

	writeSelectSQLRow(getLM("Cafe"), "cafe_id", "SELECT cafe_id, name FROM ksi.cafe " . ($GLOBALS["_settings"]["NETWORK_MODE"] == "NETWORK" ? "WHERE type_id IN (2) AND deleted IS NULL" : " WHERE deleted IS NULL") . " ORDER BY LOWER(name)", $GLOBALS["result"]["CAFE_ID"] ? $GLOBALS["result"]["CAFE_ID"] : (int)$_GET["c"], "", ($GLOBALS["result"] ? array("addTagData" => " disabled"): ""));
	writeTextInputRow(getLM("Table group"), "name", $GLOBALS["result"]["NAME"]);
	writeCheckboxRow(getLM("Default"), "is_default", $GLOBALS["result"]["IS_DEFAULT"]);
	writeTextInputRow(getLM("Position in list"), "pos", $GLOBALS["result"]["POS"]);


	echo "<tr><td colspan=2 align=center><br><b>" . getLM("Kitchen assignment") . "</b></td></tr>";



	$strSQL = "SELECT * FROM ksi.kitchen WHERE cafe_id = " . ($GLOBALS["result"]["CAFE_ID"] ? $GLOBALS["result"]["CAFE_ID"] : (int)$_GET["c"]) . " ORDER BY LOWER(kitchen.name)";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);
	
	$kArr = array();
	while ($row = oci_fetch_assoc($stmt)) {
		$kArr[$row["KITCHEN_ID"]] = $row["NAME"];
	}
	ocifreestatement($stmt);


	$strSQL = "SELECT kitchen_type.kt_id, kitchen_type.name, table_group_kitchen.kitchen_id
		FROM ksi.kitchen_type 
			LEFT JOIN ksi.table_group_kitchen ON ksi.kitchen_type.kt_id = ksi.table_group_kitchen.kitchen_type_id
				AND table_group_kitchen.table_group_id = " . (int)$GLOBALS["id"] . "
		ORDER BY kitchen_type.pos
	";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);
	
	while ($row = oci_fetch_assoc($stmt)) {
		writeValRow($row["NAME"] . (is_null($row["KITCHEN_ID"]) ? " &lt;" . getLM("not assigned") . "&gt;" : "") . "<input type=\"hidden\" name=\"kt_id[]\" value=\"" . $row["KT_ID"] . "\">", getKSelect($row["KITCHEN_ID"], $kArr));
		
	}
	ocifreestatement($stmt);

	writeFormTableFooter();
}

function getKSelect($preselected, $arr) {
	$str = "<select name=\"kitchen_id[]\">";
	foreach ($arr as $key => $val) {
		$str .= "<option value=\"" . $key . "\"" . ($preselected == $key ? " selected" : "") . ">" . $val . "</option>";
	}
	$str .= "</select>";
	return $str;
}



require($pathToSiteRoot . "common/template_dtl.php");
?>