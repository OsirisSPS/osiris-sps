import os
import osiris
import osiris.events

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		self.ajax = (session.request.getUrlParam("act") != "")
				
	def getPageName(self):
		return "main.pages.account"
		
	def onInit(self):
		osiris.IMainPage.onInit(self)	
		
		self.action = self.session.request.getUrlParam("act")	
		
		if(self.action == "register"):				
			self.name = osiris.HtmlTextBox()
			self.name.css = "os_input_full"
			self.name.id = "name"
			
			self.password = osiris.HtmlTextBox()
			self.password.id = "password"
			self.password.attributes.set("data-os-otype","password")
			self.password.attributes.set("data-os-login","page_name")
			self.password.css = "os_input_full"		
			self.password.password = True
	
			self.passwordChecker = osiris.HtmlTextBox()
			self.passwordChecker.id = "passwordChecker"
			self.passwordChecker.css = "os_input_full"
			self.passwordChecker.password = True
			
			self.secretQuestion = osiris.HtmlTextBox()
			self.secretQuestion.id = "secretQuestion"	
			self.secretQuestion.css = "os_input_full"
	
			self.secretResponse = osiris.HtmlTextBox()
			self.secretResponse.id = "secretResponse"
			self.secretResponse.css = "os_input_full"
			self.secretResponse.password = True
	
			self.secretResponseChecker = osiris.HtmlTextBox()
			self.secretResponseChecker.id = "secretResponseChecker"
			self.secretResponseChecker.password = True
			
			self.savePassword = osiris.HtmlCheckBox()
			self.savePassword.id = "savePassword"
			
			self.cmdRegister = osiris.IdeButton(self.getText("portal.pages.register.actions.register"))
			self.cmdRegister.id = "register"
			self.cmdRegister.isDefault = True
			osiris.events.connect(self.cmdRegister.eventClick, self.onRegister)
					
			self.cmdCancel = osiris.IdeButton(self.getText("common.actions.cancel"))
			self.cmdCancel.id = "cancel"
			osiris.events.connect(self.cmdCancel.eventClick, self.onCancel)		

	def onLoad(self):
		osiris.IMainPage.onLoad(self)
				
		document = osiris.XMLDocument()
		self.root = document.create("account")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "account.xsl"))
		template.document = document
		
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)
		
		
		if(self.action == ""):		
			accounts = osiris.IdeAccountsManager.instance().accounts
		
			nodeAccounts = self.root.nodes.add("accounts")
		
			for account in accounts:
				nodeAccount = nodeAccounts.nodes.add("account")	
				
				nodeAccount.setAttributeString("id", account.id)
				nodeAccount.setAttributeString("name", account.name)
				nodeAccount.setAttributeDateTime("last_access", account.lastAccessDate)
				nodeAccount.setAttributeBool("logged", osiris.IdeSystem.instance().isAccountLogged(account.id))		
				
				nodeActions = nodeAccount.nodes.add("actions")
				
				nodeActionLogin = nodeActions.nodes.add("action")
				nodeActionLogin.attributes.set("name", "enter")
				nodeActionLogin.attributes.set("href", "/main/accounts2?act=login&id=" + account.id)
				
				nodeActionRemove = nodeActions.nodes.add("action")
				nodeActionRemove.attributes.set("name", "remove")
				nodeActionRemove.attributes.set("href", "/main/accounts2?act=remove&id=" + account.id)
				
				nodeActionExport = nodeActions.nodes.add("action")
				nodeActionExport.attributes.set("name", "export")
				nodeActionExport.attributes.set("href", "/main/accounts2?act=export&id=" + account.id)

		
		elif(self.action == "register"):		
		
			template.addChildParam(self.name)	
			template.addChildParam(self.password)	
			template.addChildParam(self.passwordChecker)	
			template.addChildParam(self.savePassword)	
			template.addChildParam(self.cmdRegister)	
			template.addChildParam(self.cmdCancel)					
			
		elif(self.action == "export"):
			account = osiris.IdeAccountsManager.instance().getByID(self.request.getUrlParam("id"))
			if(account != None):
				document = account.account.exportXML()
				buffer = osiris.Buffer()
				if(document.writeBuffer(buffer) == True):
					name = account.name
					#mime = osiris.MimeManager.instance().getMimeType("xml")
					mime = "text/xml"					
					self.session.transmitBuffer2(buffer, name, mime)
					
				
		
	def onRegister(self, args):		
		name = self.name.value
		
		password = self.password.value
		passwordChecker = self.passwordChecker.value
		if password != passwordChecker:
			self.showError(self.getText("portal.pages.register.error.passwordMismatch"))
			return
		
		secretQuestion = self.secretQuestion.value
		secretResponse = self.secretResponse.value
		secretResponseChecker = self.secretResponseChecker.value
		if secretResponse != secretResponseChecker:
			self.showError(self.getText("portal.pages.register.error.answerMismatch"))
			return
		
		savePassword = self.savePassword.check	
		account = osiris.IdeAccountsManager.instance().createAccount(name, password, savePassword)
		if account == None:
			self.showError(self.getText("portal.pages.register.error.cannotCreate"))			
		else:
			self.loginAccountWithID(account.id, password)
			self.redirect(osiris.PortalsSystem.instance().getMainLink("home"))
			
	def onCancel(self, args):
		self.redirect(osiris.PortalsSystem.instance().getMainLink("home"))
		
def main(args):
	page = Page(args[0])
	page.transmit()
