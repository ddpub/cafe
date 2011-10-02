<?
$pathToSiteRoot = "../../";
$appName = "menumanager";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");
require($pathToSiteRoot . "common/class_rtree.php");

appRegister($arrApps[$appName]["appId"]);


if ($_GET["act"] == "del") {

	logTranBegin(120);

//	$strSQL = "DELETE FROM ksi.menu WHERE menu_id = " . (int)$_GET["id"];
	$strSQL = "UPDATE ksi.coupon SET deleted = sysdate WHERE coupon_id = " . (int)$_GET["id"];

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "Unexpected error";
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=del&/", "", $_SERVER["QUERY_STRING"]));
	}

}

elseif ($_GET["act"] == "delc") {

	logTranBegin(121);

	$strSQL = "DELETE FROM ksi.coupon_condition WHERE dish_id = " . (int)$_GET["c"] . " AND coupon_id = " . (int)$_GET["dc"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "unexpected error";
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=delc&/", "", $_SERVER["QUERY_STRING"]));
	}


}
elseif ($_GET["act"] == "dela") {

	logTranBegin(122);

	$strSQL = "DELETE FROM ksi.coupon_application WHERE dish_id = " . (int)$_GET["a"] . " AND coupon_id = " . (int)$_GET["dc"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "unexpected error";
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=dela&/", "", $_SERVER["QUERY_STRING"]));
	}
}
elseif ($_GET["act"] == "delcf") {

	logTranBegin(123);

	$strSQL = "DELETE FROM ksi.coupon_cafe WHERE cafe_id = " . (int)$_GET["c"] . " AND coupon_id = " . (int)$_GET["dc"];
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)@ociexecute($stmt, OCI_DEFAULT)) {
		$alrtMsg = "unexpected error";
	}
	else {
		logTranEnd();
		header("Location: " . $_SERVER["SCRIPT_NAME"] . "?" . preg_replace("/act=delcf&/", "", $_SERVER["QUERY_STRING"]));
	}
}




function writeMainContent() {

?>
<table cellspacing=0 cellpadding=0 cellspacing=5 width="100%">
<td valign=top>
<h2><? echo getLM("Discounts & ñoupons"); ?></h2>
<?

	$strSQL = "SELECT * FROM ksi.coupon WHERE deleted IS NULL and coupon_id > 0
		ORDER BY LOWER(ksi.coupon.title)";



	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	$GLOBALS["curCoupon"] = 0;
	$arrCoupon = array();
	while (ocifetch($stmt)) {
		if (!$GLOBALS["curCoupon"]) {
			$GLOBALS["curCoupon"] = ociresult($stmt, "COUPON_ID");
			$curCouponName = ociresult($stmt, "TITLE");
			$curCouponDesc = ociresult($stmt, "DESCRIPTION");
			$discount = ociresult($stmt, "DISCOUNT");
			$conditionRule = ociresult($stmt, "CONDITION_RULE");
			$applicationRule = ociresult($stmt, "APPLICATION_RULE");
			$fixedDiscount = (float)ociresult($stmt, "FIXED_DISCOUNT");
		}
		if ((int)$_GET["dc"] == ociresult($stmt, "COUPON_ID")) {
			$GLOBALS["curCoupon"] = ociresult($stmt, "COUPON_ID");
			$curCouponName = ociresult($stmt, "TITLE");
			$curCouponDesc = ociresult($stmt, "DESCRIPTION");
			$discount = ociresult($stmt, "DISCOUNT");
			$conditionRule = ociresult($stmt, "CONDITION_RULE");
			$applicationRule = ociresult($stmt, "APPLICATION_RULE");
			$fixedDiscount = (float)ociresult($stmt, "FIXED_DISCOUNT");
		}
		$arrCoupon[] = array(ociresult($stmt, "COUPON_ID"), ociresult($stmt, "TITLE"), ociresult($stmt, "CODE"), ociresult($stmt, "CREATED"), ociresult($stmt, "START_DATE"), ociresult($stmt, "STOP_DATE"), ociresult($stmt, "AUTH_REQUIRED"));
	}
	ocifreestatement($stmt);

	writeActionsBar(array(
		array(getLM("Add new"), "coupon_details.php", "images/icons/add.gif"),
		array(getLM("Edit"), "coupon_details.php?id=" . $GLOBALS["curCoupon"], "images/icons/edit.gif"),
		array(getLM("Delete"), $_SERVER["SCRIPT_NAME"] . "?act=del&id=" . $GLOBALS["curCoupon"], "images/icons/delete.gif", "onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\""),
	));




	echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr><th>" . getLM("coupon/discount") . "</th><!--th>code</th--><th>" . getLM("start date") . "</th><th>" . getLM("finish date") . "</th><th>" . getLM("auth reqrd") . "</th><th>" . getLM("action") . "</th></tr>";
	foreach ($arrCoupon as $val) {

		echo "<tr" . ($val[0] == $GLOBALS["curCoupon"] ? " class=\"selectedRow\"" : "") . "><td nowrap>";
		if ($val[0] == $GLOBALS["curCoupon"]) {
			echo $val[1];
		}
		else {
			echo "<a href=\"" . $_SERVER["SCRIPT_NAME"] . "?dc=" . $val[0] . "\">" . $val[1] . "</a>";
		}
		echo "</td><!--td>" . $val[2] . "</td--><td nowrap>" . substr($val[4], 0, 12) . "</td><td nowrap>" . substr($val[5], 0, 12) . "</td><td>" . ($val[6] ? getLM("yes") : getLM("no")) . "</td>";

		echo "<td class=tiny nowrap><a href=\"coupon_details.php?id=" . $val[0] . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=del&id=" . $val[0] . "\" onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
	}

	echo "</table>";
?>
</td>











<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<h2><? echo $curCouponName . " " . getLM("coupon/discount details"); ?></h2>
<?
if ($curCouponDesc) {
	echo "<p>" . $curCouponDesc . "</p>";
}

?>
<h3><? echo getLM("Coupon/discount condition list"); ?></h3>
<?
	writeActionsBar(array(
		array(getLM("Add new"), "condition_details.php?dc=" . $GLOBALS["curCoupon"], "images/icons/add.gif")
	));

	$strSQL = "SELECT ksi.coupon_condition.quantity AS qty, ksi.coupon_condition.dish_id, ksi.dish.name 
		FROM ksi.coupon_condition INNER JOIN ksi.dish ON ksi.coupon_condition.dish_id = ksi.dish.dish_id
		WHERE coupon_id = " . $GLOBALS["curCoupon"];


	$stmt = ociparse($GLOBALS["objConn"], $strSQL);

	ociexecute($stmt, OCI_DEFAULT);

	echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr><th>" . getLM("entree") . "</th><th>" . getLM("qty") . "</th><th>" . getLM("action") . "</th></tr>";
	while (OCIFetchInto($stmt, $row, OCI_ASSOC))  {
		echo "<tr><td>" . $row["NAME"] . "</td><td align=right>" . $row["QTY"] . "</td>";
		echo "<td class=tiny nowrap><a href=\"condition_details.php?dc=" . $GLOBALS["curCoupon"] . "&c=" . $row["DISH_ID"] . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delc&dc=" . $GLOBALS["curCoupon"] . "&c=" . $row["DISH_ID"] . "\" onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
	}
	ocifreestatement($stmt);
	echo "</table>";

	echo "<p>" . getLM("If list above is empty, coupon/discount will work without conditions.") . "</p>";

	if ($conditionRule) {
		echo "<p>" . getLM("ALL of items above make coupon/discount work.<sup>*</sup>") . "</p>";
	}
	else {
		echo "<p>" . getLM("ANY of items above make coupon/discount work.<sup>*</sup>") . "</p>";
	}
?>







<h3><? echo getLM("Coupon/discount application list"); ?></h3>
<?

	if (!$fixedDiscount) {

		writeActionsBar(array(
			array(getLM("Add new"), "application_details.php?dc=" . $GLOBALS["curCoupon"], "images/icons/add.gif")
		));
	
		$strSQL = "SELECT ksi.coupon_application.quantity AS qty, ksi.coupon_application.dish_id, coupon_application.price, ksi.dish.name 
			FROM ksi.coupon_application INNER JOIN ksi.dish ON ksi.coupon_application.dish_id = ksi.dish.dish_id
			WHERE coupon_id = " . $GLOBALS["curCoupon"] . "";
	
	
		$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	
		ociexecute($stmt, OCI_DEFAULT);
	
		echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr><th>" . getLM("entree") . "</th><th>" . getLM("qty") . "</th><th>" . getLM("price") . "</th><th>" . getLM("action") . "</th></tr>";
	
		$flag1 = false;
		while (OCIFetchInto($stmt, $row, OCI_ASSOC))  {
			$flag1 = true;
			echo "<tr><td>" . $row["NAME"] . "</td><td align=right>" . $row["QTY"] . "</td><td align=right>";
			if ($discount) {
				echo ($discount * 100) . "% off<sup>*</sup>";
			}
			else {
				echo "$" . number_format($row["PRICE"], 2);
			}
			echo "</td>";
			echo "<td class=tiny nowrap><a href=\"application_details.php?dc=" . $GLOBALS["curCoupon"] . "&a=" . $row["DISH_ID"] . "\">" . getLM("edit") . "</a> | <a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=dela&dc=" . $GLOBALS["curCoupon"] . "&a=" . $row["DISH_ID"] . "\" onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";
		}
		ocifreestatement($stmt);
		echo "</table>";
	
		if (!$flag1) {
			echo "<p>" . getLM("All the entrees in order are ") . ($discount*100) . "% off<sup>*</sup></p>";
		}
	
		echo "<p>" . getLM("If list above is empty, discount will be applied to all entrees in order.<br>If no qty is specified for an entree above, discount will be applied to all entities of that entree in order.") . "</p>";
	

		if ($applicationRule) {
			echo "<p>" . getLM("Discount is applied to ALL items above") . ".<sup>*</sup></p>";
		}
		else {
			echo "<p>" . getLM("Discount is applied to any ONE of items above") . ".<sup>*</sup></p>";
		}
	}
	else {
		echo "<p>" . getLM("This coupon gives a fixed discount to order. Discount amount is $") . number_format($fixedDiscount, 2) . ".<sup>*</sup></p>";
	}
?>
<hr width=40% size=1 align=left>
<sup>*</sup> <? echo getLM("You can change these settings via coupon/discount editing"); ?>

</td>



<td>&nbsp;&nbsp;&nbsp;</td>
<td valign=top>
<h2><? echo getLM("Participating cafes");?></h2>
<?
	writeActionsBar(array(
		array(getLM("Add new"), "c_cafe_details.php?dc=" . $GLOBALS["curCoupon"], "images/icons/add.gif")
	));

	$strSQL = "SELECT ksi.cafe.name, ksi.cafe.cafe_id FROM ksi.cafe INNER JOIN ksi.coupon_cafe ON ksi.cafe.cafe_id = ksi.coupon_cafe.cafe_id AND ksi.coupon_cafe.coupon_id = " . $GLOBALS["curCoupon"] . " ORDER BY LOWER(name)";


	$stmt = ociparse($GLOBALS["objConn"], $strSQL);

	ociexecute($stmt, OCI_DEFAULT);


	echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr><th>" . getLM("cafe") . "</th><th>" . getLM("action") ." </th></tr>";
	while (OCIFetchInto($stmt, $row, OCI_ASSOC))  {
		echo "<tr><td>" . $row["NAME"] . "</td>";
		echo "<td class=tiny nowrap><a href=\"" . $_SERVER["SCRIPT_NAME"] . "?act=delcf&dc=" . $GLOBALS["curCoupon"] . "&c=" . $row["CAFE_ID"] . "\" onclick=\"return confirm('" . getLM("Are you sure you want to delete this record?") . "')\">" . getLM("delete") . "</a></td></tr>";

	}
	ocifreestatement($stmt);
	echo "</table>";
?>
</td>








</table>
<?
}

require($pathToSiteRoot . "common/template.php");
