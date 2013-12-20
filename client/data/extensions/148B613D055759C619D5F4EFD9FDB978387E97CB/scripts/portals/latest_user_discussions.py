import osiris
import os

class Page(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)
				
	def getPageName(self):
		return "portal.pages.latest_user_discussions"		
		
	def onInit(self):
		osiris.IPortalPage.onInit(self)		
		
	def onLoad(self):
		osiris.IPortalPage.onLoad(self)		
		if(self.sessionAccount.isPortalGuest(self.database) == True):
				self.showMessage(self.getText("portals.errors.loggeduser_required"))
				return
				
		myBlock = osiris.IdeBlock(self.getText("portal.pages.latest_user_discussions.title"))
		
		query = osiris.SearchQuery()
		
		query.orderMode = osiris.searchOrderModeLastObject
		query.orderDirection = osiris.searchOrderDirectionDescendent
		query.groupMode = True
		query.showDeleted = False
		query.template = "table"
		query.byUser = "myself"
		
		ideQuery = osiris.IdeSearchQuery(query)
		ideQuery.setDirectRun(True)
		ideQuery.setShowParams(False)				
		
		ideQuery.id = "query"
		
		myBlock.body.controls.add(ideQuery)
		self.getArea(osiris.pageAreaContent).controls.add(myBlock)
			
	def onPreRender(self):
		osiris.IPortalPage.onPreRender(self)		
		
		if(self.accountSession.isGuest(self.database) == True):
				return
		
		

		
		
def main(args):
	page = Page(args[0])
	page.transmit()