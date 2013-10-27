Osiris.HtmlSnippet =
{
    play: function(id, code, html, reload, token)
    {
			jQuery(function() 
      {
        jQuery("#" + id + "_dialog").dialog(
        {
          show: 'fade',
          hide: 'fade',
          bgiframe: true,
          modal: true,
          width: '50%',          
          buttons: 
          {            
            Yes: function() 
            { 
            	jQuery(this).dialog('close');              
            	                         	
              Osiris.HtmlSnippet.response(id, code, html, reload, token, "yes", $("#" + id + "_remember").attr('checked') ? "yes":"no");
              
              
            },
            No: function() 
            { 
            	jQuery(this).dialog('close');              
            	                      
              Osiris.HtmlSnippet.response(id, code, html, reload, token, "no", $("#" + id + "_remember").attr('checked') ? "yes":"no");
            }
          },
          response: function(answer)
          {
          	
          },
          open: function()
			    {          
			    	    jQuery(":button:contains('Yes')").html($("#" + id + "_buttons_Yes").html());
                jQuery(":button:contains('No')").html($("#" + id + "_buttons_No").html());
          }
          
        });
      });
    },
    
    response: function(id, code, html, reload, token, answer, remember)
    {
    	remember = "no";
    	
    	if($("#" + id + "_remember").attr('checked'))
    	{
    		remember = "yes";
      } 
      
      // Show wait
      $("#" + id + "_play").removeClass("os_htmlsnippet_play os_htmlsnippet_denied").addClass("os_htmlsnippet_waiting");
      
      ajaxUrl = "/main/htmlsnippets?act=play&code=" + code + "&answer=" + answer + "&remember=" + remember + "&token=" + token;
      Osiris.loadUrl(ajaxUrl, function(response)
	    {
	    	  if(answer == "no")
					{
						if(remember == "yes")
						{
							$("#" + id + "_play").removeClass("os_htmlsnippet_play os_htmlsnippet_waiting").addClass("os_htmlsnippet_denied");
						}
						else
						{
							$("#" + id + "_play").removeClass("os_htmlsnippet_denied os_htmlsnippet_waiting").addClass("os_htmlsnippet_play");
						}
					}
					else
					{
			      if(reload == false)
			      {
			      	$("#" + id + "_play").fadeOut('slow', function() 
				    	{
				    		//Osiris.setInnerHtml(osGetById(id + "_placeholder"), html);
				    		$("#" + id + "_placeholder").html(html);
				    		$("#" + id + "_placeholder").fadeIn('slow');
				  		});    	        			  	
				  	}
				  	else
			  		{
			  			osSubmit();
			  		}
			  	}           
	    });
			
    }
    
}