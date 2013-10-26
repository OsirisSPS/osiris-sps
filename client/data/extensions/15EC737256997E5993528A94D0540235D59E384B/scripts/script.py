import os
import osiris

import globalvars
import omltag

def load(args):
	
	globalvars.extension = args[0]
	
	globalvars.omlTag = omltag.OmlTag("rotator")
	
	osiris.OMLManager.instance().add(globalvars.omlTag)
	
def unload(args):
	
	osiris.OMLManager.instance().remove(globalvars.omlTag)
