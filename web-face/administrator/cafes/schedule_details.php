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

$pageTitle = $id ? getLM("Delivery schedule details") : getLM("Adding a new delivery schedule");

$basePage = "index.php";

$table = "ksi.delivery_schedule";
$idField = "schedule_id";
$idNewVal = "ksi.schedule_seq.nextval";



$timeOpen = new timePicker("open_time");
$timeOpen->USformat = true;
$timeOpen->addHtmlAttrs = "style=\"width: 50px\"";
$timeClose = new timePicker("close_time");
$timeClose->USformat = true;
$timeClose->addHtmlAttrs = "style=\"width: 50px\"";





//$GLOBALS["__debuginfo"] = 1;

if ($_POST["submit"] == getLM("save")) {


	$arrFV = array(
		array("zip", $_POST["zip"]),
		array("open_time", "TO_DATE('01-01-01 " . $timeOpen->getTimeHMS() . "', 'YY-MM-DD HH24:MI:SS')"),
		array("close_time", "TO_DATE('01-01-01 " . $timeClose->getTimeHMS() . "', 'YY-MM-DD HH24:MI:SS')"),
		array("weekday_mask", array_sum($_POST["wkday"])),
		array("active", (int)($_POST["active"] != ""))
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 107);
	}
	else {
		$arrFV[] = array($idField, $idNewVal);
		$arrFV[] = array("cafe_id", (int)$_GET["c"]);
		/*$id = */insertItem($table, $arrFV, 106);
	}
	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));

}








if ($id) {
	$strSQL = "SELECT " . $table . ".zip
	, " . $table . ".weekday_mask
	, " . $table . ".active
	, TO_CHAR(delivery_schedule.open_time, 'hh24:mi') AS otime
	, TO_CHAR(delivery_schedule.close_time, 'hh24:mi') AS ctime 
	, ksi.cafe.name AS cafe
	
	FROM " . $table . " 
		INNER JOIN ksi.cafe ON ksi.cafe.cafe_id = " . $table . ".cafe_id
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
else {

	$strSQL = "SELECT name AS cafe FROM ksi.cafe WHERE cafe_id = " . (int)$_GET["c"];
	
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);
	
	ocifetchinto($stmt, $result, OCI_ASSOC);
}






function writeMainContent() {

	writeFormTableHeader();
	if ($GLOBALS["id"]) 
		writeValRow("id", $GLOBALS["id"]);
	writeValRow(getLM("Cafe"), $GLOBALS["result"]["CAFE"]);
	writeTextInputRow(getLM("ZIP"), "zip", $GLOBALS["result"]["ZIP"]);

	echo "<tr><td align=right><br>" . getLM("Delivery days") . ":</td><td>&nbsp;</td></tr>";

	for ($i = 0; $i < 7; $i++) {
		writeValRow($GLOBALS["arrWkday"][$i]["short"], "<input type=checkbox name=\"wkday[]\" value=\"" . pow(2, $i) . "\"" . ($GLOBALS["result"]["WEEKDAY_MASK"] & pow(2, $i) ? " checked" : "") . ">");
	}
	echo "<tr><td colspan=2 align=center>&nbsp;</td></tr>";

	echo "<tr><td align=right>" . getLM("Delivery hours") . ":</td><td>";
	$GLOBALS["timeOpen"]->writeTimePickerHTML();
	echo " &mdash; ";
	$GLOBALS["timeClose"]->writeTimePickerHTML();
	echo "</td></tr>";

	writeCheckboxRow(getLM("Schedule is active"), "active", $GLOBALS["result"]["ACTIVE"]);
	writeFormTableFooter();
	?>
<script>
function checkForm() {
	flag1 = false;

	if (!document.itemform.zip.value.length) {
		alert('<? getLM("You must specify the ZIP code"); ?>');
		return false;
	}

	for (i = 0; i < document.itemform['wkday[]'].length; i++) {
		if (document.itemform['wkday[]'][i].checked) {
			flag1 = true;
			break;
		}
	}

	if (!flag1) {
		alert('<? getLM("You must select at least one day"); ?>');
		return false;
	}
	else {
		return true;
	}
}

document.itemform.onsubmit = checkForm;
</script>
<?
}





require($pathToSiteRoot . "common/template_dtl.php");
?>