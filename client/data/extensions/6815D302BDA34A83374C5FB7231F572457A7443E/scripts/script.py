import os
import osiris
import osiris.events

extension = None
mainHandler = None
eventCallbackBeforePageRender = None

class HtmlPage(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		self.ajax = True
				
	def getPageName(self):
		return "~extensions.6815D302BDA34A83374C5FB7231F572457A7443E.page"
		
	def onInit(self):
		global extension
		
		osiris.IMainPage.onInit(self)
		
		#self.addJavascript("/" + extension.id.getString() + "/js/skineditor.js")
		#self.page.addCss("/" + extension.id.getString() + "/css/skineditor.css")
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)	
		
		document = osiris.XMLDocument()
		root = document.create("page")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "page.xsl"))
		template.document = document
		self.controls.add(template)	
		
		root.setAttributeString("session", self.session.state.getID())	

class MainHandler(osiris.IHttpDirectoryHandler):
	def __init__(self):
		osiris.IHttpDirectoryHandler.__init__(self)
		
	def processRequest(self, session, path):
	
		relPage = path.getUrl()
		
		page = None
	
		if(relPage == "skineditor"):
			page = HtmlPage(session)	
			
		if(page != None):
			page.transmit()				
			return True
		else:
			return False
			
	def onBeforePageRender(self, e):
		global extension
		e.page.addJavascript("/" + extension.id.getString() + "/js/skineditor.js")	
		e.page.addCss("/" + extension.id.getString() + "/css/skineditor.less")	

		

def load(args):
	global extension
	global mainHandler
	global eventCallbackBeforePageRender
	
	extension = args[0]
	mainHandler = MainHandler()
	osiris.PortalsSystem.instance().mainDirectory.addHandler(mainHandler);
	eventCallbackBeforePageRender = osiris.events.connect(osiris.IdeSystem.instance().eventBeforePageRender, mainHandler.onBeforePageRender)	
	
	
def unload(args):	
	global mainHandler
	global eventCallbackBeforePageRender
	
	if(mainHandler != None):
		eventCallbackBeforePageRender.disconnect()
		osiris.PortalsSystem.instance().mainDirectory.removeHandler(mainHandler);
	
