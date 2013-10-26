import os
import osiris
import osiris.events

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
				
	def getPageName(self):
		return "main.pages.register"
		
	def onInit(self):
		osiris.IMainPage.onInit(self)	
		
		#self.portal = self.getPortalFromUrl()
		#if self.portal == None:
		#	self.redirect("home")
		#	return		
		
		self.username = osiris.HtmlTextBox()
		self.username.css = "os_input_full"
		self.username.id = "username"
		
		self.password = osiris.HtmlTextBox()
		self.password.id = "password"
		self.password.attributes.set("data-os-otype","password")
		self.password.attributes.set("data-os-login","page_username")
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
		self.root = document.create("register")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "register_main.xsl"))
		template.document = document
		self.getArea(osiris.pageAreaContent).controls.add(template)		
		
		template.addChildParam(self.username)	
		template.addChildParam(self.password)	
		template.addChildParam(self.passwordChecker)	
		template.addChildParam(self.savePassword)	
		template.addChildParam(self.cmdRegister)	
		template.addChildParam(self.cmdCancel)	
		
		#template.addChildParam(self.secretQuestion)	
		#template.addChildParam(self.secretResponse)	
		#template.addChildParam(self.secretResponseChecker)	
		
		
	def onRegister(self, args):		
		username = self.username.value
		
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
		account = osiris.IdeSystem.instance().accountsManager.createAccount(username, password, savePassword)
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
