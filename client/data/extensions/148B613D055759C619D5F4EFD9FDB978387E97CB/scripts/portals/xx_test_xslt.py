import osiris
import os

class Page(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)
				
	def getPageName(self):
		return "xx_test_xslt"
		
	def onPreRender(self):
		osiris.IPortalPage.onPreRender(self)		

		document = osiris.XMLDocument()
		root = document.create("test")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "xx_test_xslt.xsl"))
		template.document = document
		self.getArea(osiris.pageAreaContent).controls.add(template)
		
def main(args):
	page = Page(args[0])
	page.transmit()
