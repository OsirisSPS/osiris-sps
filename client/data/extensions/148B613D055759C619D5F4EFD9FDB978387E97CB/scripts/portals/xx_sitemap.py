import osiris
import os

class Page(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)
				
	def getPageName(self):
		return "latest_user_discussions"		
		
	def onInit(self):
		osiris.IPortalPage.onInit(self)		
		
		self.showTexts = osiris.HtmlCheckBox()
		self.showTexts.id = "showTexts"
		self.showTexts.autoPostBack = True
		self.showTexts.caption = self.getText("portal.pages.sitemap.filters.texts")
		
		self.showInstances = osiris.HtmlCheckBox();
		self.showInstances.id = "showInstances"
		self.showInstances.autoPostBack = True
		self.showInstances.caption = self.getText("portal.pages.sitemap.filters.instances")
		
	def onLoad(self):
		osiris.IPortalPage.onLoad(self)		
		
		blockFilters = osiris.IdeBlock(self.getText("portal.pages.sitemap.filters"))
		blockFilters.body.controls.add(self.showTexts);
		blockFilters.body.controls.add(self.showInstances);
		self.getArea(osiris.pageAreaContent).controls.add(blockFilters)
		
		
		myBlock = osiris.IdeBlock(self.getText("portal.pages.sitemap.title"))
		
		query = osiris.SearchQuery()
		
		query.orderMode = osiris.searchOrderModePosition
		query.orderDirection = osiris.searchOrderDirectionAscendent
		query.groupMode = False
		query.showDeleted = True
		query.treeMode = True
		query.template = "row"
		#query.option(objects.text).active = page.showTexts.check;
		#query.option(objects.instance).active = page.showInstances.check;
		
		ideQuery = osiris.IdeSearchQuery(query)
		ideQuery.setDirectRun(True)
		ideQuery.setShowParams(False)						
		ideQuery.id = "query"
		
		myBlock.body.controls.add(ideQuery)
		self.getArea(osiris.pageAreaContent).controls.add(myBlock)
			
	def onPreRender(self):
		osiris.IPortalPage.onPreRender(self)		
		
		

		
		
def main(args):
	page = Page(args[0])
	page.transmit()