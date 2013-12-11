import osiris
import os
import osiris.events

class Page(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)
		self.ajax = (session.request.getUrlParam("act") != "")
				
	def getPageName(self):
		return "portal.pages.acp"	
		
	def onLoad(self):	
		osiris.IPortalPage.onLoad(self)
		
		self.act = self.session.request.getUrlParam("act")
		if(self.act == ""):
			self.act = "home"		
				
		self.document = osiris.XMLDocument()
		self.root = self.document.create(self.act)
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "acp.xsl"))
		template.document = self.document
		
		#self.document.root.setAttributeString("page_url",self.request.rawUrl);
		
		
		osiris.LogManager.instance().log(self.sessionAccount.userID.getString())
		osiris.LogManager.instance().log(self.portal.povID.getString())
		
		if(self.sessionAccount.userID.getString() == self.portal.povID.getString()):
			osiris.LogManager.instance().log("kkk")
		
		isGuest = self.sessionAccount.isPortalGuest(self.database)
		self.root.setAttributeBool("isGuest", isGuest);
				
		if(isGuest == False):
			# Hack, to fix: senza le getString le rileva diversi sempre...
			isUserOfPov = (isGuest == False and self.sessionAccount.userID.getString() == self.portal.povID.getString());
			self.root.setAttributeBool("isUserOfPov", isUserOfPov);		
			
			povOfUser = osiris.PortalsSystem.instance().getPortal(self.portal.portalID, self.sessionAccount.userID.getString())			
			self.root.setAttributeBool("povOfUserExists", (povOfUser != None));		
			
			povOfUserHref = ""
			if(povOfUser != None):
				povOfUserHref = povOfUser.getLink("view")
			else:
				params  = { }
				params["mode"] = "fork"
				params["name"] = self.portal.optionsShared.portalName
				params["id"] = self.portal.portalID.toWide()
				povOfUserHref = osiris.PortalsSystem.instance().getMainLink("subscribe", params)
				
			self.root.setAttributeString("povOfUserHref", povOfUserHref);		
				
		
		
		
		
		if(self.act == "home"):	
		
			self.saveCommand = osiris.IdeButton(self.getText("common.actions.save"))
			self.saveCommand.id = "saveCommand"
			osiris.events.connect(self.saveCommand.eventClick, self.onSave)
			template.addChildParam(self.saveCommand)
      			
			self.portalName = osiris.HtmlTextBox()
			self.portalName.id = "portalName"		
			self.portalName.size = 80			
			self.portalName.getAttributes().set("data-os-otype","string")
			self.portalName.getAttributes().set("data-os-minchars","5")
			self.portalName.getAttributes().set("data-os-submit","page-saveCommand")
			template.addChildParam(self.portalName)
						
			self.portalDescription = osiris.HtmlTextBox()
			self.portalDescription.id = "portalDescription"		
			self.portalDescription.css = "os_input_full"
			template.addChildParam(self.portalDescription)
			
			self.authorsReputationThreshold = osiris.HtmlComboBox()
			self.authorsReputationThreshold.id = "authorsReputationThreshold"
			template.addChildParam(self.authorsReputationThreshold)
			self.authorsReputationThreshold.addOption(self.getText("reputation.threshold.all"), "0")			
			self.authorsReputationThreshold.addOption(self.getText("reputation.threshold.negative"), "1")		
			self.authorsReputationThreshold.addOption(self.getText("reputation.threshold.not_negative"), "2")		
			self.authorsReputationThreshold.addOption(self.getText("reputation.threshold.positive"), "3")					
			
			self.editorsReputationThreshold = osiris.HtmlComboBox()
			self.editorsReputationThreshold.id = "editorsReputationThreshold"
			template.addChildParam(self.editorsReputationThreshold)
			self.editorsReputationThreshold.addOption(self.getText("reputation.threshold.all"), "0")			
			self.editorsReputationThreshold.addOption(self.getText("reputation.threshold.negative"), "1")		
			self.editorsReputationThreshold.addOption(self.getText("reputation.threshold.not_negative"), "2")		
			self.editorsReputationThreshold.addOption(self.getText("reputation.threshold.positive"), "3")					
			
			
			# Layout
			
			self.layoutComponent = osiris.IdePickerComponent()
			self.layoutComponent.id = "layoutComponent"		
			self.layoutComponent.css = "os_input_full"
			template.addChildParam(self.layoutComponent)
			
			#self.registerTerms = osiris.IdeOMLEditor()
			#self.registerTerms.id = "registerTerms"		
			#self.registerTerms.css = "os_input_full"
			#template.addChildParam(self.registerTerms)
			
			self.layoutTileImage = osiris.IdePickerObject()
			self.layoutTileImage.id = "layoutTileImage"		
			template.addChildParam(self.layoutTileImage)
			
			self.layoutTileColorBackground = osiris.IdePickerColor()
			self.layoutTileColorBackground.id = "layoutTileColorBackground"		
			template.addChildParam(self.layoutTileColorBackground)
			
			self.layoutTileColorForeground = osiris.IdePickerColor()
			self.layoutTileColorForeground.id = "layoutTileColorForeground"		
			template.addChildParam(self.layoutTileColorForeground)
			
			self.layoutSkinParams = osiris.IdeOMLEditor()
			self.layoutSkinParams.id = "layoutSkinParams"		
			self.layoutSkinParams.css = "os_input_full"
			template.addChildParam(self.layoutSkinParams)		
			
			self.layoutCss = osiris.IdeOMLEditor()
			self.layoutCss.id = "layoutCss"		
			self.layoutCss.css = "os_input_full"
			template.addChildParam(self.layoutCss)
			
			self.layoutHeader = osiris.IdePickerBool()
			self.layoutHeader.id = "layoutHeader"
			template.addChildParam(self.layoutHeader)
			
			# Rules
			
			self.objectsMaxSize = osiris.IdePickerNumber()
			self.objectsMaxSize.id = "objectsMaxSize"
			template.addChildParam(self.objectsMaxSize)
			
			self.badWords = osiris.HtmlTextBox()
			self.badWords.id = "badWords"		
			self.badWords.css = "os_input_full"
			template.addChildParam(self.badWords)
			
			# Rules ex
			
			self.allowObjectInFuture = osiris.IdePickerBool()
			self.allowObjectInFuture.id = "allowObjectInFuture"
			template.addChildParam(self.allowObjectInFuture)
			
			self.allowObjectUnsigned = osiris.IdePickerBool()
			self.allowObjectUnsigned.id = "allowObjectUnsigned"
			template.addChildParam(self.allowObjectUnsigned)
			
			self.objectsPhysicalRemove = osiris.IdePickerBool()
			self.objectsPhysicalRemove.id = "objectsPhysicalRemove"
			template.addChildParam(self.objectsPhysicalRemove)
			
			self.objectsPhysicalRemoveDays = osiris.IdePickerNumber()
			self.objectsPhysicalRemoveDays.id = "objectsPhysicalRemoveDays"
			template.addChildParam(self.objectsPhysicalRemoveDays)
			
			
			if(self.postBack == False):
			
				# Main
				self.portalName.value = self.portal.optionsShared.portalName
				self.portalDescription.value = self.portal.optionsShared.portalDescription
				self.authorsReputationThreshold.value = self.portal.optionsShared.authorsReputationThreshold;
				self.editorsReputationThreshold.value = self.portal.optionsShared.editorsReputationThreshold;				
				
				# Layout
				
				self.layoutComponent.value = self.portal.optionsShared.layoutComponent								
				#self.registerTerms.value = self.portal.optionsShared.registerTerms
				self.layoutTileImage.value = self.portal.optionsShared.layoutTileImage
				self.layoutTileColorBackground.value = self.portal.optionsShared.layoutTileColorBackground
				self.layoutTileColorForeground.value = self.portal.optionsShared.layoutTileColorForeground
				self.layoutSkinParams.value = self.portal.optionsShared.layoutSkinParams
				self.layoutCss.value = self.portal.optionsShared.layoutCss
				self.layoutHeader.check = self.portal.optionsShared.layoutHeader
						
				# Rules
				
				self.objectsMaxSize.value = self.portal.optionsShared.objectsMaxSize				
				self.badWords.value = self.portal.optionsShared.badWords
				
				# Rules Ex
				
				self.allowObjectInFuture.check = self.portal.optionsShared.allowObjectInFuture
				self.allowObjectUnsigned.check = self.portal.optionsShared.allowObjectUnsigned								
				self.objectsPhysicalRemove.check = self.portal.optionsShared.objectsPhysicalRemove
				self.objectsPhysicalRemoveDays.value = self.portal.optionsShared.objectsPhysicalRemoveDays
				
						
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)		
	
	def onPreRender(self):
		osiris.IPortalPage.onPreRender(self)
		
		
			
	def onSave(self, args):
	
		# Main
		
		self.portal.optionsShared.portalName = self.portalName.value
		self.portal.optionsShared.portalDescription = self.portalDescription.value
		self.portal.optionsShared.authorsReputationThreshold = osiris.ObjectsReputationThreshold(int(self.authorsReputationThreshold.value))
		self.portal.optionsShared.editorsReputationThreshold = osiris.ObjectsReputationThreshold(int(self.editorsReputationThreshold.value))		
		
		# Layout
		
		self.portal.optionsShared.layoutComponent = self.layoutComponent.value
		#self.portal.optionsShared.registerTerms = self.registerTerms.value
		self.portal.optionsShared.layoutTileImage = self.layoutTileImage.value
		self.portal.optionsShared.layoutTileColorBackground = self.layoutTileColorBackground.value
		self.portal.optionsShared.layoutTileColorForeground = self.layoutTileColorForeground.value
		self.portal.optionsShared.layoutSkinParams = self.layoutSkinParams.value
		self.portal.optionsShared.layoutCss = self.layoutCss.value
		self.portal.optionsShared.layoutHeader = self.layoutHeader.check
		
		# Rules
				
		self.portal.optionsShared.objectsMaxSize = self.objectsMaxSize.value
		self.portal.optionsShared.badWords = self.badWords.value
		
		# Rules Ex
		
		self.portal.optionsShared.allowObjectInFuture = self.allowObjectInFuture.check
		self.portal.optionsShared.allowObjectUnsigned = self.allowObjectUnsigned.check
		self.portal.optionsShared.objectsPhysicalRemove = self.objectsPhysicalRemove.check
		self.portal.optionsShared.objectsPhysicalRemoveDays = self.objectsPhysicalRemoveDays.value
		
		if(self.portal.saveUser(self.database, self.sessionAccount) == True):		
			self.showMessage("Saved settings.")
		else:
			self.showError("Settings invalid.")
			
		return
	
def main(args):
	page = Page(args[0])
	page.transmit()
