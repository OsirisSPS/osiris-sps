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
#include "objectsfile.h"

#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "entitiessnapshotmanager.h"
#include "ideeditfileeditor.h"
#include "ideviewfileviewer.h"
#include "datatree.h"
#include "filesystem.h"
#include "objectssigner.h"
#include "osirislink.h"
#include "idesession.h"
#include "portalsportal.h"
#include "searchfileoptions.h"
#include "mimemanager.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsFileDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsFile::VERSION = OS_PORTAL_OBJECT_FILE_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsFile::ObjectsFile() : CommentableBase(portalObjectTypeFile, VERSION)
{
	file_size = 0;
}

ObjectsFile::~ObjectsFile()
{

}

String ObjectsFile::getMimeType() const
{
	String mimeType;
	String extension = FileSystem::instance()->getExtension(file_name, true);

	if(MimeManager::instance()->getMimeType(extension, mimeType))
	{
		return mimeType;
	}

	return *file_type;
}

bool ObjectsFile::validate(shared_ptr<IPortalDatabase> database) const
{
	if(CommentableBase::validate(database) == false)
		return false;

	if(file_name->empty())
		return false;

	// Se il buffer  valorizzato il content-type deve essere valido
	if(file_data->empty() == false && file_type->empty())
		return false;

	if(file_data->getSize() != file_size)
		return false;

	return true;
}

void ObjectsFile::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	CommentableBase::exportXML(exporter);

	shared_ptr<XMLNode> node = exporter->getRoot();

	if(exporter->getMode() != XMLPortalExporter::emLite)
	{
		node->setAttributeString(DBTABLES::FILES::FILE_NAME, file_name);
		node->setAttributeString(DBTABLES::FILES::FILE_TYPE, file_type);
		node->setAttributeUint32(DBTABLES::FILES::FILE_SIZE, file_size);
	}

	if(exporter->getMode() == XMLPortalExporter::emFull)
	{
		node->setAttributeString(_S("mime_type"), getMimeType());
		node->setAttributeString(_S("file_href"), exporter->getPortal()->getFileLink(id));

		OsirisLink link;
		link.setParam("type", _W("file"));
		link.setParam("id", id->toWide());
		link.setParam("portal", exporter->getPortal()->getPortalID().toWide());
		//RAZORVERYURGENT: add user param
		node->setAttributeString(_S("file_osiris_href"), link.generate());
	}

	if(exporter->getMode() == XMLPortalExporter::emData)
	{
		node->setAttributeBuffer(DBTABLES::FILES::FILE_DATA, file_data);
	}
}

bool ObjectsFile::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(CommentableBase::onSign(signer) == false)
		return false;

	signer->add(file_name);
	signer->add(file_data);
	signer->add(file_type);
	signer->add(file_size);

	return true;
}

bool ObjectsFile::onRead(const ObjectID &id, const DataTree &data)
{
	if(CommentableBase::onRead(id, data) == false)
		return false;

	file_name.read(data, DBTABLES::FILES::FILE_NAME);
	file_data.read(data, DBTABLES::FILES::FILE_DATA);
	file_type.read(data, DBTABLES::FILES::FILE_TYPE);
	file_size.read(data, DBTABLES::FILES::FILE_SIZE);

	return true;
}

bool ObjectsFile::onWrite(DataTree &data) const
{
	if(CommentableBase::onWrite(data) == false)
		return false;

	file_name.write(data, DBTABLES::FILES::FILE_NAME);
	file_data.write(data, DBTABLES::FILES::FILE_DATA);
	file_type.write(data, DBTABLES::FILES::FILE_TYPE);
	file_size.write(data, DBTABLES::FILES::FILE_SIZE);

	return true;
}

bool ObjectsFile::onRead(const DataTableRow &row)
{
	if(CommentableBase::onRead(row) == false)
		return false;

	file_name.read(row, DBTABLES::FILES::FILE_NAME);
	file_data.read(row, DBTABLES::FILES::FILE_DATA);
	file_type.read(row, DBTABLES::FILES::FILE_TYPE);
	file_size.read(row, DBTABLES::FILES::FILE_SIZE);

	return true;
}

bool ObjectsFile::onWrite(DbSqlValues &values) const
{
	if(CommentableBase::onWrite(values) == false)
		return false;

	file_name.write(values, DBTABLES::FILES::FILE_NAME);
	file_data.write(values, DBTABLES::FILES::FILE_DATA);
	file_type.write(values, DBTABLES::FILES::FILE_TYPE);
	file_size.write(values, DBTABLES::FILES::FILE_SIZE);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsFileDescriptor::ObjectsFileDescriptor()
{

}

ObjectsFileDescriptor::~ObjectsFileDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsFileDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsFile());
}

String ObjectsFileDescriptor::getTableName()
{
	return DBTABLES::FILES_TABLE;
}

String ObjectsFileDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_FILE_TYPENAME;
}

String ObjectsFileDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_FILE_GROUPNAME;
}

shared_ptr<ISearchOptions> ObjectsFileDescriptor::getSearchOptions()
{
	return shared_ptr<ISearchOptions>(OS_NEW SearchFileOptions());
}

ide::edit::editor_ptr ObjectsFileDescriptor::createEditControl(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent)
{
	return ide::edit::editor_ptr(OS_NEW ide::edit::FileEditor(entity, parent));
}

ide::view::viewer_ptr ObjectsFileDescriptor::createViewControl(shared_ptr<EntitiesEntity> entity)
{
	return ide::view::viewer_ptr(OS_NEW ide::view::FileViewer(entity));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
