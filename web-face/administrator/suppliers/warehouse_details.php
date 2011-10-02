<?
$pathToSiteRoot = "../../";
$appName = "administrator";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = getLM("Adding warehouses to the supplier");

$basePage = "index.php";



if ($_POST["submit"] == getLM("save")) {

	logTranBegin(84);

	foreach ($_POST["wid"] as $val) {
		$strSQL = "INSERT INTO ksi.warehouse_supplier (warehouse_id, supplier_id) VALUES (" . (int)$val . ", " . (int)$_GET["s"] . ")";
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
	}

	logTranEnd();
//	ocicommit($GLOBALS["objConn"]);

	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));
}













function writeMainContent() {

	writeFormTableHeader();
	$strSQL = "SELECT ksi.warehouse.* 
		FROM ksi.warehouse 
			LEFT JOIN ksi.warehouse_supplier ON ksi.warehouse_supplier.warehouse_id = ksi.warehouse.warehouse_id 
				AND ksi.warehouse_supplier.supplier_id = " . (int)$_GET["s"] . "
		WHERE ksi.warehouse_supplier.warehouse_id IS NULL AND ksi.warehouse.deleted IS NULL
		ORDER BY LOWER(ksi.warehouse.name)
		";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	echo "<tr><td colspan=2 align=center><h3>" . getLM("Check warehouses you want to add") . "</h3></td></tr>";

	$flag1 = false;
	while (ocifetch($stmt)) {
		$flag1 = true;
		echo "<tr><td align=right><input type=checkbox name=\"wid[]\" value=\"" . ociresult($stmt, "WAREHOUSE_ID") . "\"></td><td>" . ociresult($stmt, "NAME") . "</td></tr>";
	}

	if (!$flag1) {
		echo "<tr><td colspan=2 align=center>" . getLM("There are no warehouses you can add to the supplier. Seems everything has already been added.") . "<br><br>input type=\"button\" name=\"cancel\" value=\"back\" onclick=\"location.href='" . $GLOBALS["basePage"] . ($GLOBALS["qstr"] ? "?" . $GLOBALS["qstr"]  : "") . "></td></tr>";

	}
	else {
		writeFormTableFooter();
	}


}





require($pathToSiteRoot . "common/template_dtl.php");
?>