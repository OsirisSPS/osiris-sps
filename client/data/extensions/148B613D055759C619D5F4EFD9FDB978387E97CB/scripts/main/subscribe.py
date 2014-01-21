import os
import osiris
import osiris.events

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
				
		self.driverOptionsControls = {}
				
	def getPageName(self):
		return "main.pages.subscribe." + self.request.getUrlParam("mode")
		
	def onInit(self):
		osiris.IMainPage.onInit(self)		

		document = osiris.XMLDocument()
		root = document.create("subscribe")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "subscribe.xsl"))
		template.document = document
		
		self.mode = self.request.getUrlParam("mode")
		document.root.setAttributeString("mode", self.mode)

		self.valid = True

		self.accountRequired = False

		if(self.mode != 'subscribe'):
			self.accountRequired = True
		
		if(self.accountRequired):
			if(self.sessionAccount.isLogged() == False):
				self.showError(self.getText("errors.account_required"))
				self.valid = False
				return

		if( (self.mode != 'subscribe') and (self.mode != 'create') and (self.mode != 'self') and (self.mode != 'fork') ):
			self.showError(self.getText("errors.invalid_params"))
			self.valid = False
			return
		
		self.portalName = osiris.HtmlTextBox()
		self.portalName.id = "name"		
		self.portalName.size = 40;
		self.portalName.getAttributes().set("data-os-otype","string")
		self.portalName.getAttributes().set("data-os-minchars","5")
		self.portalName.getAttributes().set("data-os-submit","page-subscribe")
		template.addChildParam(self.portalName)
			
		
		self.portalPassword = osiris.HtmlTextBox()
		self.portalPassword.id = "password"
		self.portalPassword.size = 40;
		self.portalPassword.password = True
		self.portalPassword.attributes.set("data-os-otype","password")
		self.portalPassword.attributes.set("data-os-login","page-name")	
		template.addChildParam(self.portalPassword)
		
		self.portalEnableExchange = osiris.IdePickerBool()
		self.portalEnableExchange.id = "enableExchange"		
		template.addChildParam(self.portalEnableExchange)

		self.portalId = osiris.HtmlTextBox()
		self.portalId.id = "id"
		self.portalId.size = 40
		self.portalId.getAttributes().set("data-os-otype","string")
		self.portalId.getAttributes().set("data-os-minchars","40")
		template.addChildParam(self.portalId)

		self.portalPov = osiris.HtmlTextBox()
		self.portalPov.id = "pov"
		self.portalPov.size = 40
		self.portalPov.getAttributes().set("data-os-otype","string")
		self.portalPov.getAttributes().set("data-os-minchars","40")
		template.addChildParam(self.portalPov)

		self.databaseDriver = osiris.HtmlComboBox()
		self.databaseDriver.id = "databaseDriver"
		self.databaseDriver.autoPostBack = True
		template.addChildParam(self.databaseDriver)
		drivers = osiris.DatabasesSystem.instance().drivers
		for driver in drivers:
			self.databaseDriver.addOption(driver.description, driver.name)		
		self.databaseDriver.value = osiris.DatabasesSystem.instance().defaultDriver.name


		subscribeLabel = "create";
		if(self.mode == "subscribe"):
			subscribeLabel = "subscribe";

		self.cmdSubscribe = osiris.IdeButton(self.getText("main.pages.subscribe.action." + subscribeLabel))
		self.cmdSubscribe.id = "subscribe"
		self.cmdSubscribe.iconHref = self.skin.getImageUrl("icons/16x16/save.png")
		self.cmdSubscribe.isDefault = True
		osiris.events.connect(self.cmdSubscribe.eventClick, self.onSubscribe)
		template.addChildParam(self.cmdSubscribe)

		self.getArea(osiris.pageAreaContent).controls.add(template)



		if(self.mode == 'self'):
			self.portalName.value = self.sessionAccount.account.name
			self.portalId.value = self.sessionAccount.userID.toAscii()
			self.portalPov.value = self.sessionAccount.userID.toAscii()
		elif(self.mode == 'fork'):
			self.portalName.value = self.getText("main.pages.subscribe.fork_of") + " " + self.request.getUrlParam("name")
			self.portalId.value = self.request.getUrlParam("id")
			self.portalPov.value = self.sessionAccount.userID.toAscii()
		elif(self.mode == 'create'):
			self.portalPov.value = self.sessionAccount.userID.toAscii()



		
		
		
	def onLoad(self):
		osiris.IMainPage.onLoad(self)
		
		if(self.valid == False):
			return
	
		if self.postBack == False:
			if(self.mode == "subscribe"):
				self.portalEnableExchange.check = True
			else:
				self.portalEnableExchange.check = False

		# Database Driver Options

		#self.driver = osiris.DatabasesSystem.instance().getDriver(str(self.databaseDriver.value))
		#if self.driver != None:
		#	self.driverOptions = self.driver.createOptions()
		#	for option in self.driverOptions.params:
		#		rowOption = table.addRow()
		#		
		#		optionLeft = rowOption.addCell()
		#		optionLeft.css = "os_label"
		#		optionLeft.controls.add(osiris.HtmlLiteral(option.key))
		#
		#		optionRight = rowOption.addCell()
		#		optionRight.css = "os_value"
		#		
		#		optionControl = self.getDriverOptionControl(self.driver.name, option)
		#		optionControl.value = option.value
		#		optionRight.controls.add(optionControl)		
		
				
	def onSubscribe(self, args):
		
		if(self.valid == False):
			return
			
		#for option in self.driverOptions.params:
		#	value = self.getDriverOptionControl(self.driver.name, option).value
		#	if value != "":
		#		self.driverOptions.params[option.key] = value
	
		options = osiris.PortalOptions()
		options.name = self.portalName.value
		options.setExchangeEnabled(self.portalEnableExchange.check)
		options.password = self.portalPassword.value
		options.databaseDriver = "sqlite"
		options.databaseOptions = osiris.DatabasesSystem.instance().getDriver("sqlite").createOptions()
		#self.driver = osiris.DatabasesSystem.instance().getDriver(str(self.databaseDriver.value))
		#if self.driver != None:
		#	self.driverOptions = self.driver.createOptions()

		if(self.portalId.value != ""):
			options.portalID = str(self.portalId.value)
		if(self.portalPov.value != ""):
			options.povID = str(self.portalPov.value)
		
		portal = osiris.PortalsSystem.instance().subscribePortal(options)

		if portal != None:
			self.redirect("home")
		else:
			self.showError(self.getText("main.pages.subscribe.error.cannotCreate"))

	def getDriverOptionControl(self, driver, option):		
		optionControl = None
		if option.key in self.driverOptionsControls:
			optionControl = self.driverOptionsControls[option.key]
		else:
			optionControl = osiris.HtmlTextBox()
			optionControl.id = driver + option.key
			self.driverOptionsControls[option.key] = optionControl
		
		return optionControl
