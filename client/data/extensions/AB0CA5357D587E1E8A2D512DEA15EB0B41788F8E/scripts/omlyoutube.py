import os
import osiris
import re

import globalvars

class OmlYouTube(osiris.IOMLCode):
	def __init__(self, tag):
		osiris.IOMLCode.__init__(self, tag)
		
		
	def processHtml(self, item, context):
				
		id = item.getSingleText()
		
		if re.match("([A-Z,a-z,0-9])",id) == False:
			return ""
		
		if context.getMode() == osiris.omlRenderModeSearch:
			return id
			
		if( (context.getMode() == osiris.omlRenderModeOsiris) or (context.getMode() == osiris.omlRenderModeExternal) ):
		
			if item.hasParam("width"):
				width = int(item.getParam("width"))
			else:
				width = 425
				
			if item.hasParam("width"):
				height = int(item.getParam("height"))
			else:
				height = 320
				
			htmlOutput = "<object width=\"{width}\" height=\"{height}\"><param name=\"movie\" value=\"http://www.youtube.com/v/{param}\"></param><embed src=\"http://www.youtube.com/v/{param}\" type=\"application/x-shockwave-flash\" width=\"{width}\" height=\"{height}\"></embed></object>"
		
			htmlOutput = htmlOutput.replace("{width}",str(width))
			htmlOutput = htmlOutput.replace("{height}",str(height))
			htmlOutput = htmlOutput.replace("{param}",id)
			
			return htmlOutput		
		
		
	
	