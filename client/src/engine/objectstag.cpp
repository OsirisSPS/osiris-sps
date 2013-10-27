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
#include "objectstag.h"

#include "objectssigner.h"
#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "entitiessnapshotmanager.h"
#include "ideedittageditor.h"
#include "ideviewtagviewer.h"
#include "datatree.h"
#include "searchtagoptions.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsTagDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsTag::VERSION = OS_PORTAL_OBJECT_TAG_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsTag::ObjectsTag() : RevisionableBase(portalObjectTypeTag, VERSION)
{

}

ObjectsTag::~ObjectsTag()
{

}

bool ObjectsTag::validate(shared_ptr<IPortalDatabase> database) const
{
	if(RevisionableBase::validate(database) == false)
		return false;

	if(name->empty())
		return false;

	return true;
}

const String & ObjectsTag::getTitle() const
{
	return name;
}

bool ObjectsTag::requireParent() const
{
	return true;
}

void ObjectsTag::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	RevisionableBase::exportXML(exporter);

	if(exporter->getMode() != XMLPortalExporter::emLite)
	{
		exporter->getRoot()->setAttributeString(DBTABLES::TAGS::NAME, name);
	}
}

bool ObjectsTag::onCreate(const shared_ptr<IPortalDatabase> &db, const Buffer &privateKey)
{
	// Il nome di un tag viene sempre archiviato in minuscolo
	name.value().to_lower();

	return RevisionableBase::onCreate(db, privateKey);
}

bool ObjectsTag::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(RevisionableBase::onSign(signer) == false)
		return false;

	signer->add(name);

	return true;
}

bool ObjectsTag::onRead(const ObjectID &id, const DataTree &data)
{
	if(RevisionableBase::onRead(id, data) == false)
		return false;

	name.read(data, DBTABLES::TAGS::NAME);

	return true;
}

bool ObjectsTag::onWrite(DataTree &data) const
{
	if(RevisionableBase::onWrite(data) == false)
		return false;

	name.write(data, DBTABLES::TAGS::NAME);

	return true;
}

bool ObjectsTag::onRead(const DataTableRow &row)
{
	if(RevisionableBase::onRead(row) == false)
		return false;

	name.read(row, DBTABLES::TAGS::NAME);

	return true;
}

bool ObjectsTag::onWrite(DbSqlValues &values) const
{
	if(RevisionableBase::onWrite(values) == false)
		return false;

	name.write(values, DBTABLES::TAGS::NAME);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsTagDescriptor::ObjectsTagDescriptor()
{

}

ObjectsTagDescriptor::~ObjectsTagDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsTagDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsTag());
}

String ObjectsTagDescriptor::getTableName()
{
	return DBTABLES::TAGS_TABLE;
}

String ObjectsTagDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_TAG_TYPENAME;
}

String ObjectsTagDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_TAG_GROUPNAME;
}

bool ObjectsTagDescriptor::isGroupable()
{
	return true;
}

shared_ptr<ISearchOptions> ObjectsTagDescriptor::getSearchOptions()
{
	return shared_ptr<ISearchOptions>(OS_NEW SearchTagOptions());
}

ide::edit::editor_ptr ObjectsTagDescriptor::createEditControl(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent)
{
	return ide::edit::editor_ptr(OS_NEW ide::edit::TagEditor(entity, parent));
}

ide::view::viewer_ptr ObjectsTagDescriptor::createViewControl(shared_ptr<EntitiesEntity> entity)
{
	return ide::view::viewer_ptr(OS_NEW ide::view::TagViewer(entity));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
