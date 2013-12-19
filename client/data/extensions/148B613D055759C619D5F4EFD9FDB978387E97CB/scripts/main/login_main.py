import os
import osiris
import osiris.events

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
				
	def getPageName(self):
		return "main.pages.login"
		
	def onInit(self):
		osiris.IMainPage.onInit(self)	
		
		self.userid = self.session.request.getUrlParam("id")
		if( (self.userid == None) or (self.userid == "") ):
			self.redirect("home")
			return		
		
		#self.username = osiris.HtmlTextBox()
		#self.username.css = "os_input_full"
		#self.username.id = "username"
		
		self.password = osiris.HtmlTextBox()
		self.password.id = "password"
		self.password.size = 20
		self.password.password = True
		
		self.savePassword = osiris.HtmlCheckBox()
		self.savePassword.id = "savePassword"
		
		self.cmdLogin = osiris.IdeButton(self.getText("main.pages.login.actions.login"))
		self.cmdLogin.id = "login"
		self.cmdLogin.isDefault = True
		osiris.events.connect(self.cmdLogin.eventClick, self.onLogin)
		
		
		

	def onLoad(self):
		osiris.IMainPage.onLoad(self)
		
		document = osiris.XMLDocument()
		self.root = document.create("login")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "login_main.xsl"))
		template.document = document
		self.getArea(osiris.pageAreaContent).controls.add(template)		
				
		#template.addChildParam(self.username)	
		template.addChildParam(self.password)
		template.addChildParam(self.savePassword)
		template.addChildParam(self.cmdLogin)
				
			
		
	def onLogin(self, args):
		
		#username = self.username.value
		#if self.portal.validateUsername(username) == False:
		#	self.showError(self.getText("portal.pages.register.error.invalid_username"))
		#	return
		
		password = self.password.value
		savePassword = self.savePassword.check	
		
		#if(self.loginWithName(self.portal, username, password, savePassword)):
		if(self.loginAccountWithID(self.userid, password, savePassword)):
			self.redirect("home")
		else:
			self.showError(self.getText('main.pages.login.denied'))
			
	