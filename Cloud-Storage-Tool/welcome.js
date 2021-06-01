var password = "", uniqueid = "", username = "";
		var rand = 0, rand_Y = 0;
		var sign = '+';
		var t1; var t2; var t3; var t4;
		var timeouts = [t1, t2, t3, t4];
		var clouds = [
				[$('#cloud_1'), $('#cloud_1').css("left")], [$('#cloud_2'), $('#cloud_2').css("left")], [$('#cloud_3'), $('#cloud_3').css("left")],
				[$('#cloud_4'), $('#cloud_4').css("left")], [$('#cloud_5'), $('#cloud_5').css("left")], [$('#cloud_6'), $('#cloud_6').css("left")],
				[$('#cloud_7'), $('#cloud_7').css("left")], [$('#cloud_8'), $('#cloud_8').css("left")], [$('#cloud_9'), $('#cloud_9').css("left")],
				[$('#cloud_10'), $('#cloud_10').css("left")], [$('#cloud_11'), $('#cloud_11').css("left")], [$('#cloud_12'), $('#cloud_12').css("left")],
				[$('#cloud_13'), $('#cloud_13').css("left")], [$('#cloud_14'), $('#cloud_14').css("left")], [$('#cloud_15'), $('#cloud_15').css("left")]
	     		     ];
			function Anim3(start){
				for(var i = start; i < (start + 3); ++i){
					if(Math.round(Math.random())){sign = '+';}
					else{sign = '-';}
					rand_Y = Math.round(Math.random() * 5);
					rand = Math.round(Math.random() * 10001);
					$(clouds[i][0]).animate(
						{left:"+=156.25%"}, (12000 + rand), "linear",
						function(){
							$(this).css("left", "-=156.25%");
							$(this).css("top", (sign + '=' + (rand_Y.toString()) ));
						}
					);
				}
			}
			function TimeoutAnim3(index){
				timeouts[index - 1] = setTimeout(function () {Anim3(index * 3);}, (index * 5000));
			}
			function AnimateClouds(){
				if(!document.hidden){
					Anim3(0);
					for (var i = 1; i < 5; ++i) {TimeoutAnim3(i);}
				}
			}
			var hidden = false;
			$(window).on("load", function(){
				document.body.style.zoom = "100%";
				AnimateClouds();
				var cloud_animation = setInterval(function(){AnimateClouds();}, 25500);
				setInterval(function(){
					if((document.hidden) && (hidden == false)){
						hidden = true;
						clearInterval(cloud_animation);
						for(var x = 0; x < 4; ++x){
							clearTimeout(timeouts[x]);
						}
						for(var x = 0; x < 15; ++x){
							$(clouds[x][0]).stop(true);
							$(clouds[x][0]).css("left", clouds[x][1]);
						}
					}
					else if((!document.hidden) && (hidden == true)){
						hidden = false;
						AnimateClouds();
						cloud_animation = setInterval(function(){AnimateClouds();}, 25500);
					}
				},500);	
			});

			$(window).resize(function(){
				let wid_big = $('#gear_big').css("width");
				$('#gear_big').css('height', wid_big);
				$('#gear_small').css('height', (wid_big/2));
			});
			
			var times = 0;
			var locked = false;
			function RotateGears(){
				if(!locked){
					locked = true;
					++times;
					$('#gear_big').animate(
						{ deg: (240*times) },
						{
							duration: 1100,
							easing: "linear",
							step: function(now){
								$(this).css({ transform: 'rotate(' + now + 'deg)' });
							}	

						}
					);
					$('#gear_small').animate(
						{deg: ((-240) * times)},
						{
							duration:1100,
							easing: "linear",
							step: function(now){
								$(this).css({ transform: 'rotate(' + now + 'deg)'});
							},
							complete: function(){
								locked = false;
							}	
						}
					);
				}
			}
	
			function ChangePanels(curr_id, next_id, sign){
				$('#err_p').css("display", "none");
				RotateGears();
				$('#' + curr_id).animate({left: sign + "=72%"}, (1060), "linear",function(){$(this).css("display", "none");});
				$('#' + next_id).css("display", "block");
				$('#' + next_id).animate({left: sign + "=72%"}, (1060), "linear",function(){});
			}
			
			function ShowMessage(msg, is_err){
				$('#err_p').text(msg);
				if(is_err){$('#err_p').css({"background-color" : "rgb(255, 173, 173)", "color" : "red", "border-color" : "red", "display" : "block"});}
				else{$('#err_p').css({"color" : "rgb(0, 115, 4)", "background-color" : "rgb(130,255,176)", "display" : "block", "border-color" : "rgb(26, 255, 0)"});}
			}

			$('#returning_btn').click(function(){
				ChangePanels("boxdiv1", "log_username", "-");			
			});

			$('#new_btn').click(function(){
				ChangePanels("boxdiv1", "reg_username", "-");
			});

			$('#reg_uniqueid_nextbtn').click(function(){
				ChangePanels("reg_uniqueid", "log_username", "-");
			});

			var new_username;
			var new_password;
			var new_uniqueid;
			$('#reg_username_nextbtn').click(function(){
				let inval = $('#reg_username_in').val();
				if(inval == ""){
					ShowMessage("Field can't be empty", 1);
					return;					
				}
				else if(inval.length > 16){
					ShowMessage("Max length: 16", 1);
					return;
				}
				else{
					$.ajax({ 
						type: 'POST',				
						dataType: 'text',
						data: { 
							action : "A",
							username : inval
						}
					})
					.done(function(response) {
					    	if(response == 'no'){
							ChangePanels("reg_username", "reg_password", "-");
							new_username = inval;
							return;
						}
						else{
							ShowMessage("Username already taken", 1);
							return;
						}
					})
				}
			});

			$('#reg_password_nextbtn').click(function(){
				let inval = $('#reg_password_in').val();
				if(inval == ""){
					ShowMessage("Field can't be empty", 1);
					return;					
				}
				else if(inval.length < 6){
					ShowMessage("Min pass length: 6", 1);
					return;					
				}
				else if(inval.length > 16){
					ShowMessage("Max length: 16", 1);
					return;
				}
				else{
					$.ajax({ 
						type: 'POST',				
						dataType: 'text',
						data: { 
							action : "C",
							username : new_username,
							password : inval
						}
					})
					.done(function(response) {						
						new_password = inval;
					    	new_uniqueid = response;
						ChangePanels("reg_password", "reg_uniqueid", "-");
					})
				}
			});
			var password = "";
			var uniqueid = "";
			var username = "";
			$('#login_usr_nextbtn').click(function(){
				username = $('#log_usr_in').val();
				if(username == ""){
					ShowMessage("Field can't be empty", 1);
					return;
				}
				else if(username.length > 16){
					ShowMessage("Max length: 16", 1);
					return;
				}
				else{
					$.ajax({ 
						type: 'POST',				
						dataType: 'text',
						data: { 
							action : "A",
							username : username
						}
					})
					.done(function(response) {
					    	if(response == 'no'){
							ShowMessage("Username doesn't exist", 1);
						}
						else{
							ChangePanels("log_username", "log_pass", "-");
							let index = 0;
							while(response[index] != '-'){++index;}
							++index;
							while(response[index] != '-'){password += response[index]; ++index;}
							++index;
							while(index < response.length){uniqueid += response[index]; ++index;}
						}
					})
				}
			});

			$('#login_pass_nextbtn').click(function(){
				let inval = $('#log_pass_in').val();
				if(inval == ""){
					ShowMessage("Field can't be empty", 1);
					return;
				}
				else if(inval.length > 16){
					ShowMessage("Max length: 16", 1);
					return;
				}
				else if (inval != password){
					ShowMessage("Incorrect password", 1);
					return;
				}
				else{
					ChangePanels("log_pass", "log_uniqueid", "-");
					password = inval;
					return;
				}
			});

			$('#login_uniqueid_nextbtn').click(function(){
				let inval = $('#log_uniqueid_in').val();
				if(inval == ""){
					ShowMessage("Field can't be empty", 1);
					return;
				}
				else if(inval.length > 16){
					ShowMessage("Max length: 16", 1);
					return;
				}
				else if (inval != uniqueid){
					ShowMessage("Incorrect unique ID", 1);
					return;
				}
				else{
					let today = new Date();
					let dd = String(today.getDate()).padStart(2, '0');
					let mm = String(today.getMonth() + 1).padStart(2, '0');
					let yyyy = today.getFullYear();
					today = dd + '/' + mm + '/' + yyyy;
					$.ajax({ 
						type: 'POST', 
						dataType: 'text', 
						data: { 
							action : "I", 
							username : username, 
							date : today 
						}
					});
					ChangePanels("log_uniqueid", "mainpage", "-");
					uniqueid = inval;
					return;
				}
			});

			$('#usr_options_btn').click(function(){
				ChangePanels("mainpage", "usr_options", "-");
			});

			$('#changename').click(function(){
				$('#usr_setting_in').attr({'type' : 'text', "placeholder" : "username"});
				$('#usr_setting_in').val("");
			});

			$('#changepass').click(function(){
				$('#usr_setting_in').attr({'type' : 'password', "placeholder" : "password"});
				$('#usr_setting_in').val("");
			});

			$('#deleteacc').click(function(){
				
			});

			

			$('#usr_options_backbtn').click(function(){
				ChangePanels("usr_options", "mainpage", "+");
			});

			$('#usr_setting_btn').click(function(){
				let newval = $('#usr_setting_in').val();
				if(newval == ""){
					ShowMessage("Field can't be empty", 1);
					return;
				}
				if(newval.length > 16){
					ShowMessage("Max length: 16", 1);
					return;					
				}
				if($('#usr_setting_in').attr('type') == 'text'){
					$.ajax({ 
						type: 'POST', 
						dataType: 'text', 
						data: { 
							action : "A", 
							username : newval 
						}
					})
					.done(function(response) {
					    	if(response == 'no'){
							$.ajax({ 
								type: 'POST', 
								dataType: 'text', 
								data: { 
									action : "D", 
									oldusername : username, 
									newusername : newval 
								}
							});
							$.ajax({
								type: 'POST',
								dataType: 'text',
								data: {
									action : "H",
									oldusername : username,
									newusername : newval
								}
							});
							ShowMessage("Successfully changed name", 0);
							username = newval;
							return;
						}
						else{
							ShowMessage("Username already taken", 1);
							return;
						}
					})
				}
				if($('#usr_setting_in').attr('type') == "password"){
					if(newval.length < 6){
						ShowMessage("Min pass length: 6", 1);
						return;						
					}
					$.ajax({ type: 'POST', dataType: 'text', data: { action : "E", id : uniqueid, newpassword : newval }});
					ShowMessage("Successfully changed password", 0);
					password = newval;
					return;					
				}
				
			});

			$('#upload_btn').click(function(){
				ChangePanels("mainpage", "add_file", "-");
			});

			$('#submitfile_btn').click(function(){
				let fd = new FormData();
				let files = $('#choosefile_btn')[0].files;
				if(files.length > 0){
					fd.append('file', files[files.length - 1]);
					let fname = files[files.length - 1].name;
					let fsize = files[files.length - 1].size;
					if(fsize < 16){ShowMessage("Minimum file size: 16 bytes", 1); return;}
					if(fname.length > 16){ShowMessage("Max filename length: 16", 1); return;}
					let uploader = username;
					$.ajax({
						type : 'post',
						dataType : 'text',
						data:{
							action : "G",
							name : fname,
							uploader : uploader,
							size : fsize
						}
					})
					.done(function(response){
						if(response == "no"){
							$.ajax({
								type: 'post',
								data: fd,
								contentType: false,
								processData: false
							})
							.done(function(response){
								if(response != 0){
									let today = new Date();
									let mm = String(today.getMonth() + 1).padStart(2, '0');
									let dd = String(today.getDate()).padStart(2, '0');
									let yyyy = today.getFullYear();
									today = dd + '/' + mm + '/' + yyyy;
									$.ajax({
										type: 'post',
										dataType: 'text',
										data:{
											action : "F",
											name : files[files.length - 1].name,
											size : files[files.length - 1].size,
											uploader : username,
											date : today
										}
									})
									.done(function(response){
										if(response == "yes"){
											ShowMessage("Upload success!", 0);
										}	
									})
								}
								else{
									ShowMessage("Error uploading file", 1);
								}
							});
						}
						else if(response == "yes1"){
							ShowMessage("Please rename the file", 1)
						}
						else if(response == "yes2"){
							ShowMessage("Not enough uploads left. Max: 6 files", 1);
						}
						else if(response == "yes3"){
							ShowMessage("Not enough storage space. Max: 4GB", 1);
						}
					});
					
				}
				else{
					ShowMessage("Please select a file", 1);
				}	
			});
