import os
import os.path
import osiris

import globalvars

import os
import pkgutil
import pprint
import sys
from cgi import escape

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
				
	def getPageName(self):
		return "extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.pythoninfo"
		
	def onInit(self):
		osiris.IMainPage.onInit(self)
		
		self.pathway.add(self.getText("extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.index.title"),"/developer_tools")
	
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)		
		
		self.page.addCss("/" + globalvars.extension.id.getString() + "/css/pythoninfo.css")
				
		document = osiris.XMLDocument()
		root = document.create("modules")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "pythoninfo.xsl"))
		template.document = document
		self.getArea(osiris.pageAreaContent).controls.add(template)
		
		from pyinfo import pyinfo
		output = pyinfo()    
		output = "<div class=\"pyinfo\">" + output + "</div>"
		self.getArea(osiris.pageAreaContent).controls.add(osiris.HtmlLiteral(output))