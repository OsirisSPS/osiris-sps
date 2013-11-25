import osiris
import osiris.events

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
				
		self.driverOptionsControls = {}
				
	def getPageName(self):
		return "main.pages.create"
		
	def onInit(self):
		osiris.IMainPage.onInit(self)		
		
		if(self.sessionAccount.isLogged() == False):
			self.showMessage(self.getText("errors.account_required"))
			return
		
		self.portalName = osiris.HtmlTextBox()
		self.portalName.id = "portalName"		
		self.portalName.css = "os_input_full"
		self.portalName.getAttributes().set("data-os-otype","string")
		self.portalName.getAttributes().set("data-os-minchars","5")
			
		
		self.portalPassword = osiris.HtmlTextBox()
		self.portalPassword.id = "portalPassword"
		self.portalPassword.password = True
		self.portalPassword.attributes.set("data-os-otype","password")
		self.portalPassword.attributes.set("data-os-login","page_portalName")
	
		self.portalPassword.css = "os_input_full"
		
		self.portalEnableExchange = osiris.IdePickerBool()
		self.portalEnableExchange.id = "portalEnableExchange"		
			
		self.databaseDriver = osiris.HtmlComboBox()
		self.databaseDriver.id = "databaseDriver"
		self.databaseDriver.autoPostBack = True

		drivers = osiris.DatabasesSystem.instance().drivers
		for driver in drivers:
			self.databaseDriver.addOption(driver.description, driver.name)
		
		self.databaseDriver.value = osiris.DatabasesSystem.instance().defaultDriver.name
		
		block = osiris.IdeBlock(self.getText("main.pages.create.title"))		
		#block = osiris.IdeBlock("main.pages.create")		
		#block.specialPage = True
		self.getArea(osiris.pageAreaContent).controls.add(block)
		
		block.body.controls.add(osiris.IdeHelpBox(self.getText("main.pages.create.help"), "2249AC226A955A36FEB8540ACD3BF856736541B2"))
		
		
		
		
		table = osiris.HtmlTable()
		self.table = table
		table.css = "os_table_properties"
		block.body.controls.add(table)
				
		# Header opzioni portale
		
		portalHeader = table.addRow().addHeader()
		portalHeader.css = "os_subtitle"
		portalHeader.colSpan = 2	
		portalHeader.controls.add(osiris.HtmlText(self.getText("main.pages.create.subtitle")))
		
		# Nome del portale
		
		rowName = table.addRow()

		nameLeft = rowName.addCell()
		nameLeft.css = "os_label"
		nameLeft.controls.add(osiris.HtmlText(self.getText("main.pages.create.name")))
		nameLeft.controls.add(osiris.HtmlText(":"))

		nameRight = rowName.addCell()
		nameRight.css = "os_value"
		nameRight.controls.add(self.portalName)
		
		# Password del portale
				
		rowPassword = table.addRow()
		self.rowPassword = rowPassword

		passwordLeft = rowPassword.addCell()
		passwordLeft.css = "os_label"
		passwordLeft.controls.add(osiris.HtmlText(self.getText("main.pages.create.password")))
		passwordLeft.controls.add(osiris.HtmlText(":"))
			
		passwordRight = rowPassword.addCell()
		passwordRight.css = "os_value"	
		passwordRight.controls.add(self.portalPassword)	
		
		# Enable Exchange
		
		rowEnableExchange = table.addRow()

		enableExchangeLeft = rowEnableExchange.addCell()
		enableExchangeLeft.css = "os_label"
		enableExchangeLeft.controls.add(osiris.HtmlText(self.getText("main.pages.create.enableExchange")))
		enableExchangeLeft.controls.add(osiris.HtmlText(":"))

		enableExchangeRight = rowEnableExchange.addCell()
		enableExchangeRight.css = "os_value"
		enableExchangeRight.controls.add(self.portalEnableExchange)		
		
		# Header opzioni database
		
		databaseHeader = table.addRow().addHeader()
		databaseHeader.css = "os_subtitle"
		databaseHeader.colSpan = 2	
		databaseHeader.controls.add(osiris.HtmlText(self.getText("main.pages.create.database.title")))	
		
		# Driver
		
		rowDriver = table.addRow()
		
		driverLeft = rowDriver.addCell()
		driverLeft.css = "os_label"
		driverLeft.controls.add(osiris.HtmlText(self.getText("main.pages.create.database.driver")))
		driverLeft.controls.add(osiris.HtmlText(":"))
		
		driverRight = rowDriver.addCell()
		driverRight.css = "os_value"
		driverRight.controls.add(self.databaseDriver)	
		
		
		# Comandi
		
		divCommands = osiris.HtmlDiv()
		divCommands.css = "os_commands_center";
		block.body.controls.add(divCommands)
				
		cmdCreate = osiris.IdeButton(self.getText("main.pages.create.action.create"))
		cmdCreate.id = "create"
		cmdCreate.isDefault = True
		osiris.events.connect(cmdCreate.eventClick, self.onCreate)
		divCommands.controls.add(cmdCreate)			
		
		
	def onLoad(self):
		osiris.IMainPage.onLoad(self)
		
		if(self.sessionAccount.isLogged() == False):
			return
	
		if self.postBack == False:
			self.portalEnableExchange.check = True
			
		table = self.table
		
		# Opzioni driver

		self.driver = osiris.DatabasesSystem.instance().getDriver(str(self.databaseDriver.value))
		if self.driver != None:
			self.driverOptions = self.driver.createOptions()
			for option in self.driverOptions.params:
				rowOption = table.addRow()
				
				optionLeft = rowOption.addCell()
				optionLeft.css = "os_label"
				optionLeft.controls.add(osiris.HtmlLiteral(option.key))
		
				optionRight = rowOption.addCell()
				optionRight.css = "os_value"
				
				optionControl = self.getDriverOptionControl(self.driver.name, option)
				optionControl.value = option.value
				optionRight.controls.add(optionControl)			
		
		

	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)
		
		if(self.sessionAccount.isLogged() == False):
			return
			
				
	def onCreate(self, args):
		
		if(self.sessionAccount.isLogged() == False):
			return
			
		for option in self.driverOptions.params:
			value = self.getDriverOptionControl(self.driver.name, option).value
			if value != "":
				self.driverOptions.params[option.key] = value
	
		options = osiris.PortalOptions()
		options.name = self.portalName.value
		options.povID = self.sessionAccount.userID
		options.enableExchange = self.portalEnableExchange.check
		options.password = self.portalPassword.value
		options.databaseDriver = self.databaseDriver.value	
		options.databaseOptions = self.driverOptions

		portal = osiris.PortalsSystem.instance().createPortal(options)
		
		if portal != None:
			self.redirect("home")
		else:
			self.showError(self.getText("main.pages.create.error.cannotCreate"))

	def getDriverOptionControl(self, driver, option):		
		optionControl = None
		if option.key in self.driverOptionsControls:
			optionControl = self.driverOptionsControls[option.key]
		else:
			optionControl = osiris.HtmlTextBox()
			optionControl.id = driver + option.key
			self.driverOptionsControls[option.key] = optionControl
		
		return optionControl
		
def main(args):
	page = Page(args[0])
	page.transmit()
