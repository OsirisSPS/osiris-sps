import os
import osiris

def load(page):

	css = ""
	css += "@import url(\"/htdocs/css/default.less\");\n"
	if(page.skin.getID() != "000"):
		css += "@import url(\"/" + page.skin.getID().getString() + "/css/skin.less\");\n"
	css += "// pov\n";
	css += "/* {@OsirisSkinEditor} */\n";
	css += "@import url(\"/htdocs/css/libraries/elements.less\");\n"
	css += "@import url(\"/htdocs/css/functions.less\");\n"		
		
	page.controls.add(osiris.HtmlLiteral(css))		

class MainPage(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		self.ajax = True
					
	def getPageName(self):
		return "~main.pages.csslib"
			
	def onLoad(self):
		osiris.IMainPage.onLoad(self)		
		
		load(self)
		
class PortalPage(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)		
		self.ajax = True

				
	def getPageName(self):
		return "~portal.pages.csslib"
		
	def onLoad(self):
		osiris.IPortalPage.onLoad(self)		
		
		load(self)