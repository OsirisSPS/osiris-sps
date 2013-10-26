import osiris
import os

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
				
	def getPageName(self):
		return "test"
		
	def onLoad(self):
		osiris.IMainPage.onLoad(self)
		
		jobs = osiris.Engine.instance().backgroundJobs
				
		self.showMessage("1")
				
def main(args):
	page = Page(args[0])
	page.transmit()
