<?
class rTree {
	var $arrTree;	// array, containing tree data after filling
	var $arrTreeIdx;	// index array
	var $treeWritten = false;
	var $plusIcon = "../../images/pl.gif";
	var $minusIcon = "../../images/mn.gif";
	var $emptyIcon = "../../images/0.gif";



	function rTree($strSQL, $objConn, $dbImpl = "oracle") {

		switch ($dbImpl) {
			case "oracle":
				$stmt = ociparse($objConn, $strSQL);
				ociexecute($stmt, OCI_DEFAULT);

				while (ocifetchinto($stmt, $row)) {
					$this->arrTree[$row[0]] = $row;
					$this->arrIdx[(int)$row[1]][] = $row[0]; // contains child items
				}
				ocifreestatement($stmt);
				break;

			case "mysql":
				$res = mysql_query($strSQL, $objConn) or die("error getting tree data<br>" . mysql_error());
				while ($row = mysql_fetch_array($res)) {
					$this->arrTree[$row[0]] = $row;
					$this->arrIdx[(int)$row[1]][] = $row[0]; // contains child items
				}
				mysql_free_result($res);
				break;

			default:
			die("error: db implementation is bad");
		}

	}



	function writeTreeChb($parentID, $level) {
		if (!$this->treeWritten) {
?>
<script>
var zi = new Image(); zi.src = '<? echo $this->emptyIcon ?>';
var pi = new Image(); pi.src = '<? echo $this->plusIcon ?>';
var mi = new Image(); mi.src = '<? echo $this->minusIcon ?>';



//switchChild

function sC(objId, chbObj) {

	var obj = document.getElementById(objId);
	var val = chbObj.checked;

	for (var i = 0; i < obj.childNodes.length; i++) {
		if (obj.childNodes[i].tagName == 'INPUT') {
			obj.childNodes[i].checked = val;
		}

		if (obj.childNodes[i].tagName == 'DIV') {
			sC(obj.childNodes[i].id, chbObj);
		}





	}

}





// switchParent

function sP(objId, chbObj) {

	var obj = document.getElementById(objId);
	var val = chbObj.checked;

	if (obj.parentNode.tagName == 'DIV' && obj.parentNode.id != "layer0") {
		if (!val) {
			obj.previousSibling.previousSibling.previousSibling.checked = val;
		}
		sP(obj.parentNode.id, chbObj)
	}
}



// switchNode

function sN(node) {

	if(document.getElementById(node).style.display == 'block') {

		document.getElementById(node).style.display = 'none'; 

		document.images['i' + node].src= pi.src; //'p.gif';

	} 

	else {

		document.getElementById(node).style.display = 'block'; 

		document.images['i' + node].src= mi.src; //'m.gif';

	}

}



function pC(obj) {

	obj.previousSibling.click()

}

</script>
<style>
.d1{display:none; padding-left:15px;font-size: 10px; font-weight:normal; white-space: nowrap}
.d2{display:none; padding-left:30px; white-space: nowrap}
.d3{display:none; padding-left:45px; white-space: nowrap}
.d4{display:none; padding-left:60px; white-space: nowrap}
.d5{display:none; padding-left:75px; white-space: nowrap}
.d6{display:none; padding-left:90px; white-space: nowrap}
.ds {display:none; padding-left:10px;font-size: 10px; white-space: nowrap}
</style>
<?
		}

		$this->treeWritten = true;
		echo "<div id=l" . $parentID . ($parentID ? " class=d" . $level  : "") . ">";
		for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
	
			// if this child of $parentID has children
			if (count($this->arrIdx[$this->arrIdx[$parentID][$i]]) ) {

				echo "<a href=# onclick=\"sN('l" . $this->arrIdx[$parentID][$i] . "');return false\"><img name=il" . $this->arrIdx[$parentID][$i] . " src=\"" . $this->plusIcon . "\" border=0></a><input type=checkbox name=\"lc[]\" value=" . $this->arrIdx[$parentID][$i] . " onclick=\"sC('l" . $this->arrIdx[$parentID][$i] . "',this);sP('l" . $parentID . "',this)\"" . /*(in_array($arrTree[$arrIdx[$parentID][$i]][0], $arrLc) ? " checked" : "") .*/ "><a href=# onclick=\"pC(this);return false\">" . $this->arrTree[$this->arrIdx[$parentID][$i]][2] . "</a><br>";
			}
			// if doesn't...
			else {
				echo "<img src=\"" . $this->emptyIcon . "\" width=9><input type=checkbox name=\"lc[]\" value=" .$this->arrIdx[$parentID][$i] . " onclick=\"sP('l" . $parentID . "',this)\"" . /*(in_array($arrTree[$arrIdx[$parentID][$i]][0], $arrLc) ? " checked" : "") .*/ "><a href=# onclick=\"pC(this);return false\">" . $this->arrTree[$this->arrIdx[$parentID][$i]][2] . "</a><br>";
			}
	
			$this->writeTree($this->arrIdx[$parentID][$i], $level+1);
		}
		echo "</div>";
	}

	function writeTreeExpandedPlain($parentID, $level) {

		echo "<div style=\"margin-left: 10px\">";
		for ($i = 0; $i < count($this->arrIdx[$parentID]); $i++) {
	
			// if this child of $parentID has children
			if (count($this->arrIdx[$this->arrIdx[$parentID][$i]]) ) {
	
				echo "<img src=\"" . $this->minusIcon . "\" border=0></a> " . $this->arrTree[$this->arrIdx[$parentID][$i]][2] . "<br>";
			}
			// if doesn't...
			else {
				echo "<img src=\"" . $this->emptyIcon . "\" width=9> " . $this->arrTree[$this->arrIdx[$parentID][$i]][2] . "<br>";
			}
	
			$this->writeTreeExpandedPlain($this->arrIdx[$parentID][$i], $level+1);
		}
	echo "</div>";
	}

}

?>