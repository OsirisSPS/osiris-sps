import osiris
import os
import osiris.events

class Page(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)
		self.ajax = (session.request.getUrlParam("act") != "")
				
	def getPageName(self):
		return "portal.pages.peers"	
		
	def isMcpModeRequired(self):
		return True
		
	def isIsisAllowed(self):
		return False
		
	def onLoad(self):	
		osiris.IPortalPage.onLoad(self)
		
		self.act = self.session.request.getUrlParam("act")
		if(self.act == ""):
			self.act = "home"		
				
		self.document = osiris.XMLDocument()
		self.root = self.document.create(self.act)
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "peers.xsl"))
		template.document = self.document
		
		self.document.root.setAttributeString("page_url",self.request.rawUrl);
		
		
		if(self.act == "home"):		
		
			self.addPeerIP = osiris.HtmlTextBox()
			self.addPeerIP.id = "addPeerIP"		
			self.addPeerIP.css = "os_input_full"
			template.addChildParam(self.addPeerIP)
			
			self.addPeerPort = osiris.HtmlTextBox()
			self.addPeerPort.id = "addPeerPort"		
			self.addPeerPort.css = "os_input_full"
			template.addChildParam(self.addPeerPort)
			
			self.addPeerCommand = osiris.IdeButton(self.getText("common.actions.add"))
			self.addPeerCommand.id = "addPeerCommand"
			osiris.events.connect(self.addPeerCommand.eventClick, self.onAddPeer)
			template.addChildParam(self.addPeerCommand)
			
		#if(self.act == "your"):
		#				
		#	client = osiris.Engine.instance().createHttpClient()		
		#		
		#	url = "check.php?port=" + str(osiris.Options.instance().getServerPort()) + "&output=xml";
		#	if(osiris.Options.instance().getOptionBool("p2p.enable") == False):
		#		url += "&notest";
		#	client.perform(osiris.HttpUrl(osiris.Options.instance().getIsisLink(url)))
		#	#osiris.LogManager.instance().log(osiris.Options.instance().getIsisLink(url))
		#	self.document.parseBuffer(client.response.content.content)						
		#	self.document.root.setAttributeBool("p2p_enabled",osiris.Options.instance().getOptionBool("p2p.enable"))
						
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)		
	
	def onPreRender(self):
		osiris.IPortalPage.onPreRender(self)
		
		if(self.act == "home"):		
			self.portal.peersManager.exportXML(self.document.root)
		
			
	def onAddPeer(self, args):
		
		ip = self.addPeerIP.value
		port = self.addPeerPort.value
		
		if(ip == ""):
			self.showError(self.getText("portal.pages.peers.error.invalidIp"))
			return
			
		if(port == ""):
			self.showError(self.getText("portal.pages.peers.error.invalidPort"))
			return
		
		endpoint = osiris.IPAddress()
		if(endpoint.setEndpoint(ip, int(port)) == False):
			self.showError(self.getText("portal.pages.peers.error.invalidIp"))
			return
		
		if(self.portal.getPeersManager().savePeer(endpoint, True, "manual")):
			self.showMessage(self.getText("portal.pages.peers.message.peerInserted"))			
		else:
			self.showError(self.getText("portal.pages.peers.message.error.cannotInsertPeer"))
			return
			

def main(args):
	page = Page(args[0])
	page.transmit()
