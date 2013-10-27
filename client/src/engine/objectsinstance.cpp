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
#include "objectsinstance.h"

#include "objectssigner.h"
#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "extensionssystem.h"
#include "iextensionsmodule.h"
#include "ideeditinstanceeditor.h"
#include "ideviewinstanceviewer.h"
#include "datatree.h"
#include "searchinstanceoptions.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsInstanceDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsInstance::VERSION = OS_PORTAL_OBJECT_INSTANCE_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsInstance::ObjectsInstance() : RevisionableBase(portalObjectTypeInstance, VERSION)
{
	area = iaUnknown;
	inherit = iiParent;
}

ObjectsInstance::~ObjectsInstance()
{

}

shared_ptr<IExtensionsModule> ObjectsInstance::getModule() const
{
	return ExtensionsSystem::instance()->getModule(module);
}

TimeDuration ObjectsInstance::getCacheDuration()
{
	// TODO: fixme
	return TimeDuration::minutes(5);
}

BlockType ObjectsInstance::getBlockType() const
{
	BOOST_STATIC_ASSERT(OS_IDE_BLOCK_TYPE_NONE == 0);
	return static_cast<BlockType>(block.get());
}

void ObjectsInstance::setBlockType(BlockType type)
{
	block = static_cast<uint32>(type);
}

bool ObjectsInstance::validate(shared_ptr<IPortalDatabase> database) const
{
	if(RevisionableBase::validate(database) == false)
		return false;

	if(title->empty())
		return false;

	if(area == static_cast<uint32>(iaUnknown))
		return false;

	// TOFIX_VALIDATING
	/*
	if(module->validate(false) == false || module->getType() != ID::idModule)
		return false;
	*/

	if(data->empty() == false)
	{
		shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
		if(document->parseString(data) == false)
			return false;
	}

	return true;
}

const String & ObjectsInstance::getTitle() const
{
	return title;
}

const String & ObjectsInstance::getContent() const
{
	// URGENT: per ottenere il content bisognerebbe chiamare il modulo
	return String::EMPTY;
}

void ObjectsInstance::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	RevisionableBase::exportXML(exporter);

	shared_ptr<XMLNode> node = exporter->getRoot();

	if(exporter->getMode() == XMLPortalExporter::emFull)
	{
		node->setAttributeString(DBTABLES::INSTANCES::TITLE, title);
	}

	if(exporter->getMode() == XMLPortalExporter::emData)
	{
		node->setAttributeString(DBTABLES::INSTANCES::TITLE, title);
		node->setAttributeUint32(DBTABLES::INSTANCES::AREA, area);
		node->setAttributeUint32(DBTABLES::INSTANCES::INHERIT, inherit);
		node->setAttributeString(DBTABLES::INSTANCES::MODULE, module->toUTF16());
		node->setAttributeString(DBTABLES::INSTANCES::DATA, data);
		node->setAttributeUint32(DBTABLES::INSTANCES::BLOCK, block);
		node->setAttributeString(DBTABLES::INSTANCES::STYLE, style);
	}

	shared_ptr<IExtensionsModule> module = getModule();
	if(module != null)
		module->exportXML(getEntityID(), exporter);	
}

bool ObjectsInstance::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(RevisionableBase::onSign(signer) == false)
		return false;

	signer->add(title);
	signer->add(area);
	signer->add(inherit);
	signer->add(module);
	signer->add(data);
	signer->add(block);
	signer->add(style);

	return true;
}

bool ObjectsInstance::onRead(const ObjectID &id, const DataTree &dt)
{
	if(RevisionableBase::onRead(id, dt) == false)
		return false;

	title.read(dt, DBTABLES::INSTANCES::TITLE);
	area.read(dt, DBTABLES::INSTANCES::AREA);
	inherit.read(dt, DBTABLES::INSTANCES::INHERIT);
	module.read(dt, DBTABLES::INSTANCES::MODULE);
	data.read(dt, DBTABLES::INSTANCES::DATA);
	block.read(dt, DBTABLES::INSTANCES::BLOCK);
	style.read(dt, DBTABLES::INSTANCES::STYLE);

	return true;
}

bool ObjectsInstance::onWrite(DataTree &dt) const
{
	if(RevisionableBase::onWrite(dt) == false)
		return false;

	title.write(dt, DBTABLES::INSTANCES::TITLE);
	area.write(dt, DBTABLES::INSTANCES::AREA);
	inherit.write(dt, DBTABLES::INSTANCES::INHERIT);
	module.write(dt, DBTABLES::INSTANCES::MODULE);
	data.write(dt, DBTABLES::INSTANCES::DATA);
	block.write(dt, DBTABLES::INSTANCES::BLOCK);
	style.write(dt, DBTABLES::INSTANCES::STYLE);

	return true;
}

bool ObjectsInstance::onRead(const DataTableRow &row)
{
	if(RevisionableBase::onRead(row) == false)
		return false;

	title.read(row, DBTABLES::INSTANCES::TITLE);
	area.read(row, DBTABLES::INSTANCES::AREA);
	inherit.read(row, DBTABLES::INSTANCES::INHERIT);
	module.read(row, DBTABLES::INSTANCES::MODULE);
	data.read(row, DBTABLES::INSTANCES::DATA);
	block.read(row, DBTABLES::INSTANCES::BLOCK);
	style.read(row, DBTABLES::INSTANCES::STYLE);

	return true;
}

bool ObjectsInstance::onWrite(DbSqlValues &values) const
{
	if(RevisionableBase::onWrite(values) == false)
		return false;

	title.write(values, DBTABLES::INSTANCES::TITLE);
	area.write(values, DBTABLES::INSTANCES::AREA);
	inherit.write(values, DBTABLES::INSTANCES::INHERIT);
	module.write(values, DBTABLES::INSTANCES::MODULE);
	data.write(values, DBTABLES::INSTANCES::DATA);
	block.write(values, DBTABLES::INSTANCES::BLOCK);
	style.write(values, DBTABLES::INSTANCES::STYLE);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsInstanceDescriptor::ObjectsInstanceDescriptor()
{

}

ObjectsInstanceDescriptor::~ObjectsInstanceDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsInstanceDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsInstance());
}

String ObjectsInstanceDescriptor::getTableName()
{
	return DBTABLES::INSTANCES_TABLE;
}

String ObjectsInstanceDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_INSTANCE_TYPENAME;
}

String ObjectsInstanceDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_INSTANCE_GROUPNAME;
}

ObjectsIDescriptor::OrderMode ObjectsInstanceDescriptor::getOrderMode() const
{
	return omPosition;
}

shared_ptr<ISearchOptions> ObjectsInstanceDescriptor::getSearchOptions()
{
	return shared_ptr<ISearchOptions>(OS_NEW SearchInstanceOptions());
}

ide::edit::editor_ptr ObjectsInstanceDescriptor::createEditControl(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent)
{
	return ide::edit::editor_ptr(OS_NEW ide::edit::InstanceEditor(entity, parent));
}

ide::view::viewer_ptr ObjectsInstanceDescriptor::createViewControl(shared_ptr<EntitiesEntity> entity)
{
	return ide::view::viewer_ptr(OS_NEW ide::view::InstanceViewer(entity));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
