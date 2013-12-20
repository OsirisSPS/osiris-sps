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

#ifndef _ENGINE_OBJECTSIREVISIONABLE_H
#define _ENGINE_OBJECTSIREVISIONABLE_H

#include "objectsientry.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Portal;

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsIRevisionable : public ObjectsIEntry
{
	typedef ObjectsIEntry EntryBase;

// Construction
public:
	ObjectsIRevisionable(PortalObjectType object_type, uint32 object_version);
	virtual ~ObjectsIRevisionable();

// Attributes
public:
	inline bool hasParent() const;
	// Restituisce il padre corrente dell'oggetto
	const EntityID & getParent() const;
	
	bool isVisible() const;

	//bool isEntity() const; // OS_TODOCIP: Si riesce ad abolire? Forse, è usata spesso con OS_TODOCIP non definito (vecchia versione no CIP).
#ifdef OS_TODOCIP
	const EntityID & getEntityID() const; // vecchia getPrimary
	const ObjectID & getEntityAuthor() const;
	const ObjectID & getEntityPov() const;
	const DateTime & getEntityDate() const;
	const EntityID & getEntityParent() const;
#else
	EntityID getEntityID() const; // vecchia getPrimary
#endif

	//bool isPrimary() const; // OS_TODOCIP: da abolire
	//bool isRevision() const;	// OS_TODOCIP: da abolire
	//const ObjectID & getPrimary() const; // OS_TODOCIP: da abolire

// Operations
public:
	void changeToRevision(shared_ptr<ObjectsIRevisionable> object = nullptr);
	bool signEntity(const Buffer &private_key, const PortalID &portal);

// Overridables
public:
	// Indica se il padre  richiesto dall'oggetto
	virtual bool requireParent() const;

// IEntry overrides
public:
	//virtual bool isObjectPrimary() const;
	//virtual bool isObjectVisible() const;

	//virtual void exportBase(shared_ptr<XMLPortalExporter> exporter);

	//virtual const ObjectID & getPrimary() const;

	virtual void getDependencies(const shared_ptr<IPortalDatabase> &db, list<ObjectID>::type &dependencies);

	virtual bool validate(shared_ptr<IPortalDatabase> database) const;
	LanguageResult acceptable(shared_ptr<IPortalDatabase> database) const;

	virtual void exportXML(shared_ptr<XMLPortalExporter> exporter);
	virtual void exportAuthor(shared_ptr<XMLPortalExporter> exporter);

protected:
	virtual void onObjectCreated(const shared_ptr<IPortalDatabase> &db, shared_ptr<Portal> portal);

protected:
	virtual bool onCreate(const shared_ptr<IPortalDatabase> &db, const Buffer &privateKey);
	virtual bool onSign(const shared_ptr<ObjectsSigner> &signer) const;

	virtual bool onRead(const ObjectID &id, const DataTree &data);
	virtual bool onWrite(DataTree &data) const;

	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:	

	DbValue<EntityID> parent;	
#ifdef OS_TODOCIP
	DbValue<EntityID> entity;
	DbValue<ObjectID> entity_author; 
	DbValue<DateTime> entity_date; 
	DbValue<EntityID> entity_parent; 
	DbValue<Buffer> entity_signature; 
#else
	DbValue<EntityID> revision;
#endif
	DbValue<bool> visible;
	DbValue<double> position;
	DbValue<String> comment;
};

//////////////////////////////////////////////////////////////////////

inline bool ObjectsIRevisionable::hasParent() const { return getParent().empty() == false; }
inline bool ObjectsIRevisionable::isVisible() const { return visible; }

#ifdef OS_TODOCIP
//inline bool ObjectsIRevisionable::isEntity() const { return entity == id->toEntityID(); } 
inline const EntityID & ObjectsIRevisionable::getEntityID() const { return entity; }
inline const ObjectID & ObjectsIRevisionable::getEntityAuthor() const { return entity_author; }
inline const DateTime & ObjectsIRevisionable::getEntityDate() const { return entity_date; }
inline const EntityID & ObjectsIRevisionable::getEntityParent() const { return entity_parent; }
#else
inline bool ObjectsIRevisionable::isEntity() const { return revision->empty(); }
//inline const EntityID & ObjectsIRevisionable::getEntityID() const { return ( isEntity() ? id->toEntityID() : revision); }
inline EntityID ObjectsIRevisionable::getEntityID() const { return ( isEntity() ? id->toEntityID() : revision); }
#endif

//inline bool IRevisionable::isPrimary() const { return isEntity(); } 
//inline bool IRevisionable::isRevision() const { return !isEntity(); } 
//inline const ObjectID & IRevisionable::getPrimary() const { return getEntityID(); }

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ObjectsIRevisionable> objects_revisionable_cast(shared_ptr<ObjectsIObject> object) { return boost::dynamic_pointer_cast<ObjectsIRevisionable>(object); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_OBJECTSIREVISIONABLE_H
