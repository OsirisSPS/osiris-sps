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
#include "objectsattachment.h"

#include "dbtables.h"
#include "filesystem.h"
#include "ideeditattachmenteditor.h"
#include "ideviewattachmentviewer.h"
#include "mimemanager.h"
#include "objectssigner.h"
#include "portalsportal.h"
#include "searchattachmentoptions.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsAttachmentDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsAttachment::VERSION = OS_PORTAL_OBJECT_ATTACHMENT_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsAttachment::ObjectsAttachment() : RevisionableBase(portalObjectTypeAttachment, VERSION)
{

}

ObjectsAttachment::~ObjectsAttachment()
{

}

String ObjectsAttachment::getMimeType() const
{
	String mimeType;
	String extension = FileSystem::instance()->getExtension(file_name, true);

	if(MimeManager::instance()->getMimeType(extension, mimeType))
	{
		return mimeType;
	}

	return *file_type;
}

bool ObjectsAttachment::requireParent() const
{
	return true;
}

bool ObjectsAttachment::validate(shared_ptr<IPortalDatabase> database) const
{
	if(RevisionableBase::validate(database) == false)
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

void ObjectsAttachment::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	RevisionableBase::exportXML(exporter);

	if(exporter->getMode() == XMLPortalExporter::emFull)
	{
		shared_ptr<XMLNode> node = exporter->getRoot();

		node->setAttributeString(DBTABLES::ATTACHMENTS::FILE_NAME, file_name);
		node->setAttributeString(DBTABLES::ATTACHMENTS::FILE_TYPE, file_type);
		node->setAttributeUint32(DBTABLES::ATTACHMENTS::FILE_SIZE, file_size);
		node->setAttributeString(_S("mime_type"), getMimeType());
		node->setAttributeString(_S("file_href"), exporter->getPortal()->getFileLink(id));
	}
}

bool ObjectsAttachment::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(RevisionableBase::onSign(signer) == false)
		return false;

	signer->add(file_name);
	signer->add(file_data);
	signer->add(file_type);
	signer->add(file_size);

	return true;
}

bool ObjectsAttachment::onRead(const ObjectID &id, const DataTree &data)
{
	if(RevisionableBase::onRead(id, data) == false)
		return false;

	file_name.read(data, DBTABLES::ATTACHMENTS::FILE_NAME);
	file_data.read(data, DBTABLES::ATTACHMENTS::FILE_DATA);
	file_type.read(data, DBTABLES::ATTACHMENTS::FILE_TYPE);
	file_size.read(data, DBTABLES::ATTACHMENTS::FILE_SIZE);

	return true;
}

bool ObjectsAttachment::onWrite(DataTree &data) const
{
	if(RevisionableBase::onWrite(data) == false)
		return false;

	file_name.write(data, DBTABLES::ATTACHMENTS::FILE_NAME);
	file_data.write(data, DBTABLES::ATTACHMENTS::FILE_DATA);
	file_type.write(data, DBTABLES::ATTACHMENTS::FILE_TYPE);
	file_size.write(data, DBTABLES::ATTACHMENTS::FILE_SIZE);

	return true;
}

bool ObjectsAttachment::onRead(const DataTableRow &row)
{
	if(RevisionableBase::onRead(row) == false)
		return false;

	file_name.read(row, DBTABLES::ATTACHMENTS::FILE_NAME);
	file_data.read(row, DBTABLES::ATTACHMENTS::FILE_DATA);
	file_type.read(row, DBTABLES::ATTACHMENTS::FILE_TYPE);
	file_size.read(row, DBTABLES::ATTACHMENTS::FILE_SIZE);

	return true;
}

bool ObjectsAttachment::onWrite(DbSqlValues &values) const
{
	if(RevisionableBase::onWrite(values) == false)
		return false;

	file_name.write(values, DBTABLES::ATTACHMENTS::FILE_NAME);
	file_data.write(values, DBTABLES::ATTACHMENTS::FILE_DATA);
	file_type.write(values, DBTABLES::ATTACHMENTS::FILE_TYPE);
	file_size.write(values, DBTABLES::ATTACHMENTS::FILE_SIZE);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsAttachmentDescriptor::ObjectsAttachmentDescriptor()
{

}

ObjectsAttachmentDescriptor::~ObjectsAttachmentDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsAttachmentDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsAttachment());
}

String ObjectsAttachmentDescriptor::getTableName()
{
	return DBTABLES::ATTACHMENTS_TABLE;
}

String ObjectsAttachmentDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_ATTACHMENT_TYPENAME;
}

String ObjectsAttachmentDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_ATTACHMENT_GROUPNAME;
}

bool ObjectsAttachmentDescriptor::isGroupable()
{
	return true;
}

shared_ptr<ISearchOptions> ObjectsAttachmentDescriptor::getSearchOptions()
{
	return shared_ptr<ISearchOptions>(OS_NEW SearchAttachmentOptions());
}

ide::edit::editor_ptr ObjectsAttachmentDescriptor::createEditControl(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent)
{
	return ide::edit::editor_ptr(OS_NEW ide::edit::AttachmentEditor(entity, parent));
}

ide::view::viewer_ptr ObjectsAttachmentDescriptor::createViewControl(shared_ptr<EntitiesEntity> entity)
{
	return ide::view::viewer_ptr(OS_NEW ide::view::AttachmentViewer(entity));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
