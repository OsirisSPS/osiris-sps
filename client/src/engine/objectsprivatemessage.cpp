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
#include "objectsprivatemessage.h"

#include "objectssigner.h"
#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "datatree.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsPrivateMessageDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsPrivateMessage::VERSION = OS_PORTAL_OBJECT_PRIVATEMESSAGE_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsPrivateMessage::ObjectsPrivateMessage() : EntryBase(portalObjectTypePrivateMessage, VERSION)
{

}

ObjectsPrivateMessage::~ObjectsPrivateMessage()
{

}

void ObjectsPrivateMessage::getDependencies(const shared_ptr<IPortalDatabase> &db, list<ObjectID>::type &dependencies)
{
	EntryBase::getDependencies(db, dependencies);

	dependencies.push_back(adressee);
}

bool ObjectsPrivateMessage::validate(shared_ptr<IPortalDatabase> database) const
{
	if(EntryBase::validate(database) == false)
		return false;

	// La chiave di criptazione deve essere valida
	if(session->empty())
		return false;

	// Nota: un messaggio senza oggetto  valido
	if(content->empty())
		return false;

	// Verifica che il destinatario sia valido
	if(adressee->empty())
		return false;
	
	// Verifica che il destinatario non coincida col mittente
	if(adressee == getAuthor())
		return false;

	return true;
}

bool ObjectsPrivateMessage::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(EntryBase::onSign(signer) == false)
		return false;

	signer->add(adressee);
	signer->add(session);
	signer->add(content);

	return true;
}

bool ObjectsPrivateMessage::onRead(const ObjectID &id, const DataTree &data)
{
	if(EntryBase::onRead(id, data) == false)
		return false;

	adressee.read(data, DBTABLES::PRIVATE_MESSAGES::ADRESSEE);
	session.read(data, DBTABLES::PRIVATE_MESSAGES::SESSION);
	content.read(data, DBTABLES::PRIVATE_MESSAGES::CONTENT);

	return true;
}

bool ObjectsPrivateMessage::onWrite(DataTree &data) const
{
	if(EntryBase::onWrite(data) == false)
		return false;

	adressee.write(data, DBTABLES::PRIVATE_MESSAGES::ADRESSEE);
	session.write(data, DBTABLES::PRIVATE_MESSAGES::SESSION);
	content.write(data, DBTABLES::PRIVATE_MESSAGES::CONTENT);

	return true;
}

bool ObjectsPrivateMessage::onRead(const DataTableRow &row)
{
	if(EntryBase::onRead(row) == false)
		return false;

	adressee.read(row, DBTABLES::PRIVATE_MESSAGES::ADRESSEE);
	session.read(row, DBTABLES::PRIVATE_MESSAGES::SESSION);
	content.read(row, DBTABLES::PRIVATE_MESSAGES::CONTENT);

	return true;
}

bool ObjectsPrivateMessage::onWrite(DbSqlValues &values) const
{
	if(EntryBase::onWrite(values) == false)
		return false;

	adressee.write(values, DBTABLES::PRIVATE_MESSAGES::ADRESSEE);
	session.write(values, DBTABLES::PRIVATE_MESSAGES::SESSION);
	content.write(values, DBTABLES::PRIVATE_MESSAGES::CONTENT);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsPrivateMessageDescriptor::ObjectsPrivateMessageDescriptor()
{

}

ObjectsPrivateMessageDescriptor::~ObjectsPrivateMessageDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsPrivateMessageDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsPrivateMessage());
}

String ObjectsPrivateMessageDescriptor::getTableName()
{
	return DBTABLES::PRIVATE_MESSAGES_TABLE;
}

String ObjectsPrivateMessageDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_PRIVATEMESSAGE_TYPENAME;
}

String ObjectsPrivateMessageDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_PRIVATEMESSAGE_GROUPNAME;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
