import os
import osiris
import osiris.events

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
				
	def getPageName(self):
		return "xx_clodo"
		
	def isMcpModeRequired(self):
		return False
		
	def onInit(self):
		osiris.IMainPage.onInit(self)		
		
		self.document = osiris.XMLDocument()
		self.document.create("test")
		
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "xx_clodo.xsl"))
		template.document = self.document		
		self.getArea(osiris.pageAreaContent).controls.add(template)
		
		#job = osiris.MainAddonsJob(osiris.Engine.instance().peekBackgroundJobID(), self.portal)
		job = osiris.MainAddonsJob(osiris.Engine.instance().peekBackgroundJobID(), "test")
		osiris.Engine.instance().addBackgroundJob(job);
		
			
	def onLoad(self):
		osiris.IMainPage.onLoad(self)		
		
		
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)		

		
		
def main(args):
	page = Page(args[0])
	page.transmit()
