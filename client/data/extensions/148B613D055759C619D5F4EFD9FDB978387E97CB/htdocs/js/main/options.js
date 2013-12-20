Osiris.Options =
{
		countersFilter: new Array(),
		countersTotal: new Array(),
		
		init: function()
		{
			$('#filterinput').change(function() {
  			Osiris.Options.doFilter(false);
			});
			
			$('#filterinput').keyup(function() {
  			Osiris.Options.doFilter(false);
			});
			
			Osiris.Options.doFilter(true);
		},
		
    doFilter: function(first)
    {
    	Osiris.Options.countersFilter = new Array();
    	
			var search = $("#filterinput").val();
			//search = "p2p";
			
			$(".os_area_content").find(".os_option").each(function(){
				var option = $(this);
				//alert(option);
								
				var pageName = option.parents("[data-os-tabtype=body]").attr("data-os-optionPage");
				//alert(pageName);				
				
				var pageHeader  = $("#" + pageName);
				
				pageHeader.fadeTo(Osiris.effectsSpeedSlow, 0.1);
				
				var counter = $("#" + pageName + "_counter");
				//var counterDom = counter.get(0);
				if(pageName in Osiris.Options.countersFilter)
				{					
				}
				else
				{
					Osiris.Options.countersFilter[pageName] = 0;
					if(first)
						Osiris.Options.countersTotal[pageName] = 0;
				}
												
				var text = $(this).text();				
				
				var show = true;
				if( (search != "") && (text.indexOf(search) == -1) )
					show = false;
				
				if(show)
					Osiris.Options.countersFilter[pageName]++;
				if(first)
					Osiris.Options.countersTotal[pageName]++;
				
				if(show)
				{
					//pageHeader.fadeTo(Osiris.effectsSpeedSlow, 1);
					$(this).slideDown(Osiris.effectsSpeedSlow);
				}
				else
				{
					$(this).slideUp(Osiris.effectsSpeedSlow);
				}
					
				counter.text(Osiris.Options.countersFilter[pageName] + " / " + Osiris.Options.countersTotal[pageName]);
				
			});						
    }
}

$(function() {
 // Handler for .ready() called.
 	Osiris.Options.init();
});