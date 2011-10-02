<?
$pathToSiteRoot = "../../";
$appName = "menumanager";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

//$id = (int)$_GET["c"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);
$table = "ksi.coupon_cafe";

$basePage = "index.php";



if ($_POST["submit"] == getLM("save")) {

	$arrFV = array(
		array("coupon_id", (int)$_GET["dc"]),
		array("cafe_id", (int)$_POST["cafe_id"])
	);


	if ($id) {
/*
		logTranBegin(114);

		$strSQL = "UPDATE " . $table . " 
			SET quantity = " . (int)$_POST["quantity"] . "
			WHERE coupon_id = " . (int)$_GET["dc"] . " AND dish_id = " . (int)$_GET["c"];
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);

		logTranEnd();
*/
	}
	else {
		insertItem($table, $arrFV, 117);
	}
	header("Location: " . $basePage . "?dc=" . (int)$_GET["dc"]/*($qstr ? "?" . $qstr  : "")*/);

}


$pageTitle = getLM("Adding a cafe to the coupon/discount");




function writeMainContent() {

	writeFormTableHeader();
	writeSelectSQLRow(getLM("Cafe"), "cafe_id", "SELECT ksi.cafe.cafe_id, ksi.cafe.name 
		FROM ksi.cafe 
		WHERE ksi.cafe.type_id = 2 
			AND deleted IS NULL
			AND ksi.cafe.cafe_id NOT IN (SELECT ksi.cafe.cafe_id FROM ksi.cafe INNER JOIN ksi.coupon_cafe ON ksi.coupon_cafe.cafe_id = ksi.cafe.cafe_id AND ksi.coupon_cafe.coupon_id = " . (int)$_GET["dc"] . ") ORDER BY LOWER(name)", "");

?>
<tr>
	<td align=right valign=top><? echo getLM("Note");?>: </td>
	<td><? echo getLM("If you don't see a cafe in the list,<br>it means that the missing cafe is already added.");?></td>
</tr>
<?
	writeFormTableFooter();
}





require($pathToSiteRoot . "common/template_dtl.php");
?>