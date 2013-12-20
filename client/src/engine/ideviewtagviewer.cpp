// <osiris_sps_source_header>
// This file is part of Osiris Serverless Portal System.
// Copyright (C)2005-2012 Osiris Team (info@osiris-sps.org) / http://www.osiris-sps.org )
//
// Osiris Serverless Portal System is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Osiris Serverless Portal System is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Osiris Serverless Portal System.  If not, see <http://www.gnu.org/licenses/>.
// </osiris_sps_source_header>

#include "stdafx.h"
#include "ideviewtagviewer.h"

#include "convert.h"
#include "conversions.h"
#include "iportaldatabase.h"
#include "dbsqlselect.h"
#include "dbtables.h"
#include "entitiesentity.h"
#include "entitiessnapshotmanager.h"
#include "ideskin.h"
#include "objectstag.h"
#include "idesession.h"
#include "portalsportal.h"
#include "httprequest.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace view {

//////////////////////////////////////////////////////////////////////

TagViewer::TagViewer(shared_ptr<EntitiesEntity> entity) : ControlBase(entity)
{

}

TagViewer::~TagViewer()
{

}

String TagViewer::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("tag_viewer.xsl"));
}

void TagViewer::onLoad()
{
	ControlBase::onLoad();

	shared_ptr<ObjectsTag> tag = objects_tag_cast(getObject());
	if(tag == nullptr)
		return;

	shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(getDocument()->create(tag->getDescriptor()->getTypeName()), getPage(), XMLPortalExporter::emFull));
	tag->exportXML(exporter);

	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::SNAPSHOT_OBJECTS_TABLE));
	select->fields.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::PARENT, DBTABLES::SNAPSHOT_OBJECTS_TABLE));

	select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::VISIBLE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), Convert::toSQL(true));
	select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::TYPE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), Convert::toSQL(static_cast<uint32>(portalObjectTypeTag)));

	// Crea la join tra la tabella di stabilit e quella dei tags
	select->tables.add(DbSqlTable(DBTABLES::TAGS_TABLE));
	select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::CURRENT, DBTABLES::SNAPSHOT_OBJECTS_TABLE), DbSqlField(DBTABLES::TAGS::ID, DBTABLES::TAGS_TABLE));

	// Imposta la condizione che il nome del tag
	select->where.add(DbSqlField(DBTABLES::TAGS::NAME, DBTABLES::TAGS_TABLE), Convert::toSQL(tag->name));

	select->count = true;

	DataTable result;
	if(getDatabase()->execute(select, result))
	{
		uint32 total_items = *result[0][0];
		if(total_items > 0)
		{
			select->count = false;

			// Imposta l'ordinamento per data di creazione
			DbSqlField submitdate_field(DBTABLES::SNAPSHOT_OBJECTS::SUBMIT_DATE, DBTABLES::SNAPSHOT_OBJECTS_TABLE);
			submitdate_field.setOrder(DbSqlField::foDesc);
			select->orderBy.fields.add(submitdate_field);

			// Calcola l'offset di partenza
			uint32 offset = conversions::from_utf16<uint32>(getRequest()->getUrlParam(OS_URL_PARAM_OFFSET));
			// Calcola il numero di elementi per pagina
			uint32 items_to_show = getSkin()->getPagerItems();

			// Corregge l'offset se necessario
			adjustOffset(total_items, items_to_show, offset);

			// Imposta il range di selezione
			select->limit.setRange(offset, items_to_show);

			result.clear();
			if(getDatabase()->execute(select, result))
			{
				shared_ptr<XMLNode> node_objects;

				uint32 rows = result.rows();
				for(uint32 i = 0; i < rows; i++)
				{
					shared_ptr<EntitiesEntity> tagged_entity = getPortal()->getEntity(getDatabase(), static_cast<String>(*result[i][0]).to_ascii());
					if(tagged_entity != nullptr)
					{
						if(node_objects == nullptr)
							node_objects = exporter->getNode(OS_PORTAL_OBJECT_OBJECT_GROUPNAME);

						shared_ptr<XMLPortalExporter> objectExporter(OS_NEW XMLPortalExporter(node_objects->addChild(OS_PORTAL_OBJECT_OBJECT_TYPENAME), exporter->getPage(), XMLPortalExporter::emLite));
						tagged_entity->exportXML(objectExporter);
					}
				}
			}

			// Crea il pager
			createPager(getSkin(), exporter->getRoot(), total_items, items_to_show, offset);
		}
	}
}

//////////////////////////////////////////////////////////////////////

} // view
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
