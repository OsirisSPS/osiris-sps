import os
import osiris

class MyModuleEditor(osiris.ExtensionsModuleEditor):

	global g_extension

	def createDocument(self):
		osiris.ExtensionsModuleEditor.createDocument(self)
		
		# Here, we create the document from zero. Generally, it's better to avoid that attribute of older properties not used anymore was serialized.
		nodeRoot = self.moduleDocument.create("flash_clocks_com")
		nodeRoot.setAttributeInt32("width",int(self.txtWidth.value))
		nodeRoot.setAttributeInt32("height",int(self.txtHeight.value))
		nodeRoot.setAttributeInt32("number",int(self.txtNumber.value))
		
		return True
		
	def onInit(self):
		osiris.ExtensionsModuleEditor.onInit(self)
		
		self.txtWidth = osiris.HtmlTextBox()
		self.txtWidth.id = "width"
		
		self.txtHeight = osiris.HtmlTextBox()
		self.txtHeight.id = "height"
		
		self.txtNumber = osiris.HtmlTextBox()
		self.txtNumber.id = "number"
		
		
				
	def onLoad(self):
		osiris.ExtensionsModuleEditor.onLoad(self)
		
		nodeRoot = self.moduleDocument.root		
		self.txtWidth.value = nodeRoot.getAttributeInt32("width")
		self.txtHeight.value = nodeRoot.getAttributeInt32("height")
		self.txtNumber.value = nodeRoot.getAttributeInt32("number")
				
		self.page.showMessage(nodeRoot.getAttributeString("data"))		
				
		self.xsl = osiris.HtmlXSLControl()
		self.xsl.stylesheet = self.page.loadStylesheet(os.path.join(os.path.dirname(__file__), "editor.xsl"))
		self.xsl.document = self.moduleDocument
		self.controls.add(self.xsl)
		self.xsl.addChildParam(self.txtWidth,"width")
		self.xsl.addChildParam(self.txtHeight,"height")
		self.xsl.addChildParam(self.txtNumber,"number")
		
		self.xsl.document.root.setAttributeString("extension_id",g_extension.id.getString())
		

class MyModuleViewer(osiris.ExtensionsModuleViewer):
	def onLoad(self):
		osiris.ExtensionsModuleViewer.onLoad(self)		
		
		self.xsl = osiris.HtmlXSLControl()
		self.xsl.stylesheet = self.page.loadStylesheet(os.path.join(os.path.dirname(__file__), "viewer.xsl"))
		self.xsl.document = self.moduleDocument
		self.controls.add(self.xsl)

class MyModule(osiris.IExtensionsModule):
	def getID(self):
		return "000000038E5642DD32A5AC697DC74DAE727ABA1B53732225"
		
	def getName(self):
		return "Clocks from http://flash-clocks.com"
		
	def createEditor(self):
		return MyModuleEditor()
		
	def createViewer(self):
		return MyModuleViewer()
		
	def initXML(self,document):
		osiris.IExtensionsModule.initXML(self,document)
		
		nodeRoot = document.root
		
		if nodeRoot == None:
			nodeRoot = document.create("flash_clocks_com")
			
		if nodeRoot.attributes.exists("width") == False:
			nodeRoot.setAttributeInt32("width",200)
		if nodeRoot.attributes.exists("height") == False:
			nodeRoot.setAttributeInt32("height",200)
		if nodeRoot.attributes.exists("number") == False:
			nodeRoot.setAttributeInt32("number",169)

g_module = MyModule()

def load(args):

	global g_extension
	g_extension = args[0]
	
	global g_module
	osiris.ExtensionsSystem.instance().registerModule(g_module)
	
def unload(args):
	global g_module
	osiris.ExtensionsSystem.instance().unregisterModule(g_module)
