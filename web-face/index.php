<?
$pathToSiteRoot = "";
header("Content-type: text/html; charset=windows-1251");
require($pathToSiteRoot . "common/const.php");
require($pathToSiteRoot . "common/lang.php");

?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">

<html>
<head>
	<title>������ ���-����������</title>
	<link rel="stylesheet" href="<? echo $pathToSiteRoot; ?>common/styles/common.css" type="text/css">
</head>

<body>
<h1>������ ���-����������</h1>
�������� ����������:
<ul>
<?
foreach ($arrApps as $key => $val) {
	echo "<li><a href=\"" . $pathToSiteRoot . $key . "/\">" . getLM($val["appTitle"]) . "</a></li>";
}

?>


</body>
</html>
