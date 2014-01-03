import globalvars
import os
import osiris
import osiris.events

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
				
	def getPageName(self):
		return "main.pages.mcp"
		
	def isMcpModeRequired(self):
		return False
		
	def onInit(self):
		osiris.IMainPage.onInit(self)		
		
		self.document = osiris.XMLDocument()
		self.document.create("mcp")
		
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "mcp.xsl"))
		template.document = self.document		
		self.getArea(osiris.pageAreaContent).controls.add(template)
		
		if(self.getMcpMode()):
			if(osiris.Options.instance().getWebMcpPassword() != ""):
				self.cmdLogout = osiris.IdeButton(self.getText("main.pages.mcp.action.logout"))
				self.cmdLogout.id = "logout"
				self.cmdLogout.isDefault = True
				osiris.events.connect(self.cmdLogout.eventClick, self.onLogout)
				template.addChildParam(self.cmdLogout)
		else:
			self.txtPassword = osiris.HtmlTextBox()
			self.txtPassword.id = "password"
			self.txtPassword.password = True
			template.addChildParam(self.txtPassword)
		
			self.cmdLogin = osiris.IdeButton(self.getText("main.pages.mcp.action.login"))
			self.cmdLogin.id = "login"
			self.cmdLogin.isDefault = True
			osiris.events.connect(self.cmdLogin.eventClick, self.onLogin)			
			template.addChildParam(self.cmdLogin)
			
	def onLoad(self):
		osiris.IMainPage.onLoad(self)		
		self.addCss(self.skin.getResourceUrl("css/main/mcp.css"))
		
	def onLogin(self, args):		
		passwordSpecified = self.txtPassword.value
		passwordRequested = osiris.Options.instance().getWebMcpPassword()
		
		if(passwordSpecified == passwordRequested):
			self.session.getState().set("mcp.password",passwordSpecified)
			self.redirect("/main/home")
		else:
			self.document.root.setAttributeBool("wrong",True)
			
	def onLogout(self, args):
		self.session.getState().set("mcp.password","")		
		self.reload();
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)		

		
