import os
import osiris

import globalvars

# Samples:
# [citation]Citation text[/citation]
# [citation="Author"]Citation text[/citation]
# [citation="Author" src="http://www.google.it"]Citation text[/citation]
# [citation="[url="http://www.google.it"]Author[/url]"]Citation text[/citation]
# [citation author="Author" src="http://www.google.it" text="Citation text" /]

class OmlCitation(osiris.IOMLCode):
	def __init__(self, tag):
		osiris.IOMLCode.__init__(self, tag)
		
		
	def processHtml(self, item, context):
		
		extensionID = globalvars.extension.getID().getString()
			
		self.mapDefaultParamTo(item, "author")
		
		author = item.getParam("author")
		text = item.getParam("text")
		src = item.getParam("src")
		
		if(text == ""):
			text = item.getSingleText()
		if(src != ""):
			author = "[url=\"" + src + "\"]" + author + "[/url]"
		
		if context.getMode() == osiris.omlRenderModeSearch:
			return ""
		
		if( (context.getMode() == osiris.omlRenderModeOsiris) or (context.getMode() == osiris.omlRenderModeExternal) ):
		
			oml = ""
			oml += "[div style=\"border:1px solid gray;padding:10px;text-align:center;font-style:italic;font-size:1.4em;margin:auto;width:70%;\"]"
			oml += "[img]/" + extensionID + "/open.png[/img]"
			oml += text;
			oml += "[img]/" + extensionID + "/close.png[/img]"
			if(author != ""):
				oml += "[div style=\"text-align:right;font-style:normal;font-size:0.8em\"]"
				oml += author
				oml += "[/div]"
			oml += "[/div]"			
						
			html = self.encodeOML(context, oml)
						
			return html
	
	