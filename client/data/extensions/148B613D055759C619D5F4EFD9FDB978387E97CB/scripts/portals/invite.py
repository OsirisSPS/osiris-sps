import os
import osiris

class Page(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)
		
		if session.request.getUrlParam("mode") == "dialog":
			self.ajax = True
				
	def getPageName(self):
		return "portal.pages.invite"
		
	def onLoad(self):
		osiris.IPortalPage.onLoad(self)		
		
		document = osiris.XMLDocument()
		self.root = document.create("invite")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "invite.xsl"))
		template.document = document
		
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)
			
		document.root.setAttributeString("mode", self.request.getUrlParam("mode"))
		
		self.root.setAttributeString("subscribe_href",self.portal.generateInviteLink(False));
		self.root.setAttributeString("isis_subscribe_href",self.portal.generateInviteLink(True));
		self.root.setAttributeString("export_href",self.portal.generateExportLink());		
		
	def onPreRender(self):
		osiris.IPortalPage.onPreRender(self)		

		
