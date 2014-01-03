import os
import osiris

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		
		if session.request.getUrlParam("mode") == "dialog":
			self.ajax = True
		
	def isMcpModeRequired(self):
		return False
				
	def getPageName(self):
		return "main.pages.isis"

	def onInit(self):
		osiris.IMainPage.onInit(self)

		document = osiris.XMLDocument()
		self.root = document.create("isis")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "isis.xsl"))
		template.document = document
		
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)

		self.act = self.session.request.getUrlParam("act")	
		if(self.act == ""):
			self.act = "home"

		self.root.setAttributeString("action", self.act)

		if(self.act == "home"):

			#osiris.events.connect(self.events.get("onAdd"), self.onAdd)
			#osiris.events.connect(self.events.get("onEdit"), self.onEdit)
			osiris.events.connect(self.events.get("onRemove"), self.onRemove)
		elif( (self.act == "add") or (self.act == "edit") ):
			
			self.saveCommand = osiris.IdeButton(self.getText("common.actions.save"))
			self.saveCommand.id = "save"
			self.saveCommand.iconHref = self.skin.getImageUrl("icons/16x16/save.png")
			osiris.events.connect(self.saveCommand.eventClick, self.onSave)
			template.addChildParam(self.saveCommand)

			self.cboPortal = osiris.HtmlComboBox()
			self.cboPortal.id = "portal"
			self.cboPortal.size = 40
			template.addChildParam(self.cboPortal)

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
		
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)		
		
		
		if(self.act == "home"):
		
			subscribedPortals = osiris.PortalsSystem.instance().portals
			if len(subscribedPortals) == 0:
				return
			
			nodePortals = self.root.nodes.add("portals")
			for portal in subscribedPortals:
					
				nodePortal = nodePortals.nodes.add("portal")

				nodePortal.attributes.set("id", portal.portalID.string)
				nodePortal.attributes.set("pov", portal.povID.string)
				nodePortal.attributes.set("name", portal.name)
				if portal.optionsShared.portalDescription != "":
					nodePortal.attributes.set("description", portal.optionsShared.portalDescription)

				isisEndpoints = portal.options.isisEndpoints;		
				for isisEndpointID in isisEndpoints.keys():
					isisEndpoint = isisEndpoints[isisEndpointID];
					nodeIsis = nodePortal.nodes.add("isis")
			
					nodeIsis.attributes.set("id", portal.portalID.string)
					nodeIsis.attributes.set("pov", portal.povID.string)
					nodeIsis.attributes.set("portal_name",portal.name);
					nodeIsis.attributes.set("portal_description",portal.optionsShared.portalDescription);
					nodeIsis.attributes.set("name",isisEndpoint.getName());
					nodeIsis.attributes.set("url",isisEndpoint.url.toString());
					nodeIsis.attributes.set("enabled",isisEndpoint.enabled);			
					nodeIsis.attributes.set("last_event",isisEndpoint.getLastEvent());			

					nodeIsis.attributes.set("edit_href", "/main/isis?act=edit&id=" + str(isisEndpointID))
					nodeIsis.attributes.set("remove_href", self.getEventCommand("onRemove", str(isisEndpointID)))

			self.root.attributes.set("add_href", "/main/isis?act=add")
		

	def onPathway(self):
		self.getPathway().add(self.getText("main.pages.isis.title"), osiris.PortalsSystem.instance().getMainLink("isis"));

		if(self.act != "home"):
			self.getPathway().add(self.getText("main.pages.isis." + self.act + ".title"),"")

	def onSave(self, args):		
		isisID = args[0]

	def onRemove(self, args):		

		isisID = args[0]
		osiris.IdeAccountsManager.instance().remove(args[0])		

		