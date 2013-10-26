import osiris
import osiris.events

class Page(osiris.IMainPage):
	def __init__(self, session):
		osiris.IMainPage.__init__(self, session)
				
	def getText(self, name):
		return osiris.IMainPage.getText(self, name)
	
	def getPageName(self):
		return "test"
		
	def onLoad(self):
		osiris.IMainPage.onLoad(self)		
		
		link = osiris.Options.instance().getIsisLink("foo", { "p1" : "v1", "p2" : "v2" })
		self.showMessage(link)
		
		button = osiris.HtmlButton()
		button.caption = "press me"
		button.id = "test"
		button.attributes.set("alfa", "beta")
		osiris.events.connect(button.eventClick, self.onTestButton)
		self.getArea(osiris.pageAreaContent).controls.add(button)
		
	def onTestButton(self, args):
		self.showMessage(str(args.size()))
		
def main(args):
	page = Page(args[0])
	page.transmit()
