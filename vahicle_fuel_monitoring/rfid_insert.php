<?php 
include ('connect.php');

$phonenumber = $_POST['Number'];
$Rfid = $_POST ['Rfid'];

$sql_query = "UPDATE User SET rfid_no = '$Rfid' WHERE phone_no = '$phonenumber'";

$result = mysqli_query($conn,$sql_query);

if($result)
{
	echo "successfully added";
}


mysqli_close($conn);

 ?>

