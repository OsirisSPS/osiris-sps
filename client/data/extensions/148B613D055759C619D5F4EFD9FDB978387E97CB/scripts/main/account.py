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
		
		self.document.root.setAttributeString("alfa", "beta2")
		
		self.saveCommand = osiris.IdeButton(self.getText("common.actions.save"))
		self.saveCommand.id = "saveCommand"
		osiris.events.connect(self.saveCommand.eventClick, self.onSave)
		template.addChildParam(self.saveCommand)
		
				
						
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)		
	
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)
		
		
			
	def onSave(self, args):
	
		self.showMessage("Saved settings.")
		
	
