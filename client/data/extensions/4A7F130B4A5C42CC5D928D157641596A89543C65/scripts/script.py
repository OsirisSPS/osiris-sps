import os
import osiris

import globalvars
import index
import omltest
import htmltest
import regextest
import pythondoc
import pythondocgenerator
import pythoninfo
import images
import stresshttp
import elements
import testsuite

class MyDirectory(osiris.IHttpDirectory):
	def __init__(self, name):
		osiris.IHttpDirectory.__init__(self, name)
		
	def handle(self, session, path):
	
		relPage = path.getUrl()
	
		if(relPage == "omltest"):
			page = omltest.Page(session)			
		elif(relPage  == "htmltest"):
			page = htmltest.Page(session)			
		elif(relPage  == "regextest"):
			page = regextest.Page(session)			
		elif(relPage  == "pythondoc"):
			page = pythondoc.Page(session)			
		elif(relPage  == "pythoninfo"):
			page = pythoninfo.Page(session)			
		elif(relPage  == "pythondocgenerator"):
			page = pythondocgenerator.Page(session)			
		elif(relPage  == "images"):
			page = images.Page(session)			
		elif(relPage  == "stresshttp"):
			page = stresshttp.Page(session)			
		elif(relPage  == "elements"):
			page = elements.Page(session)			
		elif(relPage  == "testsuite"):
			page = testsuite.Page(session)			
		else:
			page = index.Page(session)
			
		if(page != None):
			page.transmit()				
			return True
		else:
			return False

def load(args):
	globalvars.extension = args[0]
	
	globalvars.extension.enterUrl = "/developer_tools"

	globalvars.httpDirectory = MyDirectory("developer_tools")
	osiris.IdeSystem.instance().server.addDirectory(globalvars.httpDirectory);
	
	# TODO: posso mappare una resourcelocation in un sotto-path? /developer_tools/pythondoc al posto di developer_tools_pythondoc?
	path = os.path.join(globalvars.extension.path, "pythondoc")
	globalvars.httpDirectoryPythonDoc = osiris.IdeSystem.instance().server.addResourceLocation("developer_tools_pythondoc", path);
	
def unload(args):

	if globalvars.httpDirectoryPythonDoc != None:
		osiris.IdeSystem.instance().server.removeDirectory(globalvars.httpDirectoryPythonDoc)

	osiris.IdeSystem.instance().server.removeDirectory(globalvars.httpDirectory)
	
