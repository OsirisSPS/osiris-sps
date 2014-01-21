import os
import osiris

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		
		if session.request.getUrlParam("mode") == "dialog":
			self.ajax = True
		
	def isMcpModeRequired(self):
		return False
				
	def getPageName(self):
		return "main.pages.skinpreview"
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)		
		
		document = osiris.XMLDocument()
		root = document.create("skinpreview")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "skinpreview.xsl"))
		template.document = document
		
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)

		self.saveCommand = osiris.IdeButton(self.getText("common.actions.save"))
		self.saveCommand.id = "save"
		self.saveCommand.iconHref = self.skin.getImageUrl("icons/16x16/save.png")
		template.addChildParam(self.saveCommand)

		self.chkBoolean = osiris.IdePickerBool()
		self.chkBoolean.id = "boolean"
		template.addChildParam(self.chkBoolean)

		self.txtText = osiris.HtmlTextBox()
		self.txtText.id = "text"
		self.txtText.size = 60
		self.txtText.value = "Neque porro quisquam est qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit..."
		template.addChildParam(self.txtText)