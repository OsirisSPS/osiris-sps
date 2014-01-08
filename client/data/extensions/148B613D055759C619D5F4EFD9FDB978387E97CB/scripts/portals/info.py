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

		self.act = self.session.request.getUrlParam("act")	
		if(self.act == ""):
			self.act = "home"

		if(self.act == "home"):
				
			if(self.sessionAccount.isLogged()):
				#self.showMessage("logged")
				self.isUserOfPov = (self.sessionAccount.userID.getString() == self.portal.povID.getString());
				self.root.setAttributeBool("isUserOfPov", self.isUserOfPov);		
			
				self.povOfUser = osiris.PortalsSystem.instance().getPortal(self.portal.portalID, self.sessionAccount.userID.getString())			
				self.root.setAttributeBool("povOfUserExists", (self.povOfUser != None));		
		
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
			osiris.events.connect(self.events.get("onIsisRemove"), self.onIsisRemove)
			for isisEndpointID in isisEndpoints.keys():
				isisEndpoint = isisEndpoints[isisEndpointID];
				nodeIsis = nodeIsisEndpoints.nodes.add("isis")
			
				nodeIsis.attributes.set("name",isisEndpoint.getName());
				nodeIsis.attributes.set("url",isisEndpoint.url.toString());
				nodeIsis.attributes.set("enabled",isisEndpoint.enabled);
				nodeIsis.attributes.set("last_event",isisEndpoint.getLastEvent());	

				nodeIsis.attributes.set("edit_href", self.portal.getLink("info") + "?act=isis_edit&id=" + str(isisEndpointID))
				nodeIsis.attributes.set("remove_href", self.getEventCommand("onIsisRemove", str(isisEndpointID)))
			nodeIsisEndpoints.attributes.set("add_href", self.portal.getLink("info") + "?act=isis_add")
			
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
		elif( (self.act == "isis_add") or (self.act == "isis_edit") ):
			self.saveCommand = osiris.IdeButton(self.getText("common.actions.save"))
			self.saveCommand.id = "save"
			self.saveCommand.iconHref = self.skin.getImageUrl("icons/16x16/save.png")
			osiris.events.connect(self.saveCommand.eventClick, self.onIsisSave)
			template.addChildParam(self.saveCommand)

			self.txtName = osiris.HtmlTextBox()
			self.txtName.id = "name"
			self.txtName.size = 40
			template.addChildParam(self.txtName)

			self.txtUrl = osiris.HtmlTextBox()
			self.txtUrl.id = "url"
			self.txtUrl.size = 40
			template.addChildParam(self.txtUrl)
			
			self.txtPassword = osiris.HtmlTextBox()
			self.txtPassword.id = "password"
			self.txtPassword.size = 40
			template.addChildParam(self.txtPassword)

			self.chkEnabled = osiris.IdePickerBool()
			self.chkEnabled.id = "enabled"
			template.addChildParam(self.chkEnabled)
			
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

	def onIsisSave(self, args):

		self.showMessage("todo")

	def onIsisRemove(self, args):

		self.showMessage("todo")

	def onPathway(self):
		self.getPathway().add(self.getText("portal.pages.info.title"), self.portal.getLink("info"));

		if(self.act != "home"):
			self.getPathway().add(self.getText("portal.pages.info." + self.act + ".title"),"")
		
	def onPreRender(self):
		osiris.IPortalPage.onPreRender(self)		

		
