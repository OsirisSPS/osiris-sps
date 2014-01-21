#winpdb
#import rpdb2; rpdb2.start_embedded_debugger("osiris")

# Questo script va invocato dalla versione console altrimenti lancia delle eccezioni

import sys
import osiris
import epydoc.cli
import globalvars

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
				
	def getPageName(self):
		return "extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.pythondocgenerator"
			
	def onInit(self):
		osiris.IMainPage.onInit(self)		
		
		block = osiris.IdeBlock("Python Documentation Generator")
		self.getArea(osiris.pageAreaContent).controls.add(block)
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)		
		
		sys.argv.append("--debug")	
		sys.argv.append("--html")	
		sys.argv.append("osiris")
		sys.argv.append("osirislib")			
		
		sys.argv.append("--graph")
		sys.argv.append("all")
		sys.argv.append("--dotpath")
		sys.argv.append(globalvars.extension.path + "../../../../../utils/graphviz/bin/dot.exe")
		
		sys.argv.append("--url")
		sys.argv.append("http://www.osiris-sps.org")	
		
		sys.argv.append("-o")
		sys.argv.append(globalvars.extension.path + "pythondoc")
		
		epydoc.cli.cli()
		
		self.showMessage("Generated.")
		