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
		
		<link rel="stylesheet" type="text/css" href="pageCSS_code.css">
			
	</head>
	
	<body>
	
		<p id="welcome" class="glow">WELCOME</p>
		<svg id="cool_wall" width="1400" height="250">
			<polygon id="blade" points="5,10 105,250, 1405,250 1305,10"
			style="fill:rgb(30, 14, 120);stroke:rgb(3, 252, 232);stroke-width:5;fill-rule:evenodd;" />
		</svg>
					<p id="err_p"></p>
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
		<script type="text/javascript" src="jQuery_scripts.js"></script>
	</body>
</html>