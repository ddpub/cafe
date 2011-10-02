<?
$pathToSiteRoot = "../";
$appName = "recipebuilder";

require($pathToSiteRoot . "common/const.php");
//require($pathToSiteRoot . "common/functions.php");

header("Location: " . $arrPages[$appName][0][1]);
?>