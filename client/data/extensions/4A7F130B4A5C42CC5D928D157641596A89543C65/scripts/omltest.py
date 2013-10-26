import osiris

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		
		self.editor = osiris.IdeOMLEditor()
		self.editor.id = "editor"
		self.editor.reportDetailed = True
		
	def getPageName(self):
		return "extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.omltest"
			
	def onInit(self):
		osiris.IMainPage.onInit(self)		
		
		self.pathway.add(self.getText("extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.index.title"),"/developer_tools")
		
		blockEditor = osiris.IdeBlock("OML Editor")
		self.getArea(osiris.pageAreaContent).controls.add(blockEditor)
				
		blockEditor.body.controls.add(self.editor)
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)		
		
		blockHtmlGenerated = osiris.IdeBlock("HTML Output")
		self.getArea(osiris.pageAreaContent).controls.add(blockHtmlGenerated)

		output = osiris.HtmlDiv()
		output.id="output"
		output.controls.add(osiris.HtmlLiteral(self.parseOml("[code]" + self.parseOml(self.editor.value) + "[/code]")))
		blockHtmlGenerated.body.controls.add(output)
