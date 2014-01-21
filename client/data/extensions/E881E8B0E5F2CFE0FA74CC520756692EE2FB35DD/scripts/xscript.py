import os
import osiris

global skinLightBlue
global skinDarkBlue

def load(args):

	global skinLightBlue
	global skinDarkBlue
	
	extension = args[0]
	
	skinLightBlue = osiris.IdeSkinSimple(extension, "skin_light_blue.xml", "light_blue")
	skinDarkBlue = osiris.IdeSkinSimple(extension, "skin_dark_blue.xml", "dark_blue")
		
def unload(args):

	global skinLightBlue
	global skinDarkBlue
	
	skinLightBlue = None
	skinDarkBlue = None

	
	