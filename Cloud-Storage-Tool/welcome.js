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
						rand_Y = Math.round(Math.random() * 15);
						rand = Math.round(Math.random() * 10001);
						$(clouds[i][0]).animate(
							{left:"+=3000"}, (12000 + rand), "linear",
							function(){
								$(this).css("left", "-=3000");
								$(this).css("top", (sign + '=' + (rand_Y.toString()) ));
							}
						);
					}
					t1 = setTimeout(function(){
						for(var i = 3; i < 6; ++i){
							if(Math.round(Math.random())){sign = '+';}
							else{sign = '-';}
							rand_Y = Math.round(Math.random() * 15);
							rand = Math.round(Math.random() * 10001);
							$(clouds[i][0]).animate(
								{left:"+=3000"}, (12000 + rand), "linear",
								function(){
									$(this).css("left", "-=3000");
									$(this).css("top", (sign + '=' + (rand_Y.toString()) ));
								}
							);
						}
						t2 = setTimeout(function(){
							for(var i = 6; i < 9; ++i){
								if(Math.round(Math.random())){sign = '+';}
								else{sign = '-';}
								rand_Y = Math.round(Math.random() * 15);
								rand = Math.round(Math.random() * 10001);
								$(clouds[i][0]).animate(
									{left:"+=3000"}, (12000 + rand), "linear",
									function(){
										$(this).css("left", "-=3000");
										$(this).css("top", (sign + '=' + (rand_Y.toString()) ));
									}
								);
							}
							t3 = setTimeout(function(){
								for(var i = 9; i < 12; ++i){
									if(Math.round(Math.random())){sign = '+';}
									else{sign = '-';}
									rand_Y = Math.round(Math.random() * 15);
									rand = Math.round(Math.random() * 10001);
									$(clouds[i][0]).animate(
										{left:"+=3000"}, (12000 + rand), "linear",
										function(){
											$(this).css("left", "-=3000");
											$(this).css("top", (sign + '=' + (rand_Y.toString()) ));
										}
									);
								}
								t4 = setTimeout(function(){
									for(var i = 12; i < 15; ++i){
										if(Math.round(Math.random())){sign = '+';}
										else{sign = '-';}
										rand_Y = Math.round(Math.random() * 15);
										rand = Math.round(Math.random() * 10001);
										$(clouds[i][0]).animate(
											{left:"+=3000"}, (12000 + rand), "linear",
											function(){
												$(this).css("left", "-=3000");
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
							step: function(now){
								$(this).css({ transform: 'rotate(' + now + 'deg)' });
							}	

						}
					);
					$('#gear_small').animate(
						{deg: ((-240) * times)},
						{
							duration:1100,
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
