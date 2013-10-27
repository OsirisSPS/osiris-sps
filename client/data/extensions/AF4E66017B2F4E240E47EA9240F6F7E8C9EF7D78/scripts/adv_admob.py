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
		
	#def getPageName(self):
	#	return "advertising"
		
	def onLoad(self):
		osiris.IMainPage.onLoad(self)		
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)
		
		# Android - Nexus One
		#user_agent = "Mozilla/5.0 (Linux; U; Android 2.1; en-us; Nexus One Build/ERD62) AppleWebKit/530.17 (KHTML, like Gecko) Version/4.0 Mobile Safari/530.17"
		# iPhone
		#user_agent = "Mozilla/5.0 (iPhone; U; CPU like Mac OS X; en) AppleWebKit/420+ (KHTML, like Gecko) Version/3.0 Mobile/1A543a Safari/419.3"
		# iPad
		#user_agent = "Mozilla/5.0 (iPad; U; CPU OS 3_2 like Mac OS X; en-us) AppleWebKit/531.21.10 (KHTML, like Gecko) Version/4.0.4 Mobile/7B334b Safari/531.21.10"
		# Symbian Nokia N97
		user_agent = "Mozilla/5.0 (SymbianOS/9.4; Series60/5.0 NokiaN97-1/12.0.024; Profile/MIDP-2.1 Configuration/CLDC-1.1; en-us) AppleWebKit/525 (KHTML, like Gecko) BrowserNG/7.1.12344"
		# Real
		#user_agent = self.session.request.userAgent
		
		admob_params = {}
	
		# Required 
		#admob_params["s"] = "a14dcd2bb92d218"				# id del banner, test
		admob_params["s"] = "a14dcd450dcc134"				# id del banner, official
		admob_params["rt"] = "0"							# tipologia di richiesta 0 = richiesta ad
		admob_params["u"] = user_agent						# user agent
		admob_params["i"] = "127.0.0.1"						# ip address
		
		# Unique Machine ID
		if(hasattr(osiris.Engine.instance(),"getMachineID")):
			admob_params["o"] = osiris.Engine.instance().getMachineID() # Osiris >= 0.15
		else:
			admob_params["o"] = "de9b5e23ad4fb248b6bf151a34d56d26"	# Osiris < 0.15
			
		#admob_params["h[Referer]"] = "http://www.osiris-sps.org"		# headers
		
		# Recommended
		admob_params["e"] = "UTF-8"						# output encoding
		admob_params["v"] = "20071126-PHP-1234567890123456"	# per android 20100322-ANDROID-3312276cc1406347
		#admob_params["f"] = "jsonp"
		#admob_params["p"] = "http://127.0.0.1"
		#admob_params["t"] = md5("123123").hexdigest()
		admob_params["k"] = "cars sports baseball golf"
		#admob_params["m"] = "test"	
		
		opener = urllib2.build_opener()
		#opener.addheaders = [('User-agent', user_agent)]
		data = opener.open("http://r.admob.com/ad_source.php", urllib.urlencode(admob_params)).read()
		
		self.controls.add(osiris.HtmlLiteral(str(data)))		
		
		
def main(args):	
	page = Page(args[0])
	page.transmit()
