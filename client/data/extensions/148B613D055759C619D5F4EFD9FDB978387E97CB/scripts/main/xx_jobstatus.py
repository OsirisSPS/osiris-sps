import os
import osiris

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
		#self.ajax = True
				
	def getPageName(self):
		return "jobstatus"
		
	def onPreRender(self):
		osiris.IMainPage.onPreRender(self)	
		
		helpID = self.session.request.getUrlParam("id")	
		
		
		xjob = osiris.MainAddonsJob(osiris.Engine.instance().peekBackgroundJobID(), "uninstall", "xxx")
		osiris.Engine.instance().addBackgroundJob(xjob)
		
		document = osiris.XMLDocument()
		root = document.create(self.getPageName())
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "jobstatus.xsl"))
		template.document = document
		self.controls.add(template)
		
		#jobs = osiris.Engine.instance().backgroundJobs;
		jobs = osiris.Engine.instance().lastCompletedBackgroundJobs;
		
		for job in jobs:
			self.showMessage(str(type(job)))			
			self.showMessage(isinstance(job,osiris.IPortalBackgroundJob))
			#self.showMessage(repr(type(job).__name__))			
			#self.showMessage(job.details.getName())
			#self.controls.add(osiris.HtmlText(job.name))
			#self.controls.add(osiris.HtmlLiteral("<hr>"))
		
		#document.root.setAttributeString("id", helpID)
		#document.root.setAttributeString("title", title)
		
def main(args):
	page = Page(args[0])
	page.transmit()


