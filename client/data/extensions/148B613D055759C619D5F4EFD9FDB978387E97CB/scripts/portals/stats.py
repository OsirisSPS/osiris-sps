import os
import osiris

class Page(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)
		
		if session.request.getUrlParam("mode") == "dialog":
			self.ajax = True
				
	def getPageName(self):
		return "portal.pages.stats"
		
	def onLoad(self):
		osiris.IPortalPage.onLoad(self)		
		
		document = osiris.XMLDocument()
		self.root = document.create("stats")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "stats.xsl"))
		template.document = document
		
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)
			
		document.root.setAttributeString("mode", self.request.getUrlParam("mode"))		
		
		document.root.setAttributeString("machine_id", osiris.Engine.instance().getMachineID())
		
		document.root.setAttributeString("align_hash", self.portal.options.getAlignHash().getString())
		
		document.root.setAttributeString("acceptable_hash", self.portal.optionsShared.getAcceptableHash())
		
		
		self.query = osiris.IdeTableQuery()
		self.query.id = "stats_table"
		
		sql = "select "
		sql += "(select count(*) from os_entries) as objects_total, "
		sql += "(select count(*) from os_snapshot_objects) as entities_total, "
		sql += "(select count(*) from os_snapshot_objects where visible=0) as entities_invisible, "
		sql += "(select count(*) from os_entries where rank<0) as objects_trash, "
		sql += "(select min(stability_date) from os_snapshot_objects) as min_stab, "
		sql += "(select max(stability_date) from os_snapshot_objects) as max_stab "
		
		self.query.setSql(sql)
		self.query.setColumnType(0,osiris.IdeTableQuery.ctString)
		self.query.setColumnType(1,osiris.IdeTableQuery.ctString)
		self.query.setColumnType(2,osiris.IdeTableQuery.ctString)
		self.query.setColumnType(3,osiris.IdeTableQuery.ctString)
		self.query.setColumnType(4,osiris.IdeTableQuery.ctShortDateTime)		
		self.query.setColumnType(5,osiris.IdeTableQuery.ctShortDateTime)		
		template.addChildParam(self.query)	

		dataTrashReason = osiris.DataTable()
		sql = "select accept_msg, count(*) from os_entries group by accept_msg"
		self.database.execute(sql, dataTrashReason)

		osiris.LogManager.instance().log(str(dataTrashReason.rows()))

		for r in range(dataTrashReason.rows()):
			osiris.LogManager.instance().log(str(r))
			osiris.LogManager.instance().log(dataTrashReason.get(r,0).getString())
			#osiris.LogManager.instance().log("record")
		
		self.tableTrashReason = osiris.IdeTableQuery()
		self.tableTrashReason.id = "table_trash_reason"
		self.tableTrashReason.setSql(sql)
		template.addChildParam(self.tableTrashReason)	
		
		
	def onPreRender(self):
		osiris.IPortalPage.onPreRender(self)		

		
