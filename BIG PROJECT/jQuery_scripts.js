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