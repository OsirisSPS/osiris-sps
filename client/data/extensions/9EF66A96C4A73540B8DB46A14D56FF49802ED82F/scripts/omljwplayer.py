import os
import osiris

import globalvars

class OmlJWPlayer(osiris.IOMLCode):
	def __init__(self, tag):
		osiris.IOMLCode.__init__(self, tag)
		
		
	def processHtml(self, item, context):
		
		extensionID = globalvars.extension.getID().getString()
		
		if context.getMode() == osiris.omlRenderModeSearch:
			return ""
			
		self.mapDefaultParamTo(item, "url")
		
		if( (context.getMode() == osiris.omlRenderModeOsiris) or (context.getMode() == osiris.omlRenderModeExternal) ):
		
			if item.hasParam("width"):
				width = int(item.getParam("width"))
			else:
				width = 425
				
			if item.hasParam("width"):
				height = int(item.getParam("height"))
			else:
				height = 320
				
			if item.hasParam("url"):
				url = item.getParam("url")
				url = self.encodeResourceUrl(context, item, url)
			else:
				url = ""
				
			if(url == ""):
				url = "/" + extensionID + "/video.flv"
			
			objID = context.getContextNextID();
			objID2 = context.getContextNextID();
			player = "/" + extensionID + "/player.swf"			
			
			vars = "file=" + self.encodeToUrlGet(context, url)
			
			# Variables, two syntax allowed:  1) [var name="myname" value="myvalue" /]    2) [var="myname"]myvalue[/var]			
			for childItem in item.getChilds():
				if(childItem.getTagName() == "var"):
					self.mapDefaultParamTo(childItem, "name")
					varName = childItem.getParam("name")
					if(childItem.hasParam("value")):
						varValue = childItem.getParam("value")
					else:
						varValue = childItem.getSingleText()
					vars += "&" + self.encodeToUrlGet(context, varName) + "=" + self.encodeToUrlGet(context, varValue)
					
			html = ""
			html += "<object id=\"" + objID + "\" classid=\"clsid:D27CDB6E-AE6D-11cf-96B8-444553540000\" name=\"" + objID + "\" width=\"" + str(width) + "\" height=\"" + str(height) + "\">	"
			html += "<param name=\"movie\" value=\"" + player + "\" />	"
			html += "<param name=\"allowfullscreen\" value=\"true\" />	"
			html += "<param name=\"allowscriptaccess\" value=\"always\" />	"
			html += "<param name=\"wmode\" value=\"opaque\" />	"
			html += "<param name=\"flashvars\" value=\"" + vars + "\" />	"
			html += "<embed	"
			html += "	type=\"application/x-shockwave-flash\"	"
			html += "	id=\"" + objID2 + "\"	"
			html += "	name=\"" + objID2 + "\"	"
			html += "	src=\"" + player + "\" 	"
			html += "	width=\"" + str(width) + "\" 	"
			html += "	height=\"" + str(height) + "\"	"
			html += "	allowscriptaccess=\"always\" 	"
			html += "	allowfullscreen=\"true\"	"
			html += "	wmode=\"opaque\"	"
			html += "	flashvars=\"" + vars + "\" 	"
			html += "/>	"
			html += "</object>	"
						
			return html
	
	