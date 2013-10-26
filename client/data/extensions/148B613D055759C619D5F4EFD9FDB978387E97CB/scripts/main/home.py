import globalvars
import os
import osiris

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		
	def getPageName(self):
		return "main.pages.home"
			
	def onLoad(self):
		osiris.IMainPage.onLoad(self)
		
		self.addCss(self.skin.getResourceUrl("css/main/home.less"))
		
		osiris.events.connect(self.events.get("onPortalRemove"), self.onPortalRemove)
		osiris.events.connect(self.events.get("onPortalSelfCreate"), self.onPortalSelfCreate)
		
				
		
	
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)			
		
		document = osiris.XMLDocument()
		root = document.create("page")
		
		self.renderPageActions(root)
		self.renderSubscribedPortals(root)
		
		if(osiris.ExtensionsSystem.instance().knownUpgradableCounter != -1):
			root.attributes.set("upgradable_counter",osiris.ExtensionsSystem.instance().getKnownUpgradableCounter())
			
		if(osiris.IsisSystem.instance().getLatestOsirisVersion() != ""):
			if(osiris.Engine.instance().getVersionName(False) != osiris.IsisSystem.instance().getLatestOsirisVersion()):
				root.attributes.set("latest_osiris_notes", osiris.IsisSystem.instance().getLatestOsirisNotes())			
			
		root.attributes.set("subscribe_self_portal_href",self.getEventCommand("onPortalSelfCreate"))		
		
		if(self.sessionAccount.isLogged() == True):
			root.attributes.set("session_user", self.sessionAccount.getUserID().string)
		else:
			root.attributes.set("session_user", "")
			
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "home.xsl"))
		template.document = document
		self.getArea(osiris.pageAreaContent).controls.add(template)
		
	def renderPageActions(self, node):
		nodeActions = node.nodes.add("actions")
		
		actionPreferences = nodeActions.nodes.add("action")
		actionPreferences.attributes.set("name", "options")
		actionPreferences.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("options"))
		
		actionAddons = nodeActions.nodes.add("action")
		actionAddons.attributes.set("name", "addons")
		actionAddons.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("addons"))
		
		actionAbout = nodeActions.nodes.add("action")
		actionAbout.attributes.set("name", "about")
		actionAbout.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("about"))
		
		actionImport = nodeActions.nodes.add("action")
		actionImport.attributes.set("name", "import")
		actionImport.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("import"))
		
		actionSubscribePortal = nodeActions.nodes.add("action")
		actionSubscribePortal.attributes.set("name", "subscribe")
		actionSubscribePortal.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("subscribe"))
		
		if(self.sessionAccount.isLogged() == True):
			actionCreatePortal = nodeActions.nodes.add("action")
			actionCreatePortal.attributes.set("name", "create")
			actionCreatePortal.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("create"))		
		
		actionNetwork = nodeActions.nodes.add("action")
		actionNetwork.attributes.set("name", "network")
		actionNetwork.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("network"))		
		
		actionIsis = nodeActions.nodes.add("action")
		actionIsis.attributes.set("name", "isis")
		actionIsis.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("isis"))		
		
		actionJobs = nodeActions.nodes.add("action")
		actionJobs.attributes.set("name", "jobs")
		actionJobs.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("jobs"))
		
		actionMaintenance = nodeActions.nodes.add("action")
		actionMaintenance.attributes.set("name", "maintenance")
		actionMaintenance.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("maintenance"))
		
		if(self.getMcpMode()):
			if(osiris.Options.instance().getWebMcpPassword() != ""):
				actionMcpLogout = nodeActions.nodes.add("action")
				actionMcpLogout.attributes.set("name", "mcp")
				actionMcpLogout.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("mcp"))
		
				self.cmdLogout = osiris.IdeButton(self.getText("main.pages.mcp.action.logout"))
	
	def renderSubscribedPortals(self, root):
		subscribedPortals = osiris.PortalsSystem.instance().portals
		if len(subscribedPortals) == 0:
			return
			
		nodePortals = root.nodes.add("subscribedPortals")
		for portal in subscribedPortals:
					
			nodePortal = nodePortals.nodes.add("portal")
			
			self.renderPortal(nodePortal, portal)			
			
			
	def renderPortal(self, node, portal):
	
		
		
		node.attributes.set("id", portal.portalID.string)
		node.attributes.set("pov", portal.povID.string)
		node.attributes.set("name", portal.name)
		if portal.optionsShared.portalDescription != "":
			node.attributes.set("description", portal.optionsShared.portalDescription)
				
		# Manca da wrappare ObjectID...	
		#nodeUser = node.nodes.add("user")		
		#portalTransaction = portal.startTransaction(False)
		#userObject = portal.getObject(portalTransaction.database, portal.user)
		#if(userObject):
		#	exporter = osiris.XMLPortalExporter(nodeUser, self, "full", false)
		#	userObject.exportXML(exporter)
			
		node.attributes.set("exchangeEnabled", "true" if portal.options.exchangeEnabled else "false")
						
		if(osiris.Options.instance().getWebMcpPassword() != ""):
			node.attributes.set("publicEnabled", "true" if portal.options.publicEnabled else "false")
			node.attributes.set("default", "true" if (osiris.Options.instance().getWebDefaultPortal() == portal.getFullPovID().toUTF16()) else "false")				
				
		node.attributes.set("lastObjectDate", portal.options.lastObjectDate.toXML())
		
		href = 	portal.getLink("view")
		node.attributes.set("href", href)
				
		nodeIsisEndpoints = node.nodes.add("isis")
		isisEndpoints = portal.options.isisEndpoints;		
		for isisEndpointID in isisEndpoints.keys():
			isisEndpoint = isisEndpoints[isisEndpointID];
			nodeIsis = nodeIsisEndpoints.nodes.add("isis")
			
			nodeIsis.attributes.set("name",isisEndpoint.getName());
			nodeIsis.attributes.set("url",isisEndpoint.url.toString());
			nodeIsis.attributes.set("enabled",isisEndpoint.enabled);
			
	
		nodeActions = node.nodes.add("actions")
			
		nodeAction = nodeActions.nodes.add("action")
		nodeAction.attributes.set("name", "enter")
		nodeAction.attributes.set("href", href)
		
		nodeAction = nodeActions.nodes.add("action")
		nodeAction.attributes.set("name", "info")
		nodeAction.attributes.set("href", portal.getLink("info"))
		
		nodeAction = nodeActions.nodes.add("action")
		nodeAction.attributes.set("name", "accounts")
		nodeAction.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("accounts?portal=" + portal.getFullPovID()))
		
		nodeAction = nodeActions.nodes.add("action")
		nodeAction.attributes.set("name", "settings")
		nodeAction.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("settings?portal=" + portal.getFullPovID()))
		
		nodeAction = nodeActions.nodes.add("action")
		nodeAction.attributes.set("name", "invite")
		nodeAction.attributes.set("call", portal.getLink("invite?mode=dialog"))
					
		nodeAction = nodeActions.nodes.add("action")
		nodeAction.attributes.set("name", "remove")
		#nodeAction.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("removeportal?portal=" + portal.getFullPovID()))
		nodeAction.attributes.set("href", self.getEventCommand("onPortalRemove",portal.getFullPovID()))					
		nodeAction.attributes.set("confirm", "true")
		
	def onPortalRemove(self, args):		
		self.showMessage(args[0])					
		
		portal = osiris.PortalsSystem.instance().getPortalByFullPov(args[0]);
		if portal:
			osiris.PortalsSystem.instance().removePortal(portal.portalID, portal.povID)
		
	def onPortalSelfCreate(self, args):		
		self.showMessage("selfcreate!")					
		

