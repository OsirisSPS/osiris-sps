import os
import osiris

import core

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		self.ajax = (self.request.getUrlParam("act") == "play")
				
	def getPageName(self):
		return "extensions.9A53510BB471C48AF9B7954466D123438C387368.htmlsnippet"
		
	def onInit(self):
		osiris.IMainPage.onInit(self)
		
		self.addJavascript("/" + core.extension.id.getString() + "/js/htmlsnippet.js")
		self.page.addCss("/" + core.extension.id.getString() + "/css/htmlsnippet.css")
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)	
		
		act = self.session.request.getUrlParam("act")
		
		token = self.session.request.getUrlParam("token")	
		snippetCode = self.session.request.getUrlParam("code")		
		answer = self.session.request.getUrlParam("answer")
		remember = self.session.request.getUrlParam("remember")
		
		if(act == "play"):
			if((self.session.state != None) and (token == self.session.state.token)):
				
				if(remember == "yes"):
					osiris.Options.instance().setOptionString("extensions.htmlsnippet." + snippetCode, answer)
				else:
					if(answer == "yes"):
						self.session.state.set("extensions.htmlsnippet." + snippetCode,"yes")						
					
			else:
				osiris.LogManager.instance().log("invalid token")				
			
		else:
			document = osiris.XMLDocument()
			root = document.create("htmlsnippet")
			template = osiris.HtmlXSLControl()
			template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "pagehtml.xsl"))
			template.document = document
			self.controls.add(template)
			
			document.root.attributes.set("extension_id", core.extension.id.getString())			
			
			if(self.request.hasPostParam("action_global")):
					osiris.Options.instance().setOptionString("extensions.htmlsnippet.global", self.request.getPostParam("action_global").getString())
			
			globalAction = osiris.Options.instance().getOptionString("extensions.htmlsnippet.global")
			document.root.attributes.set("action_global", globalAction)
					
			for snippet in core.snippetsList:
				snippetNode = document.root.addChild("snippet")
				
				snippetNode.setAttributeString("code", snippet.code)
				snippetNode.setAttributeString("name", snippet.name)
				snippetNode.setAttributeString("category", snippet.category)						
				snippetNode.attributes.set("url", snippet.url)						
				snippetNode.attributes.set("href", osiris.PortalsSystem.instance().getExternalLink(snippet.url, True))									
				snippetNode.setAttributeString("security", snippet.security)			
				snippetNode.setAttributeBool("notes", snippet.notes)						
				
								
				if(self.request.hasPostParam("action_" + snippet.code)):
					newValue = self.request.getPostParam("action_" + snippet.code).getString()
					oldValue = osiris.Options.instance().getOptionString("extensions.htmlsnippet." + snippet.code)
					if(newValue != oldValue):
						if(self.session.state != None):
							self.session.state.set("extensions.htmlsnippet." + snippet.code, "") # Reset
						osiris.Options.instance().setOptionString("extensions.htmlsnippet." + snippet.code, newValue)
								
				snippetAction = osiris.Options.instance().getOptionString("extensions.htmlsnippet." + snippet.code)		
				snippetNode.attributes.set("action", snippetAction)
				
				snippetNode.setAttributeString("status", snippet.getCurrentAction(self))			
				
				
			
		
