<?
$pathToSiteRoot = "../../";
$appName = "diag";
require($pathToSiteRoot . "common/functions.php");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/login.php");

appRegister($arrApps[$appName]["appId"]);


function writeMainContent() {

?>
<table cellspacing=0 cellpadding=0 cellspacing=5 width="100%">
<td valign=top>
<h2 style="display:inline"><? echo getLM("DB sync status"); ?></h2> 
&nbsp; <? echo "on " . date("F j, g:i:s a");?> &nbsp;<a href="<? echo $_SERVER["SCRIPT_NAME"]; ?>">refresh</a><br>
<?
	$criticalDelay = 300 / 86400;// in seconds, then degraded to day


	$strSQL = "SELECT cafe.cafe_id
        , cafe.name
        , cafe.last_transferred AS last_upload
        , SYSDATE - cafe.last_transferred AS last_upload_d
        , cafe.last_connected
        , SYSDATE - cafe.last_connected AS last_connected_d
        , MIN(tr.time) AS first_pending
        , SYSDATE - MIN(tr.time) AS first_pending_d
        , COUNT(tr.transaction_id) as trns
        , COUNT(tr.transaction_id) - COUNT(tr.ready) AS opened

    FROM ksi.cafe LEFT JOIN 
    (SELECT transaction.time
            , transaction.cafe_id
            , transaction.transaction_id
            , transaction.ready  
        FROM ksi.transaction 
        LEFT JOIN ksi.transaction_action 
            ON transaction_action.transaction_id = transaction.transaction_id 
            AND transaction.cafe_id = transaction_action.cafe_id
        WHERE transaction_action.num IS NOT NULL  OR transaction.ready IS NULL
        GROUP BY transaction.time, transaction.transaction_id, transaction.ready, transaction.cafe_id
         ) tr ON tr.cafe_id = cafe.cafe_id 

    WHERE deleted IS NULL AND cafe.type_id = 2

    GROUP BY cafe.cafe_id, cafe.name, cafe.last_transferred, cafe.last_connected 
	ORDER BY name
	";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	ociexecute($stmt, OCI_DEFAULT);

	echo "<table cellpadding=1 cellspacing=0 border=0 class=\"grid\"><tr>
	<th>" . getLM("cafe") . "</th>
	<th>" . getLM("last time connected") . "</th>
	<th>" . getLM("last time made upload") . "</th>
	<th>" . getLM("dowload queue<br>(all / opened)") . "</th>
	<th>" . getLM("first pending download transaction") . "</th>
	</tr>";

	while (ocifetch($stmt)) {
		$i = $i ? 0 : 1;
		if ((ociresult($stmt, "LAST_CONNECTED_D")  > $criticalDelay  && ociresult($stmt, "LAST_UPLOAD_D") > $criticalDelay)
			|| ociresult($stmt, "FIRST_PENDING_D") > $criticalDelay 
			|| ociresult($stmt, "OPENED") > 0) {
			$err = 1;
		}
		else {
			$err = 0;
		}

		echo "<tr style=\"background-color: " .($err ? "#ff8080" : ($i ? "#ffffff" : "#f6f6f6")) . "\">";
		echo "<td>" . ociresult($stmt, "NAME") . "</td>";
		echo "<td" . (ociresult($stmt, "LAST_CONNECTED_D")  > $criticalDelay ? " style=\"font-weight: 900\"" : "") . ">" . ociresult($stmt, "LAST_CONNECTED") . 
		"<br><font class=\"tiny\" color=\"gray\"> <nobr>" . roundPeriod(ociresult($stmt, "LAST_CONNECTED_D")) . "</nobr></font></td>";
		echo "<td>" . ociresult($stmt, "LAST_UPLOAD") . 
		"<br><font class=\"tiny\" color=\"gray\"> <nobr>" . roundPeriod(ociresult($stmt, "LAST_UPLOAD_D")) . "</nobr></font></td>";
		echo "<td" . (ociresult($stmt, "OPENED") > 0 ? " style=\"font-weight: 900\"" : "") . ">" . ociresult($stmt, "TRNS") . " / " . ociresult($stmt, "OPENED") . "</td>";
		echo "<td" . (ociresult($stmt, "FIRST_PENDING_D")  > $criticalDelay ? " style=\"font-weight: 900\"" : "") . ">" . ociresult($stmt, "FIRST_PENDING") . 
		"<br><font class=\"tiny\" color=\"gray\"> <nobr>" . roundPeriod(ociresult($stmt, "FIRST_PENDING_D")) . "</nobr></font></td>";

		echo "</tr>";
	}
	ocifreestatement($stmt);
	echo "</table>";
?>
</td>
</table>
<?
}











function roundPeriod($val) {
	if (!$val) return "";

/*days*/	$tmpArr[0] = (int)$val;
/*hours*/	$tmpArr[1] = (int)(($val - $tmpArr[0]) * 24);
/*mins*/	$tmpArr[2] = (int)((($val - $tmpArr[0]) - $tmpArr[1] / 24) * 60 * 24);
/*secs*/	$tmpArr[3] = (int)(((($val - $tmpArr[0]) - $tmpArr[1] / 24) - $tmpArr[2] / 60 / 24) * 60 * 60 * 24);





	$tmpVar = 0;
	$precision = 2;
	if ((int)$tmpArr[0]) {
		$res .= $tmpArr[0] . " day" . ($tmpArr[0] == 1 ? "" : "s") . " ";
	}
	if ($res) $tmpVar++;
	if ($tmpVar == $precision) return $res . " ago";

	if ((int)$tmpArr[1]) {
		$res .= $tmpArr[1] . " hour" . ($tmpArr[1] == 1 ? "" : "s") . " ";
	}
	if ($res) $tmpVar++;
	if ($tmpVar == $precision) return $res . " ago";

	if ((int)$tmpArr[2]) {
		$res .= $tmpArr[2] . " minute" . ($tmpArr[2] == 1 ? "" : "s") . " ";
	}
	if ($res) $tmpVar++;
	if ($tmpVar == $precision) return $res . " ago";

	if ((int)$tmpArr[3]) {
		$res .= $tmpArr[3] . " second" . ($tmpArr[3] == 1 ? "" : "s") . " ";
		$tmpVar++;
	}

	return $res . " ago";
}

require($pathToSiteRoot . "common/template.php");
?>