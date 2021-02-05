<?php
    if($_POST["action"]=="update_db"){
        $email_variable = $_POST['email'];
		$phone_variable = $_POST['phone'];
		$pass_variable = $_POST['pass'];
		$myfile = fopen("SERVER_SQL_COMMANDS.txt", "a") or die("Unable to open server sql commands file.");
		$txt = "USERS_DB-INSERT-INTO-People-VALS-$email_variable-$phone_variable-$pass_variable";
		fwrite($myfile, $txt);
		fclose($myfile);
    }
?>
