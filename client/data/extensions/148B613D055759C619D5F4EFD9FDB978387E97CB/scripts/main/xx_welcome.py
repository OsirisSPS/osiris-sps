import osiris
import os

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
				
	def getPageName(self):
		return "welcome"
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)
		
		client = osiris.HttpClient()		
		
		#document = osiris.XMLDocument()
		#root = document.create("help")
		#template = osiris.HtmlXSLControl()
		#template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "help.xsl"))
		#template.document = document
		#self.getArea(osiris.pageAreaContent).controls.add(template)		
		
def main(args):
	page = Page(args[0])
	page.transmit()
