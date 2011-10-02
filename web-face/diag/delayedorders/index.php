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
			$where = "created >= (sysdate - 7)";
			break;
		case 2:
			$where = "created >= (sysdate - 30)";
			break;
		case 3:
			$where = "";
			break;
		default:
			$where = "created >= (sysdate - 1)";
			break;
	
	}

?>
<table cellspacing=0 cellpadding=0 cellspacing=5 width="100%">
<td valign=top>
<form action="<? echo $_SERVER["SCRIPT_NAME"]; ?>" method=get name="form1"><h2 style="display:inline"><? echo getLM("Delayed orders created "); ?></h2> &nbsp;&nbsp; <select name=period onchange="this.form.submit();">
<?
foreach ($periodArr as $key => $val) {
	echo "<option value=\"" . $key . "\"" . ((int)$_GET["period"] == $key ? " selected" : "") . ">" . $val . "</option>";
}
?>
</select>
&nbsp; <? echo "generated on " . date("F j, g:i:s a");?> &nbsp;<a href="<? echo $_SERVER["SCRIPT_NAME"] . $_SERVER["QUERY_STRING"] ? "?" . $_SERVER["QUERY_STRING"] : ""; ?>" onclick="document.form1.submit();return false;">refresh</a>


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





//------------------- delayed orders -------------------------------

	$strSQL = "SELECT cafe_id
			, SUM(subtotal) as subtotal
			, SUM (CASE WHEN type_id = 3 THEN 1 ELSE 0 END) AS cnt3
			, SUM (CASE WHEN type_id = 4 THEN 1 ELSE 0 END) AS cnt4 

		FROM ksi.orders 
		
		WHERE " . ($where ? $where . " AND " : "") . " type_id IN (3, 4) AND status_id NOT IN (4, 5) AND delivery_id IS NOT NULL

	    GROUP BY orders.cafe_id";

//pr($strSQL);

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	while (ocifetch($stmt)) {
		$cafeArr[ociresult($stmt, "CAFE_ID")]["subtotal"] = ociresult($stmt, "SUBTOTAL");
		$cafeArr[ociresult($stmt, "CAFE_ID")]["cnt3"] = ociresult($stmt, "CNT3");
		$cafeArr[ociresult($stmt, "CAFE_ID")]["cnt4"] = ociresult($stmt, "CNT4");
	}

	ocifreestatement($stmt);










//-------------------------------------------------------------

	echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr>
	<th>" . getLM("cafe") . "</th>
	<th>" . getLM("delayed orders created (delivery / pickup)") . "</th>
	<th>" . getLM("subtotal value") . "</th>";

	echo "</tr>";

	foreach ($cafeArr as $key => $val) {
		$i = $i ? 0 : 1;

		echo "<tr style=\"background-color: " . ($i ? "#ffffff" : "#f6f6f6") . "\">";

		echo "<td>" . $val["cafe"] . "</td>";
		echo "<td>" . (($val["cnt3"] + $val["cnt4"]) ? ($val["cnt3"] + $val["cnt4"])  . " (" . (int)$val["cnt3"] . "/" . (int)$val["cnt4"] . ")" : "-") . "</td>";
		echo "<td align=right> $" . number_format($val["subtotal"], 2) . "</td>";
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