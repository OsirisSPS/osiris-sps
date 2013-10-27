import os
import osiris

import core

import snippets

import omlhtml
import pagehtml


class MainHandler(osiris.IHttpDirectoryHandler):
	def __init__(self):
		osiris.IHttpDirectoryHandler.__init__(self)
		
	def processRequest(self, session, path):
	
		relPage = path.getUrl()
		
		page = None
	
		if(relPage == "htmlsnippets"):
			page = pagehtml.Page(session)	
			
		if(page != None):
			page.transmit()				
			return True
		else:
			return False

def load(args):	
	core.extension = args[0]
	core.mainHandler = MainHandler()
		
	core.omlHtml = omlhtml.OmlHtml("html")
	
	osiris.OMLManager.instance().add(core.omlHtml)
	osiris.PortalsSystem.instance().mainDirectory.addHandler(core.mainHandler);
	
	core.extension.enterUrl = "/main/htmlsnippets"
	
	# Register Snippets
	core.snippetsList.append(snippets.HtmlYouTube())
	core.snippetsList.append(snippets.HtmlYouTubeOld())
	core.snippetsList.append(snippets.HtmlGoogleMaps())
	core.snippetsList.append(snippets.HtmlVimeo())
	core.snippetsList.append(snippets.HtmlVimeoOld())
	core.snippetsList.append(snippets.HtmlIntenseDebateBadge())
	core.snippetsList.append(snippets.HtmlIntenseDebateWidget())
	core.snippetsList.append(snippets.HtmlIntenseDebateDiscussion())
	core.snippetsList.append(snippets.HtmlMegaVideo())
	core.snippetsList.append(snippets.HtmlMibbit())
	core.snippetsList.append(snippets.HtmlJamendo())
	core.snippetsList.append(snippets.HtmlDisqus())
	core.snippetsList.append(snippets.HtmlDailyMotion())
	core.snippetsList.append(snippets.HtmlMetaCafe())
	core.snippetsList.append(snippets.HtmlInternetArchive())
	core.snippetsList.append(snippets.HtmlLiveStream())
	core.snippetsList.append(snippets.HtmlSoundCloud())
	
	core.snippetsList.append(snippets.HtmlXVideos())
	core.snippetsList.append(snippets.HtmlPornHub())
	core.snippetsList.append(snippets.HtmlTube8())
	core.snippetsList.append(snippets.HtmlPornTube())
	core.snippetsList.append(snippets.HtmlSlutload())
	core.snippetsList.append(snippets.HtmlTnaFlix())
	core.snippetsList.append(snippets.HtmlRedTube())
	core.snippetsList.append(snippets.HtmlEmpflix())
	
	core.snippetsList.append(snippets.HtmlGeneric())
	
def unload(args):	
	
	osiris.PortalsSystem.instance().mainDirectory.removeHandler(core.mainHandler);
	osiris.OMLManager.instance().remove(core.omlHtml)