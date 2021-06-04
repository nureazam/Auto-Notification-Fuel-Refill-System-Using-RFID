<?php
require "connect.php";


$phone_no = $_POST['phone_no'];
$pass = $_POST['password'];

$sql_query = "SELECT * FROM User WHERE phone_no = '$phone_no' AND pass = '$pass'";

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
