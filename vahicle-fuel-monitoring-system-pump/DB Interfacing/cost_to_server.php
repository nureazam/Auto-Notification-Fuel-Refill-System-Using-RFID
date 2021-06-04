<?php
    // Import PHPMailer classes into the global namespace
    // These must be at the top of your script, not inside a function
    use PHPMailer\PHPMailer\PHPMailer;
    use PHPMailer\PHPMailer\Exception;

    include ('connect.php');
    //Load Composer's autoloader
    require 'phpmailer/vendor/autoload.php';
	
    $now = new DateTime(null, new DateTimeZone('Asia/Dhaka'));
    $date_time = $now->format('Y-m-d H:i:s');    // MySQL     datetime format
    $quantity = $_GET['quantity'];
    $rfid_no = $_GET['rfid_no'];
    $cost = 0.0;
    $per_unit_price = 0;
    $previous_balance = 0;
    $current_balance = 0;
    $phone_no = "";
    $fuel_type = "Petrol";
    $email = "";
    $name = "";
 
    $sql_query = "SELECT * FROM User where rfid_no = '$rfid_no'";
    $sql_fuel = "SELECT * FROM fuel_info where fuel_type = 'Petrol'";

    $result_query = mysqli_query($conn, $sql_query);
    $result_fuel = mysqli_query($conn, $sql_fuel);
    


/*....................................... get phone no of user............................*/

   if($result_query)
    {
        while ($row_query = mysqli_fetch_array($result_query))
            {
                $phone_no = $row_query['phone_no'];
		$email = $row_query['email'];
    		$name = $row_query['name'];	        
            }
    }
    else
    {
        echo mysqli_error($conn);
    }
/*....................................... get phone no of user...........................*/

/*....................................... get per unit fuel price............................*/

    if ($result_fuel)
    {
        while($row_fuel = mysqli_fetch_array($result_fuel))
            {
 	        $per_unit_price = $row_fuel['per_unit_price'];
	    }
    }
    else
    {
        echo mysqli_error($conn);
    }
/*....................................... get per unit fuel price............................*/
    
/*....................................... get balance of user...........................................*/    
    
    $sql_balance = "SELECT * FROM Balance where phone_no = '$phone_no'";
    $result_balance = mysqli_query($conn, $sql_balance);
    if($result_balance)
    {
        while ($row_balance = mysqli_fetch_array($result_balance))
            {
                $current_balance = $row_balance['balance'];	        
            }
    }
    else
    {
        echo mysqli_error($conn);
    }
/*....................................... get balance of user............................*/   

    $cost = $quantity*$per_unit_price;
    $previous_balance = $current_balance;
    $current_balance = $previous_balance-$cost;
    echo $cost;
    echo $previous_balance;
    echo $current_balance;

/*....................................... Store data to statement table............................*/ 

    $sql_statement = "INSERT INTO `statement`(`current_balance`, `previous_balance`, `cost`, `phone_no`, `fuel_type`, `fuel_qnt`, `date_time`) VALUES ('$current_balance', '$previous_balance', '$cost', '$phone_no', '$fuel_type', '$quantity', '$date_time')";
    $result_statement = mysqli_query($conn, $sql_statement);
    if($result_statement)
    {
        echo "successfull";
    }
    else
    {
        echo mysqli_error($conn);
    }

/*....................................... Store data to statement table............................*/   

/*....................................... Update data to balance table............................*/ 

    $sql_update_balance = "UPDATE `Balance` SET `balance`= '$current_balance' WHERE phone_no = '$phone_no'";
    $result_update_balance = mysqli_query($conn, $sql_update_balance);
    if($result_update_balance)
    {
        echo "successfull balance update";
    }
    else
    {
        echo mysqli_error($conn);
    }

/*....................................... Update data to balance table............................*/

/*.......................................Send mail to user......................................*/

    if($result_query)
    {
        echo $email;

        $mail = new PHPMailer(true);                              // Passing `true` enables exceptions
    try {
    //Server settings
    //$mail->SMTPDebug = 2;                                 // Enable verbose debug output
    $mail->isSMTP();                                      // Set mailer to use SMTP
    $mail->Host = 'smtp.gmail.com';  // Specify main and backup SMTP servers
    $mail->SMTPAuth = true;                               // Enable SMTP authentication
    $mail->Username = 'digitechbd151@gmail.com';                 // SMTP username
    $mail->Password = 'digitechbd18';                           // SMTP password
    $mail->SMTPSecure = 'tls';                            // Enable TLS encryption, `ssl` also accepted
    $mail->Port = 587;                                    // TCP port to connect to

    //Recipients
    $mail->setFrom('digitechbd151@gmail.com', 'DigitechBD');
    $mail->addAddress($email);     // Add a recipient
    //Content
    $style = "height: 30px; align='center'";
    $table_style = "border-color: #000000; margin-left: auto; margin-right: auto; text-align:center; border='2'";
    $table = "<table style=".$table_style.">
    <tbody>
    <tr style=".$style.">
    <td style=".$style.">Date-Time</td>
    <td style=".$style.">Fuel Type</td>
    <td style=".$style.">Fuel Quantity (Liter)</td>
    <td style=".$style.">Per Unit Price</td>
    <td style=".$style.">Cost</td>
    <td style=".$style.">Current Balance</td>
    <td style=".$style.">Previous Balance</td>
    </tr>
    <tr style=".$style.">
    <td style=".$style.">$date_time</td>
    <td style=".$style.">$fuel_type</td>
    <td style=".$style.">$quantity</td>
    <td style=".$style.">$per_unit_price</td>
    <td style=".$style.">$cost</td>
    <td style=".$style.">$current_balance</td>
    <td style=".$style.">$previous_balance</td>
    </tr>
    </tbody>
    </table>
    <p>&nbsp;</p> <br>";

    $massage = "Dear ". "$name". ", <br>". "<b>You have a latest transaction: <b> <br>". $table;





    $mail->isHTML(true);                                  // Set email format to HTML
    $mail->Subject = 'Latest transaction';
    $mail->Body    = $massage;
    //$mail->AltBody = 'Nunu khaben?';

    $mail->send();
    echo 'Message has been sent';
    } catch (Exception $e) {
    echo 'Message could not be sent. Mailer Error: ', $mail->ErrorInfo;
}
    }

/*.........................................Send mail to user.....................................*/   
    
?>