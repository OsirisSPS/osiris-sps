import osiris

class MyModuleEditor(osiris.ExtensionsModuleEditor):
	def onLoad(self):
		osiris.ExtensionsModuleEditor.onLoad(self)
		self.controls.add(osiris.HtmlText("MyModuleEditor"))

class MyModuleViewer(osiris.ExtensionsModuleViewer):
	def onLoad(self):
		osiris.ExtensionsModuleViewer.onLoad(self)		
		self.controls.add(osiris.HtmlText("MyModuleViewer"))

class MyModule(osiris.IExtensionsModule):
	def getID(self):
		return "00000003{unique_id}"
		
	def getName(self):
		return "{extension_name}"
		
	def createEditor(self):
		return MyModuleEditor()
		
	def createViewer(self):
		return MyModuleViewer()

g_module = MyModule()

def load(args):
	global g_module
	osiris.ExtensionsSystem.instance().registerModule(g_module)
	
def unload(args):
	global g_module
	osiris.ExtensionsSystem.instance().unregisterModule(g_module)
