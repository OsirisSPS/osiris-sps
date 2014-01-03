import osiris
import os
from osiris import events

class Page(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)
				
	def getPageName(self):
		return "portal.pages.trash"		
		
	def onInit(self):
		osiris.IPortalPage.onInit(self)		
				
	def onLoad(self):
		osiris.IPortalPage.onLoad(self)		
		
		myBlock = osiris.IdeBlock(self.getText("portal.pages.trash.title"))
		myBlock.specialPage = True
		myBlock.outerStyle = "clear:both";
		
		self.query = osiris.IdeTableQuery()
		sql = "select id, author, submit_date, insert_date, rank, accept_msg from os_entries te where rank=-1"
		self.query.setSql(sql)
		self.query.setColumnType(0,osiris.IdeTableQuery.ctObjectID)
		self.query.setColumnType(1,osiris.IdeTableQuery.ctUserID)
		self.query.setColumnType(2,osiris.IdeTableQuery.ctShortDateTime)
		self.query.setColumnType(3,osiris.IdeTableQuery.ctShortDateTime)
						
		myBlock.body.controls.add(self.query)
		self.getArea(osiris.pageAreaContent).controls.add(myBlock)
		
	def onPreRender(self):
		osiris.IPortalPage.onPreRender(self)			
				