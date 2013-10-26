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
#include "objectsavatar.h"

#include "datatree.h"
#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "image.h"
#include "objectssigner.h"
#include "portalsoptionsshared.h"
#include "portalsportal.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"


//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsAvatarDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsAvatar::VERSION = OS_PORTAL_OBJECT_AVATAR_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsAvatar::ObjectsAvatar() : EntryBase(portalObjectTypeAvatar, VERSION)
{
	file_size = 0;
}

ObjectsAvatar::~ObjectsAvatar()
{

}

bool ObjectsAvatar::validateFormat(shared_ptr<Portal> portal, const Buffer &buffer)
{
	shared_ptr<Image> image(OS_NEW Image());
	if(image->load(buffer) == false)
		return false;

	switch(image->getType())
	{
		case Image::itBmp:
		case Image::itGif:
		case Image::itJpg:
		case Image::itPng:
								break;

		default:
								return false;
	}

	uint32 width = image->getWidth();
	if(width == 0 || width > portal->getOptionsShared()->getAvatarMaxWidth())
		return false;

	uint32 height = image->getHeight();
	if(height == 0 || height > portal->getOptionsShared()->getAvatarMaxHeight())
		return false;

	return true;
}

bool ObjectsAvatar::validate(shared_ptr<IPortalDatabase> database) const
{
	if(EntryBase::validate(database) == false)
		return false;


	
	return true;
}

LanguageResult ObjectsAvatar::acceptable(shared_ptr<IPortalDatabase> database) const
{
	LanguageResult result = EntryBase::acceptable(database);
	if(result.empty() == false)
		return result;

	if(IPortalDatabase::getUserAvatarID(getAuthor()) != id)
		return LanguageResult("id_mismatch");

	if(file_type->empty())
		return LanguageResult("mimetype_required");

	if(file_size == 0)
		return LanguageResult("data_required");

	if(file_data->getSize() != file_size)
		return LanguageResult("size_mismatch");

	// Valida il formato dell'immagine
	if(validateFormat(database->getPortal(), file_data) == false)
		return LanguageResult("image_invalid");
	
	return LanguageResult();
}


void ObjectsAvatar::ensureID(const shared_ptr<IPortalDatabase> &db)
{
	id = db->getUserAvatarID(author);
}


void ObjectsAvatar::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	EntryBase::exportXML(exporter);

	shared_ptr<XMLNode> node = exporter->getRoot();

	if(exporter->getMode() == XMLPortalExporter::emData)
	{
		node->setAttributeString(DBTABLES::AVATARS::FILE_NAME, file_name);
		node->setAttributeBuffer(DBTABLES::AVATARS::FILE_DATA, file_data);
		node->setAttributeString(DBTABLES::AVATARS::FILE_TYPE, file_type);
		node->setAttributeUint32(DBTABLES::AVATARS::FILE_SIZE, file_size);
	}
}

bool ObjectsAvatar::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(EntryBase::onSign(signer) == false)
		return false;

	signer->add(file_name);
	signer->add(file_data);
	signer->add(file_type);
	signer->add(file_size);

	return true;
}

bool ObjectsAvatar::onRead(const ObjectID &id, const DataTree &data)
{
	if(EntryBase::onRead(id, data) == false)
		return false;

	file_name.read(data, DBTABLES::AVATARS::FILE_NAME);
	file_data.read(data, DBTABLES::AVATARS::FILE_DATA);
	file_type.read(data, DBTABLES::AVATARS::FILE_TYPE);
	file_size.read(data, DBTABLES::AVATARS::FILE_SIZE);

	return true;
}

bool ObjectsAvatar::onWrite(DataTree &data) const
{
	if(EntryBase::onWrite(data) == false)
		return false;

	file_name.write(data, DBTABLES::AVATARS::FILE_NAME);
	file_data.write(data, DBTABLES::AVATARS::FILE_DATA);
	file_type.write(data, DBTABLES::AVATARS::FILE_TYPE);
	file_size.write(data, DBTABLES::AVATARS::FILE_SIZE);

	return true;
}

bool ObjectsAvatar::onRead(const DataTableRow &row)
{
	if(EntryBase::onRead(row) == false)
		return false;

	file_name.read(row, DBTABLES::AVATARS::FILE_NAME);
	file_data.read(row, DBTABLES::AVATARS::FILE_DATA);
	file_type.read(row, DBTABLES::AVATARS::FILE_TYPE);
	file_size.read(row, DBTABLES::AVATARS::FILE_SIZE);

	return true;
}

bool ObjectsAvatar::onWrite(DbSqlValues &values) const
{
	if(EntryBase::onWrite(values) == false)
		return false;

	file_name.write(values, DBTABLES::AVATARS::FILE_NAME);
	file_data.write(values, DBTABLES::AVATARS::FILE_DATA);
	file_type.write(values, DBTABLES::AVATARS::FILE_TYPE);
	file_size.write(values, DBTABLES::AVATARS::FILE_SIZE);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsAvatarDescriptor::ObjectsAvatarDescriptor()
{

}

ObjectsAvatarDescriptor::~ObjectsAvatarDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsAvatarDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsAvatar());
}

String ObjectsAvatarDescriptor::getTableName()
{
	return DBTABLES::AVATARS_TABLE;
}

String ObjectsAvatarDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_AVATAR_TYPENAME;
}

String ObjectsAvatarDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_AVATAR_GROUPNAME;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
