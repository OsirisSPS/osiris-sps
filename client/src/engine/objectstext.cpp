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
#include "objectstext.h"

#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "entitiessnapshotmanager.h"
#include "ideedittexteditor.h"
#include "ideviewtextviewer.h"
#include "datatree.h"
#include "objectssigner.h"
#include "idesession.h"
#include "searchtextoptions.h"
#include "xmlportalexporter.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsTextDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsText::VERSION = OS_PORTAL_OBJECT_TEXT_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsText::ObjectsText() : CommentableBase(portalObjectTypeText, VERSION)
{

}

ObjectsText::~ObjectsText()
{

}

//////////////////////////////////////////////////////////////////////

ObjectsTextDescriptor::ObjectsTextDescriptor()
{

}

ObjectsTextDescriptor::~ObjectsTextDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsTextDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsText());
}

String ObjectsTextDescriptor::getTableName()
{
	return DBTABLES::TEXTS_TABLE;
}

String ObjectsTextDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_TEXT_TYPENAME;
}

String ObjectsTextDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_TEXT_GROUPNAME;
}

shared_ptr<ISearchOptions> ObjectsTextDescriptor::getSearchOptions()
{
	return shared_ptr<ISearchOptions>(OS_NEW SearchTextOptions());
}

ide::edit::editor_ptr ObjectsTextDescriptor::createEditControl(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent)
{
	return ide::edit::editor_ptr(OS_NEW ide::edit::TextEditor(entity, parent));
}

ide::view::viewer_ptr ObjectsTextDescriptor::createViewControl(shared_ptr<EntitiesEntity> entity)
{
	return ide::view::viewer_ptr(OS_NEW ide::view::TextViewer(entity));
}


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
