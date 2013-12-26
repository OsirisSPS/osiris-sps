import os
import osiris

class Page(osiris.IPortalPage):
	def __init__(self, session):
		osiris.IPortalPage.__init__(self, session)
		
		if session.request.getUrlParam("mode") == "dialog":
			self.ajax = True
				
	def getPageName(self):
		return "portal.pages.stabilization_stats"
		
	def onLoad(self):
		osiris.IPortalPage.onLoad(self)		
		
		document = osiris.XMLDocument()
		self.root = document.create("stats")
		template = osiris.HtmlXSLControl()
		template.stylesheet = self.loadStylesheet(os.path.join(os.path.dirname(__file__), "stabilization_stats.xsl"))
		template.document = document
		
		if(self.ajax):
			self.controls.add(template)
		else:
			self.getArea(osiris.pageAreaContent).controls.add(template)

		document.root.setAttributeString("acp_href", self.portal.getLink("acp"))
		
		document.root.setAttributeString("trash_href", self.portal.getLink("trash"))
			
		document.root.setAttributeString("mode", self.request.getUrlParam("mode"))		
		
		document.root.setAttributeString("machine_id", osiris.Engine.instance().getMachineID())
		
		document.root.setAttributeString("align_hash", self.portal.options.getAlignHash().getString())
		
		document.root.setAttributeString("acceptable_hash", self.portal.optionsShared.getAcceptableHash())
		

		document.root.setAttributeString("total_objects", self.database.getConnection().queryValue("select count(*) from os_entries").getString())
		document.root.setAttributeString("unchecked_objects", self.database.getConnection().queryValue("select count(*) from os_entries where rank=-2").getString())
		document.root.setAttributeString("trash_objects", self.database.getConnection().queryValue("select count(*) from os_entries where rank=-1").getString())
		
		self.query = osiris.IdeTableQuery()
		self.query.id = "stats_table"
		
		sql = "select "
		sql += "(select count(*) from os_entries) as objects_total, "
		sql += "(select count(*) from os_snapshot_objects) as entities_total, "
		sql += "(select count(*) from os_snapshot_objects where visible=0) as entities_invisible, "
		sql += "(select count(*) from os_entries where rank<0) as objects_trash, "
		sql += "(select min(stability_date) from os_snapshot_objects) as min_stab, "
		sql += "(select max(stability_date) from os_snapshot_objects) as max_stab "
		
		#self.query.setSql(sql)
		self.query.setColumnType(0,osiris.IdeTableQuery.ctString)
		self.query.setColumnType(1,osiris.IdeTableQuery.ctString)
		self.query.setColumnType(2,osiris.IdeTableQuery.ctString)
		self.query.setColumnType(3,osiris.IdeTableQuery.ctString)
		self.query.setColumnType(4,osiris.IdeTableQuery.ctShortDateTime)		
		self.query.setColumnType(5,osiris.IdeTableQuery.ctShortDateTime)		
		#template.addChildParam(self.query)	
				
		dataTrashReason = osiris.DataTable()
		sql = "select accept_msg, count(*) n from os_entries group by accept_msg"
		self.database.execute(sql, dataTrashReason)
		nodeTrashReason = document.root.nodes.add("trash_reasons")
		for r in range(dataTrashReason.rows()):
			nodeReason = nodeTrashReason.nodes.add("reason")
			nodeReason.setAttributeString("id", dataTrashReason.get(r,"accept_msg").getString())
			nodeReason.setAttributeString("n", dataTrashReason.get(r,"n").getString())			

		nodeObjectsTypes = document.root.nodes.add("objects_types")

		self.queryObjectType("user",1,nodeObjectsTypes)		

		self.queryObjectType("section",3,nodeObjectsTypes)
		self.queryObjectType("instance",4,nodeObjectsTypes)
		self.queryObjectType("text",5,nodeObjectsTypes)
		self.queryObjectType("file",8,nodeObjectsTypes)
		self.queryObjectType("model",15,nodeObjectsTypes)
		self.queryObjectType("poll",12,nodeObjectsTypes)
		self.queryObjectType("poll_option",13,nodeObjectsTypes)		
		self.queryObjectType("calendar_event",16,nodeObjectsTypes)
		#self.queryObjectType("attachment",18,nodeObjectsTypes)
		#self.queryObjectType("tag",9,nodeObjectsTypes)
		self.queryObjectType("post",6,nodeObjectsTypes)

		self.queryObjectType("reputation",2,nodeObjectsTypes)
		self.queryObjectType("avatar",7,nodeObjectsTypes)
				
		#self.queryObjectType("attribute",10,nodeObjectsTypes)
		#self.queryObjectType("privatemessage",11,nodeObjectsTypes)
		
		self.queryObjectType("vote",14,nodeObjectsTypes)
		self.queryObjectType("poll_vote",17,nodeObjectsTypes)
		
		
		
		
		
		#self.tableTrashReason = osiris.IdeTableQuery()
		#self.tableTrashReason.id = "table_trash_reason"
		#self.tableTrashReason.setSql(sql)
		#template.addChildParam(self.tableTrashReason)	
	
	def queryObjectType(self, objectTypeName, objectTypeCode, node):
		nodeType = node.nodes.add("object_type")	
		nodeType.setAttributeString("type_code", objectTypeCode)
		nodeType.setAttributeString("name", objectTypeName)
		nodeType.setAttributeString("total", self.database.getConnection().queryValue("select count(*) from os_entries where type=" + str(objectTypeCode)).getString())
		nodeType.setAttributeString("unchecked", self.database.getConnection().queryValue("select count(*) from os_entries where rank=-2 and type=" + str(objectTypeCode)).getString())
		nodeType.setAttributeString("trash", self.database.getConnection().queryValue("select count(*) from os_entries where rank=-1 and type=" + str(objectTypeCode)).getString())
		
	def onPreRender(self):
		osiris.IPortalPage.onPreRender(self)		

		
