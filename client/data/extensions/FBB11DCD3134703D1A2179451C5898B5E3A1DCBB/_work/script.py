import os
import osiris

class MyModuleEditor(osiris.ExtensionsModuleEditor):

	def createDocument(self):
		osiris.ExtensionsModuleEditor.createDocument(self)
		
		# Here, we create the document from zero. Generally, it's better to avoid that attribute of older properties not used anymore was serialized.
		nodeRoot = self.moduleDocument.create("custom_css")
		nodeRoot.setAttributeString("css",self.txtCss.value)
		
		return True
		
	def onInit(self):
		osiris.ExtensionsModuleEditor.onInit(self)
		
		self.txtCss = osiris.IdeOMLEditor()
		self.txtCss.id = "css"
		
				
	def onLoad(self):
		osiris.ExtensionsModuleEditor.onLoad(self)
		
		nodeRoot = self.moduleDocument.root		
		self.txtCss.value = nodeRoot.getAttributeString("css")
				
		self.xsl = osiris.HtmlXSLControl()
		self.xsl.stylesheet = self.page.loadStylesheet(os.path.join(os.path.dirname(__file__), "editor.xsl"))
		self.xsl.document = self.moduleDocument
		self.controls.add(self.xsl)
		self.xsl.addChildParam(self.txtCss,"css")
		

class MyModuleViewer(osiris.ExtensionsModuleViewer):
	def onLoad(self):
		osiris.ExtensionsModuleViewer.onLoad(self)		
		
		self.xsl = osiris.HtmlXSLControl()
		self.xsl.stylesheet = self.page.loadStylesheet(os.path.join(os.path.dirname(__file__), "viewer.xsl"))
		self.xsl.document = self.moduleDocument
		self.controls.add(self.xsl)
		
		css = self.moduleDocument.root.getAttributeString("css");
		css = osiris.OMLManager.instance().parseCss(css, self.page, False, False);
		self.xsl.document.root.setAttributeString("dump",css);
		

class MyModule(osiris.IExtensionsModule):
	def getID(self):
		return "000000035B171EB48C6CC531752BDFF163EA968A0FDDEFD3"
		
	def getName(self):
		return "Custom CSS"
		
	def createEditor(self):
		return MyModuleEditor()
		
	def createViewer(self):
		return MyModuleViewer()
		
	def initXML(self,document):
		osiris.IExtensionsModule.initXML(self,document)
		
		nodeRoot = document.root
		
		if nodeRoot == None:
			nodeRoot = document.create("custom_css")
			
		if nodeRoot.attributes.exists("css") == False:
			nodeRoot.setAttributeString("css","font-size:Times New Roman")

g_module = MyModule()

def load(args):
	global g_module
	osiris.ExtensionsSystem.instance().registerModule(g_module)
	
def unload(args):
	global g_module
	osiris.ExtensionsSystem.instance().unregisterModule(g_module)
