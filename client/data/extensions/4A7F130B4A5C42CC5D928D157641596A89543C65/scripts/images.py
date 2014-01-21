import os
import os.path
import osiris

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
				
	def getPageName(self):
		return "extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.images"

	def onInit(self):
		osiris.IMainPage.onInit(self)
		
		self.pathway.add(self.getText("extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.index.title"),"/developer_tools")
				
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)		

		document = osiris.XMLDocument()
		root = document.create("folder")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "images.xsl"))
		template.document = document
		self.getArea(osiris.pageAreaContent).controls.add(template)
		
		self.listFiles(document.root,osiris.Options.instance().getDataPath() + "/extensions/138B613D055759C619D5F4EFD9FDB978387E97CB/htdocs","images")
		
	def listFiles(self, node, basedir, dir):
		node.setAttributeString("name",dir)
		
		dir2 = os.path.join(basedir, dir)
		
		for item in os.listdir(dir2):
			if( (item.startswith(".") == False) and (item.startswith("xx_") == False) and (item.startswith("_work") == False) ):			
				if os.path.isdir(os.path.join(dir2, item)):
					nodeDir = node.addChild("folder")
					self.listFiles(nodeDir, basedir, os.path.join(dir,item))
				else:
					nodeFile = node.addChild("file")
					nodeFile.setAttributeString("name",item)				
					nodeFile.setAttributeString("path",os.path.join(dir,item))
					nodeFile.setAttributeString("osiris",str("osiris:?type=res&path=" + osiris.HttpParser.urlEncodeW(os.path.join(dir,item))))
			