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
#include "objectssection.h"

#include "objectssigner.h"
#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "extensionssystem.h"
#include "iextensionscomponent.h"
#include "ideeditsectioneditor.h"
#include "ideviewsectionviewer.h"
#include "datatree.h"
#include "objectssystem.h"
#include "portalsportal.h"
#include "portalsoptionsshared.h"
#include "searchsectionoptions.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsSectionDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsSection::VERSION = OS_PORTAL_OBJECT_SECTION_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsSection::ObjectsSection() : RevisionableBase(portalObjectTypeSection, VERSION)
{

}

ObjectsSection::~ObjectsSection()
{

}

ExtensionsComponentID ObjectsSection::getComponentID(shared_ptr<Portal> portal) const
{
	if(getEntityID() == ObjectsSystem::instance()->getRootID())
	{
		// Root component
		ExtensionsComponentID component = portal->getOptionsShared()->getLayoutComponent().to_ascii();
		return component;		
	}
	else
	{
		return component;
	}

	return ExtensionsComponentID::EMPTY;
}

shared_ptr<IExtensionsComponent> ObjectsSection::getComponent(shared_ptr<Portal> portal) const
{
	ExtensionsComponentID component = getComponentID(portal);
	if(component != ExtensionsComponentID::EMPTY)
		return ExtensionsSystem::instance()->getComponent(component);
	else
		return null;
}

// VERYURGENT: threadsafe nelle varie ObjectsSection::get...

shared_ptr<ObjectsSection> ObjectsSection::getRootSection()
{
	static shared_ptr<ObjectsSection> root_section;
	if(root_section == null)
	{
		root_section.reset(OS_NEW ObjectsSection());
		root_section->entity = ObjectsSystem::instance()->getRootID();
		root_section->id = ObjectsSystem::instance()->getVirtualObjectID(root_section->entity);
		root_section->entity_parent = EntityID::EMPTY;
		root_section->parent = root_section->entity_parent;
		root_section->title = String(_S("Home"));
		root_section->visible = true;
	}

	return root_section;
}

shared_ptr<ObjectsSection> ObjectsSection::getRecursiveSection()
{
	static shared_ptr<ObjectsSection> recursive_section;
	if(recursive_section == null)
	{
		recursive_section.reset(OS_NEW ObjectsSection());
		recursive_section->entity = ObjectsSystem::instance()->getRecursiveID();
		recursive_section->id = ObjectsSystem::instance()->getVirtualObjectID(recursive_section->entity);
		recursive_section->entity_parent = ObjectsSystem::instance()->getRootID();
		recursive_section->parent = recursive_section->entity_parent;
		recursive_section->title = String(_S("(virtual recursive)"));
		recursive_section->visible = true;
	}

	return recursive_section;
}

shared_ptr<ObjectsSection> ObjectsSection::getOrphanSection()
{
	static shared_ptr<ObjectsSection> orphan_section;
	if(orphan_section == null)
	{
		orphan_section.reset(OS_NEW ObjectsSection());
		orphan_section->entity = ObjectsSystem::instance()->getOrphanID();
		orphan_section->id = ObjectsSystem::instance()->getVirtualObjectID(orphan_section->entity);
		orphan_section->entity_parent = ObjectsSystem::instance()->getRootID();
		orphan_section->parent = orphan_section->entity_parent;
		orphan_section->title = String(_S("(virtual orphans)"));
		orphan_section->visible = true;
	}

	return orphan_section;
}

shared_ptr<ObjectsSection> ObjectsSection::getSkippedSection()
{
	static shared_ptr<ObjectsSection> skipped_section;
	if(skipped_section == null)
	{
		skipped_section.reset(OS_NEW ObjectsSection());
		skipped_section->entity = ObjectsSystem::instance()->getSkippedID();
		skipped_section->id = ObjectsSystem::instance()->getVirtualObjectID(skipped_section->entity);
		skipped_section->entity_parent = ObjectsSystem::instance()->getRootID();
		skipped_section->parent = skipped_section->entity_parent;
		skipped_section->title = String(_S("(virtual skipped)"));
		skipped_section->visible = false;
	}

	return skipped_section;
}

bool ObjectsSection::validate(shared_ptr<IPortalDatabase> database) const
{
	if(RevisionableBase::validate(database) == false)
		return false;

	if(title->empty())
		return false;

	// TOFIX_VALIDATING
	/*
	if(component.isNull() == false)
	{
		if(component->validate(false) == false || component->getType() != ID::idComponent)
			return false;
	}
	*/

	if(data->empty() == false)
	{
		shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
		if(document->parseString(data) == false)
			return false;
	}

	return true;
}

const String & ObjectsSection::getTitle() const
{
	return title;
}

const String & ObjectsSection::getDescription() const
{
	return description;
}

void ObjectsSection::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	RevisionableBase::exportXML(exporter);

	shared_ptr<XMLNode> node = exporter->getRoot();

	if(exporter->getMode() != XMLPortalExporter::emLite)
	{
		node->setAttributeString(DBTABLES::SECTIONS::TITLE, title);
		node->setAttributeString(DBTABLES::SECTIONS::DESCRIPTION, description);
	}

	if(exporter->getMode() == XMLPortalExporter::emData)
	{
		node->setAttributeString(DBTABLES::SECTIONS::COMPONENT, component->toUTF16());
		node->setAttributeString(DBTABLES::SECTIONS::DATA, data);
	}

	shared_ptr<IExtensionsComponent> component = getComponent(exporter->getPortal());
	if(component != null)
		component->exportXML(getEntityID(), exporter);
}

bool ObjectsSection::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(RevisionableBase::onSign(signer) == false)
		return false;

	signer->add(title);
	signer->add(description);
	signer->add(component);
	signer->add(data);

	return true;
}

bool ObjectsSection::onRead(const ObjectID &id, const DataTree &dt)
{
	if(RevisionableBase::onRead(id, dt) == false)
		return false;

	title.read(dt, DBTABLES::SECTIONS::TITLE);
	description.read(dt, DBTABLES::SECTIONS::DESCRIPTION);
	component.read(dt, DBTABLES::SECTIONS::COMPONENT);
	data.read(dt, DBTABLES::SECTIONS::DATA);

	return true;
}

bool ObjectsSection::onWrite(DataTree &dt) const
{
	if(RevisionableBase::onWrite(dt) == false)
		return false;

    title.write(dt, DBTABLES::SECTIONS::TITLE);
	description.write(dt, DBTABLES::SECTIONS::DESCRIPTION);
	component.write(dt, DBTABLES::SECTIONS::COMPONENT);
	data.write(dt, DBTABLES::SECTIONS::DATA);

	return true;
}

bool ObjectsSection::onRead(const DataTableRow &row)
{
	if(RevisionableBase::onRead(row) == false)
		return false;

	title.read(row, DBTABLES::SECTIONS::TITLE);
	description.read(row, DBTABLES::SECTIONS::DESCRIPTION);
	component.read(row, DBTABLES::SECTIONS::COMPONENT);
	data.read(row, DBTABLES::SECTIONS::DATA);

	return true;
}

bool ObjectsSection::onWrite(DbSqlValues &values) const
{
	if(RevisionableBase::onWrite(values) == false)
		return false;

	title.write(values, DBTABLES::SECTIONS::TITLE);
	description.write(values, DBTABLES::SECTIONS::DESCRIPTION);
	component.write(values, DBTABLES::SECTIONS::COMPONENT);
	data.write(values, DBTABLES::SECTIONS::DATA);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsSectionDescriptor::ObjectsSectionDescriptor()
{

}

ObjectsSectionDescriptor::~ObjectsSectionDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsSectionDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsSection());
}

String ObjectsSectionDescriptor::getTableName()
{
	return DBTABLES::SECTIONS_TABLE;
}

String ObjectsSectionDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_SECTION_TYPENAME;
}

String ObjectsSectionDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_SECTION_GROUPNAME;
}

void ObjectsSectionDescriptor::getAllowedChilds(ObjectsTypes &allowed_childs) const
{
	DescriptorBase::getAllowedChilds(allowed_childs);

	allowed_childs.push_back(portalObjectTypeCalendarEvent);
	allowed_childs.push_back(portalObjectTypeSection);
	allowed_childs.push_back(portalObjectTypeFile);
	allowed_childs.push_back(portalObjectTypeInstance);
	allowed_childs.push_back(portalObjectTypeModel);
	allowed_childs.push_back(portalObjectTypePoll);
	allowed_childs.push_back(portalObjectTypeText);
}

ObjectsIDescriptor::OrderMode ObjectsSectionDescriptor::getOrderMode() const
{
	return omPosition;
}

bool ObjectsSectionDescriptor::hasStatistics()
{
	return true;
}

shared_ptr<ISearchOptions> ObjectsSectionDescriptor::getSearchOptions()
{
	return shared_ptr<ISearchOptions>(OS_NEW SearchSectionOptions());
}

ide::edit::editor_ptr ObjectsSectionDescriptor::createEditControl(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent)
{
	return ide::edit::editor_ptr(OS_NEW ide::edit::SectionEditor(entity, parent));
}

ide::view::viewer_ptr ObjectsSectionDescriptor::createViewControl(shared_ptr<EntitiesEntity> entity)
{
	return ide::view::viewer_ptr(OS_NEW ide::view::SectionViewer(entity));
}

void ObjectsSectionDescriptor::createStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	DescriptorBase::createStatistics(db, object);

	shared_ptr<ObjectsSection> section = objects_section_cast(object);
	if(section != null)
	{
		shared_ptr<IExtensionsComponent> component = section->getComponent(db->getPortal());
		if(component != null)
			component->createStatistics(db, object);
	}
}

void ObjectsSectionDescriptor::computeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	DescriptorBase::computeStatistics(db, object);

	shared_ptr<ObjectsSection> section = objects_section_cast(object);
	if(section != null)
	{
		shared_ptr<IExtensionsComponent> component = section->getComponent(db->getPortal());
		if(component != null)
			component->computeStatistics(db, object);
	}
}

void ObjectsSectionDescriptor::removeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	DescriptorBase::removeStatistics(db, object);

	shared_ptr<ObjectsSection> section = objects_section_cast(object);
	if(section != null)
	{
		shared_ptr<IExtensionsComponent> component = section->getComponent(db->getPortal());
		if(component != null)
			component->removeStatistics(db, object);
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
