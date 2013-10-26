import osiris
import os

class Page(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)
				
	def getPageName(self):
		return "stats"
		
	def createControl(self, id, sql):
		myBlock = osiris.IdeBlock("Objects Types and count");
		
		myControl = osiris.IdeTableQuery()		
		myControl.id = id
		myControl.setSql(sql)
		
		myBlock.body.controls.add(myControl)
		self.getArea(osiris.pageAreaContent).controls.add(myBlock)
		return myControl;
		
	def onPreRender(self):
		osiris.IPortalPage.onPreRender(self)
		
		sql = "";
		sql += " select 'Users' Type, (select count(*) from os_users) Total"
		sql += " union"
		sql += " select 'Sections', (select count(*) from os_sections) Total"
		sql += " union"
		sql += " select 'Texts', (select count(*) from os_texts) Total"
		sql += " union"
		sql += " select 'Posts', (select count(*) from os_posts) Total"
		sql += " union"
		sql += " select 'Polls', (select count(*) from os_polls) Total"
		sql += " union"
		sql += " select 'PollsOptions', (select count(*) from os_polls_options) Total"
		sql += " union"
		sql += " select 'PollsVotes', (select count(*) from os_polls_votes) Total"
		sql += " union"
		sql += " select 'Files', (select count(*) from os_files) Total"
		sql += " union"
		sql += " select 'Models', (select count(*) from os_models) Total"
		sql += " union"
		sql += " select 'Instances', (select count(*) from os_instances) Total"
		sql += " union"
		sql += " select 'CalendarEvents', (select count(*) from os_calendar_events) Total"
		sql += " union"
		sql += " select 'Tags', (select count(*) from os_tags) Total"
		sql += " union"
		sql += " select 'Votes', (select count(*) from os_votes) Total"
		sql += " union"
		sql += " select 'Reputations', (select count(*) from os_reputations) Total"
		sql += " union"
		sql += " select 'Avatars', (select count(*) from os_avatars) Total"
		sql += " union"
		sql += " select 'PrivateMessages', (select count(*) from os_private_messages) Total"
		sql += " union"
		sql += " select 'Peers', (select count(*) from os_peers) Total"
		sql += " order by Total desc"

		myStats = self.createControl("totals",sql);
				
def main(args):
	page = Page(args[0])
	page.transmit()
