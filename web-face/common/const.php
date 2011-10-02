<?
$_localDebug = false;


$_useHTTPS = false; //$_localDebug ? false : true;
$_langId = 1;


$arrApps = array(
	"recipebuilder" => array("appId" => 1, "appTitle" => "Recipe Builder"),
	"menumanager" => array("appId" => 3, "appTitle" => "Menu Manager"),
	"administrator" => array("appId" => 2, "appTitle" => "System Administrator"),
	"diag" => array("appId" => 14, "appTitle" => "Diag")
);



$arrPages["recipebuilder"] = array(
	array("items", "items/", 
		"Items are used to cook entrees. For more convinience, items are organized into groups. After a item is created, it can be used to make ingredients, which are being parts of  entrees. For example, in 
order to create a entree &ldquo;french fries&rdquo; you need to create items &ldquo;potato&rdquo;, &ldquo;salt&rdquo; and &ldquo;oil&rdquo;."), 
	array("ingredients", "ingredients/", 
		"Entrees are made of ingredients and items. Ingredients can be made of items and other ingredients. Each ingredient can contain a textual recipe, how it can be done. For example, in order to make a &ldquo;french fries&rdquo; 
entree you would need ingredient &ldquo;cleaned potato&rdquo; and items &ldquo;oil&rdquo; and &ldquo;salt&rdquo;. 1lb of &ldquo;cleaned potato&rdquo; ingredient is made of 1.1lb &ldquo;potato&rdquo; item."), 
	array("entrees", "entrees/", 
		"This tab contains entrees, its contents (entree can be made of ingredients and other entrees) and recipes. For example, one item of a entree &ldquo;french fries&rdquo; would be consist of 0.3lb of &ldquo;cleaned potato&rdquo;, 
0.5pint of &ldquo;oil&rdquo; and 0.1oz of &ldquo;salt&rdquo;. Also it is possible to assign modifiers to entrees. Likewise items and ingredients, entrees are organized in groups."), 
	array("modifiers", "modifiers/", 
		"Modifiers are changes in entrees' cooking or serving technologies, upon customer's wishes. Modifier can be a cooking feature (like a grill power) or a entree (like burrito stuff). It can have an additional price 
or can be free. Modifiers, which are intended to be applied for one (or several similar) dish, are needed to be consolidated into one group. For each group it is possible to specify minimal and maximal 
quantity of group items can be selected by customer at the same time."), 
	array("measures and tare", "measures/", 
		"You can add new measures of any type (quantity/volume/weight) upon necessity. It is needed to specify its rate to base measure (item/litre/kilo)."),
	array("search", "search/", 
		"Here you can search for needed item.")

);



$arrPages["menumanager"] = array(
	array("menu editor", "menueditor/", ""),
	array("cashier hotkeys setup", "hotkeys/", ""),
	array("discounts", "discounts/", "")
);




$arrPages["administrator"] = array(
	array("cafes", "cafes/", ""),
	array("suppliers", "suppliers/", "")
);


$arrPages["diag"] = array(
	array("sync stats", "dbsync/", ""),
	array("order generation stats", "stat/", ""),
	array("delayed orders stats", "delayedorders/", ""),
	array("application accessibility", "apps/", "")
);


$arrHotkeyColor = array(
	array("FFFFFF", "white"),
	array("EFEBE0", "ivory"),
	array("FFFF00", "yellow"),
	array("FF9900", "orange"),
	array("FF0000", "red"),
	array("805A00", "brown"),
	array("800000", "maroon"),
	array("00FF00", "lime"),
	array("008000", "green"),
	array("00FFFF", "aqua"),
	array("008080", "teal"),
	array("0000FF", "blue"),
	array("FF00FF", "fuchsia"),
	array("330099", "purple"),
	array("808080", "gray"),
	array("000000", "black")
);



$arrWkday = array(
	0 => array("short" => "Su"),
	1 => array("short" => "Mo"),
	2 => array("short" => "Tu"),
	3 => array("short" => "We"),
	4 => array("short" => "Th"),
	5 => array("short" => "Fr"),
	6 => array("short" => "Sa")
);
?>
