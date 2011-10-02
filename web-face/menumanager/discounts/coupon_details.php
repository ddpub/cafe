<?
$pathToSiteRoot = "../../";
$appName = "menumanager";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_datepicker.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = $id ? getLM("Coupon/discount details") : getLM("Adding a new coupon/discount");

$basePage = "index.php";

$table = "ksi.coupon";
$idField = "coupon_id";
$idNewVal = "ksi.coupon_seq.nextval";

//$__debuginfo = true;

$date1 = new datePicker("date1");
$date1->USformat = true;
$date1->yearFrom = date("Y");
$date1->yearTo = date("Y") + 5;
$date2 = new datePicker("date2");
$date2->USformat = true;
$date2->yearFrom = date("Y");
$date2->yearTo = date("Y") + 5;

if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("title", $_POST["title"]),
		array("code", $_POST["code"]),
		array("discount", (float)$_POST["discount"] ? (float)$_POST["discount"] : null),
		array("fixed_discount", (float)$_POST["fixed_discount"] ? (float)$_POST["fixed_discount"] : null),
		array("start_date", $date1->getDateYMD() ? "TO_DATE('" . $date1->getDateYMD() . "', 'YYYY-MM-DD')" : null),
		array("stop_date", $date2->getDateYMD() ? "TO_DATE('" . $date2->getDateYMD() . "', 'YYYY-MM-DD')" : null),
		array("subtotal_condition", (float)$_POST["subtotal_condition"]),
		array("auth_required", (int)($_POST["auth_required"] != "")),
		array("autoscalable", (int)($_POST["autoscalable"] != "")),
		array("condition_rule", (int)$_POST["condition_rule"]),
		array("application_rule", (int)$_POST["application_rule"]),
		array("apply2condition", (int)($_POST["apply2condition"] != "")),
		array("combinable", (int)($_POST["combinable"] != "")),
		array("description", $_POST["description"])
	);


	if ($id) {
		updateItem($table, $arrFV, $idField, $id, 60);
	}
	else {
		$arrFV[] = array($idField, $idNewVal);
		/*$id = */insertItem($table, $arrFV, 59);
	}
	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));

}








if ($id) {
	$strSQL = "SELECT title, code, discount, auth_required, condition_rule, application_rule, description, TO_CHAR(start_date, 'YYYY-MM-DD') AS start_date, TO_CHAR(stop_date, 'YYYY-MM-DD') AS stop_date, autoscalable, fixed_discount, apply2condition, combinable, subtotal_condition FROM " . $table . " WHERE " . $idField . " = " . $id;
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);
	
	ocifetchinto($stmt, $result, OCI_ASSOC);

	$date1->setDate($result["START_DATE"]);
	$date2->setDate($result["STOP_DATE"]);

	if (!$result) {
		echo "error: record not found";
		return;
	}
}







function writeMainContent() {




	writeFormTableHeader();
	if ($GLOBALS["id"]) 
		writeValRow("id", $GLOBALS["id"]);

	writeTextInputRow(getLM("Title"), "title", $GLOBALS["result"]["TITLE"]);
	writeTextInputRow(getLM("Code"), "code", $GLOBALS["result"]["CODE"]);
	writeTextareaRow(getLM("Description"), "description", $GLOBALS["result"]["DESCRIPTION"]);

	writeTextInputRow(getLM("Subtotal condition") . ", " . getLM("$") . "<br><font class=tiny>" . getLM("Minimal subtotal amount enabling the coupon") . "</font>", "subtotal_condition", $GLOBALS["result"]["SUBTOTAL_CONDITION"]);

	echo "<tr><td colspan=2><hr size=1 color=silver width=50%>";

	echo "<tr style=\"border: 1px black\"><td align=right>" . getLM("Condition rule") . ":</td><td><input type=radio name=\"condition_rule\" value=0" . ($GLOBALS["result"]["CONDITION_RULE"] == 0 ? " checked" : "") . "> " . getLM("ANY ONE of condition entrees make coupon/discount work") . "<br>";
	echo "<input type=radio name=\"condition_rule\" value=1" . ($GLOBALS["result"]["CONDITION_RULE"] == 1 ? " checked" : "") . ">" . getLM("ALL of condition entrees make coupon work") . "</td></tr>";


	echo "<tr><td colspan=2><hr size=1 color=silver width=50%>";

	echo "<tr><td align=right>" . getLM("Application rule") . ":</td><td><input type=radio name=\"application_rule\" value=0" . ($GLOBALS["result"]["APPLICATION_RULE"] == 0 ? " checked" : "") . "> " . getLM("discount is applied to any ONE of application entrees") . "<br>";
	echo "<input type=radio name=\"application_rule\" value=1" . ($GLOBALS["result"]["APPLICATION_RULE"] == 1 ? " checked" : "") . ">" . getLM("discount is applied to ALL application entrees") . "</td></tr>";

	echo "<tr><td colspan=2><hr size=1 color=silver width=50%>";

	writeValRow(getLM("Discounted prices specified in the application list"), "<input type=radio value=0 name=\"dct\" onclick=\"chckForm()\"> ");


	writeValRow(getLM("One discount to whole application list, in percents") . "<br><font class=tiny>" . getLM("Type the absolute value, e.g. 15% discount is 0.15") . "</font>", "<input type=radio value=1 name=\"dct\" onclick=\"chckForm()\"> <input type=\"text\" name=\"discount\" value=\"" . (!(float)$GLOBALS["result"]["DISCOUNT"] ? "" : $GLOBALS["result"]["DISCOUNT"]) . "\">");


	writeValRow(getLM("Fixed order discount") . ", " . getLM("$"), "<input type=radio value=2 name=\"dct\" onclick=\"chckForm()\"> <input type=\"text\" name=\"fixed_discount\" value=\"" . (!(float)$GLOBALS["result"]["FIXED_DISCOUNT"] ? "" : $GLOBALS["result"]["FIXED_DISCOUNT"]) . "\">");

	echo "<tr><td colspan=2><hr size=1 color=silver width=50%>";


	echo "<tr><td align=right>" . getLM("Coupon/discount start date") . ":</td><td>";
	$GLOBALS["date1"]->writeDatePickerHTML();
	echo "</td></tr>";
	echo "<tr><td align=right>" . getLM("Coupon/discount finish date") . ":</td><td>";
	$GLOBALS["date2"]->writeDatePickerHTML();
	echo "</td></tr>";

	echo "<tr><td colspan=2><hr size=1 color=silver width=50%>";

	writeCheckboxRow(getLM("Apply the discount to entrees in the condition list"), "apply2condition", $GLOBALS["result"]["APPLY2CONDITION"]);
	writeCheckboxRow(getLM("Automatically scale the discount to entrees, according to condition entrees presence (this is NOT a coupon)"), "autoscalable", $GLOBALS["result"]["AUTOSCALABLE"]);
	writeCheckboxRow(getLM("Manager authorization required"), "auth_required", $GLOBALS["result"]["AUTH_REQUIRED"]);
	writeCheckboxRow(getLM("Can be combined with other coupons"), "combinable", $GLOBALS["result"]["COMBINABLE"]);

	writeFormTableFooter();

?>
<script>
<?
if ((float)$GLOBALS["result"]["FIXED_DISCOUNT"]) {
	echo "document.itemform.dct[2].checked = true;";
}
elseif((float)$GLOBALS["result"]["DISCOUNT"]) {
	echo "document.itemform.dct[1].checked = true;";
}
else {
	echo "document.itemform.dct[0].checked = true;";
}

?>
function chckForm() {
	if (document.itemform.dct[0].checked || document.itemform.dct[1].checked) {
		document.itemform.application_rule[0].disabled = false;
		document.itemform.application_rule[1].disabled = false;
		document.itemform.fixed_discount.disabled = true;
		if (document.itemform.dct[1].checked) {
			document.itemform.discount.disabled = false;
			document.itemform.fixed_discount.value = '';
		}
		else {
			document.itemform.discount.disabled = true;
			document.itemform.discount.value = '';
		}
	}
	else if (document.itemform.dct[2].checked) {
		document.itemform.application_rule[0].disabled = true;
		document.itemform.application_rule[1].disabled = true;
		document.itemform.discount.disabled = true;
		document.itemform.fixed_discount.disabled = false;
		document.itemform.discount.value = '';
	}
}

//document.onload = chckForm;
chckForm();
</script>
<?
}





require($pathToSiteRoot . "common/template_dtl.php");
?>