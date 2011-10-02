<?php
$_ENV["ORACLE_HOME"] = "/opt/oracle/product/10.2.0/cafe/";
ocilogon("login", "pass", "(DESCRIPTION =
 (ADDRESS_LIST =
 (ADDRESS = 
 (PROTOCOL = TCP)(HOST = host)(PORT = 1521)))
 (CONNECT_DATA = (SERVICE_NAME = xe)))");
phpinfo();

