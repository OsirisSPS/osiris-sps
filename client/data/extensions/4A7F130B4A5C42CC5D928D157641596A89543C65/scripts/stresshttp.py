import globalvars
import os
import osiris
#import osiris.events

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		self.ajax = (session.request.getUrlParam("number") != "")
		
	def getPageName(self):
		return "extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.stress"
		
	def onInit(self):
		osiris.IMainPage.onInit(self)
		
		self.pathway.add(self.getText("extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.index.title"),"/developer_tools")
		
	def onLoad(self):
		osiris.IMainPage.onLoad(self)		
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)
		
		number = self.session.request.getUrlParam("number")
		
		if(number == ""):
			for i in range(1,100):
				self.controls.add(osiris.HtmlLiteral("<div style=\"border:1px solid red;padding:5px;margin:5px;\" data-os-url=\"/main/pcrash?number=" + str(i) + "\">Loading...</div>"));
		else:		
			self.controls.add(osiris.HtmlText("Success! - " + number));
