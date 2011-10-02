<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = getLM("Modifier group copy");
$basePage = "index.php";





if ($_POST["submit"] == getLM("save")) {

	$strSQL = "SET TRANSACTION NAME 'Copy_modifier_group_" . $id . "'";
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	logTranBegin(111);


	$strSQL = "INSERT INTO ksi.mod_group (group_id, name, max_weight, pos) VALUES (ksi.mod_group_seq.nextval, '" . sqlFixUp($_POST["name"]) . "', " . (int)$_POST["max_weight"] . ", " . (int)$_POST["pos"] . ") RETURNING group_id INTO :group_id";
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	OCIBindByName($stmt, ":group_id", &$newGroupId, 32);

	if (!@ociexecute($stmt, OCI_DEFAULT)) {
		$strError = getLM("Could not create a modifier group. Probably group with the same name already exists");

		logTranEnd();

		$strSQL = "ROLLBACK";
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
	}
	else {
		$strSQL = "INSERT INTO ksi.modifier (modifier_id, name, group_id, def_price, pos, dish_id, weight) (SELECT ksi.modifier_seq.nextval, name, " . $newGroupId . ", def_price, pos, dish_id, weight FROM ksi.modifier WHERE group_id = " . $id . ")";

		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);

		logTranEnd();

		$strSQL = "COMMIT";
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);

		$strError = getLM("Modifier group copied successfully");

	}
}








if ($id) {
	$strSQL = "SELECT * FROM ksi.mod_group WHERE group_id = " . $id;
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
		echo "<center><h2>" . ucfirst($GLOBALS["result"]["NAME"]) . " " . getLM("modifier group copy status") . "</h2><p>" . $GLOBALS["strError"] . "</p>";
		echo "<br><input type=\"button\" value=\"" . getLM("return to modifiers") . "\" onclick=\"location.href='" . $GLOBALS["basePage"] . ($GLOBALS["qstr"] ? "?" . $GLOBALS["qstr"]  : "") . "'\"></center>";
		return;
	}

	writeFormTableHeader();
	if ($GLOBALS["id"]) 

		echo "<tr><td colspan=2><p>" . getLM("You are about to create new modifier group via copy exisitng modifier group with its contents") ."</p><p>" . getLM("Existing modifier group") . ":</p></td></tr>";

		writeValRow("id", $GLOBALS["id"]);
		writeValRow(getLM("Modifier group title"), $GLOBALS["result"]["NAME"]);
		writeValRow(getLM("Max. abstract weight of modifiers to be selected"), $GLOBALS["result"]["MAX_WEIGHT"]);

		echo "<tr><td align=right valign=top>" . getLM("Modifiers in the group") . ":</td><td>";

		$strSQL = "SELECT * FROM ksi.modifier WHERE group_id = " . $GLOBALS["id"] . " ORDER BY pos";
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);

		echo "<table border=0 cellspacing=0>";
		while (ocifetch($stmt)) {
			echo "<tr><td>" .  ociresult($stmt, "NAME") . "</td><td align=right>" .  ociresult($stmt, "WEIGHT") . "</td></tr>";
		}
		echo "</table>";
		ocifreestatement($stmt);

		echo "</td></tr>";

		echo "<tr><td colspan=2><p>" . getLM("Modifier group to be created") . ":</p></td></tr>";

		writeTextInputRow(getLM("Modifier group title"), "name", "");
		writeTextInputRow(getLM("Position in list"), "pos", $GLOBALS["result"]["POS"]);
		writeTextInputRow(getLM("Max. abstract weight of modifiers to be selected"), "max_weight", $GLOBALS["result"]["MAX_WEIGHT"]);
		writeFormTableFooter();
?>
<script>
document.itemform.onsubmit = checkForm;

function checkForm() {
	if (document.itemform.name.value == '') {
		alert('Modifier group name is required');
		document.itemform.name.focus();
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