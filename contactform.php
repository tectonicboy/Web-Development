<?php
	function showline(){
		echo __LINE__;
		echo "<br>";
		return;
	}
	function printNews($str, $bool1){
		if($bool1){
			echo "<p style='background-color:rgb(212, 255, 212);
			color:rgb(0, 122, 0);border-style:ridge;border-width:6px;
			border-color:rgb(7, 235, 212);border-radius:8px;'>$str</p>";
			return;
		}
		else{
			echo "<p style='background-color:rgb(255, 214, 243);color:red;border-style:ridge;
			border-width:6px;border-color:purple;border-radius:8px;'>$str</p>";
			return;
		}
	}
	function validateEmail($email){
		$at_pos = -1; $dot_pos = -1;
		for($i = 0; $i < strlen($email); $i++){
			if($email[$i] == '@'){
				$at_pos = $i;
				for($j = $i+1; $j < strlen($email); $j++){
					if($email[$j] == '.'){
						$dot_pos = $j;
						break;
					}
				}
				if($dot_pos < 4){
					break;
				}
			}
		}
		if($at_pos == -1 || $at_pos == 0){
			return false;
		}
		else{
			if($dot_pos < 4 || ((strlen($email) - $dot_pos) < 3)){
				return false;
			}
			else{
				return true;
			}
		}
	}
	
	if(sizeof($_POST)>2){
		$email = $_POST["target_email"];
		$subject = $_POST["subject"];
		$message = $_POST["text"];
		$headers = "From: kevinlondonstudy@abv.bg";
		if(validateEmail($email)){
			printNews("Your email is valid.", true);
			mail($email, $subject, $message, $headers);
			printNews("Your responce was sent. Congratulations!", true);
		}
		else{
			printNews("You have entered an invalid email. Try again.", false);
		}
	}


?>
<html>
	<head>
		<title>PHP Challenge</title>
		<link href="https://fonts.googleapis.com/css2?family=Text+Me+One&display=swap" rel="stylesheet">
		<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js">
		</script>
		<style type="text/css">
			#container{
				margin: 0px 150px;
			}
			body{
				font-family: 'Text Me One', sans-serif;
			}
			#header{
				color: rgb(71, 191, 103);
				font-weight: normal;
			}
			#email_in{
				width: 1000px;
				height: 32px;
				border: ridge 6px pink;
				border-radius: 8px;
				margin-bottom:5px;
				font-family: 'Text Me One', sans-serif;
			}
			#disclaimer{
				color:gray;
				font-size:80%;
				margin-bottom:20px;
			}
			#email_in:focus{
				outline: none;
				border-color: hotpink;
			}
			#subject_in{
				width: 1000px;
				height: 32px;
				border: ridge 6px pink;
				border-radius: 8px;
				margin-bottom:40px;
				font-family: 'Text Me One', sans-serif;
			}
			#subject_in:focus{
				outline: none;
				border-color: hotpink;
			}
			#message{
				font-family: 'Text Me One', sans-serif;
				width: 1000px;
				height: 100px;
				border: ridge 6px pink;
				border-radius: 8px;
				margin-bottom:20px;
				position:relative;
				left: -60px;
			}
			#message:focus{
				outline: none;
				border-color: hotpink;
			}
			p{
				margin: 5px;
			}
			#smile{
				position:relative;
				top: -171.5px;
				left: 250.5px;
			}
			.eye{
				position: relative;
				top: -190px;
				left: 211.5px;
			}
			#face{
				position:relative;
				top:-120px;
				left: 240px;
			}
			#btn{
				position:relative;
				left:-1065px;
				top:10px;
				width:70px;
				height:35px;
				background-color: rgb(255, 125, 125);
				color:white;
				font-weight:bold;
				font-family: 'Text Me One', sans-serif;
				font-size: 108%;
				border: solid 0.5px rgb(255, 125, 125);
				border-radius: 15px;
			}
			#btn:focus{
				outline: none;
			}
			#btn:hover{
				cursor: not-allowed;
			}
		</style>
	</head>
	
	<body>
		<div id="container">
			<h1 id="header">Get in touch!</h1>
			<p>Email address</p1>
			<form method="post">
				<input name="target_email" id="email_in" class="writable" placeholder="Enter email" type="text">
				<p id="disclaimer"><b>We'll never share your email with anyone else.</b></p>
				<p>Topic</p>
				<input name="subject" id="subject_in" class="writable" type="text">
				<p>What would you like to ask us?</p>
			
				<svg id="face" height="50" width="50">
					<circle cx="25" cy="25" r="20" stroke="yellow" stroke-width="3" fill="yellow">
				</svg>
				<textarea name="text" id="message" class="writable"></textarea>
				<input id="btn" type="submit" value="Submit">
			</form>

			<svg id="smile" height="14" width="32">
				<circle style="z-index:10;" cx="15" cy="0" r="14" stroke="red" stroke-width="0.8" fill="yellow">
			</svg>
			<svg id="left_eye"  class="eye" height="5" width="10">
				<circle style="z-index:10;" cx="5" cy="2" r="2" stroke="black" stroke-width="0.8" fill="black">
			</svg>
			<svg id="right_eye"  class="eye" height="5" width="10">
				<circle style="z-index:10;" cx="5" cy="2" r="2" stroke="black" stroke-width="0.8" fill="black">
			</svg>
		</div>
		
		
		<script type="text/javascript">
		
			$("#btn").prop("disabled", true);
			
			$(".writable").keyup(function(){
				if(    ($("#email_in").val() != "") 
					&& ($("#subject_in").val() != "")
					&& ($("#message").val() != "")
				){
					$("#btn").prop("disabled", false);
					
					$("#btn").hover(function(){
						$(this).css(
							"cursor", "pointer"
						);
					});
					
					$("#btn").css("background-color", "rgb(0, 237, 250)");
					$("#btn").css("border-color", "rgb(0, 237, 250)");	
				}
				else{
					$("#btn").prop("disabled", true);
					$("#btn").hover(function(){
						$(this).css(
							"cursor", "not-allowed"
						);
					});
					$("#btn").css("background-color", "rgb(255, 125, 125)");
					$("#btn").css("border-color", "rgb(255, 125, 125)");	
				}
			});
		</script>
	</body>

</html>