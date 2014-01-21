import os
import osiris

import globalvars

class OmlRotator(osiris.OMLHtmlWrapper):
	def __init__(self, tag):
		osiris.OMLHtmlWrapper.__init__(self, tag, "div", False, "", "", "")				
		
	def processHtml(self, item, context):
	
		extensionID = globalvars.extension.getID().getString()
		
		context.page.addJavascript("/htdocs/js/oml/rotator.js")
		
		item.setParam("id","rotator_" + osiris.UniqueID.generate().getString())
		
		script = "<script type=\"text/javascript\">Rotator.init('" + item.getParam("id") + "');</script>";		
			
		return osiris.OMLHtmlWrapper.processHtml(self, item, context) + script;
		