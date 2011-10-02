<?
$pathToSiteRoot = "../../";
$appName = "administrator";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);

if ($_GET["act"] == "dels") {

	logTranBegin(83);
	$strSQL = "DELETE FROM ksi.supplier WHERE supplier_id = " . (int)$_GET["id"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = getLM("cannot delete supplier: it is being used");
	}
	else {
		logTranEnd();
	}

}
elseif ($_GET["act"] == "delw") {
	logTranBegin(85);
	$strSQL = "DELETE FROM ksi.warehouse_supplier WHERE warehouse_id = " . (int)$_GET["w"] . " AND supplier_id = " . (int)$_GET["s"];

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "unexpected error";
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=delw&/", "", $_SERVER["QUERY_STRING"]));
	}

}
elseif ($_GET["act"] == "deli") {
	logTranBegin(88);

	$strSQL = "DELETE FROM ksi.supplier_product WHERE product_id = " . (int)$_GET["i"] . " AND supplier_id = " . (int)$_GET["s"];

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "unexpected error";
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=deli&/", "", $_SERVER["QUERY_STRING"]));
	}

}

function writeMainContent() {

?>
<table cellspacing=0 cellpadding=0 cellspacing=5 width="100%">
<td valign=top>
<h2><? echo getLM("Suppliers");?></h2>
<?
	$strSQL = "SELECT * 
		FROM ksi.supplier 
		ORDER BY LOWER(name)";



	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	$curSpl = 0;
	while (ocifetch($stmt)) {
		if (!$curSpl) {
			$curSpl = ociresult($stmt, "SUPPLIER_ID");
			$curSplName = ociresult($stmt, "NAME");
		}
		if ((int)$_GET["s"] == ociresult($stmt, "SUPPLIER_ID")) {
			$curSpl = ociresult($stmt, "SUPPLIER_ID");
			$curSplName = ociresult($stmt, "NAME");
		}
		$arrSpl[] = array(ociresult($stmt, "SUPPLIER_ID"), ociresult($stmt, "NAME"), ociresult($stmt, "PHONE"), ociresult($stmt, "EMAIL"));
	}
	ocifreestatement($stmt);

	$tmpArr = array(
		array(getLM("Add new"), "supplier_details.php", "images/icons/add.gif")
	);

	if ($curSpl) {
		array_push($tmpArr, 
			array(getLM("Edit"), "supplier_details.php?id=" . $curSpl, "images/icons/edit.gif"),
			array(getLM("Delete"), $_SERVER["SCRIPT_NAME"] . "?act=delm&id=" . $curSpl, "images/icons/delete.gif", "onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\"")
		);

	}

	writeActionsBar($tmpArr);

	echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr><th>" . getLM("name") . "</th><th>" . getLM("phone") . "</th><th>" . getLM("e-mail") . "</th><th>" . getLM("action") . "</th></tr>";

	if ($arrSpl) {
		foreach ($arrSpl as $val) {
			echo "<tr" . ($val[0] == $curSpl ? " class=\"selectedRow\"" : "") . "><td>";
	
	
			if ($val[0] == $curSpl) {
				echo $val[1];
			}
			else {
				echo "<a href=\"" . $_SERVER["SCRIPT_NAME"] . "?s=" . $val[0] . "\">" . $val[1] . "</a>";
			}
	
			echo "<td>" . $val[2] . "</td><td>" . $val[3] . "</td><td class=tiny nowrap><a href=\"supplier_details.php?id=" . $val[0] . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=dels&id=" . $val[0] . "\" onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
		}
	}
	echo "</table>";


?>
</td>
<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<h2><? echo ucfirst($curSplName) . " " . getLM("warehouses");?></h2>
<?



	$strSQL = "SELECT ksi.warehouse.* 
		FROM ksi.warehouse 
			INNER JOIN ksi.warehouse_supplier ON ksi.warehouse_supplier.warehouse_id = ksi.warehouse.warehouse_id
		WHERE ksi.warehouse_supplier.supplier_id = " . $curSpl . " AND ksi.warehouse.deleted IS NULL";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);


	writeActionsBar(array(
		array(getLM("Add new"), "warehouse_details.php?s=" . $curSpl, "images/icons/additem.gif"),
	));


	echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
	echo "<tr><th>" . getLM("warehouse") . "</th><th>" . getLM("action") . "</th></tr>";
	while (ocifetch($stmt)) {
		echo "<tr><td>" . ociresult($stmt, "NAME") . "</td>
		<td class=tiny nowrap><a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delw&w=" . ociresult($stmt, "WAREHOUSE_ID") . "&s=" . $curSpl . "\" onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
	}
	ocifreestatement($stmt);

	echo "</table>";

?>
</td>
<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<h2><? echo ucfirst($curSplName) . " " . getLM("items");?> </h2>
<?



	$strSQL = "SELECT ksi.product.product_id, ksi.product.name, ksi.product_group.name AS pgroup 
		FROM ksi.product 
			INNER JOIN ksi.product_group ON ksi.product.group_id = ksi.product_group.group_id
			INNER JOIN ksi.supplier_product ON ksi.product.product_id = ksi.supplier_product.product_id
		WHERE ksi.supplier_product.supplier_id = " . $curSpl . "
		ORDER BY LOWER(ksi.product_group.name), LOWER(ksi.product.name)";


	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);


	writeActionsBar(array(
		array(getLM("Add new"), "item_details.php?s=" . $curSpl, "images/icons/additem.gif"),
	));


	echo "<table cellspacing=0 cellpadding=1 border=0 class=\"grid\">";
	echo "<tr><th>" . getLM("item") . "</th><th>" .getLM("action")  . "</th></tr>";
	$curG = "";
	while (ocifetch($stmt)) {
		if ($curG != ociresult($stmt, "PGROUP")) {
			echo "<tr><td colspan=2><h3 style=\"margin-top: 10px; margin-left: -10px; margin-bottom: 0px\"><img src=\"" . $GLOBALS["pathToSiteRoot"] . "images/icons/fld_c.gif\" width=16 height=16 alt=\"\" align=absmiddle> " . ociresult($stmt, "PGROUP") . "</h3></td></tr>";
			$curG = ociresult($stmt, "PGROUP");
		}
		echo "<tr><td>" . ociresult($stmt, "NAME") . "</td>
		<td class=tiny nowrap><a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=deli&i=" . ociresult($stmt, "PRODUCT_ID") . "&s=" . $curSpl . "\" onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
	}
	ocifreestatement($stmt);

	echo "</table>";

?>

</td>

</table>
<?
}

require($pathToSiteRoot . "common/template.php");
?>