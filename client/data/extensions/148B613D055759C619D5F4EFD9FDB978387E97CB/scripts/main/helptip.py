import os
import osiris

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		self.ajax = True
				
	#def getPageName(self):
	#	return "main.pages.helptip"
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)	
		
		helpID = self.session.request.getUrlParam("id")	
		title = self.session.request.getUrlParam("title")	
		hide = self.session.request.getUrlParam("hide")	
				
		if(hide == "true"):			
			osiris.Options.instance().setHelpTipStatus(helpID, True)
		else:			
			hide = osiris.Options.instance().getHelpTipStatus(helpID)
			
			if(hide == False):
				document = osiris.XMLDocument()
				root = document.create("helptip")
				template = osiris.HtmlXSLControl()
				template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "helptip.xsl"))
				template.document = document
				self.controls.add(template)
				
				document.root.setAttributeString("id", helpID)
				document.root.setAttributeString("title", title)
		
