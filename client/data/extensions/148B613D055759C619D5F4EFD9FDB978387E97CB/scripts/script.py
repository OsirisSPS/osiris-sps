import os
import osiris
import sys

import globalvars

# "Main" pages
sys.path.append(os.path.join(os.path.dirname(__file__), "main"))
import addons
import omlhelp
import bbobject
import create
import error
import external
import help
import helptip
import home
#import jobstatus
import login_main
import mcp
import network
import redirect
import register_main
import account
import accounts2

# "Portals" pages
sys.path.append(os.path.join(os.path.dirname(__file__), "portals"))
import assistant
import changes
import latest_discussions
import latest_user_discussions
import login
import more
import register
import peers
import trash
import invite
import acp
import stats
import importer


# "Main" and "Portals" pages
sys.path.append(os.path.join(os.path.dirname(__file__), "both"))
import lesslib
import about



class MainHandler(osiris.IHttpDirectoryHandler):
	def __init__(self):
		osiris.IHttpDirectoryHandler.__init__(self)
		
	def processRequest(self, session, path):
	
		relPage = path.getUrl()
		
		page = None
		
		if(relPage == "lesslib"):
			page = lesslib.MainPage(session)			
		
		if(relPage == "about"):
			page = about.MainPage(session)			
	
		if(relPage == "addons"):
			page = addons.Page(session)			
			
		if(relPage == "omlhelp"):
			page = omlhelp.Page(session)			
			
		if(relPage == "bbobject"):
			page = bbobject.Page(session)			
			
		if(relPage == "create"):
			page = create.Page(session)			
			
		if(relPage == "error"):
			page = error.Page(session)			
			
		if(relPage == "external"):
			page = external.Page(session)			
			
		if(relPage == "help"):
			page = help.Page(session)			
			
		if(relPage == "helptip"):
			page = helptip.Page(session)			
			
		if(relPage == "home"):
			page = home.Page(session)			
			
		#if(relPage == "jobstatus"):
		#	page = jobstatus.Page(session)			
			
		if(relPage == "login"):
			page = login_main.Page(session)			
			
		if(relPage == "mcp"):
			page = mcp.Page(session)			
			
		if(relPage == "network"):
			page = network.Page(session)			
			
		if(relPage == "redirect"):
			page = redirect.Page(session)			
			
		if(relPage == "register"):
			page = register_main.Page(session)			
			
		if(relPage == "account"):
			page = account.Page(session)			
			
		if(relPage == "accounts2"):
			page = accounts2.Page(session)			
									
		if(page != None):
			page.transmit()				
			return True
		else:
			return False
		
class PortalsHandler(osiris.IHttpDirectoryHandler):
	def __init__(self):
		osiris.IHttpDirectoryHandler.__init__(self)
		
	def processRequest(self, session, path):
	
		relPage = path.getUrl()
		
		page = None
		
		if(relPage == "lesslib"):
			page = lesslib.PortalPage(session)			
		
		if(relPage == "about"):
			page = about.PortalPage(session)			
			
		if(relPage == "assistant2"):
			page = assistant.Page(session)			
	
		if(relPage == "changes"):
			page = changes.Page(session)			
			
		if(relPage == "latest_discussions"):
			page = latest_discussions.Page(session)			
			
		if(relPage == "latest_user_discussions"):
			page = latest_user_discussions.Page(session)			
			
		if(relPage == "login"):
			page = login.Page(session)			
			
		if(relPage == "more"):
			page = more.Page(session)			
			
		if(relPage == "register"):
			page = register.Page(session)			
			
		if(relPage == "peers"):
			page = peers.Page(session)			
			
		if(relPage == "trash"):
			page = trash.Page(session)			
		
		if(relPage == "invite"):
			page = invite.Page(session)			
			
		if(relPage == "acp"):
			page = acp.Page(session)			
			
		if(relPage == "stats"):
			page = stats.Page(session)			
			
		if(relPage == "importer"):
			page = importer.Page(session)			
		
				
		if(page != None):
			page.transmit()				
			return True
		else:
			return False

def load(args):	
	globalvars.extension = args[0]
	globalvars.mainHandler = MainHandler()
	globalvars.portalsHandler = PortalsHandler()
	
	globalvars.skin = osiris.IdeSkin()
	if(globalvars.skin.loadXml(globalvars.extension.path + "skin.xml")):
		osiris.IdeSystem.instance().registerSkin(globalvars.skin)	
	
	osiris.PortalsSystem.instance().mainDirectory.addHandler(globalvars.mainHandler);
	osiris.PortalsSystem.instance().portalsDirectory.addHandler(globalvars.portalsHandler);
		
def unload(args):
	osiris.PortalsSystem.instance().portalsDirectory.removeHandler(globalvars.portalsHandler);
	osiris.PortalsSystem.instance().mainDirectory.removeHandler(globalvars.mainHandler);
	
	
	osiris.IdeSystem.instance().unregisterSkin(globalvars.skin);
	
	


