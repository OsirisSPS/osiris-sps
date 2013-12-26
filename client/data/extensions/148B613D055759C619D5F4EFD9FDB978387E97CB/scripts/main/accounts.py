import os
import osiris
import osiris.events

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		self.ajax = False
				
	def getPageName(self):
		return "main.pages.accounts"
		
	def onInit(self):
		osiris.IMainPage.onInit(self)	

		document = osiris.XMLDocument()
		self.root = document.create("accounts")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "accounts.xsl"))
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

			osiris.events.connect(self.events.get("onExport"), self.onExport)
			osiris.events.connect(self.events.get("onRemove"), self.onRemove)

			nodeActions = self.root.nodes.add("actions")
		
			actionRegisterParams = { }
			actionRegisterParams["act"] = "register"			
			actionRegister = nodeActions.nodes.add("action")
			actionRegister.attributes.set("name", "register")
			actionRegister.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("accounts", actionRegisterParams))

			actionImportParams = { }
			actionImportParams["act"] = "import"			
			actionImport = nodeActions.nodes.add("action")
			actionImport.attributes.set("name", "import")
			actionImport.attributes.set("href", osiris.PortalsSystem.instance().getMainLink("accounts", actionImportParams))

		if(self.act == "login"):
			self.userid = self.session.request.getUrlParam("id")
			if( (self.userid == None) or (self.userid == "") ):
				self.redirect("accounts")
				return		

			account = osiris.IdeAccountsManager.instance().getByID(self.userid)
			if(account == None):
				return

			savedPassword = account.getRealPassword()
			if(savedPassword != ""):
				if(self.loginAccountWithID(self.userid, savedPassword)):
					self.redirect("home")

			self.txtPassword = osiris.HtmlTextBox()
			self.txtPassword.id = "password"
			self.txtPassword.size = 20
			self.txtPassword.attributes.set("data-os-otype","password")
			self.txtPassword.attributes.set("data-os-login","page_name")
			self.txtPassword.password = True
			template.addChildParam(self.txtPassword)	

			self.chkSavePassword = osiris.IdePickerBool()
			self.chkSavePassword.id = "savePassword"
			template.addChildParam(self.chkSavePassword)	

			self.cmdLogin = osiris.IdeButton(self.getText("main.pages.accounts.enter"))
			self.cmdLogin.id = "login"
			self.cmdLogin.isDefault = True			
			osiris.events.connect(self.cmdLogin.eventClick, self.onLogin)
			template.addChildParam(self.cmdLogin)	
		
		if(self.act == "register"):				
			self.txtName = osiris.HtmlTextBox()			
			self.txtName.id = "name"
			self.txtName.size = 20
			template.addChildParam(self.txtName)	
			
			self.txtPassword = osiris.HtmlTextBox()
			self.txtPassword.id = "password"
			self.txtPassword.size = 20
			self.txtPassword.attributes.set("data-os-otype","password")
			self.txtPassword.attributes.set("data-os-login","page_name")
			self.txtPassword.password = True
			template.addChildParam(self.txtPassword)	
	
			self.txtPasswordChecker = osiris.HtmlTextBox()
			self.txtPasswordChecker.id = "passwordChecker"
			self.txtPasswordChecker.size = 20
			self.txtPasswordChecker.password = True
			template.addChildParam(self.txtPasswordChecker)	
			
			#self.secretQuestion = osiris.HtmlTextBox()
			#self.secretQuestion.id = "secretQuestion"	
			#self.secretQuestion.css = "os_input_full"
	
			#self.secretResponse = osiris.HtmlTextBox()
			#self.secretResponse.id = "secretResponse"
			#self.secretResponse.css = "os_input_full"
			#self.secretResponse.password = True
	
			#self.secretResponseChecker = osiris.HtmlTextBox()
			#self.secretResponseChecker.id = "secretResponseChecker"
			#self.secretResponseChecker.password = True
			
			self.chkSavePassword = osiris.IdePickerBool()
			self.chkSavePassword.id = "savePassword"
			template.addChildParam(self.chkSavePassword)	
			
			self.cmdRegister = osiris.IdeButton(self.getText("main.pages.accounts.register"))
			self.cmdRegister.id = "register"
			self.cmdRegister.isDefault = True
			osiris.events.connect(self.cmdRegister.eventClick, self.onRegister)						
			template.addChildParam(self.cmdRegister)
			
		if(self.act == "import"):								

			self.txtFile = osiris.HtmlFileBrowser()
			self.txtFile.id = "file"
			self.txtFile.size = 20
			template.addChildParam(self.txtFile)	

			self.cmdImport = osiris.IdeButton(self.getText("main.pages.accounts.import"))
			self.cmdImport.id = "import"
			self.cmdImport.isDefault = True
			osiris.events.connect(self.cmdImport.eventClick, self.onImport)			
			template.addChildParam(self.cmdImport)

	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)
				
		
		
		if(self.act == "home"):		
			accounts = osiris.IdeAccountsManager.instance().accounts
		
			nodeAccounts = self.root.nodes.add("accounts")
		
			for account in accounts:
				nodeAccount = nodeAccounts.nodes.add("account")	
				
				nodeAccount.setAttributeString("id", account.id)
				nodeAccount.setAttributeString("name", account.name)
				nodeAccount.setAttributeDateTime("last_access", account.lastAccessDate)
				nodeAccount.setAttributeBool("saved_password", (account.getRealPassword() != ""))
				nodeAccount.setAttributeBool("logged", osiris.IdeSystem.instance().isAccountLogged(account.id))		
				
				nodeActions = nodeAccount.nodes.add("actions")
				
				nodeActionLogin = nodeActions.nodes.add("action")
				nodeActionLogin.attributes.set("name", "enter")
				nodeActionLogin.attributes.set("href", "/main/accounts?act=login&id=" + account.id)
				
				nodeActionRemove = nodeActions.nodes.add("action")
				nodeActionRemove.attributes.set("name", "remove")
				#nodeActionRemove.attributes.set("href", "/main/accounts?act=remove&id=" + account.id)
				nodeActionRemove.attributes.set("href", self.getEventCommand("onRemove", account.id))
				nodeActionRemove.attributes.set("confirm", "true")
				
				nodeActionExport = nodeActions.nodes.add("action")
				nodeActionExport.attributes.set("name", "export")
				#nodeActionExport.attributes.set("href", "/main/accounts?act=export&id=" + account.id)				
				nodeActionExport.attributes.set("href", self.getEventCommand("onExport", account.id))
			

	def onPathway(self):
		self.getPathway().add(self.getText("main.pages.accounts.title"), osiris.PortalsSystem.instance().getMainLink("accounts"));

		if(self.act != "home"):
			self.getPathway().add(self.getText("main.pages.accounts." + self.act + ".title"),"")
					
	def onImport(self, args):				

		if(osiris.IdeAccountsManager.instance().importXml(self.txtFile.getFileBuffer())):
			self.redirect(osiris.PortalsSystem.instance().getMainLink("accounts"))
		else:
			self.showError(self.getText("main.pages.accounts.error.cannot_import"))			


	def onExport(self, args):		
		accountID = args[0]		
		account = osiris.IdeAccountsManager.instance().getByID(args[0])
		if(account != None):
			document = account.account.exportXML()
			buffer = osiris.Buffer()
			if(document.writeBuffer(buffer) == True):
				name = account.name
				#mime = osiris.MimeManager.instance().getMimeType("xml")
				mime = "text/xml"					
				self.session.transmitBuffer(buffer, name, mime)

	def onRemove(self, args):		
		accountID = args[0]
		osiris.IdeAccountsManager.instance().remove(args[0])		

	def onLogin(self, args):		
		password = self.txtPassword.value
		savePassword = self.chkSavePassword.check	
		
		if(self.loginAccountWithID(self.userid, password, savePassword)):
			self.redirect("home")
		else:
			self.showError(self.getText('main.pages.accounts.error.password_mismatch'))

	def onRegister(self, args):		

		name = self.txtName.value
		
		password = self.txtPassword.value
		passwordChecker = self.txtPasswordChecker.value
		if password != passwordChecker:
			self.showError(self.getText("main.pages.accounts.error.password_mismatch"))
			return
		
		savePassword = self.chkSavePassword.check	
		account = osiris.IdeAccountsManager.instance().createAccount(name, password, savePassword)
		if account == None:
			self.showError(self.getText("main.pages.accounts.error.cannot_create"))			
		else:
			self.loginAccountWithID(account.id, password)
			self.redirect(osiris.PortalsSystem.instance().getMainLink("home"))
			
	