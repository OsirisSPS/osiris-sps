#winpdb
#import rpdb2; rpdb2.start_embedded_debugger("osiris")

import os
import osiris

import globalvars
import omlanubis

def load(args):	
	globalvars.extension = args[0]
		
	globalvars.omlAnubis = omlanubis.OmlAnubis("anubis")
	
	osiris.OMLManager.instance().add(globalvars.omlAnubis)
	
def unload(args):	
	
	osiris.OMLManager.instance().remove(globalvars.omlAnubis)
