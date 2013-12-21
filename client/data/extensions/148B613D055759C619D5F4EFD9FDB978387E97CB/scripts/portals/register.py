import os
import osiris
import osiris.events

class Page(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)
				
	def getPageName(self):
		return "portal.pages.register"
		
	def onInit(self):
		osiris.IPortalPage.onInit(self)	
		
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
		osiris.IPortalPage.onLoad(self)
		
		document = osiris.XMLDocument()
		self.root = document.create("register")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "register.xsl"))
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
		portal = self.getPortalFromUrl()
		if portal == None:
			return
	
		username = self.username.value
		if portal.validateUsername(username) == False:
			self.showError(self.getText("portal.pages.register.error.invalid_username"))
			return
		
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
		accountID = portal.createAccount(self.database, username, password, secretQuestion, secretResponse, savePassword)
		if accountID == "":
			self.showError(self.getText("portal.pages.register.error.cannotCreate"))			
		else:
			self.loginWithID(portal, accountID, password)
			self.redirect(portal.getLink("view"))
			
	def onCancel(self, args):
		self.redirect(self.portal.getLink("view"))
