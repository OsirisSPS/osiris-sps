import os
import osiris

import adv

extension = None
httpDirectory = None

class MyDirectory(osiris.IHttpDirectory):
	def __init__(self, name):
		osiris.IHttpDirectory.__init__(self, name)
		
	def handle(self, session, path):
	
		relPage = path.getUrl()
		
		page = None
	
		if(relPage == "adv"):
			page = adv.Page(session)			
			
		if(page != None):
			page.transmit()				
			return True
		else:
			return False

def load(args):
	global extension
	global httpDirectory
	
	extension = args[0]

	httpDirectory = MyDirectory(extension.getID().getString())
	osiris.IdeSystem.instance().server.addDirectory(httpDirectory);
	
	
def unload(args):

	global extension
	global httpDirectory

	osiris.IdeSystem.instance().server.removeDirectory(httpDirectory)
	
