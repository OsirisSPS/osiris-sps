import os
import osiris
import re

import globalvars

class OmlAnubis(osiris.IOMLCode):
	def __init__(self, tag):
		osiris.IOMLCode.__init__(self, tag)
	
	def generateIFrame(self, url, context, width, height, fullLink):
		fullUrl = "http://www.osiris-sps.org" + url + "?lang=" + context.page.getLanguage();
		html = "";
		if(fullLink):
			html += context.page.parseOml("[div style=\"text-align:right;padding:3px;font-size:0.8em\"][a target=\"_blank\" href=\"" + fullUrl + "\"][lang inv=\"Open in new window\" it=\"Apri in una nuova finestra\"/][/a][/div]")
		html += "<div style=\"text-align:center\"><iframe id=\"anubisframe_" + url + "\" style=\"border:1px solid #CCCCCC;margin:auto;padding:0px;\" width=\"" + width + "\" height=\"" + height + "\" src=\"" + fullUrl + "\"></iframe></div>"			
		
		
		
		html += "<script type=\"text/javascript\">\n";
		html += "function anubisResizeIframe(dynheight)\n";
		html += "{\n";
		html += "	document.getElementById(\"anubisframe_" + url + "\").height=parseInt(dynheight)+40;\n";
		html += "}\n";
		html += "</script>";
 
				
		return html
			
	def generateIFrameAnubis(self, name, context, width, height, fullLink):
		return self.generateIFrame("/anubis/" + name + ".php", context, width, height, fullLink)			
		
	def processHtml(self, item, context):
	
		# NO CRASH
		#p = context.getPage()
		#l = p.getLanguage()
		#return l
		
		# NO CRASH
		#return context.page.getLanguage()
		
		# CRASH
		#return context.page.language
				
				
				
		page = item.getParam("page")
		
		if (page == "chat"):
			return self.generateIFrameAnubis("chat", context, "99%","420", True)
		if (page == "id_generator"):
			return self.generateIFrameAnubis("id_generator", context, "99%","250", False)
		if (page == "translations"):
			return self.generateIFrameAnubis("translations", context, "99%","700", True)
		if (page == "stats"):
			return self.generateIFrameAnubis("stats", context, "99%","700", True)
		if (page == "download"):
			return self.generateIFrameAnubis("download", context, "99%","700", False)

		if (page == "forums"):
			return self.generateIFrame("/forums", context, "99%","700", True)
			
		if (page == "wiki"):
			return self.generateIFrame("/wiki", context, "99%","700", True)
			
		if (page == "websvn"):
			return self.generateIFrame("/websvn", context, "99%","700", True)