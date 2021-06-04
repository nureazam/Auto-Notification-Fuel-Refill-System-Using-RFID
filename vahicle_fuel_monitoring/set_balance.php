<?php 
include ('connect.php');

$phonenumber = $_POST['Number'];
$recharge = $_POST ['balance'];
$previous_balance = 0;
$new_balance = 0;

$sql_query_fetch = "SELECT * FROM Balance WHERE phone_no = '$phonenumber'";
$result_fetch = mysqli_query($conn, $sql_query_fetch);

if($result_fetch)
{
    while ($row = mysqli_fetch_assoc($result_fetch))
        {
            $previous_balance = $row['balance'];
        }
}

if($result_fetch)
{
    $new_balance = $previous_balance+$recharge;
    $sql_query_update = "UPDATE Balance SET balance = '$new_balance' WHERE phone_no = '$phonenumber'";

    $result_update = mysqli_query($conn,$sql_query_update);

    if($result_update)
    {
	echo "Recharge Successfull!!";
    }

}



mysqli_close($conn);

 ?>

