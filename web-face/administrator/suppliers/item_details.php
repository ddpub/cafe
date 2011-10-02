<?
$pathToSiteRoot = "../../";
$appName = "administrator";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

$id = (int)$_GET["id"];
$qstr = $_POST["qstr"] ? $_POST["qstr"] : substr(strstr($_SERVER["HTTP_REFERER"], "?"), 1);

$pageTitle = getLM("Adding Items to the supplier");

$basePage = "index.php";



if ($_POST["submit"] == getLM("save")) {

	logTranBegin(87);
	foreach ($_POST["iid"] as $val) {
		$strSQL = "INSERT INTO ksi.supplier_product (product_id, supplier_id) VALUES (" . (int)$val . ", " . (int)$_GET["s"] . ")";
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
		ociexecute($stmt, OCI_DEFAULT);
	}
	logTranEnd();
//	ocicommit($GLOBALS["objConn"]);

	header("Location: " . $basePage . ($qstr ? "?" . $qstr  : ""));
}













function writeMainContent() {

	writeFormTableHeader();
	$strSQL = "SELECT ksi.product.product_id, ksi.product.name, ksi.product_group.name AS gname 
		FROM ksi.product
			INNER JOIN ksi.product_group ON  ksi.product_group.group_id = ksi.product.group_id
			LEFT JOIN ksi.supplier_product ON ksi.supplier_product.product_id = ksi.product.product_id 
				AND ksi.supplier_product.supplier_id = " . (int)$_GET["s"] . "
		WHERE ksi.supplier_product.product_id IS NULL 
		ORDER BY LOWER(ksi.product_group.name), LOWER(ksi.product.name)
		";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	echo "<tr><td colspan=2 align=center><h3>" . getLM("Check items you want to add") . "</h3></td></tr>";

	$flag1 = false;
	$curG = "";
	while (ocifetch($stmt)) {
		$flag1 = true;
		if ($curG != ociresult($stmt, "GNAME")) {
			echo "<tr><td colspan=2 align=left><b>" . ociresult($stmt, "GNAME") . "</b></td></tr>";
			$curG = ociresult($stmt, "GNAME");
		}
		echo "<tr><td align=right><input type=checkbox name=\"iid[]\" value=\"" . ociresult($stmt, "PRODUCT_ID") . "\"></td><td>" . ociresult($stmt, "NAME") . "</td></tr>";
	}

	if (!$flag1) {
		echo "<tr><td colspan=2 align=center>There is no items you can add to the supplier. Seems everything has already been added.<br><br>input type=\"button\" name=\"cancel\" value=\"back\" onclick=\"location.href='" . $GLOBALS["basePage"] . ($GLOBALS["qstr"] ? "?" . $GLOBALS["qstr"]  : "") . "></td></tr>";

	}
	else {
		writeFormTableFooter();
	}


}





require($pathToSiteRoot . "common/template_dtl.php");
?>