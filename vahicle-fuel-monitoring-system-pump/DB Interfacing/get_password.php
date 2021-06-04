<?php   
include ('connect.php');

$sql_query = "SELECT `pass` FROM `User` WHERE rfid_no = '".$_GET["rfid_no"]."'";

$result = mysqli_query($conn, $sql_query);

if($result)
{
    while ($row = mysqli_fetch_array($result))
        {
            echo "<br>";
            echo $row['pass'];
	    echo "<br>";
        }
}
else
{
    echo mysqli_error($conn);
}


mysqli_close($conn);
?>