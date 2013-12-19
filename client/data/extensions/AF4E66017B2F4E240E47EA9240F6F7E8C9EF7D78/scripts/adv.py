import osiris

import urllib2
import urllib
urllib.getproxies_macosx_sysconf = lambda: {} # This fix a "No module named _ctypes" error in Mac OS-X.
from hashlib import md5
import os

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		self.ajax = True
			
	def onLoad(self):
		osiris.IMainPage.onLoad(self)		
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)
		
		client = osiris.Engine.instance().createHttpClient()		
				
		url = "http://www.osiris-sps.org/adv/osiris.php";
		if(client.perform(url) == True):
		
			html = client.response.content.toString()
			self.controls.add(osiris.HtmlLiteral(html))
