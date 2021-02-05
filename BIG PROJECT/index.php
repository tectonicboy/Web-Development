<html>
	<head>
		<title>Registration</title>
		<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
		
		<link rel="preconnect" href="https://fonts.gstatic.com">
		<link href="https://fonts.googleapis.com/css2?family=Orbitron:wght@900&display=swap" rel="stylesheet">
		
		<link rel="preconnect" href="https://fonts.gstatic.com">
		<link href="https://fonts.googleapis.com/css2?family=Roboto&display=swap" rel="stylesheet">
		
		<link rel="preconnect" href="https://fonts.gstatic.com">
	<link href="https://fonts.googleapis.com/css2?family=Share+Tech&display=swap" rel="stylesheet">
		
		<style type="text/css">
		
		
			body{
				background-image: url('pic5.jpg');
				overflow-x: hidden;
			}
		
			div#container{
				width: 700px;
				margin: 200px auto;
				border: ridge 20px cyan;
				border-radius: 60px;
				padding: 15px 15px 15px 15px;
				font-family: 'Amatic SC', cursive;
				color: rgb(230, 32, 98);
			}
			input{
				background-color: rgb(255, 214, 246);
				border: ridge 3.5px rgb(230, 32, 98);
				color: rgb(21, 191, 49);
				font-weight:bold;
				font-family: 'Roboto', sans-serif;
			}
			
			input:-webkit-autofill, 
			input:-webkit-autofill:hover, 
			input:-webkit-autofill:focus{
				border: ridge 3.5px rgb(230, 32, 98);
				-webkit-box-shadow: 0 0 0px 1000px #ffd6f6 inset;
				-webkit-text-fill-color: rgb(21, 191, 49);
				font-weight:bold;
				font-family: 'Roboto', sans-serif;
			}
			
			label{
				float:left;
				padding-right:20px;
				font-weight:bold;
				font-family: 'Share Tech', sans-serif;
			}	
			input#pass{
				position:relative;
				left: 62px;
				width: 200px;
			}
			input#phone{
				position:relative;
				left: 82px;
				width: 200px;
			}
			input#email{
				position:relative;
				left: 87px;
				width: 200px;
			}
			input#con_pass{
				position:relative;
				left: 10px;
				width: 200px;
			}
			input::placeholder{
				color:rgb(21, 191, 49);
			}
			input:focus{
				outline-color: cyan;
				outline-style: ridge;
				outline-width: 0.5px;
			}
			div.form_elem{
				margin-bottom: 5px;
			}
			
			button#btn{
				background-color: rgb(0, 5, 135);
				font-weight:bold;
				font-family: 'Orbitron', sans-serif;
				color:white;
				border-color: rgb(31, 39, 255);
			}
			
			button#btn:focus{
				outline-color: cyan;
				outline-style: ridge;
				outline-width: 0.5px;
			}
			
			p#success_p{
				color: rgb(17, 255, 0);
				display: none;
				font-family: 'Orbitron', sans-serif;
				position:absolute;
				left:470px;
				top:342px;
			}
			
			p#err_p{
				position:absolute;
				left:766px;
				top:218;
				font-family: 'Share Tech', sans-serif;
				color: red;
				font-size:17.5;
			}
			
			svg#cool_wall{
				position: absolute;
				left: 300px;
				top: 190px;
				z-index: 10;
			}
			
			.glow {
			  /*font-size: 80px;*/
			  color: #fff;
			  text-align: center;
			  -webkit-animation: glow 1s ease-in-out infinite alternate;
			  -moz-animation: glow 1s ease-in-out infinite alternate;
			  animation: glow 1s ease-in-out infinite alternate;
			}

			@-webkit-keyframes glow {
			  from {
				text-shadow: 0 0 2px #fff, 0 0 20px #fff, 0 0 30px #e60073, 0 0 40px #e60073, 0 0 50px #e60073, 0 0 60px #e60073, 0 0 70px #e60073;
			  }
			  to {
				text-shadow: 0 0 20px #fff, 0 0 30px #ff4da6, 0 0 40px #ff4da6, 0 0 50px #ff4da6, 0 0 60px #ff4da6, 0 0 70px #ff4da6, 0 0 80px #ff4da6;
			  }
			}
			
			#welcome{
				font-size:100px;
				font-family: 'Orbitron', sans-serif;
				font-style:italic;
				position:absolute;
				top:-12%;
				left:30%;
			}

		</style>	
	</head>
	
	<body>
	
		<p id="welcome" class="glow">WELCOME</p>
		<svg id="cool_wall" width="1400" height="250">
			<polygon id="blade" points="5,10 105,250, 1405,250 1305,10"
			style="fill:rgb(30, 14, 120);stroke:rgb(3, 252, 232);stroke-width:5;fill-rule:evenodd;" />
		</svg>
					<p id="err_p"></p>
		
			<!-- <form id="form1" method="POST"> -->
				<div id="container">
					<p id="success_p">Success!</p>
					
					<div class="form_elem">
						<label for="email" id="email_label">Email</label>
						<input name="EMAIL" id="email" name="email" type="text" placeholder="eg. name123@gmail.com">
					</div>
					
					<div class="form_elem">
						<label for="phone" id="phone_label">Phone</label>
						<input name="PHONE" id="phone" name="phone" type="text" placeholder="eg. 0123456789">	
					</div>
					
					<div class="form_elem">
						<label for="pass" id="pass_label">Password</label>
						<input name="PASSWORD" id="pass" name="pass" type="password">	
					</div>
					<div class="form_elem">
						<label for="con_pass" id="con_pass_label">Confirm Password</label>
						<input name="CON_PASSWORD" id="con_pass" name="con_pass" type="password">	
					</div>
					<button id="btn">GO</button>
				</div>
			<!-- </form> -->
		<script type="text/javascript">
			var bool1 = true;
			var err_msg = "";
			var email_regex = /^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4}$/i;

			function isNumeric(str){
				
				for(var i = 0; i < str.length; i++){
					if((str[i] != '0') && (str[i] != '1') && (str[i] != '2') 
						&& (str[i] != '3') && (str[i] != '4') && (str[i] != '5')
						&& (str[i] != '6') && (str[i] != '7') && (str[i] != '8')
						&& (str[i] != '9'))
					{
						return false;
					}
				}
				return true;
			}
			
			$("button#btn").click(function (){
				err_msg = "";
				bool1 = true;
				if(($("input#email").val()) == ""){
					err_msg += "Email address field must not be empty.<br>";
				}
				else{
					if(!(email_regex.test($("input#email").val()))){
						err_msg += "The email you entered is invalid.<br>";
					}
				}
				if($("input#phone").val() == ""){
					err_msg += "Phone number field must not be empty.<br>";
				}
				else{
					if(!isNumeric($("input#phone").val())){
						err_msg += "The phone number you entered is invalid.<br>";
					}
				}
				if($("input#pass").val() == ""){
					bool1 = false;
					err_msg += "The password field must not be empty.<br>";
				}
				if($("input#con_pass").val() == ""){
					bool1 = false;
					err_msg += "The password confirmation field must not be empty.<br>";
				}
				if(bool1){
					if(($("input#pass").val()) != ($("input#con_pass").val())){
						err_msg += "The two passwords do not match.<br>";
					}
				}
				if(!(err_msg == "")){
					$("p#err_p").html(err_msg);
					$("p#err_p").css("display", "block");
					$("p#success_p").css("display", "none");
				}
				else{
					$email_var = $("input#email").val();
					$phone_var = $("input#phone").val();
					$pass_var = $("input#pass").val();
					$.ajax({
						type: 'POST',
						data: {
							action: 'update_db',
							email: $email_var,
							phone: $phone_var,
							pass: $pass_var
						},
						url: 'POST_PROCESSOR.php',
						success: function() {
							$("p#success_p").css("display", "block");
							$("p#err_p").css("display","none");
						},
						error: function(){
							alert("There was an error sending your data to the server. Apologies for that, please try again later.");
						}
					})
				}
			});
			
			
			var locked = false;
			$(document).on( "mousemove", function(event) {
				if(locked == false){
					if((event.pageX > 300) && (event.pageY > 190) && (event.pageY < 440)){
						locked = true;
						$("svg#cool_wall").animate(
							{left: "+=1000"},
							200,
							function(){
								$("input").css("z-index", "21");
								$("button#btn").css("z-index","22");
							}
						);
					}
				}
				else {
					if((event.pageX < 300) || (event.pageY < 190) || (event.pageY > 440)){
						locked = false;
						$("svg#cool_wall").animate(
							{left: "-=1000"},
							200,
							function(){
								$("input").css("z-index", "auto");
								$("button#btn").css("z-index","auto");
							}
						);
					}
				}
			});
		</script>
	</body>
</html>
