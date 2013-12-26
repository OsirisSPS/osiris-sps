import osiris
import os
import osiris.events

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		self.ajax = (session.request.getUrlParam("act") != "")
				
	def getPageName(self):
		return "main.pages.account"	
		
	def onLoad(self):	
		osiris.IMainPage.onLoad(self)
		
		self.act = self.session.request.getUrlParam("act")
		if(self.act == ""):
			self.act = "home"		
				
		self.document = osiris.XMLDocument()
		self.root = self.document.create(self.act)
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "account.xsl"))
		template.document = self.document
				
		self.saveCommand = osiris.IdeButton(self.getText("common.actions.save"))
		self.saveCommand.id = "saveCommand"
		self.saveCommand.iconHref = self.skin.getImageUrl("icons/16x16/save.png")
		osiris.events.connect(self.saveCommand.eventClick, self.onSave)
		template.addChildParam(self.saveCommand)
		
		self.txtName = osiris.HtmlTextBox()
		self.txtName.id = "name"
		self.txtName.size = 40
		template.addChildParam(self.txtName)

		self.txtPassword = osiris.HtmlTextBox()
		self.txtPassword.id = "password"
		self.txtPassword.size = 40
		self.txtPassword.setPassword(True)
		template.addChildParam(self.txtPassword)
		
		self.skinPicker = osiris.IdePickerSkin()
		self.skinPicker.id = "skin"		
		self.skinPicker.setShowSystem(True)
		template.addChildParam(self.skinPicker)


		self.languagePicker = osiris.IdePickerCulture()
		self.languagePicker.id = "language"		
		self.languagePicker.setShowSystem(True)
		template.addChildParam(self.languagePicker)
				
		osiris.events.connect(self.events.get("onAccountExport"), self.onAccountExport)
		self.document.root.attributes.set("export_href",self.getEventCommand("onAccountExport"))		

		osiris.events.connect(self.events.get("onAccountRemove"), self.onAccountRemove)
		self.document.root.attributes.set("remove_href",self.getEventCommand("onAccountRemove"))		

		
		
		if(self.postBack == False):
			self.txtName.value = self.getSessionAccount().account.name
			self.txtPassword.value = self.getSessionAccount().account.getRealPassword()
			self.skinPicker.value = self.getSessionAccount().account.skinID.getString()
			self.languagePicker.value = self.getSessionAccount().language;
						
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)		
	
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)
		
	def onAccountExport(self, args):
		self.showMessage(args[0])		

	def onAccountRemove(self, args):
		self.showMessage(args[0])		
			
	def onSave(self, args):
	
		self.showMessage(self.getText("common.save_success"))

		database = osiris.Engine.instance().createSystemConnection()
		
		self.getSessionAccount().account.name = self.txtName.value
		if(self.getSessionAccount().account.setRealPassword(self.txtPassword.value) == False):
			self.showError(self.getText("common.invalid_password"))
			return
		self.getSessionAccount().account.setSkin(str(self.skinPicker.value))
		self.getSessionAccount().account.setLanguage(self.languagePicker.value)
		
		osiris.IdeAccountsManager.instance().save(self.getSessionAccount().account)
	
