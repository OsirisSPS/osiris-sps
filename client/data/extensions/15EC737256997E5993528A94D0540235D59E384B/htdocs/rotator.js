var Rotator = 
{
    init: function(srcID)
    {
    	src = osGetById(srcID);
    	src.rotatorCurrent = 0;
    	setInterval("Rotator.step('" + src.id + "');", 5000);
    },
    
    step: function(srcID)
    {
    	src = osGetById(srcID);
    	
    	jQuery(src.childNodes[src.rotatorCurrent]).fadeOut(1500);
    		
    	src.rotatorCurrent++;
    	src.rotatorCurrent = src.rotatorCurrent % src.childNodes.length;
    	
    	jQuery(src.childNodes[src.rotatorCurrent]).fadeIn(1500);    	
    	
    }
}
