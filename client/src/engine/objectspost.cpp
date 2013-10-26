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
#include "objectspost.h"

#include "objectssigner.h"
#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "ideeditposteditor.h"
#include "ideviewpostviewer.h"
#include "datatree.h"
#include "objectssystem.h"
#include "objectsuser.h"
#include "idesession.h"
#include "portalsportal.h"
#include "searchpostoptions.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsPostDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsPost::VERSION = OS_PORTAL_OBJECT_POST_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsPost::ObjectsPost() : RevisionableBase(portalObjectTypePost, VERSION)
{

}

ObjectsPost::~ObjectsPost()
{

}

bool ObjectsPost::requireParent() const
{
	return true;
}

bool ObjectsPost::validate(shared_ptr<IPortalDatabase> database) const
{
	if(RevisionableBase::validate(database) == false)
		return false;

	if(content->empty())
		return false;

	// TOFIX_VALIDATING
	/*
	if(reference->empty() == false)
	{
		if(reference->validateRevisionable(true, true) == false)
			return false;

		if(reference->getObjectType() != portalObjectTypePost)
		{
			// Il campo reference di un post pu essere assegnato solo ad un post o ad un oggetto che accetta come figli i post

			shared_ptr<ObjectsIDescriptor> descriptor = ObjectsSystem::instance()->getDescriptor(reference->getObjectType());
			if(descriptor == null || descriptor->allowChild(portalObjectTypePost) == false)
				return false;
		}
	}
	*/

    return true;
}

LanguageResult ObjectsPost::acceptable(shared_ptr<IPortalDatabase> database) const
{
	LanguageResult result = RevisionableBase::acceptable(database);
	if(result.empty() == false)
		return result;

	result = acceptableForBadWords(database, title);
	if(result.empty() == false)
		return result;
		
	result = acceptableForBadWords(database, content);
	if(result.empty() == false)
		return result;

	return LanguageResult();
}

const String & ObjectsPost::getTitle() const
{
	return title;
}

const String & ObjectsPost::getContent() const
{
	return content;
}

void ObjectsPost::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	RevisionableBase::exportXML(exporter);

	if(exporter->getMode() == XMLPortalExporter::emFull)
	{
		shared_ptr<XMLNode> node = exporter->getRoot();
		node->setAttributeString(DBTABLES::POSTS::TITLE, title);
		node->setAttributeString(DBTABLES::POSTS::CONTENT, content);
	}
	else if(exporter->getMode() == XMLPortalExporter::emData)
	{
		shared_ptr<XMLNode> node = exporter->getRoot();
		node->setAttributeString(DBTABLES::POSTS::TITLE, title);
		node->setAttributeString(DBTABLES::POSTS::CONTENT, content);
		node->setAttributeString(DBTABLES::POSTS::REFERENCE, reference->toUTF16());
	}
}

bool ObjectsPost::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(RevisionableBase::onSign(signer) == false)
		return false;

	signer->add(title);
	signer->add(content);
	signer->add(reference);

	return true;
}

bool ObjectsPost::onRead(const ObjectID &id, const DataTree &data)
{
	if(RevisionableBase::onRead(id, data) == false)
		return false;

	title.read(data, DBTABLES::POSTS::TITLE);
	content.read(data, DBTABLES::POSTS::CONTENT);
	reference.read(data, DBTABLES::POSTS::REFERENCE);

	return true;
}

bool ObjectsPost::onWrite(DataTree &data) const
{
	if(RevisionableBase::onWrite(data) == false)
		return false;

	title.write(data, DBTABLES::POSTS::TITLE);
	content.write(data, DBTABLES::POSTS::CONTENT);
	reference.write(data, DBTABLES::POSTS::REFERENCE);

	return true;
}

bool ObjectsPost::onRead(const DataTableRow &row)
{
	if(RevisionableBase::onRead(row) == false)
		return false;

	title.read(row, DBTABLES::POSTS::TITLE);
	content.read(row, DBTABLES::POSTS::CONTENT);
	reference.read(row, DBTABLES::POSTS::REFERENCE);

	return true;
}

bool ObjectsPost::onWrite(DbSqlValues &values) const
{
	if(RevisionableBase::onWrite(values) == false)
		return false;

	title.write(values, DBTABLES::POSTS::TITLE);
	content.write(values, DBTABLES::POSTS::CONTENT);
	reference.write(values, DBTABLES::POSTS::REFERENCE);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsPostDescriptor::ObjectsPostDescriptor()
{

}

ObjectsPostDescriptor::~ObjectsPostDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsPostDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsPost());
}

String ObjectsPostDescriptor::getTableName()
{
	return DBTABLES::POSTS_TABLE;
}

String ObjectsPostDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_POST_TYPENAME;
}

String ObjectsPostDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_POST_GROUPNAME;
}

bool ObjectsPostDescriptor::isGroupable()
{
	return true;
}

shared_ptr<ISearchOptions> ObjectsPostDescriptor::getSearchOptions()
{
	return shared_ptr<ISearchOptions>(OS_NEW SearchPostOptions());
}

ide::edit::editor_ptr ObjectsPostDescriptor::createEditControl(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent)
{
	return ide::edit::editor_ptr(OS_NEW ide::edit::PostEditor(entity, parent));
}

ide::view::viewer_ptr ObjectsPostDescriptor::createViewControl(shared_ptr<EntitiesEntity> entity)
{
	return ide::view::viewer_ptr(OS_NEW ide::view::PostViewer(entity));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
