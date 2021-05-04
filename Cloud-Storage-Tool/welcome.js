		var rand = 0;
		var rand_Y = 0;
		var sign = '+';
		var t1; var t2; var t3; var t4;
		var clouds = [
				[$('#cloud_1'), $('#cloud_1').css("left")], [$('#cloud_2'), $('#cloud_2').css("left")], [$('#cloud_3'), $('#cloud_3').css("left")],
				[$('#cloud_4'), $('#cloud_4').css("left")], [$('#cloud_5'), $('#cloud_5').css("left")], [$('#cloud_6'), $('#cloud_6').css("left")],
				[$('#cloud_7'), $('#cloud_7').css("left")], [$('#cloud_8'), $('#cloud_8').css("left")], [$('#cloud_9'), $('#cloud_9').css("left")],
				[$('#cloud_10'), $('#cloud_10').css("left")], [$('#cloud_11'), $('#cloud_11').css("left")], [$('#cloud_12'), $('#cloud_12').css("left")],
				[$('#cloud_13'), $('#cloud_13').css("left")], [$('#cloud_14'), $('#cloud_14').css("left")], [$('#cloud_15'), $('#cloud_15').css("left")]
	     		     ];
		
			function AnimateClouds(){
				if(!document.hidden){
					for(var i = 0; i < 3; ++i){
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
					t1 = setTimeout(function(){
						for(var i = 3; i < 6; ++i){
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
						t2 = setTimeout(function(){
							for(var i = 6; i < 9; ++i){
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
							t3 = setTimeout(function(){
								for(var i = 9; i < 12; ++i){
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
								t4 = setTimeout(function(){
									for(var i = 12; i < 15; ++i){
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
								},5000);
							},5000);									
						},5000);	
					},5000);
				}
			}

			var hidden = false;
			$(window).on("load", function(){
				AnimateClouds();
				var cloud_animation = setInterval(function(){AnimateClouds();}, 25500);
				setInterval(function(){
					if((document.hidden) && (hidden == false)){
						hidden = true;
						clearInterval(cloud_animation);
						clearTimeout(t1); clearTimeout(t2);
						clearTimeout(t3); clearTimeout(t4);
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
	
			function ChangePanels(curr_id, next_id){
				RotateGears();
				$('#' + curr_id).animate({left:"-=72%"}, (1060), "linear",function(){$(this).css("display", "none");});
				$('#' + next_id).css("display", "block");
				$('#' + next_id).animate({left:"-=72%"}, (1060), "linear",function(){});
			}

			$('#returning_btn').click(function(){
				ChangePanels("boxdiv1", "log_username");			
			});

			$('#new_btn').click(function(){
				ChangePanels("boxdiv1", "reg_username");
			});

			$('#reg_uniqueid_nextbtn').click(function(){
				ChangePanels("reg_uniqueid", "log_username");
			});

			var password = "";
			var uniqueid = "";

			$('#login_usr_nextbtn').click(function(){
				let username = $('#log_usr_in').val();
				if(username == ""){
					$('#err_p').text("Field can't be empty"); 
					$('#err_p').css("display", "block");
					return;
				}
				else if(username.length > 16){
					$('#err_p').text("Max length: 16 characters"); 
					$('#err_p').css("display", "block");
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
							$('#err_p').text("The username doesn't exist."); 
							$('#err_p').css("display", "block");
						}
						else{
							$('#err_p').css("display", "none");
							ChangePanels("log_username", "log_pass");
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
					$('#err_p').text("Field can't be empty"); 
					$('#err_p').css("display", "block");
					return;
				}
				else if(inval.length > 16){
					$('#err_p').text("Max length: 16 characters"); 
					$('#err_p').css("display", "block");
					return;
				}
				else if (inval != password){
					$('#err_p').text("Incorrect password entered"); 
					$('#err_p').css("display", "block");
					return;
				}
				else{
					$('#err_p').css("display", "none");
					ChangePanels("log_pass", "log_uniqueid");
					password = "";
					return;
				}
			});

			$('#login_uniqueid_nextbtn').click(function(){
				let inval = $('#log_uniqueid_in').val();
				if(inval == ""){
					$('#err_p').text("Field can't be empty"); 
					$('#err_p').css("display", "block");
					return;
				}
				else if(inval.length > 16){
					$('#err_p').text("Max length: 16 characters"); 
					$('#err_p').css("display", "block");
					return;
				}
				else if (inval != uniqueid){
					$('#err_p').text("Incorrect unique ID entered"); 
					$('#err_p').css("display", "block");
					return;
				}
				else{
					$('#err_p').css("display", "none");
					ChangePanels("log_uniqueid", "dummy");
					uniqueid = "";
					return;
				}
			});

			var new_username;
			var new_password;
			var new_uniqueid;
			$('#reg_username_nextbtn').click(function(){
				let inval = $('#reg_username_in').val();
				if(inval == ""){
					$('#err_p').text("Field can't be empty"); 
					$('#err_p').css("display", "block");
					return;					
				}
				else if(inval.length > 16){
					$('#err_p').text("Max length: 16 characters");
					$('#err_p').css("display","block");
					return;
				}
				else{
					$.ajax({ 
						type: 'POST',				
						dataType: 'text',
						data: { 
							action : "B",
							username : inval
						}
					})
					.done(function(response) {
					    	if(response == 'no'){
							$('#err_p').css("display", "none");
							ChangePanels("reg_username", "reg_password");
							new_username = inval;
							return;
						}
						else{
							$('#err_p').text("This username is taken"); 
							$('#err_p').css("display", "block");
							return;
						}
					})
				}
			});

			$('#reg_password_nextbtn').click(function(){
				let inval = $('#reg_password_in').val();
				if(inval == ""){
					$('#err_p').text("Field can't be empty"); 
					$('#err_p').css("display", "block");
					return;					
				}
				else if(inval.length < 6){
					$('#err_p').text("Min length: 6 characters");
					$('#err_p').css("display","block");
					return;					
				}
				else if(inval.length > 16){
					$('#err_p').text("Max length: 16 characters");
					$('#err_p').css("display","block");
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
						ChangePanels("reg_password", "reg_uniqueid");
					})
				}
			});
