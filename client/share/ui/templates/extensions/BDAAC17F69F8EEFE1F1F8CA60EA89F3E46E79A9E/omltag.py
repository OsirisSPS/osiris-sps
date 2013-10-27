import os
import osiris

import globalvars

class OmlTag(osiris.IOMLCode):
	def __init__(self, tag):
		osiris.IOMLCode.__init__(self, tag)		
		
	def processHtml(self, item, context):
		
		extensionID = globalvars.extension.getID().getString()
		
		return "<i>It works.</i>";
		