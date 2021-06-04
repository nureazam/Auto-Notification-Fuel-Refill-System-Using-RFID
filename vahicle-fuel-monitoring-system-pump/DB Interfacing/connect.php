<?php
$db_name = "Vahicle Fuel Monitoring";
$mysql_username = "root";
$mysql_password = "";
$server_name = "localhost";

$conn = mysqli_connect($server_name, $mysql_username, $mysql_password, $db_name);

if($conn)
{
    //echo "Connect to Database";
}
else
{
    echo "Can't connect to server. Please try again leter";
}

?>
