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
#include "datalocalmessage.h"

#include "convert.h"
#include "conversions.h"
#include "dbbinder.h"
#include "dbtables.h"
#include "iideportalpage.h"
#include "idesession.h"
#include "objectsmessage.h"
#include "objectsuser.h"
#include "portalsportal.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace conversions {

//////////////////////////////////////////////////////////////////////

template<>
EngineExport String to_utf16<MessageFolder>(const MessageFolder &v)
{
	return to_utf16<int32>(static_cast<int32>(v));
}

template<>
EngineExport MessageFolder from_utf16<MessageFolder>(const String &v)
{
	int32 folder = from_utf16<int32>(v);
	switch(folder)
	{
	case messageFolderNone:
	case messageFolderInbox:
	case messageFolderTrash:
	case messageFolderSentItems:
								return static_cast<MessageFolder>(folder);

	default:					OS_ASSERTFALSE();
								break;
	}

	return messageFolderNone;
}

template<>
EngineExport String to_utf16<MessageStatus>(const MessageStatus &v)
{
	return to_utf16<int32>(static_cast<int32>(v));
}

template<>
EngineExport MessageStatus from_utf16<MessageStatus>(const String &v)
{
	int32 status = from_utf16<int32>(v);
	switch(status)
	{
	case messageStatusNone:
	case messageStatusNew:
	case messageStatusUnread:
	case messageStatusRead:
	case messageStatusDeleted:
								return static_cast<MessageStatus>(status);

	default:					OS_ASSERTFALSE();
								break;
	}

	return messageStatusNone;
}

//////////////////////////////////////////////////////////////////////

} // conversions

//////////////////////////////////////////////////////////////////////

DataLocalMessage::DataLocalMessage() : RecordBase(dataRecordTypeLocalMessage)
{
	status = messageStatusRead;
	folder = messageFolderInbox;
}

DataLocalMessage::~DataLocalMessage()
{

}

MessageFolder DataLocalMessage::getFolder() const
{
	switch(folder)
	{
	case messageFolderInbox:
	case messageFolderTrash:
	case messageFolderSentItems:
									return static_cast<MessageFolder>(folder.get());

	default:						OS_ASSERTFALSE();
									break;
	}

	return messageFolderInbox;
}

MessageStatus DataLocalMessage::getStatus() const
{
	switch(status)
	{
	case messageStatusNew:
	case messageStatusUnread:
	case messageStatusRead:
	case messageStatusDeleted:
								return static_cast<MessageStatus>(status.get());

	default:					OS_ASSERTFALSE();
								break;
	}

	return messageStatusRead;
}

void DataLocalMessage::setStatus(MessageStatus status)
{
	this->status = static_cast<uint32>(status);
}

String DataLocalMessage::toString(MessageFolder folder)
{
	switch(folder)
	{
	case messageFolderInbox:		return _S("inbox");
	case messageFolderTrash:		return _S("trash");
	case messageFolderSentItems:	return _S("sentItems");

	default:						OS_ASSERTFALSE();
									break;
	}

	return String::EMPTY;
}

String DataLocalMessage::toString(MessageStatus status)
{
	switch(status)
	{
	case messageStatusNew:		return _S("new");
	case messageStatusUnread:	return _S("unread");
	case messageStatusRead:		return _S("read");
	case messageStatusDeleted:	return _S("deleted");

	default:					OS_ASSERTFALSE();
								break;
	}

	return String::EMPTY;
}

shared_ptr<ObjectsMessage> DataLocalMessage::decode(const Buffer &private_key)
{
	shared_ptr<ObjectsMessage> message(OS_NEW ObjectsMessage());
	if(message->decode(private_key, session, content))
		return message;
		
	return null;
}

void DataLocalMessage::exportXML(shared_ptr<XMLPortalExporter> exporter, const String &subject, const String &body, bool secure)
{
	shared_ptr<XMLNode> node = exporter->getRoot();

	node->setAttributeString(_S("subject"), subject);
	node->setAttributeString(_S("body"), body);
	node->setAttributeBool(_S("secure"), secure);

	node->setAttributeString(DBTABLES::LOCAL_MESSAGES::ID, id.toXML());
	node->setAttributeDateTime(DBTABLES::LOCAL_MESSAGES::SUBMIT_DATE, submit_date);
	node->setAttributeDateTime(DBTABLES::LOCAL_MESSAGES::READ_DATE, read_date);
	node->setAttributeString(DBTABLES::LOCAL_MESSAGES::FOLDER, toString(getFolder()));
	node->setAttributeString(DBTABLES::LOCAL_MESSAGES::STATUS, toString(getStatus()));
	node->setAttributeString(_S("href"), exporter->getPortal()->getPrivateMessageLink(id, true));

	shared_ptr<ObjectsUser> user = objects_user_cast(exporter->getPage()->getObject(author));
	if(user != null)
		user->exportXML(exporter->createChild<XMLPortalExporter>(exporter->createNode(DBTABLES::AUTHOR)));	
}

String DataLocalMessage::getTableName() const
{
	return DBTABLES::LOCAL_MESSAGES_TABLE;
}

String DataLocalMessage::getRecordKeyName() const
{
	return DBTABLES::LOCAL_MESSAGES::PROFILE;
}

String DataLocalMessage::getRecordKeyValue() const
{
	return profile;
}

String DataLocalMessage::getRecordKey2Name() const
{
	return DBTABLES::LOCAL_MESSAGES::ID;
}

String DataLocalMessage::getRecordKey2Value() const
{
	return id->toUTF16();
}

bool DataLocalMessage::onRead(const DataTableRow &row)
{
	profile.read(row, DBTABLES::LOCAL_MESSAGES::PROFILE);			 	
	id.read(row, DBTABLES::LOCAL_MESSAGES::ID);			 	
	author.read(row, DBTABLES::LOCAL_MESSAGES::AUTHOR);			 	
	session.read(row, DBTABLES::LOCAL_MESSAGES::SESSION);			 	
	content.read(row, DBTABLES::LOCAL_MESSAGES::CONTENT);			 	
	submit_date.read(row, DBTABLES::LOCAL_MESSAGES::SUBMIT_DATE);			 	
	read_date.read(row, DBTABLES::LOCAL_MESSAGES::READ_DATE);			 	
	status.read(row, DBTABLES::LOCAL_MESSAGES::STATUS);			 	
	folder.read(row, DBTABLES::LOCAL_MESSAGES::FOLDER);			 	

	return true;
}

bool DataLocalMessage::onWrite(DbSqlValues &values) const
{
	profile.write(values, DBTABLES::LOCAL_MESSAGES::PROFILE);			 	
	id.write(values, DBTABLES::LOCAL_MESSAGES::ID);			 	
	author.write(values, DBTABLES::LOCAL_MESSAGES::AUTHOR);			 	
	session.write(values, DBTABLES::LOCAL_MESSAGES::SESSION);			 	
	content.write(values, DBTABLES::LOCAL_MESSAGES::CONTENT);			 	
	submit_date.write(values, DBTABLES::LOCAL_MESSAGES::SUBMIT_DATE);			 	
	read_date.write(values, DBTABLES::LOCAL_MESSAGES::READ_DATE);			 	
	status.write(values, DBTABLES::LOCAL_MESSAGES::STATUS);			 	
	folder.write(values, DBTABLES::LOCAL_MESSAGES::FOLDER);			 	

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
