import os
import osiris
import osiris.events
import re

#import globalvars

instance = None
eventCallbackBefore = None
eventCallbackAfter = None

class Html2OsmlEx():
	def onBeforeReverseParse(self, e):
		# Add here the rules for trasforming the source -before- the conversion to OML occur.
		# Check e.format : maybe "html" or "bbc+html".
		# - html : HTML5 format. BBCode tags must be considered plain text.
		# - bbc+html : HTML5 format, but BBCode tags must be treat as BBCode. Format generally used when import contents from other bulletin-board software, like Invision.
		
		# Simple replace example
		#e.source = e.source.replace("Alfa","Beta") # Replace Example
		e.source = e.source # Remove this line after we create the first rules. It's only to avoid a python indentation error.
				
	def onAfterReverseParse(self, e):
		# Add here the rules for trasforming the final OML.
				
		# Remove advertising. Tested on RSS of punto-informatico.it
		e.oml = re.sub(r'\[a href=\"http:\/\/da.feedsportal.com(.*?)\[\/a\]','',e.oml)
		

def load(args):
	
	global instance
	global eventCallbackBefore
	global eventCallbackAfter
	
	instance = Html2OsmlEx()
	eventCallbackBefore = osiris.events.connect(osiris.OMLManager.instance().eventBeforeReverseParse, instance.onBeforeReverseParse)	
	eventCallbackAfter = osiris.events.connect(osiris.OMLManager.instance().eventAfterReverseParse, instance.onAfterReverseParse)	
	
def unload(args):

	global eventCallbackBefore
	global eventCallbackAfter
		
	eventCallbackBefore.disconnect()
	eventCallbackAfter.disconnect()
