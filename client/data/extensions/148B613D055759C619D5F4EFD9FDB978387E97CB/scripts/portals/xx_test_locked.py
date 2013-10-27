import osiris
import os

class Page(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)
				
	def getPageName(self):
		return "test"
		
	def onLoad(self):
		osiris.IPortalPage.onLoad(self)
				
		portal = self.portal
		options = portal.options
		self.showMessage("1")
		isisEndpoints = options.isisEndpoints
		self.showMessage("2")
				
def main(args):
	page = Page(args[0])
	page.transmit()
