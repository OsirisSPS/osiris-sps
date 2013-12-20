import os
import osiris

class Page(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)
		
		self.ajax = True
		
	def isMcpModeRequired(self):
		return False
				
	def getPageName(self):
		return "portal.pages.assistant"
		
	def onLoad(self):
		osiris.IPortalPage.onLoad(self)		
		
		document = osiris.XMLDocument()
		root = document.create("assistant")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "assistant.xsl"))
		template.document = document		
		self.controls.add(template)
		
		#document.root.setAttributeString("mode", self.request.getUrlParam("mode"))		
		
