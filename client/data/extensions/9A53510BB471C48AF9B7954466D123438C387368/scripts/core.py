import os
import osiris

extension = None
mainHandler = None
omlHtml = None

snippetsList = []

class ISnippet:
	def getCurrentAction(self, page):
	
		isIsis = (page.getRequestSource() != osiris.IPage.rsOsiris)
		
		globalAction = page.getOption("extensions.htmlsnippet.global").getString()
		settingsAction = page.getOption("extensions.htmlsnippet." + self.code).getString()
		
		sessionAction = ""
		if(isIsis == False):
			if(page.getSession().state != None):
				sessionAction = page.getSession().state.get("extensions.htmlsnippet." + self.code)
		
		result = "ask"
		if(sessionAction != ""):
			result = "session"
		elif(settingsAction != ""):
			result = settingsAction
		elif(globalAction != ""):
			result = globalAction
		else:
			result = self.action
		
		return result