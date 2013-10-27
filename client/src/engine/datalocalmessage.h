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

#ifndef _ENGINE_DATALOCALMESSAGE_H
#define _ENGINE_DATALOCALMESSAGE_H

#include "datetime.h"
#include "dbvalue.h"
#include "ideide.h"
#include "buffer.h"
#include "datairecord.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ObjectsMessage;
class XMLPortalExporter;

//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////

class EngineExport DataLocalMessage : public DataIRecord
{
public:
	typedef list<shared_ptr<DataLocalMessage> >::type DataLocalMessageList;

private:
	typedef DataIRecord RecordBase;

// Construction
public:
	DataLocalMessage();
	virtual ~DataLocalMessage();

// Attributes
public:
	MessageFolder getFolder() const;

	MessageStatus getStatus() const;
	void setStatus(MessageStatus status);

	static String toString(MessageFolder folder);
	static String toString(MessageStatus status);

// Operations
public:
	// Decodifica il messaggio
	shared_ptr<ObjectsMessage> decode(const Buffer &private_key);
	// Esporta il messaggio
	void exportXML(shared_ptr<XMLPortalExporter> exporter, const String &subject, const String &body, bool secure);

// IRecord interface
protected:
	virtual String getTableName() const;
	virtual String getRecordKeyName() const;
	virtual String getRecordKeyValue() const; 
	virtual String getRecordKey2Name() const;
	virtual String getRecordKey2Value() const; 

	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:
	DbValue<String> profile;			// Account a cui il messaggio si riferisce
	DbValue<ObjectID> id;				// ID del messaggio originale
	DbValue<ObjectID> author;			// Autore del messaggio
	DbValue<Buffer> session;			// Chiave (criptata con la chiave pubblica del destinatario) con cui  criptato il contenuto del messaggio
	DbValue<Buffer> content;			// Contenuto (criptato con la chiave in chiaro) del messaggio
	DbValue<DateTime> submit_date;	// Data di invio del messaggio
	DbValue<DateTime> read_date;		// Data di lettura del messaggio (DateTime::EMPTY se non  ancora stato letto)
	DbValue<uint32> status;			// Stato del messaggio
	DbValue<uint32> folder;			// ID della directory virtuale del messaggio
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<DataLocalMessage> local_message_cast(shared_ptr<DataIRecord> record) { return boost::dynamic_pointer_cast<DataLocalMessage>(record); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_DATALOCALMESSAGE_H
