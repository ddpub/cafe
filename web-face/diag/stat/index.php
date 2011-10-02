<?
set_time_limit(300);

$pathToSiteRoot = "../../";
$appName = "diag";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);





function writeMainContent() {

	$showBz = $_GET["bz"] || !$_SERVER["QUERY_STRING"];

	$periodArr = array(
		0 => "last 24 hours",
		1 => "last 7 days",
		2 => "last 30 days",
		3 => "since opening"
	);
	
	
	switch ((int)$_GET["period"]) {
		case 1:
			$where = "WHERE closed >= (sysdate - 7)";
			break;
		case 2:
			$where = "WHERE closed >= (sysdate - 30)";
			break;
		case 3:
			$where = "";
			break;
		default:
			$where = "WHERE closed >= (sysdate - 1)";
			break;
	
	}

?>
<table cellspacing=0 cellpadding=0 cellspacing=5 width="100%">
<td valign=top>
<form action="<? echo $_SERVER["SCRIPT_NAME"]; ?>" method=get name="form1"><h2 style="display:inline"><? echo getLM("Order generation statistics for"); ?></h2> &nbsp;&nbsp; <select name=period onchange="this.form.submit();">
<?
foreach ($periodArr as $key => $val) {
	echo "<option value=\"" . $key . "\"" . ((int)$_GET["period"] == $key ? " selected" : "") . ">" . $val . "</option>";
}
?>
</select>
&nbsp; <? echo "generated on " . date("F j, g:i:s a");?> &nbsp;<a href="<? echo $_SERVER["SCRIPT_NAME"] . $_SERVER["QUERY_STRING"] ? "?" . $_SERVER["QUERY_STRING"] : ""; ?>" onclick="document.form1.submit();return false;">refresh</a>

<br><input type=checkbox name="bz"<? echo $showBz ? " checked" : ""; ?> onclick="this.form.submit()"> <span onclick="document.form1.bz.click()" style="cursor: default;">Show productivity data</span>

</form>
<?
	$cafeArr = array();

	$strSQL = "SELECT cafe.cafe_id, name 
		FROM ksi.cafe 
		WHERE deleted IS NULL AND type_id = 2 
		ORDER BY name";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	while (ocifetch($stmt)) {
		$cafeArr[ociresult($stmt, "CAFE_ID")]["cafe"] = ociresult($stmt, "NAME");
	}
	ocifreestatement($stmt);



// -------------------------- cashier version -----------------------------------

	$strSQL = "SELECT DISTINCT cafe_id, CASE WHEN pos_version IS NULL THEN 'prior to 5 1.0.15.65
' ELSE pos_version END AS pos_version
	    FROM ksi.orders
	    " . $where . "
	    ORDER BY cafe_id";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	while (ocifetch($stmt)) {
		$cafeArr[ociresult($stmt, "CAFE_ID")]["pos"] .= ($cafeArr[ociresult($stmt, "CAFE_ID")]["pos"] ? "<br>" : "") . ociresult($stmt, "POS_VERSION");
	}
	ocifreestatement($stmt);



// -------------------------- kitchen informer version -----------------------------------

	$strSQL = "SELECT DISTINCT orders.cafe_id, /*CASE WHEN ki_version IS NULL THEN 'prior to 1.0.3.16' ELSE ki_version END AS*/ ki_version
	    FROM ksi.order_dish
	        INNER JOIN ksi.orders ON orders.order_id = order_dish.order_id AND orders.cafe_id = order_dish.cafe_id
	    " . $where . "
	    ORDER BY orders.cafe_id";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	while (ocifetch($stmt)) {
		$cafeArr[ociresult($stmt, "CAFE_ID")]["ki"] .= ($cafeArr[ociresult($stmt, "CAFE_ID")]["ki"] ? "<br>" : "") . ociresult($stmt, "KI_VERSION");
	}
	ocifreestatement($stmt);



//------------------------ productivity data ------------------------

	$strSQL = "SELECT cafe_id, COUNT(DISTINCT order_id) AS orders, SUM(CASE WHEN dish_id IS NULL THEN 0 ELSE dish_count END) AS dish_count, SUM((dish_price + mod_price) * dish_count) AS order_amount, AVG((closed - created) * 86400) AS order_time
    FROM ksi.order_dish_sales_v
	    " . $where . ($where ? " AND" : " WHERE ") . " type_id IN (1, 2, 3, 4) AND status_id IN (3)
    GROUP BY cafe_id";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	while (ocifetch($stmt)) {
		$cafeArr[ociresult($stmt, "CAFE_ID")]["orders"] = ociresult($stmt, "ORDERS");
		$cafeArr[ociresult($stmt, "CAFE_ID")]["dishes"] = ociresult($stmt, "DISH_COUNT");
		$cafeArr[ociresult($stmt, "CAFE_ID")]["order_amount"] = ociresult($stmt, "ORDER_AMOUNT");
		$cafeArr[ociresult($stmt, "CAFE_ID")]["order_time"] = ociresult($stmt, "ORDER_TIME");
	}
	ocifreestatement($stmt);




//------------------------ missed orders ------------------------

	$strSQL = "SELECT cafe_id, order_id, TO_CHAR(created, 'mon dd hh12:mipm') AS created, pos_version FROM ksi.orders 
	    " . str_replace("closed", "created", $where) . "
    ORDER BY cafe_id, order_id";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	$curCafeId = 0;
	$tmpStr = "";
	while (ocifetch($stmt)) {
		if ($curCafeId != ociresult($stmt, "CAFE_ID")) {
			if ($curCafeId) {
				$cafeArr[$curCafeId]["missed_orders"] = $tmpStr;
			} 
			$tmpStr = "";
			$curCafeId = ociresult($stmt, "CAFE_ID");

			$minOrderId = ociresult($stmt, "ORDER_ID");
			$minOrderCreated = ociresult($stmt, "CREATED");
			$minPosVersion = ociresult($stmt, "POS_VERSION");
		} else {
			if (ociresult($stmt, "ORDER_ID") - $minOrderId > 1) {
				$tmpStr2  = "";
				$tmpStr .= "created between " . $minOrderCreated . "(" . $minPosVersion . ") and " . ociresult($stmt, "CREATED") . " (" . ociresult($stmt, "POS_VERSION") . "): ";
				for ($i = $minOrderId + 1; $i < ociresult($stmt, "ORDER_ID"); $i++) {
					$tmpStr2 .= ", <b>" . $i . "</b>";
				}
				$tmpStr .= substr($tmpStr2, 2) . ".<br>";
			}
			$minOrderId = ociresult($stmt, "ORDER_ID");
			$minOrderCreated = ociresult($stmt, "CREATED");
			$minPosVersion = ociresult($stmt, "POS_VERSION");
		}

	}
	$cafeArr[$curCafeId]["missed_orders"] = $tmpStr;
	ocifreestatement($stmt);





//------------------------ cc transaction count ------------------------

	$strSQL = "SELECT cafe_id, COUNT(DISTINCT transaction_id) AS cctrn_count, COUNT(DISTINCT order_id) AS cctrn_orders FROM ksi.order_cctrans
	    " . str_replace("closed", "tran_date", $where) . "
	    GROUP BY cafe_id";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	while (ocifetch($stmt)) {
		$cafeArr[ociresult($stmt, "CAFE_ID")]["cctrn_count"] = ociresult($stmt, "CCTRN_COUNT");
		$cafeArr[ociresult($stmt, "CAFE_ID")]["cctrn_orders"] = ociresult($stmt, "CCTRN_ORDERS");
	}

	ocifreestatement($stmt);




//------------------- cc amount -------------------------------

	$strSQL = "select sum(order_cctrans.amount) as val1,  t1.cafe_id
from 
  (select order_cctrans.order_id, order_cctrans.cafe_id,  max(order_cctrans.trans_id) as tttid 
         from ksi.order_cctrans  
                  left join ksi.order_cctrans cc2 on order_cctrans.ref_transaction_id = cc2.ref_transaction_id
                       and cc2.type_id = 2
                  
         where order_cctrans.type_id in (1, 4) and cc2.trans_id is null
  group by order_cctrans.order_id, order_cctrans.cafe_id) t1

inner join ksi.order_cctrans on order_cctrans.trans_id = t1.tttid and order_cctrans.cafe_id = t1.cafe_id

	    " . str_replace("closed", "tran_date", $where) . "
	    GROUP BY t1.cafe_id";

//pr($strSQL);

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	while (ocifetch($stmt)) {
		$cafeArr[ociresult($stmt, "CAFE_ID")]["cc_amnt"] = ociresult($stmt, "VAL1");
	}

	ocifreestatement($stmt);










//-------------------------------------------------------------

	echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr>
	<th>" . getLM("cafe") . "</th>
	<th>" . getLM("poses used") . "</th>
	<th>" . getLM("k.i.'s used") . "</th>
	<th>" . getLM("missed orders") . "</th>
	<th>" . getLM("cc trns / orders / amount") . "</th>";

	if ($showBz) {
		echo "<th>" . getLM("closed orders / entrees") . "</th>
		<th>" . getLM("net sales") . "</th>
		<th colspan=4>" . getLM("AVG values:<br>entree price / entrees per order / order subtotal / order time") . "</th>";
	}
	echo "</tr>";

	foreach ($cafeArr as $key => $val) {
		$i = $i ? 0 : 1;

		echo "<tr style=\"background-color: " .($err ? "#ff8080" : ($i ? "#ffffff" : "#f6f6f6")) . "\">";

		echo "<td>" . $val["cafe"] . "</td>";
		echo "<td>" . $val["pos"] . "</td>";
		echo "<td>" . $val["ki"] . "</td>";
		echo "<td>" . $val["missed_orders"] . "</td>";
		echo "<td align=right>" . (int)$val["cctrn_count"] . " / " . (int)$val["cctrn_orders"] . " / $" . number_format($val["cc_amnt"], 2, ".", ",") . "</td>";
		if ($showBz) {
			echo "<td align=right>" . number_format($val["orders"], 0) . " / " . number_format($val["dishes"]) . "</td>";
			echo "<td align=right>$" . number_format($val["order_amount"], 2, ".", ",") . "</td>";
			echo "<td align=right>" . ((int)$val["dishes"] ? "$" . number_format($val["order_amount"] / $val["dishes"], 2, ".", ",") : "n/a") . "</td>";
			echo "<td align=right>" . ((int)$val["orders"] ? number_format($val["dishes"] / $val["orders"], 2, ".", ",") : "n/a") . "</td>";
			echo "<td align=right>" . ((int)$val["orders"] ? "$" . number_format($val["order_amount"] / $val["orders"], 2, ".", ",") : "n/a") . "</td>";
			echo "<td align=right>" . ( is_null($val["order_time"]) ? "n/a" : min_sec($val["order_time"])) . "</td>";

		}
		echo "</tr>";
	}
	echo "</table>";
?>
</td>
</table>
<?
}








function min_sec($val) {
	$val = (int)($val + 0.5);
	return (int)($val / 60) . ":" . substr("0" . ($val % 60), strlen("0" . ($val % 60)) - 2, 2);
}




require($pathToSiteRoot . "common/template.php");
?>