import os
import osiris

def load(page):
	document = osiris.XMLDocument()
	document.parseFile(os.path.join(os.path.dirname(__file__), "about.xml"))
	template = osiris.HtmlXSLControl()
	template.stylesheet = page.loadStylesheet(os.path.join(os.path.dirname(__file__), "about.xsl"))
	template.document = document
	
	mode = page.session.request.getUrlParam("mode")
	filter = page.session.request.getUrlParam("filter")
	
	document.root.attributes.set("mode", mode)
	document.root.attributes.set("filter", filter)
	document.root.attributes.set("version", osiris.Engine.instance().getVersionName(False))
	document.root.attributes.set("href_home", osiris.Options.instance().getIsisLink("link.php", "id=home"))
	
	if( (filter == "") or (filter == "changelog") ):
		pathChangelog = os.path.join(osiris.Options.instance().getSharePath() , "changelog.txt")
		changelog = osiris.TextFile.readFile(pathChangelog)
		document.root.attributes.set("changelog", changelog)
		
	if( (filter == "") or (filter == "license") ):
		culture = osiris.LanguageManager.instance().getCulture(page.getLanguage())
		while(culture != None):			
			pathLicense = os.path.join(osiris.Options.instance().getSharePath(), "license/" + culture.getID() + ".txt");			
			license = osiris.TextFile.readFile(pathLicense)
			if(license != ""):
				document.root.attributes.set("license", license)
				break
				
			culture = culture.parent
		
	if(page.getMcpMode()):
		if( (filter == "") or (filter == "sysinfo") ):
			nodeInstaller = document.root.addChild("installer")
			nodeInstaller.attributes.set("folders.share", osiris.Options.instance().getSharePath())
			nodeInstaller.attributes.set("folders.data", osiris.Options.instance().getDataPath())
			nodeInstaller.attributes.set("folders.temp", osiris.Options.instance().getTempPath())
			nodeInstaller.attributes.set("folders.log", osiris.Options.instance().getLogPath())
			
	if( (filter == "") or (filter == "libraries") ):
		nodeLibraries = document.root.addChild("libraries")
		
		libraries = osiris.ThirdPartyLibrariesReporter.instance().libraries
		for library in libraries:
			nodeLibrary = osiris.XMLNode()
			nodeLibrary.name = "library"
			
			nodeLibrary.attributes.set("name", library.name)
			if(library.version != ""):
				nodeLibrary.attributes.set("version", library.version)
			if(library.description != ""):
				nodeLibrary.attributes.set("description", library.description)
			if(library.url != ""):
				nodeLibrary.attributes.set("home", library.url)
				#nodeLibrary.attributes.set("href_home", osiris.PortalsSystem.instance().getExternalLink(library.url, True))
				nodeLibrary.attributes.set("href_home", library.url)
			if(library.license != ""):
				nodeLibrary.attributes.set("license", library.license)				
			if(library.licenseUrl != ""):
				#nodeLibrary.attributes.set("href_license", osiris.PortalsSystem.instance().getExternalLink(library.licenseUrl, True))
				nodeLibrary.attributes.set("href_license", library.licenseUrl)
			
			nodeLibraries.addChild(nodeLibrary)
		
		
	if(filter == "general"):
		page.pathway.add(page.getText("main.pages.about.title.general"), page.request.rawUrl)
	if(filter == "changelog"):
		page.pathway.add(page.getText("main.pages.about.title.changelogs"), page.request.rawUrl)
	if(filter == "license"):
		page.pathway.add(page.getText("main.pages.about.title.license"), page.request.rawUrl)
	if(filter == "libraries"):
		page.pathway.add(page.getText("main.pages.about.title.third_parties"), page.request.rawUrl)
	if(filter == "sysinfo"):
		page.pathway.add(page.getText("main.pages.about.title.system_info"), page.request.rawUrl)
	
	if(page.ajax):
		page.controls.add(template)
	else:
		page.getArea(osiris.pageAreaContent).controls.add(template)

class MainPage(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		
		if session.request.getUrlParam("mode") == "dialog":
			self.ajax = True
				
	def getPageName(self):
		return "portal.pages.about"
		
	def getPagePathway(self):
		return osiris.PortalsSystem.instance().getMainLink((self.getPageName()))
		
	def onLoad(self):
		osiris.IMainPage.onLoad(self)		
		
		load(self)
		
class PortalPage(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)
		
		if session.request.getUrlParam("mode") == "dialog":
			self.ajax = True
				
	def getPageName(self):
		return "portal.pages.about"
		
	def getPagePathway(self):
		return self.portal.getLink(self.getPageName())
		
	def onLoad(self):
		osiris.IPortalPage.onLoad(self)		
		
		load(self)