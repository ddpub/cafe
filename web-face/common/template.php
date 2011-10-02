<?
foreach ($arrPages[$appName] as $val) {
	if (stristr($_SERVER["SCRIPT_NAME"], $appName . "/" . $val[1])) {
		$str = getLM($val[0]);
		$hint = getLM($val[2]);
	}
}
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">

<html>
<head>
	<title><? echo getLM($arrApps[$appName]["appTitle"]) . " / " . ucwords($str) . " &bull; " . $_person_name; ?></title>
	<link rel="stylesheet" href="<? echo $pathToSiteRoot; ?>common/styles/common.css" type="text/css">
</head>

<body>
<table cellspacing=0 cellpadding=0 border=0 width="100%" height="100%">
<tr><td valign=top>
<table cellspacing=5 cellpadding=0 border=0 width="100%">
	<td><h1 style=\"display: inline\"><? echo ucwords($str);?></h1></td>
	<td align=right class="tiny"><b><? 
//	echo getLM($arrApps[$appName]["appTitle"]);
	echo "<select name=\"app\" class=\"tiny\" onchange=\"document.location.href='" . $pathToSiteRoot . "' + this.options[this.options.selectedIndex].value + '/'\" >";
	foreach ($arrApps as $key => $val) {
		echo "<option value=\"" . $key . "\"" . ($key == $appName ? " selected" : "") . ">" . $val["appTitle"] . "</option>";
	}
	echo "</select>";
	
	 ?></b><br><br> <? echo getLM("logged as") . ": " . $_person_name . " (" . $_person_roles_str . ")";?> | <a href="<? echo $_SERVER["SCRIPT_NAME"]?>?logout"><? echo getLM("logout"); ?></a></td>
</table>
<?

writeNav();

if ($hint && $_langId == 0) {
	echo "<p class=\"hint\">" . $hint . "</p>";
}

echo "<p>";

writeMainContent();

if ($alrtMsg) {
	echo "<script>alert('" . addslashes($alrtMsg) . "')</script>";
}
?>
</td></tr>
<tr><td valign=bottom align=right class=tiny style="color: #808080; padding-top: 10px">
&copy; 2005&mdash;<? echo date("Y");?> KSI Corporation. All rights reserved.
</td></tr></table>
</body>
</html>
<?



function writeNav() {
	global $arrPages, $arrApps, $appName, $pathToSiteRoot;

	echo "<table cellspacing=0 cellpadding=5 border=0 width=\"100%\">";
	foreach ($arrPages[$appName] as $val) {
		if (stristr($_SERVER["SCRIPT_NAME"], $appName . "/" . $val[1])) {
			$str = "<td class=\"mi_a\" nowrap>" . getLM($val[0]) . "</td>";
		}
		else {
			$str = "<td class=\"mi\" nowrap><a href=\"" . $pathToSiteRoot . $appName . "/" . $val[1] . "\">" . getLM($val[0]) . "</a></td>";
		}
		echo $str;
	}
	echo "<td class=\"mi\" width=\"99%\">&nbsp;</td></table>";
}



function writeActionsBar($arr) {
	echo "<table cellpadding=0 border=0>";
	foreach ($arr as $val) {
		if (is_array($val)) {
			echo "<td class=\"tiny\" style=\"padding-right:10px\" align=center><a href=\"" . $val[1] . "\"" . ($val[3] ? " " . $val[3] : "") . "><img src=\"" . $GLOBALS["pathToSiteRoot"] . $val[2] . "\" width=16 height=16 alt=\"" . $val[0] . "\" border=0><br>" . $val[0] . "</a></td>";
		}
	}
	echo "</table>";
}


?>
