import os
import osiris
import re

import core
import macros

##############################################
# Main Code
##############################################

class OmlHtml(osiris.IOMLCode):

	def __init__(self, tag):
		osiris.IOMLCode.__init__(self, tag)
		
	def processHtml(self, item, context):
	
		output = ""
		
		if context.getMode() == osiris.omlRenderModeSearch:
			return "{HTML-Snippet}"
		
		#if(context.checkPortalPageAvailable(item) == False):
		#	return ""
		
		#if(context.getPortalPage() == None):
		#	context.addWarning("HTML Snippet: Must be used in portal pages.")
		#	return ""
		
		if(context.page == None): # Don't know why, but occur...
			return ""
			
		html = item.getSingleText()
		
		# Replaces
		html = html.replace("{os:ref_id}", context.getRefID());
		html = html.replace("{os:obj_id}", context.page.getRequest().getUrlParam("id"));
		html = html.replace("{os:url}", context.getFullUrl());
		html = html.replace("{os:lang}", context.page.getLanguage());		
				
		# Debugging box helper
		if( (item.getParam("mode") == "debug") and (context.getMode() == osiris.omlRenderModeOsiris) ):
			output += "<div class=\"os_htmlsnippet_debug1\">";
			output += osiris.OMLManager.instance().parse("[code=\"" + str(item.getParam("title")) + "\"]" + str(html) + "[/code]", context.page, True, True, False, osiris.omlRenderModeOsiris, "", "")				
			output += "<div class=\"os_htmlsnippet_debug2\">";
		
		# Start cleaning HTML, pre removing lines.
				
		html = re.sub(" // [^\n]*?\n","\n", html) # Remove JS // comments. TODO: Check if inside a <script> tag. Note: very ugly regex. I'm unable to use lookbehind or regex advanced syntax to accomplish that.
			
		# Start cleaning HTML, Removing lines.		
		html = html.replace("\n","")
		html = html.replace("\r","")
		
		# Start cleaning HTML, post removing lines.
		html = html.replace("\t","")
		html = re.sub("/\*(.*?)\*/","", html) # Remove JS comments. TODO: Check if inside a <script> tag.				
		html = re.sub("\<\!\-\-(.*?)\-\-\>","", html) # Remove HTML comments.
		
		# TODO: Remove <noscript>. Ininfluent in Osiris, that require it.
		html = re.sub("\<noscript\>([\s\S]*?)\</noscript\>","",html)
		
		ready = False
		while (ready == False):
			oldHtml = html			
			#html = html.replace("  "," ")
			# TODO: The following must be done only OUTSIDE <script> tag.
			#html = html.replace("> <","><")			
			#html = html.replace("> ",">")
			#html = html.replace(" <","<")			
			# TODO: The following must be done only INSIDE <script> tag.
			html = html.replace("; ",";")
			html = html.replace(" ;",";")
			html = html.replace("= ","=")
			html = html.replace(" =","=")
			html = html.replace(" {","{")
			html = html.replace("{ ","{")
			html = html.replace(" }","{")
			html = html.replace("} ","{")
			
			ready = (html == oldHtml)
			
		html = html.strip() # Trim
			
		# End cleaning HTML
				
		result = "unknown"		
				
		isIsis = (context.page.getRequestSource() != osiris.IPage.rsOsiris)
						
		for snippet in core.snippetsList:
		
			if( (hasattr(snippet,"debug")) and (snippet.debug) ):
				osiris.LogManager.instance().log(snippet.name + " : " + "Debug mode.")
				osiris.LogManager.instance().log(snippet.name + " : " + "HTML : " + html)
				
			if(hasattr(snippet,"matchRe")):		
				
				if( (hasattr(snippet,"debug")) and (snippet.debug) ):
					osiris.LogManager.instance().log(snippet.name + " : " + "Matching regular expressions")					
					
				for matchReSingle in snippet.matchRe:
					regex = "^" + matchReSingle + "$"
					
					regexWithMacro = regex
					
					for macroName in macros.regexMacros:
						macroValue = macros.regexMacros[macroName];
						regex = regex.replace("{@" + macroName + "}",macroValue);
					
					if( (hasattr(snippet,"debug")) and (snippet.debug) ):
						osiris.LogManager.instance().log(snippet.name + " : " + "Regex: " + regex)
					
					prog = re.compile(regex, re.M)
						
					# Test if be macro-optimized
					macroEdition = regex
					for macroName in macros.regexMacros:
						macroValue = macros.regexMacros[macroName];
						macroEdition = macroEdition.replace(macroValue, "{@" + macroName + "}");
					if(macroEdition != regexWithMacro):
						if macroEdition.endswith("$"): 
							macroEdition = macroEdition[:-1]
						if macroEdition.startswith("^"): 
							macroEdition = macroEdition[1:]
						macroEdition = macroEdition.replace("\\","\\\\")
						macroEdition = macroEdition.replace("\"","\\\"")
						macroEdition = "\"" + macroEdition + "\""

						if( (hasattr(snippet,"debug")) and (snippet.debug) ):
							osiris.LogManager.instance().log(snippet.name + " : " + "Warning: Suggested Macro edition python variable: " + macroEdition)
							
		
					if prog.match(html) is not None:
						if( (hasattr(snippet,"debug")) and (snippet.debug) ):
							osiris.LogManager.instance().log(snippet.name + " : " + "Match regex.")
						result = snippet.getCurrentAction(context.page)
			
			if(hasattr(snippet,"match")):
				if(snippet.match(html)):
					if( (hasattr(snippet,"debug")) and (snippet.debug) ):
						osiris.LogManager.instance().log(snippet.name + " : " + "Match method.")
					result = snippet.getCurrentAction(context.page)
						
			if(result == "session"):
				result = "yes"
				
			if( (result == "ask") and (isIsis) ):
				result = "no"
				
			if( (context.getMode() == osiris.omlRenderModeSearch) and (result == "ask") ):
				result = "no"
				
			if( (context.getMode() == osiris.omlRenderModeExternal) and (result == "ask") ):
				result = "no"
					
			if(result == "ask"):
				
				#context.page.addJavascript("/9A53510BB471C48AF9B7954466D123438C387368/js/htmlsnippet.js")
				#context.page.addCss("/9A53510BB471C48AF9B7954466D123438C387368/css/htmlsnippet.css")
				
				document = osiris.XMLDocument()
				root = document.create("html")
				
				root.attributes.set("extension_id", core.extension.id.getString())			
				
				root.attributes.set("code", snippet.code)		
				root.attributes.set("name", snippet.name)		
				root.attributes.set("url", snippet.url)						
				root.attributes.set("href", osiris.PortalsSystem.instance().getExternalLink(snippet.url, True))						
				root.attributes.set("security", snippet.security)
				root.attributes.set("notes", snippet.notes)
				root.attributes.set("reload", snippet.reload)
				root.attributes.set("token", context.page.getSession().state.token)				
				
				root.attributes.set("uid", context.getContextNextID())
				
				
				# Javascript encoding
				htmlEncoded = html
				htmlEncoded = htmlEncoded.replace("\n","\\n")
				htmlEncoded = htmlEncoded.replace("\r","\\r")
				htmlEncoded = htmlEncoded.replace("\"","\\\"")				
				htmlEncoded = htmlEncoded.replace("'","\\'")				
				root.attributes.set("html", htmlEncoded)		
				
				# Detect / suggest width & height
				width = 0
				height = 0
				
				# Detect Width
				progC = re.compile("^(.*?)width\s?\=\s?['\"]?([0-9]*)['\"]?(.*?)$", re.M)
				progM = progC.match(html)
				if progM is not None:
					width = int(progM.group(2))
					
				# Detect Height
				progC = re.compile("^(.*?)height\s?\=\s?['\"]?([0-9]*)['\"]?(.*?)$", re.M)
				progM = progC.match(html)
				if progM is not None:
					height = int(progM.group(2))

				
				# Ask width & height to snippet
				if( (width == 0) and (hasattr(snippet,"width")) ):
					width = snippet.width
				
				if( (height == 0) and (hasattr(snippet,"height")) ):
					height = snippet.height
					
				# Use default width & height
				if(width == 0):
					width = 160
				if(height == 0):
					height = 160
				
				root.attributes.set("width", width)		
				root.attributes.set("height", height)		
				
				stylesheet = context.page.loadStylesheet(os.path.join(os.path.dirname(__file__), "omlhtml.xsl"))				
				outputAsk = stylesheet.applyToString2(document)
								
				output += outputAsk;
				break
		
			elif(result == "yes"):				
				output += str(html)
				break
		
			elif(result == "no"):
				context.addWarning("HTML Snippet: '" + snippet.name + "' is detected, but output is disabled.")				
				break			
		
		if(result == "unknown"):
			context.addWarning("HTML Snippet: Unknown html code.")	
			
		if( (item.getParam("mode") == "debug") and (context.getMode() == osiris.omlRenderModeOsiris) ):
			output += "<div style=\"clear:both\"></div></div></div>";
			
		return output
		
		