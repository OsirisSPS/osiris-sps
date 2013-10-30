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
		return "main.pages.help"
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)		
		
		document = osiris.XMLDocument()
		root = document.create("help")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "help.xsl"))
		template.document = document
		
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)
		
		document.root.setAttributeString("mode", self.request.getUrlParam("mode"))
		
		document.root.setAttributeString("version", osiris.Engine.instance().getVersionName(True))
		
		document.root.setAttributeString("href_about", osiris.PortalsSystem.instance().getMainLink("about"))		
		document.root.setAttributeString("href_home", osiris.IsisSystem.instance().resolveItemLink("home"))
		document.root.setAttributeString("href_forum", osiris.IsisSystem.instance().resolveItemLink("forum"))
