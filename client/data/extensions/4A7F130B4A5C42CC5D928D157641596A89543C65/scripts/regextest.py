import osiris

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		
		self.input = osiris.IdeOMLEditor()
		self.input.id = "input"		
		
		self.expression = osiris.IdeOMLEditor()
		self.expression.id = "expression"
		
		self.noCase = osiris.HtmlCheckBox()
		self.noCase.caption = "Case insensitive"
		self.noCase.id = "nocase"
		
		self.format = osiris.IdeOMLEditor()
		self.format.id = "format"
		
		self.compute = osiris.HtmlButton()
		self.compute.caption = "Compute"
		self.compute.id = "compute"
		
	def getPageName(self):
		return "extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.regexTest"
			
	def onInit(self):
		osiris.IMainPage.onInit(self)		
		
		self.pathway.add(self.getText("extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.index.title"),"/developer_tools")
		
		if self.postBack == False:
			self.noCase.check = True
		
		blockInput = osiris.IdeBlock("Input")
		self.getArea(osiris.pageAreaContent).controls.add(blockInput)				
		blockInput.body.controls.add(self.input)
		
		blockExpression = osiris.IdeBlock("Expression")
		self.getArea(osiris.pageAreaContent).controls.add(blockExpression)				
		blockExpression.body.controls.add(self.expression)
		blockExpression.body.controls.add(self.noCase)
		
		blockFormat = osiris.IdeBlock("Format")
		self.getArea(osiris.pageAreaContent).controls.add(blockFormat)				
		blockFormat.body.controls.add(self.format)
		
		self.getArea(osiris.pageAreaContent).controls.add(self.compute)
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)		
				
		if(self.getPostBack()):
						
			try:
				match = False
				out = ""
				match = osiris.RegexManager.instance().match(self.input.value, self.expression.value, self.noCase.check)
				out = osiris.RegexManager.instance().replace(self.input.value, self.expression.value, self.format.value, self.noCase.check)
				
				bbOut = "[center][size=\"2em\"]"
				if(match):
					bbOut += "Match!"
				else:
					bbOut += "Not match."
					
				bbOut += "[/size][/center]"
				if(out != ""):
					bbOut += "[code]" + out + "[/code]"
				
				bbOut = self.parseOml(bbOut)
			
				blockOutput = osiris.IdeBlock("Output")
				self.getArea(osiris.pageAreaContent).controls.add(blockOutput)
				blockOutput.body.controls.add(osiris.HtmlLiteral(bbOut))
				
			except Exception as ex:
				self.showMessage(ex.message)
						
			
