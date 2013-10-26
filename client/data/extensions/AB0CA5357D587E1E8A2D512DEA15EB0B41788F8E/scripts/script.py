import os
import osiris

import globalvars
import omlyoutube

def load(args):	
	globalvars.extension = args[0]
		
	globalvars.omlYouTube = omlyoutube.OmlYouTube("youtube")
	
	osiris.OMLManager.instance().add(globalvars.omlYouTube)
	
def unload(args):	
	
	osiris.OMLManager.instance().remove(globalvars.omlYouTube)
