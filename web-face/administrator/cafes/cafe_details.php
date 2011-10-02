<?
$pathToSiteRoot = "../../";
$appName = "administrator";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_datepicker.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = $id ? getLM("Cafe details") : getLM("Adding a new cafe");

$basePage = "index.php";

$table = "ksi.cafe";
$idField = "cafe_id";
$idNewVal = "ksi.cafe_seq.nextval";



$timeOpen = new timePicker("open_time");
$timeOpen->USformat = true;
$timeOpen->addHtmlAttrs = "style=\"width: 50px\"";
$timeClose = new timePicker("close_time");
$timeClose->USformat = true;
$timeClose->addHtmlAttrs = "style=\"width: 50px\"";





//$GLOBALS["__debuginfo"] = 1;

if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("name", $_POST["name"]),
		array("sales_tax", (float)$_POST["sales_tax"]),
		array("state_id", $_POST["state_id"]),
		array("city", $_POST["city"]),
		array("streetaddress", $_POST["streetaddress"]),
		array("zip", $_POST["zip"]),
		array("phone", $_POST["phone"]),
		array("delivery_fee", $_POST["delivery_fee"]),
		array("delivery_order_setup_time", $_POST["delivery_order_setup_time"]),
		array("delivery_min_subtotal", $_POST["delivery_min_subtotal"]),
		array("delivery_free_subtotal", $_POST["delivery_free_subtotal"]),
		array("meal_free_limit", (float)$_POST["meal_free_limit"]),
		array("meal_discount_percent", (float)$_POST["meal_discount_percent"]),
		array("receipt_footer", $_POST["receipt_footer"]),
		array("chain_name", $_POST["chain_name"]),
		array("open_time", "TO_DATE('01-01-01 " . $timeOpen->getTimeHMS() . "', 'YY-MM-DD HH24:MI:SS')"),
		array("close_time", "TO_DATE('01-01-01 " . $timeClose->getTimeHMS() . "', 'YY-MM-DD HH24:MI:SS')"),
		array("weekday_mask", @array_sum($_POST["wkday"])),
		array("prepare_order_time", $_POST["prepare_order_time"]),
		array("delivery_notes", substr($_POST["delivery_notes"], 0, 4000))
	);


	if ($id) {
		if ($GLOBALS["_settings"]["NETWORK_MODE"] != "STANDALONE") {
			$arrFV[] = array("type_id", $_POST["type_id"]);
		}
		updateItem($table, $arrFV, $idField, $id, 45);
	}
	elseif ($GLOBALS["_settings"]["NETWORK_MODE"] != "STANDALONE") {
		$arrFV[] = array("type_id", $_POST["type_id"]);
		$arrFV[] = array($idField, $idNewVal);
		/*$id = */insertItem($table, $arrFV, 44);
	}
	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));

}








if ($id) {
	$strSQL = "SELECT " . $table . ".* 
		, TO_CHAR(cafe.open_time, 'hh24:mi') AS otime
		, TO_CHAR(cafe.close_time, 'hh24:mi') AS ctime 
	FROM " . $table . "
	WHERE " . $idField . " = " . $id;
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);
	
	ocifetchinto($stmt, $result, OCI_ASSOC);

	if (!$result) {
		echo "error: record not found";
		return;
	}

	$timeOpen->setTime($result["OTIME"]);
	$timeClose->setTime($result["CTIME"]);
}
elseif ($GLOBALS["_settings"]["NETWORK_MODE"] == "STANDALONE") {
	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));
}






function writeMainContent() {

	writeFormTableHeader();
	if ($GLOBALS["id"]) 
		writeValRow("id", $GLOBALS["id"]);
	writeTextInputRow(getLM("Cafe"), "name", $GLOBALS["result"]["NAME"]);
	writeTextInputRow(getLM("Chain"), "chain_name", $GLOBALS["result"]["CHAIN_NAME"]);
	writeSelectSQLRow(getLM("Type"), "type_id", "SELECT type_id, name FROM ksi.cafe_type ORDER BY type_id", $GLOBALS["result"]["TYPE_ID"] ? $GLOBALS["result"]["TYPE_ID"] : 2, "", ($GLOBALS["_settings"]["NETWORK_MODE"] == "STANDALONE" ? array("addTagData" => " disabled"): ""));
	echo "<tr><td colspan=2 align=center><b><br>" . getLM("Cafe taxes, fees etc") . "</b></td></tr>";
	writeTextInputRow(getLM("Sales tax<br><font class=tiny>Type the absolute value, e.g. 8.5% tax is 0.085</font>"), "sales_tax", (float)$GLOBALS["result"]["SALES_TAX"]);
	writeTextInputRow(getLM("Delivery fee, $"), "delivery_fee", (float)$GLOBALS["result"]["DELIVERY_FEE"]);

	writeTextInputRow(getLM("Minimal order subtotal available for delivery, $"), "delivery_min_subtotal", (float)$GLOBALS["result"]["DELIVERY_MIN_SUBTOTAL"]);
	writeTextInputRow(getLM("Free delivery for orders with subtotal greater or equal to, $"), "delivery_free_subtotal", (float)$GLOBALS["result"]["DELIVERY_FREE_SUBTOTAL"]);


	echo "<tr><td colspan=2 align=center><b><br>" . getLM("Geo info") . "</b></td></tr>";
	writeSelectSQLRow(getLM("State"), "state_id", "SELECT state_id, name FROM ksi.state ORDER BY name", $GLOBALS["result"]["STATE_ID"] ? $GLOBALS["result"]["STATE_ID"] : 6);
	writeTextInputRow(getLM("ZIP"), "zip", $GLOBALS["result"]["ZIP"]);
	writeTextInputRow(getLM("City"), "city", $GLOBALS["result"]["CITY"]);
	writeTextInputRow(getLM("Street address"), "streetaddress", $GLOBALS["result"]["STREETADDRESS"]);
writeTextInputRow(getLM("Phone"), "phone", $GLOBALS["result"]["PHONE"]);

	echo "<tr><td colspan=2 align=center><b><br>" . getLM("Working hours, timing etc") . "</b><br><font class=tiny>affects pickup and delivery orders</font></td></tr>";

	echo "<tr><td align=right><br>" . getLM("Working days") . ":</td><td><table><tr>";
	for ($i = 0; $i < 7; $i++) {
		echo "<td>" . $GLOBALS["arrWkday"][$i]["short"] . "</td>";
	}
	echo "</tr><tr>";
	for ($i = 0; $i < 7; $i++) {
		echo "<td><input type=checkbox name=\"wkday[]\" value=\"" . pow(2, $i) . "\"" . ($GLOBALS["result"]["WEEKDAY_MASK"] & pow(2, $i) ? " checked" : "") . "></td>";
	}
	echo "</tr></table></td></tr>";


	echo "<tr><td colspan=2 align=center>&nbsp;</td></tr>";

	echo "<tr><td align=right>" . getLM("Working hours") . ":</td><td>";
	$GLOBALS["timeOpen"]->writeTimePickerHTML();
	echo " &mdash; ";
	$GLOBALS["timeClose"]->writeTimePickerHTML();
	echo "</td></tr>";
	writeTextInputRow(getLM("Avg time for delivery, mins"), "delivery_order_setup_time", (float)$GLOBALS["result"]["DELIVERY_ORDER_SETUP_TIME"]);
	writeTextInputRow(getLM("Avg time for order to prepare, mins"), "prepare_order_time", (float)$GLOBALS["result"]["PREPARE_ORDER_TIME"]);


	echo "<tr><td colspan=2 align=center><b><br>" . getLM("Employee meal settings") . "</b></td></tr>";
	writeTextInputRow(getLM("Free meal limit, per shift, $"), "meal_free_limit", (float)$GLOBALS["result"]["MEAL_FREE_LIMIT"]);
//	writeTextInputRow("Meal discount percent<br><font class=tiny>Type the absolute value, e.g. 15% discount is 0.15<br>Will be applied after free meal limit exhausted</font>", "meal_discount_percent", (float)$GLOBALS["result"]["MEAL_DISCOUNT_PERCENT"]);

	echo "<tr><td colspan=2 align=center><b><br>" . getLM("Miscellaneous settings") . "</b></td></tr>";
	writeTextInputRow(getLM("Receipt footer text"), "receipt_footer", $GLOBALS["result"]["RECEIPT_FOOTER"]);
	writeTextareaRow(getLM("Delivery notes"), "delivery_notes", $GLOBALS["result"]["DELIVERY_NOTES"]);
	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>