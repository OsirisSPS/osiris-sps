import osiris

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
				
	def getPageName(self):
		return "main.pages.error"
		
	def onLoad(self):
		osiris.IMainPage.onLoad(self)		

		self.showError(self.request.getUrlParam("message"));	
		
def main(args):
	page = Page(args[0])
	page.transmit()
