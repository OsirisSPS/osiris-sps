import os
import osiris

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		
		if session.request.getUrlParam("mode") == "dialog":
			self.ajax = True
			
	def isMcpModeRequired(self):
		return False
				
	def getPageName(self):
		return "main.pages.omlhelp"
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)		

		document = osiris.XMLDocument()
		root = document.create("omlhelp")		
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "omlhelp.xsl"))
		template.document = document		
		
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)
			
		document.root.setAttributeString("mode", self.request.getUrlParam("mode"))
		
