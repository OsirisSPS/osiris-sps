import os
import osiris

class Page(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)
		
		if session.request.getUrlParam("mode") == "dialog":
			self.ajax = True
				
	def getPageName(self):
		return "portal.pages.info"
		
	def onLoad(self):
		osiris.IPortalPage.onLoad(self)		
		
		osiris.events.connect(self.events.get("onPortalRemove"), self.onPortalRemove)
		
		document = osiris.XMLDocument()
		self.root = document.create("info")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "info.xsl"))
		template.document = document
		
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)
		
		
		
		node = self.root

		node.setAttributeString("mode", self.request.getUrlParam("mode"))
				
		if(self.sessionAccount.isLogged()):
			self.showMessage("logged")
			self.isUserOfPov = (self.sessionAccount.userID.getString() == self.portal.povID.getString());
			self.root.setAttributeBool("isUserOfPov", self.isUserOfPov);		
			
			self.povOfUser = osiris.PortalsSystem.instance().getPortal(self.portal.portalID, self.sessionAccount.userID.getString())			
			self.root.setAttributeBool("povOfUserExists", (self.povOfUser != None));		
		else:
			self.showMessage("not logged")
		
		nodeActions = node.nodes.add("actions")
			
		nodeAction = nodeActions.nodes.add("action")
		nodeAction.attributes.set("name", "home")
		nodeAction.attributes.set("href", self.portal.getLink("view"))

		if(self.sessionAccount.isLogged()):
			if(self.isUserOfPov):
				nodeAction = nodeActions.nodes.add("action")
				nodeAction.attributes.set("name", "acp")
				nodeAction.attributes.set("href", self.portal.getLink("acp"))
			elif(self.povOfUser == None):				
				params  = { }
				params["mode"] = "fork"
				params["name"] = self.portal.name
				params["id"] = self.portal.portalID.toWide()
				nodeAction = nodeActions.nodes.add("action")
				nodeAction.attributes.set("name", "fork")
				nodeAction.attributes.set("prefix", "main.pages.subscribe")
				nodeAction.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("subscribe", params))
		
		#nodeAction = nodeActions.nodes.add("action")
		#nodeAction.attributes.set("name", "info")
		#nodeAction.attributes.set("href", self.portal.getLink("info"))
		
		#nodeAction = nodeActions.nodes.add("action")
		#nodeAction.attributes.set("name", "accounts")
		#nodeAction.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("accounts?portal=" + self.portal.getFullPovID()))
		
		nodeAction = nodeActions.nodes.add("action")
		nodeAction.attributes.set("name", "settings")
		nodeAction.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("settings?portal=" + self.portal.getFullPovID()))
		
		#nodeAction = nodeActions.nodes.add("action")
		#nodeAction.attributes.set("name", "invite")
		#nodeAction.attributes.set("call", self.portal.getLink("invite?mode=dialog"))
					
		nodeAction = nodeActions.nodes.add("action")
		nodeAction.attributes.set("name", "remove")
		#nodeAction.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("removeportal?portal=" + self.portal.getFullPovID()))
		nodeAction.attributes.set("prefix", "portal.pages.info")
		nodeAction.attributes.set("href", self.getEventCommand("onPortalRemove",self.portal.getFullPovID()))					
		nodeAction.attributes.set("confirm", "true")
		
		
		
		# Details
		
		node.attributes.set("id", self.portal.portalID.string)
		node.attributes.set("pov", self.portal.povID.string)
		node.attributes.set("name", self.portal.name)
		if self.portal.optionsShared.portalDescription != "":
			node.attributes.set("description", self.portal.optionsShared.portalDescription)
			
		#node.setAttributeString("machine_id", osiris.Engine.instance().getMachineID())		
		#node.setAttributeString("align_hash", self.portal.options.getAlignHash().getString())
		#node.setAttributeString("acceptable_hash", self.portal.optionsShared.getAcceptableHash())
		
		
		
		nodeIsisEndpoints = node.nodes.add("isis")
		isisEndpoints = self.portal.options.isisEndpoints;		
		for isisEndpointID in isisEndpoints.keys():
			isisEndpoint = isisEndpoints[isisEndpointID];
			nodeIsis = nodeIsisEndpoints.nodes.add("isis")
			
			nodeIsis.attributes.set("name",isisEndpoint.getName());
			nodeIsis.attributes.set("url",isisEndpoint.url.toString());
			nodeIsis.attributes.set("enabled",isisEndpoint.enabled);
			
		node.attributes.set("exchangeEnabled", "true" if self.portal.options.exchangeEnabled else "false")
						
		if(osiris.Options.instance().getWebMcpPassword() != ""):
			node.attributes.set("publicEnabled", "true" if self.portal.options.publicEnabled else "false")
			node.attributes.set("default", "true" if (osiris.Options.instance().getWebDefaultPortal() == self.portal.getFullPovID().toUTF16()) else "false")				
		
		node.attributes.set("lastObjectDate", self.portal.options.lastObjectDate.toXML())
		node.attributes.set("lastExchangeDate", self.portal.options.lastExchangeDate.toXML())
		node.attributes.set("lastDownloadedObjectDate", self.portal.options.lastDownloadedObjectDate.toXML())
		node.attributes.set("lastUploadedObjectDate", self.portal.options.lastUploadedObjectDate.toXML())		
		node.attributes.set("lastOptimizationDate", self.portal.options.lastOptimizationDate.toXML())
		node.attributes.set("lastSyncDate", self.portal.options.lastSyncDate.toXML())		
		
		node.setAttributeUint32("nodesSameAlign", self.portal.nodesSameAlign)
		node.setAttributeUint32("nodesSamePortalPov", self.portal.nodesSamePortalPov)
		
		# Invite
		self.root.setAttributeString("subscribe_href",self.portal.generateInviteLink(False));
		self.root.setAttributeString("isis_subscribe_href",self.portal.generateInviteLink(True));
		self.root.setAttributeString("export_href",self.portal.generateExportLink());		

		# Settings
		self.saveCommand = osiris.IdeButton(self.getText("common.actions.save"))
		self.saveCommand.id = "save"
		self.saveCommand.iconHref = self.skin.getImageUrl("icons/16x16/save.png")
		osiris.events.connect(self.saveCommand.eventClick, self.onSave)
		template.addChildParam(self.saveCommand)

		self.chkExchange = osiris.IdePickerBool()
		self.chkExchange.id = "exchange"
		template.addChildParam(self.chkExchange)

		self.txtPassword = osiris.HtmlTextBox()
		self.txtPassword.id = "password"
		self.txtPassword.size = 20
		template.addChildParam(self.txtPassword)

		self.cboSync = osiris.HtmlComboBox()
		self.cboSync.id = "sync"
		template.addChildParam(self.cboSync)
		self.cboSync.addOption(self.getText("portal.pages.info.settings.sync.none"), "")	
		subscribedPortals = osiris.PortalsSystem.instance().portals
		for portal2 in subscribedPortals:
			#osiris.LogManager.instance().log(portal2.getPovName() + "," + self.portal.getPortalID().getString() + "-" + portal2.getPortalID().getString())			
			if(self.portal.getPortalID().getString() == portal2.getPortalID().getString()):				
				if(self.portal.getPovID().getString() != portal2.getPovID().getString()):
					self.cboSync.addOption(portal2.getPovName(), portal2.getFullPovID())
		

		if(self.postBack == False):
			self.chkExchange.check = self.portal.options.getExchangeEnabled()
			self.txtPassword.value = self.portal.options.getPassword()
			self.cboSync.value = self.portal.getSync()
		# Stats
		
		#self.query = osiris.IdeTableQuery()
		#self.query.id = "stats_table"
		
		#sql = "select "
		#sql += "(select count(*) from os_entries) as objects_total, "
		#sql += "(select count(*) from os_snapshot_objects) as entities_total, "
		#sql += "(select count(*) from os_snapshot_objects where visible=0) as entities_invisible, "
		#sql += "(select count(*) from os_entries where rank<0) as objects_trash, "
		#sql += "(select min(stability_date) from os_snapshot_objects) as min_stab, "
		#sql += "(select max(stability_date) from os_snapshot_objects) as max_stab "
		
		#self.query.setSql(sql)
		#self.query.setColumnType(0,osiris.IdeTableQuery.ctString)
		#self.query.setColumnType(1,osiris.IdeTableQuery.ctString)
		#self.query.setColumnType(2,osiris.IdeTableQuery.ctString)
		#self.query.setColumnType(3,osiris.IdeTableQuery.ctString)
		#self.query.setColumnType(4,osiris.IdeTableQuery.ctShortDateTime)		
		#self.query.setColumnType(5,osiris.IdeTableQuery.ctShortDateTime)		
		#template.addChildParam(self.query)	
			
	def onPortalRemove(self, args):		
		#self.showMessage(args[0])					
		
		#portal = osiris.PortalsSystem.instance().getPortalByFullPov(args[0]);
		#if portal:
		osiris.PortalsSystem.instance().removePortal(self.portal.portalID, self.portal.povID)		
		
		self.redirect(osiris.PortalsSystem.instance().getMainLink("home"))

	def onSave(self, args):

		self.portal.options.setExchangeEnabled(self.chkExchange.check)
		self.portal.options.setPassword(self.txtPassword.value)
		self.portal.setSync(self.cboSync.value)

		self.showMessage(self.getText("common.save_success"))
		
	def onPreRender(self):
		osiris.IPortalPage.onPreRender(self)		

		
