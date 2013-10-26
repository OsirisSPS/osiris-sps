/**
 * jquery.passStrength.js
 * Copyright (c) 2010 Bug Software Limited (bugsoftware.co.uk)
 * Copyright (c) 2009 myPocket technologies (www.mypocket-technologies.com)
 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * View the GNU General Public License <http://www.gnu.org/licenses/>.

 * @author Ritchie Comley
 * @date 17/01/2010
 * @projectDescription jquery.passStrength.js is is a jQuery plug-in provide you smart algorithm to detect a password strength. This is a tinkered with iteration of Password Strength Meter - http://mypocket-technologies.com/. Based on Firas Kassem orginal plugin - http://phiras.wordpress.com/2007/04/08/password-strength-meter-a-jquery-plugin/
 * @version 0.9.1
 * 
 * @requires jquery.js (tested with 1.3.2)
 * @param shortPass:	"shortPass",	//optional
 * @param badPass:		"badPass",		//optional
 * @param goodPass:		"goodPass",		//optional
 * @param strongPass:	"strongPass",	//optional
 * @param baseStyle:	"testresult",	//optional
 * @param userid:		"",				//required override
 * @param messageloc:	1				//before == 0 or after == 1
 * 
*/

(function(jQuery){ 

	jQuery.fn.passStrengthener = function(options)
		{
			// PRIVATE VARIABLES		
			var defaults = {
				badLimit:		34,									//optional
				goodLimit:		68,									//optional
				shortLimit:		4,									//optional
				shortPass: 		"Too short",						//optional
				badPass:		"Weak",								//optional
				goodPass:		"Good",								//optional
				strongPass:		"Strong",							//optional
				samePass:		"Username and Password identical", 	//optional
				css:{												//optional
						bad:		"badPass",						
						good:		"goodPass",						
						strong: 	"strongPass"					
					},
				baseStyle:		"jquery_passStrengthener",			//optional
				userid:			"",									//required override
				messageloc:		1,									//before == 0 or after == 1
				strengthCallback:	function(score, strength) {}	//do something extra with the response feedback
			};
			
		 	var opts = jQuery.extend(defaults, options);
			
			var strengths = {
					tooshort: {message:opts.shortPass, cssclass:opts.css.bad},
					bad: {message:opts.badPass, cssclass:opts.css.bad},
					good: {message:opts.goodPass, cssclass:opts.css.good},
					strong: {message:opts.strongPass, cssclass:opts.css.strong},
					same: {message:opts.samePass, cssclass:opts.css.bad}
				};
			
			var allCSSArr = [];
			
			jQuery.each(opts.css, function(i, val)
				{
					allCSSArr.push(val);
				});
		      
		 	return this.each(function() { 
		 		var obj = jQuery(this);
		 		
		 		jQuery(obj).unbind().keyup(function()
		 		{
					var jPass = jQuery(this);
					var password = jPass.val()
					var userID = jQuery(opts.userid).val();

					var score = teststrength(jPass.val(), userID, opts);
					
					if(typeof(score) == 'string')
					{
						strength = score;
						score = 0;
					}
					else if (score < opts.badLimit)
					{
						strength = 'bad';
					} 
					else if (score < opts.goodLimit)
					{
						strength = 'good';
					}
					else
					{
						strength = 'strong';
					}
					
					var cssClass = strengths[strength].cssclass;
					var message = strengths[strength].message;
					
					var jFeedback = jPass.parent().find("span." + opts.baseStyle);
					
					if(!jFeedback.size())
					{
						jFeedback = jQuery("<span class=\""+opts.baseStyle+"\"><span class='score'><span></span></span></span>");
					
						if(opts.messageloc === 1)
						{
							jFeedback.insertAfter(jPass);
						}
						else
						{
							jFeedback.insertBefore(jPass);
						}
					}
					
					if(!jFeedback.hasClass(cssClass))
					{
						jFeedback.removeClass(allCSSArr.join(' ')).addClass(cssClass);
						
					}
					
					jFeedback.find("span.score span").html('&nbsp;' + message);
					jFeedback.find("span.score span").css({width: score + '%'});
					
					opts.strengthCallback(score, strength);
					
				});
		 		 

			});
	};  
	
	//PRIVATE STATIC FUNCTIONS
	function teststrength(password, username, option)
	{
	    //password < 4
	    if (password.length < option.shortLimit)
		{
			return 'tooshort';
		}
	    
	    //password == user name
	    if (password.toLowerCase()==username.toLowerCase())
		{
			return 'same';
		}
	    
		var score = 0; 
		
	    //password length
	    score += password.length * 4;
	    score += ( checkRepetition(1,password).length - password.length ) * 1;
	    score += ( checkRepetition(2,password).length - password.length ) * 1;
	    score += ( checkRepetition(3,password).length - password.length ) * 1;
	    score += ( checkRepetition(4,password).length - password.length ) * 1;
	
	    //password has 3 numbers
	    if (password.match(/(.*[0-9].*[0-9].*[0-9])/))
		{
			score += 5;
		} 
	    
	    //password has 2 symbols
	    if (password.match(/(.*[!,@,#,$,%,^,&,*,?,_,~].*[!,@,#,$,%,^,&,*,?,_,~])/))
		{
			score += 5;
		}
	    
	    //password has Upper and Lower chars
	    if (password.match(/([a-z].*[A-Z])|([A-Z].*[a-z])/))
		{
			score += 10;
		} 
	    
	    //password has number and chars
	    if (password.match(/([a-zA-Z])/) && password.match(/([0-9])/))
		{
			score += 15;
		}
		
	    //
	    //password has number and symbol
	    if (password.match(/([!,@,#,$,%,^,&,*,?,_,~])/) && password.match(/([0-9])/))
		{
			score += 15;
		} 
	    
	    //password has char and symbol
	    if (password.match(/([!,@,#,$,%,^,&,*,?,_,~])/) && password.match(/([a-zA-Z])/))
		{
			score += 15;
		}
	    
	    //password is just a numbers or chars
	    if (password.match(/^\w+$/) || password.match(/^\d+$/) )
		{
			score -= 10;
		}
	    
	    //verifying 0 < score < 100
	    if (score < 0)
		{
			score = 0;
		} 
		
	    if (score > 100)
		{
			score = 100;
		} 
	    
		return score;
	};

	function checkRepetition(pLen,str)
	{
		var res = "";
		
		for (var i = 0; i < str.length; i++) 
		{
			var repeated=true;
	 
			for (var j=0;j < pLen && (j+i+pLen) < str.length;j++)
			{
				repeated=repeated && (str.charAt(j+i)==str.charAt(j+i+pLen));
			}
			
			if (j<pLen)
			{
				repeated=false;
			}
			
			if (repeated)
			{
				 i+=pLen-1;
				 repeated=false;
			}
			else
			{
				res+=str.charAt(i);
			}
		}
		
		return res;
	};

})(jQuery);