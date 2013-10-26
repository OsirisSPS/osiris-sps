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

#ifndef _ENGINE_OBJECTSIOBJECT_H
#define _ENGINE_OBJECTSIOBJECT_H

#include "datairecord.h"
#include "dbvalue.h"
#include "buffer.h"
#include "datetime.h"
#include "languageresult.h"
#include "ideide.h"
#include "portalsportals.h"
#include "objectsobjects.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IdeSession;
class ObjectsSigner;
class Portal;
class PortalID;
class XMLPortalExporter;

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsIObject : public DataIRecord
{
	typedef DataIRecord RecordBase;

// Construction
public:
	ObjectsIObject(PortalObjectType object_type, uint32 object_version);
	virtual ~ObjectsIObject();

// Attributes
public:
	inline PortalObjectType getObjectType() const;

	inline bool isSigned() const;
	inline bool isUser() const;
	//inline bool isPrimary() const;
	//inline bool isRevision() const;
	//inline bool isVisible() const;

	bool isVotable() const;

	// Restituisce il descrittore dell'oggetto
	shared_ptr<ObjectsIDescriptor> getDescriptor() const;

	// Restituisce la dimensione in bytes dell'oggetto
	uint32 getSize() const;

	// Restituisce l'hash dell'oggetto
	String getHash() const;

	// Restituisce il contenuto da indicizzare per la ricerca
	String getSearchContent() const;

	// Indica se la tipologia di padre specificata  valida per l'oggetto
	bool allowParent(PortalObjectType type) const;
	// Indica se la tipologia di figlio specificata  valida per l'oggetto
	bool allowChild(PortalObjectType type) const;

// Operations
public:
	// Firma l'oggetto con una chiave privata
	bool sign(const Buffer &private_key, const PortalID &portal);

	// Verifica che l'oggetto sia stato firmato con la chiave privata associata a quella specificata
	bool verify(const Buffer &public_key, const PortalID &portal) const;

	// Carica la struttura da un DataTree
	bool read(const ObjectID &id, const DataTree &data);
	// Salva la struttura in un DataTree
	bool write(DataTree &data) const;

	// Carica la struttura da una riga di una tabella
	bool read(const DataTableRow &row);
	// Salva la struttura in una struttura VALUES
	bool write(DbSqlValues &values) const;

	// Clona i parametri dell'oggetto specificato
	shared_ptr<ObjectsIObject> clone() const;

	// Crea l'oggetto e lo inserisce nel database
	//bool create(const shared_ptr<IPortalDatabase> &db, shared_ptr<IdeSession> ideSession);
	LanguageResult create(const shared_ptr<IPortalDatabase> &db, shared_ptr<IdeSession> session);
	LanguageResult create(const shared_ptr<IPortalDatabase> &db, const Buffer &privateKey);	

	LanguageResult store(const shared_ptr<IPortalDatabase> &db);

	// Helper for isAcceptable
	LanguageResult acceptableForBadWords(const shared_ptr<IPortalDatabase> &database, const String& text) const;

// Overridables
public:
	virtual void ensureID(const shared_ptr<IPortalDatabase> &db); // TOCLEAN: il db non serve, ma dovrei spostare roba da IPortalDatabase...
	//virtual bool isObjectPrimary() const;
	//virtual bool isObjectVisible() const;

	//virtual const ObjectID & getPrimary() const;

	// Carica gli id di dipendenza dell'oggetto (nota: l'autore non viene contemplato)
	virtual void getDependencies(const shared_ptr<IPortalDatabase> &db, list<ObjectID>::type &dependencies);

	// Restituisce il titolo dell'oggetto
	virtual const String & getTitle() const;
	// Restituisce la descrizione dell'oggetto
	virtual const String & getDescription() const;
	// Restituisce il contenuto dell'oggetto
	virtual const String & getContent() const;

	// Enumera i valori di ricerca (di default titolo + contenuto)
	virtual void enumerateSearchValues(StringList &values) const;

	// Esporta l'oggetto in versione di riepilogo
	virtual void exportXML(shared_ptr<XMLPortalExporter> exporter);
	
	// Evento richiamato per effettuare eventuali formattazioni sul titolo prima dell'esportazione
	virtual void onExportTitle(String &title, shared_ptr<XMLPortalExporter> exporter);
	// Evento richiamato per effettuare eventuali formattazioni sulla descrizione prima dell'esportazione
	virtual void onExportDescription(String &description, shared_ptr<XMLPortalExporter> exporter);
	// Evento richiamato per effettuare eventuali formattazioni sul contenuto prima dell'esportazione
	virtual void onExportContent(String &content, shared_ptr<XMLPortalExporter> exporter);

protected:
	virtual void onObjectCreated(const shared_ptr<IPortalDatabase> &db, shared_ptr<Portal> portal);

// IRecord overrides
public:
	virtual bool validate(shared_ptr<IPortalDatabase> database) const;
	virtual LanguageResult acceptable(shared_ptr<IPortalDatabase> database) const;
	virtual String getTableName() const;
	virtual String getRecordKeyName() const;
	virtual String getRecordKeyValue() const;

protected:
	virtual bool onCreate(const shared_ptr<IPortalDatabase> &db, const Buffer &privateKey);

	virtual bool onSign(const shared_ptr<ObjectsSigner> &signer) const;

	virtual bool onRead(const ObjectID &id, const DataTree &data);
	virtual bool onWrite(DataTree &data) const;

// IRecord interface
protected:
	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

// Interface
public:
	virtual const ObjectID & getAuthor() const = 0;

public:	
	DbValue<ObjectID> id;
#ifdef OS_TODOCIP	
	DbValue<PovID> pov;	
#endif
	DbValue<DateTime> submit_date; // CLODOVERYURGENT: ovunque usato, probabile che serve anche lì il "pov".
	DbValue<uint32> version;
	DbValue<Buffer> signature;

protected:
	const PortalObjectType m_objectType;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ObjectsIObject> objects_object_cast(shared_ptr<DataIRecord> record) { return boost::dynamic_pointer_cast<ObjectsIObject>(record); }

//////////////////////////////////////////////////////////////////////

inline PortalObjectType ObjectsIObject::getObjectType() const { return m_objectType; }

inline bool ObjectsIObject::isSigned() const { return (signature->getSize()>1); } // Older 0.X objects has 1 byte '00' as signature.
inline bool ObjectsIObject::isUser() const { return getObjectType() == portalObjectTypeUser; }
//inline bool ObjectsIObject::isPrimary() const { return isObjectPrimary(); }
//inline bool ObjectsIObject::isRevision() const { return isObjectPrimary() == false; }
//inline bool ObjectsIObject::isVisible() const { return isObjectVisible(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_OBJECTSIOBJECT_H
