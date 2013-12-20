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
		return "main.pages.network"
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)		
		
		document = osiris.XMLDocument()
		root = document.create("help")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "network.xsl"))
		template.document = document
		
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)
			
		document.root.setAttributeDateTime("last_request", osiris.IsisSystem.instance().getLastRequest())
		document.root.setAttributeString("last_error", osiris.IsisSystem.instance().getLastError())
		document.root.setAttributeString("ip", osiris.IsisSystem.instance().getIp())
		document.root.setAttributeInt32("port", osiris.IsisSystem.instance().getPort())
		document.root.setAttributeString("country_code", osiris.IsisSystem.instance().getCountryCode())
		document.root.setAttributeString("country_name", osiris.IsisSystem.instance().getCountryName())
		
		document.root.setAttributeString("mode", self.request.getUrlParam("mode"))
				
def main(args):
	page = Page(args[0])
	page.transmit()
