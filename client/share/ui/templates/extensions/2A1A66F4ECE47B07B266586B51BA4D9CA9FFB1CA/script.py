import os
import osiris

global skin

def load(args):

	global skin
	
	extension = args[0]
	
	skin = osiris.IdeSkinSimple(extension)
		
def unload(args):

	global skin
	
	skin = None

	
	