import os
import osiris

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		
		if( (session.request.getUrlParam("mode") == "dialog") or (session.request.getUrlParam("mode") == "content") ):
			self.ajax = True
		
	def isMcpModeRequired(self):
		return False
				
	def getPageName(self):
		return "main.pages.network"
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)		
		
		document = osiris.XMLDocument()
		root = document.create("help")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "network.xsl"))
		template.document = document
		
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)
			
		if(self.request.getUrlParam("force") == "true"):
			osiris.IsisSystem.instance().doTask(True)			
			
		document.root.setAttributeString("mode", self.request.getUrlParam("mode"))
			
		document.root.setAttributeDateTime("last_request", osiris.IsisSystem.instance().getLastRequest())
		document.root.setAttributeString("last_error", osiris.IsisSystem.instance().getLastError())
		document.root.setAttributeString("ip", osiris.IsisSystem.instance().getIp())
		document.root.setAttributeInt32("port", osiris.Options.instance().getServerPort())
		document.root.setAttributeString("country_code", osiris.IsisSystem.instance().getCountryCode())
		document.root.setAttributeString("country_name", osiris.IsisSystem.instance().getCountryName())
		document.root.setAttributeBool("reachable", osiris.IsisSystem.instance().getReachable())
		document.root.setAttributeInt32("tor", osiris.IsisSystem.instance().getTor())
		
		if(osiris.IsisSystem.instance().getLastValidationDate().isNull() == False):
			document.root.setAttributeDateTime("last_validation_date", osiris.IsisSystem.instance().getLastValidationDate())
			
		document.root.setAttributeDateTime("last_check_date", osiris.IsisSystem.instance().getLastCheckDate())
		
		document.root.setAttributeBool("p2p_enabled",osiris.Options.instance().getOptionBool("p2p.enable"))
		
		
		dta = osiris.IsisSystem.instance().isInternetDateTimeAvailable()
		document.root.setAttributeBool("internet_datetime_available", dta)		
		if(dta):
			dts = osiris.DateTime.now()
			dti = osiris.IsisSystem.instance().getInternetDateTime()
			delta = osiris.TimeDuration(dts, dti)
			document.root.setAttributeDateTime("internet_datetime", dti)				
			document.root.setAttributeDateTime("system_datetime", dts)				
			document.root.setAttributeInt32("internet_datetime_delta", delta.getTotalSeconds())				
			document.root.setAttributeString("internet_datetime_sync_method", osiris.IsisSystem.instance().getInternetDateTimeSyncMethod())
				
def main(args):
	page = Page(args[0])
	page.transmit()
