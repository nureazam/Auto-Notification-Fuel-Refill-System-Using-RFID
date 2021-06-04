<?php
require "connect.php";


$phone_no = $_POST['phone_no'];

$sql_query = "SELECT date_time, fuel_type, fuel_qnt, cost FROM statement WHERE phone_no = '$phone_no' ORDER BY date_time DESC";

$result = mysqli_query($conn, $sql_query);
if($result)
{
    while ($row = mysqli_fetch_assoc($result))
        {
            $array[] = $row;
        }
}
else
{
    echo mysqli_error($conn);
}

echo json_encode($array);

mysqli_close($conn);



?>
