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

#ifndef _OBJECTS_PRIVATEMESSAGE_H
#define _OBJECTS_PRIVATEMESSAGE_H

#include "objectsdescriptor.h"
#include "objectsientry.h"

//////////////////////////////////////////////////////////////////////

/*
	La codifica di un messaggio privato avviene nel seguente modo:

	- A deve mandare un messaggio a B
	- A genera un valore casuale (session_key)
	- A cripta session_key con la chiave pubblica di B e ottiene 'key'
	- A cripta 'content' con session_key (attualmente viene utilizzato il AES come algoritmo)
	- A invia a B key e content
	- B decripta key con la propria chiave privata e riottiene 'session_key'
	- B decripta content con session_key

	Nota: content non viene criptato con la chiave pubblica sia perchè sarebbe troppo pesante sia perchè la dimensione del dato da criptare è limitata sulla base della lunghezza della chiave pubblica
*/

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsPrivateMessage : public ObjectsIEntry
{
	typedef ObjectsIEntry EntryBase;

// Construction
public:
	ObjectsPrivateMessage();
	virtual ~ObjectsPrivateMessage();

// IEntry overrides
public:
	virtual void getDependencies(const shared_ptr<IPortalDatabase> &db, list<ObjectID>::type &dependencies);
	virtual bool validate(shared_ptr<IPortalDatabase> database) const;

protected:
	virtual bool onSign(const shared_ptr<ObjectsSigner> &signer) const;

	virtual bool onRead(const ObjectID &id, const DataTree &data);
	virtual bool onWrite(DataTree &data) const;

	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:
	DbValue<ObjectID> adressee;	// Destinatario del messaggio
	DbValue<Buffer> session;		// Chiave (criptata con la chiave pubblica del destinatario) con cui  criptato il contenuto del messaggio
	DbValue<Buffer> content;		// Contenuto (criptato con la chiave in chiaro) del messaggio

public:
	static uint32 VERSION;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ObjectsPrivateMessage> objects_private_message_cast(shared_ptr<ObjectsIObject> object) { return boost::dynamic_pointer_cast<ObjectsPrivateMessage>(object); }

//////////////////////////////////////////////////////////////////////

class ObjectsPrivateMessageDescriptor : public ObjectsDescriptor<ObjectsPrivateMessageDescriptor>
{
	OS_DECLARE_DESCRIPTOR(ObjectsPrivateMessageDescriptor, portalObjectTypePrivateMessage);

	typedef ObjectsDescriptor<ObjectsPrivateMessageDescriptor> DescriptorBase;

// Construction
public:
	ObjectsPrivateMessageDescriptor();
	virtual ~ObjectsPrivateMessageDescriptor();

// IDescriptor interface
public:
	virtual shared_ptr<ObjectsIObject> createObject();
	virtual String getTableName();

	virtual String getTypeName();
	virtual String getGroupName();
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OBJECTS_PRIVATEMESSAGE_H
