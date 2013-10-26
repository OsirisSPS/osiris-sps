import os
import osiris

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
				
	def getPageName(self):
		return "extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.pythondoc"
		
	def onInit(self):
		osiris.IMainPage.onInit(self)
		
		self.pathway.add(self.getText("extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.index.title"),"/developer_tools")
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)		

		document = osiris.XMLDocument()
		document.create("pythondoc")
		
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "pythondoc.xsl"))
		template.document = document
		self.getArea(osiris.pageAreaContent).controls.add(template)
		
def main(args):
	page = Page(args[0])
	page.transmit()
