<?
class datePicker {
	var $objName;
	var $_day;
	var $_month;
	var $_year;
	var $addHtmlAttrs;
	var $disabled = false;
	var $USformat = false;

	var $arrMonths = array(
		1=> "€нвар€", 
		2=> "феврал€", 
		3=> "марта", 
		4=> "апрел€", 
		5=> "ма€", 
		6=> "июн€", 
		7=> "июл€", 
		8=> "августа", 
		9=> "сент€бр€", 
		10=>"окт€бр€", 
		11=>"но€бр€", 
		12=>"декабр€"
	);
	var $arrMonthsUS = array(
		1=> "January", 
		2=> "February", 
		3=> "March", 
		4=> "April", 
		5=> "May", 
		6=> "June", 
		7=> "July", 
		8=> "August", 
		9=> "September", 
		10=>"October", 
		11=>"Novermber", 
		12=>"December"
	);
	var $yearFrom = 1900;
	var $yearTo = 2100;

	function datePicker($name)	{
		
		$this->objName = $name;
		$curDate = date("Y-m-d");
		$month = $name . "month";
		$day = $name . "day";
		$year = $name . "year";

		if (isset($_GET[$day])) {
			$_POST[$day] = $_GET[$day];
		}
		if (isset($_GET[$month])) {
			$_POST[$month] = $_GET[$month];
		}
		if (isset($_GET[$year])) {
			$_POST[$year] = $_GET[$year];
		}


		if (isset($_POST[$month]) && isset($_POST[$day]) && isset($_POST[$year]))	{
			$this->setDate(($_POST[$year]) . "-" . ($_POST[$month]) . "-" . ($_POST[$day]));
		}
		else	{
//			$this->setDate($curDate);
		}
	}

	function setDate($newDate)	{
	

		list($this->_year, $this->_month, $this->_day) = explode("-", $newDate);

		if (@checkdate($this->_month, $this->_day, $this->_year)) {

			if ($this->_year < $this->yearFrom) $this->yearFrom = $this->_year;
			if ($this->_year > $this->yearTo) $this->yearTo = $this->_year;
		}
		else {
			$this->_year = null;
			$this->_month = null;
			$this->_day = null;
		}

		return @checkdate($this->_month, $this->_day, $this->_year);
	}

	function getDateYMD() {
		return $this->getDateForMySQL();
	}

	function getDateForMySQL()	{
		if (@checkdate($this->_month, $this->_day, $this->_year)) {
			return $this->_year . "-" . $this->_month . "-" . $this->_day;
		}
		else	{
			return false;
		}
	}
	
	function getTimestamp() {
		if (@checkdate($this->_month, $this->_day, $this->_year)) {
			return mktime(0, 0, 0, $this->_month, $this->_day, $this->_year);
		}
		else {
			return false;
		}
	}

	function writeDatePickerHTML()	{
		if (!$this->USformat) {
			echo "<select name=\"" . $this->objName . "day\" " . $this->addHtmlAttrs . " onchange=\"if (!this.selectedIndex) {this.form." . $this->objName . "month.selectedIndex = 0; this.form." . $this->objName . "year.selectedIndex = 0;}\"" . ($this->disabled ? " disabled" : "") . "><option value=\"\"> -- </option>";
			for ($i = 1; $i < 32; $i++)	{
				echo "<option value=\"" . $i . "\"" . ($this->_day == $i ? " selected" : "") . ">" . $i . "</option>";
			}
			echo "</select>&nbsp;";
			echo "<select name=\"" . $this->objName . "month\"  " . $this->addHtmlAttrs . " onchange=\"if (!this.selectedIndex) {this.form." . $this->objName . "day.selectedIndex = 0; this.form." . $this->objName . "year.selectedIndex = 0;}\"" . ($this->disabled ? " disabled" : "") . "><option value=\"\"> -- </option>";
			for ($i = 1; $i < 13; $i++)	{
				echo "<option value=\"" . $i . "\"" . ($this->_month == $i ? " selected" : "") . ">" . $this->arrMonths[$i] . "</option>";
			}
			echo "</select>&nbsp;";
		}
		else {
			echo "<select name=\"" . $this->objName . "month\"  " . $this->addHtmlAttrs . " onchange=\"if (!this.selectedIndex) {this.form." . $this->objName . "day.selectedIndex = 0; this.form." . $this->objName . "year.selectedIndex = 0;}\"" . ($this->disabled ? " disabled" : "") . "><option value=\"\"> -- </option>";
			for ($i = 1; $i < 13; $i++)	{
				echo "<option value=\"" . $i . "\"" . ($this->_month == $i ? " selected" : "") . ">" . $this->arrMonthsUS[$i] . "</option>";
			}
			echo "</select>&nbsp;";
echo "<select name=\"" . $this->objName . "day\" " . $this->addHtmlAttrs . " onchange=\"if (!this.selectedIndex) {this.form." . $this->objName . "month.selectedIndex = 0; this.form." . $this->objName . "year.selectedIndex = 0;}\"" . ($this->disabled ? " disabled" : "") . "><option value=\"\"> -- </option>";
			for ($i = 1; $i < 32; $i++)	{
				echo "<option value=\"" . $i . "\"" . ($this->_day == $i ? " selected" : "") . ">" . $i . "</option>";
			}
			echo "</select>&nbsp;";
		}


		echo "<select name=\"" . $this->objName . "year\" " . $this->addHtmlAttrs . " onchange=\"if (!this.selectedIndex) {this.form." . $this->objName . "day.selectedIndex = 0; this.form." . $this->objName . "month.selectedIndex = 0;}\"" . ($this->disabled ? " disabled" : "") . "><option value=\"\"> -- </option>";
		for ($i = $this->yearFrom; $i <= $this->yearTo; $i++)	{
			echo "<option value=\"" . $i . "\"" . ($this->_year == $i ? " selected" : "") . ">" . $i . "</option>";
		}
		echo "</select>&nbsp;";
	}


	function getDatePickerHTML()	{
		if (!$this->USformat) {
			$str =  "<select name=\"" . $this->objName . "day\" " . $this->addHtmlAttrs . " onchange=\"if (!this.selectedIndex) {this.form." . $this->objName . "month.selectedIndex = 0; this.form." . $this->objName . "year.selectedIndex = 0;}\"" . ($this->disabled ? " disabled" : "") . "><option value=\"\"> -- </option>";
			for ($i = 1; $i < 32; $i++)	{
				$str .=  "<option value=\"" . $i . "\"" . ($this->_day == $i ? " selected" : "") . ">" . $i . "</option>";
			}
			$str .= "</select>&nbsp;";
			$str .= "<select name=\"" . $this->objName . "month\"  " . $this->addHtmlAttrs . " onchange=\"if (!this.selectedIndex) {this.form." . $this->objName . "day.selectedIndex = 0; this.form." . $this->objName . "year.selectedIndex = 0;}\"" . ($this->disabled ? " disabled" : "") . "><option value=\"\"> -- </option>";
			for ($i = 1; $i < 13; $i++)	{
				$str .= "<option value=\"" . $i . "\"" . ($this->_month == $i ? " selected" : "") . ">" . $this->arrMonths[$i] . "</option>";
			}
			$str .= "</select>&nbsp;";

		}
		else {
			$str = "<select name=\"" . $this->objName . "month\"  " . $this->addHtmlAttrs . " onchange=\"if (!this.selectedIndex) {this.form." . $this->objName . "day.selectedIndex = 0; this.form." . $this->objName . "year.selectedIndex = 0;}\"" . ($this->disabled ? " disabled" : "") . "><option value=\"\"> -- </option>";
			for ($i = 1; $i < 13; $i++)	{
				$str .= "<option value=\"" . $i . "\"" . ($this->_month == $i ? " selected" : "") . ">" . $this->arrMonthsUS[$i] . "</option>";
			}
			$str .= "</select>&nbsp;";
$str .= "<select name=\"" . $this->objName . "day\" " . $this->addHtmlAttrs . " onchange=\"if (!this.selectedIndex) {this.form." . $this->objName . "month.selectedIndex = 0; this.form." . $this->objName . "year.selectedIndex = 0;}\"" . ($this->disabled ? " disabled" : "") . "><option value=\"\"> -- </option>";
			for ($i = 1; $i < 32; $i++)	{
				$str .= "<option value=\"" . $i . "\"" . ($this->_day == $i ? " selected" : "") . ">" . $i . "</option>";
			}
			$str .= "</select>&nbsp;";
		}
		$str .= "<select name=\"" . $this->objName . "year\" " . $this->addHtmlAttrs . " onchange=\"if (!this.selectedIndex) {this.form." . $this->objName . "day.selectedIndex = 0; this.form." . $this->objName . "month.selectedIndex = 0;}\"" . ($this->disabled ? " disabled" : "") . "><option value=\"\"> -- </option>";

		for ($i = $this->yearFrom; $i <= $this->yearTo; $i++)	{
			$str .= "<option value=\"" . $i . "\"" . ($this->_year == $i ? " selected" : "") . ">" . $i . "</option>";
		}
		$str .= "</select>&nbsp;";

		return $str;
	}

}















class timePicker {
	var $objName;
	var $disabled = false;
	var $_hour;
	var $_min;
	var $_sec;
	var $USformat = false;

	function timePicker($name)	{
		
		$this->objName = $name;
//		$curDate = date("Y-m-d");
		$hour = $name . "hour";
		$min = $name . "min";

		if (isset($_GET[$hour])) {
			$_POST[$hour] = $_GET[$hour];
		}
		if (isset($_GET[$min])) {
			$_POST[$min] = $_GET[$min];
		}

		$this->_hour = (int)$_POST[$hour];
		$this->_min = (int)$_POST[$min];

	}

	function setTime($newTime)	{
		list($this->_hour, $this->_min, $this->_sec) = explode(":", $newTime);
	}

	function writeTimePickerHTML()	{
		
		echo "<select name=\"" . $this->objName . "hour\" " . $this->addHtmlAttrs . ($this->disabled ? " disabled" : "") . ">";
		for ($i = 0; $i < 24; $i++)	{
		
			if ($this->USformat) {
				$_val = date("ga", mktime($i, 1, 0, 0, 0, 1));
			}
			else {
				$_val = $i;
			}


			echo "<option value=\"" . $i . "\"" . ($this->_hour == $i ? " selected" : "") . ">" . $_val . "</option>";
		}
		echo "</select>&nbsp;:&nbsp;";
		echo "<select name=\"" . $this->objName . "min\" " . $this->addHtmlAttrs . ($this->disabled ? " disabled" : "") . ">";
		for ($i = 0; $i < 60; $i++)	{
			echo "<option value=\"" . $i . "\"" . ($this->_min == $i ? " selected" : "") . ">" . substr("0" . $i, strlen($i) - 1) . "</option>";
		}
		echo "</select>&nbsp;";
	}


	function getTimePickerHTML()	{
		
		$str = "<select name=\"" . $this->objName . "hour\" " . $this->addHtmlAttrs . ($this->disabled ? " disabled" : "") . ">";
		for ($i = 0; $i < 24; $i++)	{
		
			if ($this->USformat) {
				$_val = date("ga", mktime($i, 1, 0, 0, 0, 1));
			}
			else {
				$_val = $i;
			}


			$str .= "<option value=\"" . $i . "\"" . ($this->_hour == $i ? " selected" : "") . ">" . $_val . "</option>";
		}
		$str .= "</select>&nbsp;:&nbsp;";
		$str .= "<select name=\"" . $this->objName . "min\" " . $this->addHtmlAttrs . ($this->disabled ? " disabled" : "") . ">";
		for ($i = 0; $i < 60; $i++)	{
			$str .= "<option value=\"" . $i . "\"" . ($this->_min == $i ? " selected" : "") . ">" . substr("0" . $i, strlen($i) - 1) . "</option>";
		}
		$str .= "</select>&nbsp;";
		return $str;
	}



	function getTimeHMS()	{
		return (int)$this->_hour . ":" . (int)$this->_min . ":" . (int)$this->_sec;
	}


	function getTimeStringUS() {
		return date("g:ia", mktime($this->_hour, (int)$this->_min, (int)$this->_sec, 1, 1, 1));
	}
}



?>