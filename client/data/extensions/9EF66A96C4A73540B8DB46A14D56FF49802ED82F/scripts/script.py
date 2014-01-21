import os
import osiris

import globalvars
import omljwplayer

def load(args):
	
	globalvars.extension = args[0]
	
	#path = os.path.join(globalvars.extension.path, "htdocs")
	#globalvars.httpDirectory = osiris.IdeSystem.instance().server.addResourceLocation(globalvars.extension.getID().getString(), path);
	#if globalvars.httpDirectory != None:
	#	globalvars.httpDirectory.defaultFile = "index.html";
		
	globalvars.omlJWPlayer = omljwplayer.OmlJWPlayer("jwplayer")
	
	osiris.OMLManager.instance().add(globalvars.omlJWPlayer)
	
def unload(args):
	
	#if globalvars.httpDirectory != None:
	#	osiris.IdeSystem.instance().server.removeDirectory(globalvars.httpDirectory)
		
	osiris.OMLManager.instance().remove(globalvars.omlJWPlayer)
