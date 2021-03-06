import os
import osiris
import sys

# enable debugger
#import ptvsd
#ptvsd.enable_attach(secret = '')	# optionally define a password
#ptvsd.wait_for_attach()		# optional

import globalvars

# OML
sys.path.append(os.path.join(os.path.dirname(__file__), "oml"))
import anubis
import rotator

# "Main" pages
sys.path.append(os.path.join(os.path.dirname(__file__), "main"))
import account
import accounts
import addons
import omlhelp
import bbobject
import error
import external
import help
import helptip
import home
import isis
#import jobstatus
import login_main
import mcp
import network
import redirect
import skinpreview
import subscribe


# "Portals" pages
sys.path.append(os.path.join(os.path.dirname(__file__), "portals"))
import assistant
import changes
import exporter
import info
import latest_discussions
import latest_user_discussions
import login
import more
import register
import peers
import trash
import acp
import stabilization_stats
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
			
		if(relPage == "account"):
			page = account.Page(session)			
			
		if(relPage == "accounts"):
			page = accounts.Page(session)			
	
		if(relPage == "addons"):
			page = addons.Page(session)			
			
		if(relPage == "omlhelp"):
			page = omlhelp.Page(session)			
			
		if(relPage == "bbobject"):
			page = bbobject.Page(session)			
			
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
			
		if(relPage == "isis"):
			page = isis.Page(session)			
			
		if(relPage == "login"):
			page = login_main.Page(session)			
			
		if(relPage == "mcp"):
			page = mcp.Page(session)			
			
		if(relPage == "network"):
			page = network.Page(session)			
			
		if(relPage == "skinpreview"):
			page = skinpreview.Page(session)			
			
		if(relPage == "redirect"):
			page = redirect.Page(session)			
			
		if(relPage == "subscribe"):
			page = subscribe.Page(session)					
											
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
	
		if(relPage == "info"):
			page = info.Page(session)			
			
		if(relPage == "exporter"):
			page = exporter.Page(session)			
			
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
		
		if(relPage == "acp"):
			page = acp.Page(session)			
			
		if(relPage == "stabilization_stats"):
			page = stabilization_stats.Page(session)			
			
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
	
	globalvars.omlAnubis = anubis.OmlAnubis("anubis")	
	osiris.OMLManager.instance().add(globalvars.omlAnubis)
	
	globalvars.omlRotator = rotator.OmlRotator("rotator")	
	osiris.OMLManager.instance().add(globalvars.omlRotator)
		
def unload(args):
	osiris.OMLManager.instance().remove(globalvars.omlRotator)
	osiris.OMLManager.instance().remove(globalvars.omlAnubis)
	
	osiris.PortalsSystem.instance().portalsDirectory.removeHandler(globalvars.portalsHandler);
	osiris.PortalsSystem.instance().mainDirectory.removeHandler(globalvars.mainHandler);
	
	
	osiris.IdeSystem.instance().unregisterSkin(globalvars.skin);
	
	


