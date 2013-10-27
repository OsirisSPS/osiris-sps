import globalvars
import os
import osiris

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		#self.ajax = (session.request.getUrlParam("forframe") == "true")
	
	def getPageName(self):
		return "main.pages.external"
		
	def isMcpModeRequired(self):
		return False

	def onLoad(self):
		osiris.IMainPage.onLoad(self)
		
		self.addCss(self.skin.getResourceUrl("css/main/external.css"))
		
		url = self.request.getUrlParam("url")
		if url != "":				
			document = osiris.XMLDocument()
			root = document.create("page")
			root.attributes.set("url", url)
			root.attributes.set("confirm", self.request.getUrlParam("confirm"))
			
			template = osiris.HtmlXSLControl();
			template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "external.xsl"))
			template.document = document;
			
			self.getArea(osiris.pageAreaContent).controls.add(template);
		
def main(args):
	page = Page(args[0])
	page.transmit()