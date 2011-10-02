<?

$arrDBFn = array(
	"TO_DATE"
);

function sqlFixUp($str) {
	return str_replace("'", "''", $str);
}


function pr($str) {
	echo "<pre>";
	print_r($str);
	echo "</pre>";
}


function appRegister($appId) {
	global $objConn;

	$strSQL = "begin ksi.application_register(" . $appId . ", :res, :res_str); end;";

	$stmt = OCIParse($objConn, $strSQL);

	ocibindbyname($stmt, ":res" , &$res, 32);
	ocibindbyname($stmt, ":res_str" , &$res_str, 4096);
	ociexecute($stmt);

	if ($res != 1) {
		session_destroy();
		setcookie("logged", 0, 0, "/");
		die ($res_str);
	}

	OCIFreeStatement($stmt);
}






function insertItem($table, $arrFV, $trnId = 0, $haltOnError = 1) {

	if (!count($arrFV))	{
		exit("empty insert parameters");
	}


	$sql = "INSERT INTO " . $table . " (";
	for ($i = 0; $i < count($arrFV); $i++)	{
		$sql .= $arrFV[$i][0] . ", ";
	}
	$sql = substr($sql, 0, strlen($sql)-2);

	$sql .= ") VALUES (";
	
	for ($i = 0; $i < count($arrFV); $i++)	{

		if (is_null($arrFV[$i][1])) {
			$sql .=  "NULL";
		}
		elseif ( in_array(substr($arrFV[$i][1], 0, strpos($arrFV[$i][1], "(")), $GLOBALS["arrDBFn"]) && substr($arrFV[$i][1], strlen($arrFV[$i][1]) - 1, 1) == ")" ) {
			$sql .= substr($arrFV[$i][1], 0, strpos($arrFV[$i][1], "(")) . "(" . substr($arrFV[$i][1],  strpos($arrFV[$i][1], "(") + 1, strlen($arrFV[$i][1]) - strpos($arrFV[$i][1], "(") - 2) . ")";
		}
		elseif (strpos($arrFV[$i][1], ".nextval")) {
			$sql .= $arrFV[$i][1];
		}
		else {
			$sql .= "'" . stripslashes(sqlFixUp($arrFV[$i][1])) . "'";
		}
		$sql .= ", ";
	}

	$sql = substr($sql, 0, strlen($sql)-2);

	$sql .= ") RETURNING " . $arrFV[count($arrFV) - 1][0] . " INTO :retval " ;
	

	if ($GLOBALS["__debuginfo"]) {
		echo "<br>inserting record :<br>";
		pr($sql);
	}

	if ($trnId) logTranBegin($trnId);

	$stmt = ociparse($GLOBALS["objConn"], $sql);
	
	OCIBindByName($stmt, ":retval", &$retVal, 32);

	if (!@ociexecute($stmt, OCI_DEFAULT)) {

		if ($GLOBALS["__debuginfo"]) {
			pr($e);
		}

		if ($haltOnError) {
			$e = oci_error($stmt);
			pr($e);
			die("error inserting record");
		}
		else {
			ocirollback($GLOBALS["objConn"]);
			return 0; // error
		}
	}
//	ocicommit($GLOBALS["objConn"]);

	if ($trnId) logTranEnd();
	
	return $retVal/*1*/; // success
/*
	if (isset($GLOBALS["upload"]) && is_object($GLOBALS["upload"])) {
		$GLOBALS["upload"] -> renameFiles(mysql_insert_id());
	}

	return $id;
*/
}



function updateItem($table, $arrFV, $idfield, $idvalue, $trnId = 0, $haltOnError = 1) {

	if (!count($arrFV))	{
		exit("empty update parameters");
	}

	$sql = "UPDATE " . $table . " SET ";
	for ($i = 0; $i < count($arrFV); $i++)	{

		$sql .= $arrFV[$i][0] . " = ";

		if (is_null($arrFV[$i][1])) {
			$sql .=  "NULL";
		}

		elseif ( in_array(substr($arrFV[$i][1], 0, strpos($arrFV[$i][1], "(")), $GLOBALS["arrDBFn"]) && substr($arrFV[$i][1], strlen($arrFV[$i][1]) - 1, 1) == ")" ) {
			$sql .= substr($arrFV[$i][1], 0, strpos($arrFV[$i][1], "(")) . "(" . substr($arrFV[$i][1],  strpos($arrFV[$i][1], "(") + 1, strlen($arrFV[$i][1]) - strpos($arrFV[$i][1], "(") - 2) . ")";
		}

		else {
			$sql .= "'" . stripslashes(sqlFixUp($arrFV[$i][1])) . "'";
		}
		$sql .= ", ";

	}

	$sql = substr($sql, 0, strlen($sql) - 2);

	$sql .= " WHERE " . $idfield . " = " . $idvalue;

	if ($GLOBALS["__debuginfo"]) {
		echo "<br>updating record :<br>";
		pr($sql);
	}

	if ($trnId) logTranBegin($trnId);

	$stmt = ociparse($GLOBALS["objConn"], $sql);
	if (!@ociexecute($stmt, OCI_DEFAULT)) {
		if ($haltOnError) {
			$e = oci_error($stmt);
			pr($e);
			die("error updating record");
		}
		else {
			ocirollback($GLOBALS["objConn"]);
			return 0; // error
		}
	}

	if ($trnId) logTranEnd();

	return 1; // success
}



function logTranBegin($trnId) {
	$strSQL = "BEGIN KSI.TRANSACTION_BEGIN(" . (int)$trnId . "); END;";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!ociexecute($stmt, OCI_DEFAULT)) die("error starting log transaction");
}

function logTranEnd() {
	$strSQL = "BEGIN KSI.TRANSACTION_END; END;";

	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!(int)ociexecute($stmt, OCI_DEFAULT)) die("error ending log transaction");
	ocicommit($GLOBALS["objConn"]) or die("error committing transaction");
}




function _showCurTransaction() {
	$strSQL = "SELECT ksi.aaa FROM dual";
	$stmt = ociparse($GLOBALS["objConn"], $strSQL);
	if (!ociexecute($stmt, OCI_DEFAULT)) {
		die("error getting current transaction");
	}
	OCIFetchInto ($stmt, $row, OCI_NUM);
	echo "<br>current transaction: " . $row[0] . "<br>";
}


?>