import globalvars
import os
import osiris
#import osiris.events

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		self.ajax = (session.request.getUrlParam("act") != "")
		
		#osiris.LogManager.instance().log("page addons constructor")
		
	def getPageName(self):
		return "main.pages.addons"
		
	def getIsisUrl(self, action, params = ""):
		url = "addons.php?act=" + action
		if(params != ""):
			url += "&" + params
		url = osiris.Options.instance().getIsisSecureLink(str(url))
		#osiris.LogManager.instance().log("Isis url:" + url)
		return url
		
	def processRemoteList(self, node, online):
		if(node != None):			
			for i in range(0,node.nodes.size()):
				self.processExtension(node.nodes.find(i), online, "remote", True, False)
		
	def processExtension(self, node, online, mode, showActions, showTrust):
		id = str(node.getAttributeString("id"))
				
		required = False
		
		if(id == "148B613D055759C619D5F4EFD9FDB978387E97CB"):	# Osiris Web Interface
			required = True
		if(id == "F86BDAADDDAE54753F3DC7608925780632CE4BF1"):	# Language Invariant
			required = True
		if(id == "DB535FE72E91BD84ADA5D46745F2121E1D058402"):	# Language English
			required = True
		if(id == "AF4E66017B2F4E240E47EA9240F6F7E8C9EF7D78"):	# Osiris Advertising
			required = True
				
		# Installed version
		extension = osiris.ExtensionsSystem.instance().getExtension(id)			
		
		# Repository version
		nodeRepository = None
		if(node.document.root.getNode("repository") != None): # "Home" xml
			nodeRepoItems = node.document.root.getNode("repository").nodes
			for i in range(0,nodeRepoItems.size()):			
				if(nodeRepoItems.find(i).getAttributeString("id") == id):
					nodeRepository = nodeRepoItems.find(i)
					break
			
		else:	# "Single" xml
			nodeRepository = node			
			
				
		remote = ( online and nodeRepository != None and (nodeRepository.getAttributeString("found") == "true") )
		installed = (extension != None)	
		
		if(remote == False):
			mode = "local"
				
		upgradable = ( (extension) and (remote) and (extension.versionCode < nodeRepository.getAttributeInt32("version_code")) )	
		
		#if(extension):
		#	osiris.LogManager.instance().log(extension.name + "=" + str(upgradable))
		
		if(extension):
			node.setAttributeString("active", 'true' if extension.isActive() else 'false')
			node.setAttributeString("version_installed", extension.versionName)			
		if(remote):
			node.setAttributeString("version_repository", nodeRepository.getAttributeString("version_name"))
			
		node.setAttributeBool("installed", installed)
		
		if(mode == "local"):			
			
			if(installed):
				
				node.setAttributeString("name",extension.name)
				node.setAttributeString("description",extension.description)			
				node.setAttributeString("content",extension.content)
				node.setAttributeString("category",extension.category)
				node.setAttributeString("tags",extension.tags)
				if(showTrust):					
					node.setAttributeString("verified","true")
					node.setAttributeString("trust",extension.trust)					
				else:
					node.setAttributeString("verified","true")
					node.setAttributeString("trust","")
				node.setAttributeString("author",extension.author)
				node.setAttributeString("compatibility",extension.compatibility)
				node.setAttributeString("homepage",extension.homePage)
				if(extension.icon != ""):
					node.setAttributeString("icon_href", "/main/addons?act=icon&id=" + id)
				if(extension.logo != ""):
					node.setAttributeString("logo_href", "/main/addons?act=logo&id=" + id)
		elif(mode == "remote"):
			node.setAttributeString("name",nodeRepository.getAttributeString("name"))
			node.setAttributeString("description",nodeRepository.getAttributeString("description"))
			node.setAttributeString("content",nodeRepository.getAttributeString("content"))
			node.setAttributeString("category",nodeRepository.getAttributeString("category"))
			node.setAttributeString("tags",nodeRepository.getAttributeString("tags"))
			if(showTrust):
				node.setAttributeString("verified",nodeRepository.getAttributeString("verified"))
				node.setAttributeString("trust",nodeRepository.getAttributeString("trust"))
			else:
				node.setAttributeString("verified","true")
				node.setAttributeString("trust","")
			node.setAttributeString("experimental",nodeRepository.getAttributeString("experimental"))
			node.setAttributeString("author",nodeRepository.getAttributeString("author"))
			node.setAttributeString("compatibility",nodeRepository.getAttributeString("compatibility"))
			node.setAttributeString("homepage",nodeRepository.getAttributeString("homepage"))
			node.setAttributeString("icon_href",nodeRepository.getAttributeString("icon_href"))
			node.setAttributeString("logo_href",nodeRepository.getAttributeString("logo_href"))				
		
		jobFound = False
		
		if(showActions == False):		
			# A job is running?			
			# TODO: Wrong. We need to lookup a specific job-id, otherwise we show informations from wrong, older jobs of the same extension.
			
			jobs = osiris.Engine.instance().backgroundJobs
			for job in jobs:
				if(isinstance(job,osiris.MainAddonsJob)):
					if(job.extensionID.getString() == id):
						jobFound = True
						node.setAttributeString("job_id", job.id)					
						node.setAttributeString("job_action", job.action)					
						
			jobs = osiris.Engine.instance().lastCompletedBackgroundJobs
			for job in jobs:
				if(isinstance(job,osiris.MainAddonsJob)):
					if(job.extensionID.getString() == id):
						jobFound = True
						node.setAttributeString("job_id", job.id)					
						node.setAttributeString("job_action", job.action)					
		
		if(showActions and (jobFound == False) ):				
			nodeActions = node.nodes.add("actions")
			nodeActions2 = node.nodes.add("actions2")
			
			if(installed == True):				
				
				if( (extension.isActive()) and (extension.enterUrl != "") ):
					nodeAction = nodeActions2.nodes.add("action")
					nodeAction.attributes.set("name", "enter")
					nodeAction.attributes.set("href", extension.enterUrl)
			
				if(upgradable):
					nodeAction = nodeActions.nodes.add("action")
					nodeAction.attributes.set("name", "upgrade")
					nodeAction.attributes.set("call", "/main/addons?act=upgrade&id=" + id)
				
				if(required == False):	
					nodeAction = nodeActions.nodes.add("action")
					nodeAction.attributes.set("name", "uninstall")
					nodeAction.attributes.set("call", "/main/addons?act=uninstall&id=" + id)
				
				if(required == False):
					if(extension.isActive() == False):
						nodeAction = nodeActions.nodes.add("action")
						nodeAction.attributes.set("name", "activate")
						nodeAction.attributes.set("call", "/main/addons?act=activate&id=" + id)
						
					else:
						nodeAction = nodeActions.nodes.add("action")
						nodeAction.attributes.set("name", "deactivate")
						nodeAction.attributes.set("call", "/main/addons?act=deactivate&id=" + id)					
				
			else:
				if(remote):
					nodeAction = nodeActions.nodes.add("action")
					nodeAction.attributes.set("name", "install")
					nodeAction.attributes.set("call", "/main/addons?act=install&id=" + id)
		
	def onLoad(self):
		osiris.IMainPage.onLoad(self)		
		
		if(self.ajax == False):
			self.addCss(self.skin.getResourceUrl("css/main/addons.less"))
			self.addJavascript(self.skin.getResourceUrl("js/main/addons.js"))			
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)
		
		action = self.session.request.getUrlParam("act")
		if(action == ""):
			action = "home"		
		
		document = osiris.XMLDocument()
		root = document.create(action)
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "addons.xsl"))
		
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)
					
		if( (action == "home") or (action == "upgradable") ):
			client = osiris.Engine.instance().createHttpClient()			
			mode = self.session.request.getUrlParam("mode")
			online = False
			
			document.root.addChild("lists")
			
			currents = ""
			extensions = osiris.ExtensionsSystem.instance().extensions			
			for k in extensions.keys():
				extension = extensions[k]
				if(extension.internal == False):
					#currents += extension.id.getString() + ":{0:.5f};".format(extension.versionCode);
					currents += extension.id.getString() + ":" + str(extension.versionCode) + ";";
			client.request.method = osiris.httpMethodPost
			client.request.setPostParamString("currents",currents)			
			#osiris.LogManager.instance().log("currents:" + currents)
			if(client.perform(osiris.HttpUrl(self.getIsisUrl(action)))):
				remoteDocument = osiris.XMLDocument()
				online = remoteDocument.parseBuffer(client.response.content.content)
				if(online):
					document = remoteDocument
			
			if( (online) and (action == "upgradable") and (mode == "counter") ):
				osiris.ExtensionsSystem.instance().setKnownUpgradableCounter(document.root.getAttributeInt32("upgradable_counter"))
				
			document.root.setAttributeBool("online", online)
			
			if(action == "home"):
				lists = document.root.getNode("lists")
				
				self.processRemoteList(lists.getNode("recommended"), online)
				self.processRemoteList(lists.getNode("catalog"), online)
				self.processRemoteList(lists.getNode("upgradable"), online)
			
		
				nodeInstalled = document.root.getNode("lists").addChild("installed")
				extensions = osiris.ExtensionsSystem.instance().extensions
				for k in extensions.keys():
					extension = extensions[k]
					if(extension.internal == False):
						nodeAddon = nodeInstalled.nodes.add("addon")
						nodeAddon.setAttributeString("id",extension.id.getString())					
						self.processExtension(nodeAddon, online, "local", True, False)					
			
		if(action == "addon"):
			id = self.session.request.getUrlParam("id")			
			online = False
			
			client = osiris.Engine.instance().createHttpClient()			
			if(client.perform(osiris.HttpUrl(self.getIsisUrl("single","id=" + id)))):
				remoteDocument = osiris.XMLDocument()
				online = remoteDocument.parseBuffer(client.response.content.content)
				if(online):
					document = remoteDocument
						
			mode = self.session.request.getUrlParam("mode")
			showActions = (self.session.request.getUrlParam("actions") == "yes")
			showTrust = (self.session.request.getUrlParam("trust") == "yes")
			
			osiris.LogManager.instance().log("show actions: " + self.session.request.getUrlParam("actions"))
			osiris.LogManager.instance().log(self.session.request.getRawUrl())
			
			self.processExtension(document.root, online, mode, showActions, showTrust)
				
		if( (action == "upgrade") or (action == "uninstall") or (action == "activate") or (action == "deactivate") or (action == "install") ):
			
			id = self.session.request.getUrlParam("id")
			href = "/main/addons?act=job&id=" + id + "&type=" + action;
			
			document.root.setAttributeString("id",id)
			document.root.setAttributeString("token",self.session.state.createToken())
						
		if(action == "icon"):
			id = self.session.request.getUrlParam("id")
			extension = osiris.ExtensionsSystem.instance().getExtension(str(id))
			if(extension):
				self.session.transmitFile(extension.path + "/" + extension.icon);
		
		if(action == "logo"):
			id = self.session.request.getUrlParam("id")
			extension = osiris.ExtensionsSystem.instance().getExtension(str(id))
			if(extension):
				self.session.transmitFile(extension.path + "/" + extension.logo);
				
		if(action == "job"):
			id = self.session.request.getUrlParam("id")
			jobType = self.session.request.getUrlParam("type");
			token = self.session.request.getUrlParam("token");
			
			if(self.session.state.hasToken(str(token))):			
				# osiris.LogManager.instance().log("jobs request: " + id + "," + jobType)
				
				#job = osiris.MainAddonsJob(osiris.Engine.instance().peekBackgroundJobID(), jobType)
				#job.extensionID = id
								
				#if( (jobType == "install") or (jobType == "upgrade") ):
				#	job.url = self.getIsisUrl("download","id=" + id)
				#osiris.Engine.instance().addBackgroundJob(job)
				
				url = ""
				if( (jobType == "install") or (jobType == "upgrade") ):
					url = self.getIsisUrl("download","id=" + id)
								
				osiris.Engine.instance().startExtensionsJob(jobType, str(id), url)
				
			else:
				osiris.LogManager.instance().log("Invalid token for requested addons action.")
		
		template.document = document
		
		#osiris.LogManager.instance().log("page addons completed")
		
def main(args):	
	page = Page(args[0])
	page.transmit()
	
	
