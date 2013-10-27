import os
import osiris

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
	
	def getPageName(self):
		return "main.pages.redirect"
		
	def isMcpModeRequired(self):
		return False

	def onLoad(self):
		osiris.IMainPage.onLoad(self)
		
		url = self.request.getUrlParam("url")
		if url != "":				
			redirectMetaTag = osiris.HtmlMetaTag()		
			redirectMetaTag.httpEquiv = "refresh"
			redirectMetaTag.content = "2; url=" + url
			self.addMetaTag(redirectMetaTag)

			document = osiris.XMLDocument()
			root = document.create("page")		
			root.attributes.set("url", url)
			
			template = osiris.HtmlXSLControl()
			template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "redirect.xsl"))
			template.document = document
			self.getArea(osiris.pageAreaContent).controls.add(template)			
		
def main(args):
	page = Page(args[0])
	page.transmit()