import os
import osiris

import globalvars
import omlcitation

def load(args):
	
	globalvars.extension = args[0]
	
	#path = os.path.join(globalvars.extension.path, "htdocs")
	#globalvars.httpDirectory = osiris.IdeSystem.instance().server.addResourceLocation(globalvars.extension.getID().getString(), path);
		
	globalvars.omlCitation = omlcitation.OmlCitation("citation")
	
	osiris.OMLManager.instance().add(globalvars.omlCitation)
	
def unload(args):
	
	#if globalvars.httpDirectory != None:
	#	osiris.IdeSystem.instance().server.removeDirectory(globalvars.httpDirectory)
		
	osiris.OMLManager.instance().remove(globalvars.omlCitation)
