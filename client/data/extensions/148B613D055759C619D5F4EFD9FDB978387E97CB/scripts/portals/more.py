import os
import osiris

class Page(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)
		
		if session.request.getUrlParam("mode") == "dialog":
			self.ajax = True
				
	def getPageName(self):
		return "portal.pages.more"
		
	def onLoad(self):
		osiris.IPortalPage.onLoad(self)		
		
		document = osiris.XMLDocument()
		self.root = document.create("more")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "more.xsl"))
		template.document = document
		
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)
			
		document.root.setAttributeString("mode", self.request.getUrlParam("mode"))
		
		self.renderPageActions(self.root)
		
	def renderPageActions(self, node):
		nodeActions = node.nodes.add("actions")
		
		actionHome = nodeActions.nodes.add("action")
		actionHome.attributes.set("name", "home")
		actionHome.attributes.set("icon", "home")
		actionHome.attributes.set("href", self.portal.getLink("view"))
		
		if(self.sessionAccount.isPortalGuest(self.database) == False):
			actionProfile = nodeActions.nodes.add("action")
			actionProfile.attributes.set("name", "account")
			actionProfile.attributes.set("icon", "account")
			actionProfile.attributes.set("href", self.portal.getLink("account"))
			
			actionPrivateMessages = nodeActions.nodes.add("action")
			actionPrivateMessages.attributes.set("name", "messenger")
			actionPrivateMessages.attributes.set("icon", "messenger")
			actionPrivateMessages.attributes.set("href", self.portal.getLink("messenger"))
			
		actionEdit = nodeActions.nodes.add("action")
		actionEdit.attributes.set("name", "info")
		actionEdit.attributes.set("icon", "info")
		actionEdit.attributes.set("href", self.portal.getLink("info"))
				
		actionEdit = nodeActions.nodes.add("action")
		actionEdit.attributes.set("name", "edit")
		actionEdit.attributes.set("icon", "edit")
		actionEdit.attributes.set("href", self.portal.getLink("edit"))
		
		actionSearch = nodeActions.nodes.add("action")
		actionSearch.attributes.set("name", "search")
		actionSearch.attributes.set("icon", "search")
		actionSearch.attributes.set("href", self.portal.getLink("search"))
		
		actionUsers = nodeActions.nodes.add("action")
		actionUsers.attributes.set("name", "users")
		actionUsers.attributes.set("icon", "users")
		actionUsers.attributes.set("href", self.portal.getLink("users"))
		
		actionChanges = nodeActions.nodes.add("action")
		actionChanges.attributes.set("name", "changes")
		actionChanges.attributes.set("icon", "changes")
		actionChanges.attributes.set("href", self.portal.getLink("changes"))
		
		if(self.sessionAccount.isPortalGuest(self.database) == False):
			actionLatestUserDiscussions = nodeActions.nodes.add("action")
			actionLatestUserDiscussions.attributes.set("name", "latest_user_discussions")
			actionLatestUserDiscussions.attributes.set("icon", "latest_user_discussions")
			actionLatestUserDiscussions.attributes.set("href", self.portal.getLink("latest_user_discussions"))
		
		actionLatestDiscussions = nodeActions.nodes.add("action")
		actionLatestDiscussions.attributes.set("name", "latest_discussions")
		actionLatestDiscussions.attributes.set("icon", "latest_discussions")
		actionLatestDiscussions.attributes.set("href", self.portal.getLink("latest_discussions"))
		
		if( (self.getMcpMode()) and (self.getRequestSource() == osiris.IPage.rsOsiris) ):
			actionPeers = nodeActions.nodes.add("action")
			actionPeers.attributes.set("name", "peers")
			actionPeers.attributes.set("icon", "peers")
			actionPeers.attributes.set("href", self.portal.getLink("peers"))
		
		actionInvite = nodeActions.nodes.add("action")
		actionInvite.attributes.set("name", "invite")
		actionInvite.attributes.set("icon", "invite")
		actionInvite.attributes.set("href", self.portal.getLink("invite"))
			
		if( (self.getMcpMode()) and (self.getRequestSource() == osiris.IPage.rsOsiris) ):
			actionACP = nodeActions.nodes.add("action")
			actionACP.attributes.set("name", "acp")
			actionACP.attributes.set("icon", "acp")
			actionACP.attributes.set("href", self.portal.getLink("acp"))
		
		#actionSiteMap = nodeActions.nodes.add("action")
		#actionSiteMap.attributes.set("name", "sitemap")
		#actionSiteMap.attributes.set("href", self.portal.getLink("sitemap"))		
		
		#actionStats = nodeActions.nodes.add("action")
		#actionStats.attributes.set("name", "stats")
		#actionStats.attributes.set("href", self.portal.getLink("stats"))		
		
	def onPreRender(self):
		osiris.IPortalPage.onPreRender(self)		

		
		
def main(args):
	page = Page(args[0])
	page.transmit()
