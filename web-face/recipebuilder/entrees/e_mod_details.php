<?
$pathToSiteRoot = "../../";
$appName = "recipebuilder";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);


$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);
$table = "ksi.dish_mod_group";


$basePage = "index.php";



if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("dish_id", (int)$_GET["e"]),
		array("group_id", (int)$_POST["group_id"])
	);


	insertItem($table, $arrFV, 33);

	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));

}


$strSQL = "SELECT ksi.dish.name AS dish_name
	FROM ksi.dish 
	WHERE ksi.dish.dish_id = " . (int)$_GET["e"];

$stmt = ociparse($GLOBALS["objConn"], $strSQL);
ociexecute($stmt, OCI_DEFAULT);

ocifetchinto($stmt, $result, OCI_ASSOC);

if (!$result) {
	echo "error: record not found";
	return;
}

$pageTitle =  "Adding a modifier group to the " . $result["DISH_NAME"] . " entree";




function writeMainContent() {

	writeFormTableHeader();
	writeValRow(getLM("Entree"), $GLOBALS["result"]["DISH_NAME"]);

	writeSelectSQLRow(getLM("Modifier group"), "group_id", "SELECT ksi.mod_group.group_id, ksi.mod_group.name FROM ksi.mod_group LEFT JOIN ksi.dish_mod_group ON ksi.mod_group.group_id = ksi.dish_mod_group.group_id AND ksi.dish_mod_group.dish_id = " . (int)$_GET["e"] . " WHERE ksi.dish_mod_group.dish_id IS NULL AND ksi.mod_group.deleted IS NULL ORDER BY ksi.mod_group.name", "")

?>
<tr>
	<td align=right valign=top><? echo getLM("Note")?>: </td>
	<td><? echo getLM("If you don't see a modifier group in the list,<br>it means that the missing group is already added."); ?></td>
</tr>
<?
	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>