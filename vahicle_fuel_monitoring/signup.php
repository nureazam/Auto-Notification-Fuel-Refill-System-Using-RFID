<?php
require "connect.php";

$phone_no = $_POST['phone_no'];
$name = $_POST['name'];
$address = $_POST['address'];
$email = $_POST['email'];
$age = $_POST['age'];
$gender = $_POST['gender'];
$rfid_no = $_POST['rfid_no'];
$pass = $_POST['pass'];

/*....................................Store data to registration table.................................*/
$sql_query = "INSERT INTO User (phone_no, name, address, email, age, gender, rfid_no, pass) VALUES ('$phone_no','$name','$address','$email','$age','$gender','$rfid_no', '$pass')";

$result = mysqli_query($conn, $sql_query);
if ($result) 
{
    echo "Registration Successful";
}
else {

    echo "Your user name or email or password is already registered to our server";
}
/*....................................Store data to registration table.................................*/
if($result)
{
/*.....................................Push info to balance table ......................................*/
    $sql_balance = "INSERT INTO `Balance`(`phone_no`, `balance`) VALUES ('$phone_no','0')";
    $result_balance = mysqli_query($conn, $sql_balance);
    $sql_statement = "INSERT INTO `statement`(`current_balance`, `previous_balance`, `cost`, `phone_no`, `fuel_type`, `fuel_qnt`, `date_time`) VALUES ('0', '0', '0', '$phone_no', 'none', '0', 'none')";
    $result_statement = mysqli_query($conn, $sql_statement);
          
/*.....................................Push info to balance table ......................................*/  
}

mysqli_close($conn);
?>
