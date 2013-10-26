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

#ifndef _ENTITIES_ENTITY_H
#define _ENTITIES_ENTITY_H

#include "base/object.h"
#include "boost/thread/recursive_mutex.hpp"
#include "collections.h"
#include "datetime.h"
#include "dbsqlselect.h"
#include "enable_this_ptr.h"
#include "entitiesentities.h"
#include "extensionsextensions.h"
#include "ideide.h"
#include "objectsobjects.h"
#include "objectsirevisionable.h"
#include "ordered_map.h"
#include "portalsportals.h"
#include "range.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ExtensionsComponentID;
class IPortalDatabase;
class Portal;
class XMLPortalExporter;

//////////////////////////////////////////////////////////////////////

class EngineExport EntitiesEntity : public Object,
								    public enable_this_ptr<EntitiesEntity>
{
// Types
public:
	typedef list<shared_ptr<EntitiesEntity> >::type EntitiesList;

// Enumerated types
public:
	enum CommonOrder
	{
		coSubmitDateAsc		= 0,
		coSubmitDateDesc	= 1,
		coPositionAsc		= 2,
		coPositionDesc		= 3
	};

// Internal
private:
	struct ChildsInfo
	{
		ChildsInfo(uint32 c, const String &s);

		uint32 count;		// Numero totale di figli
		String sql;			// Sql con cui  stato estratto il totale
	};

public:
	typedef Collections<String, EntitiesEntities>::shared_collection ChildsCollection;
	typedef map<String, shared_ptr<ChildsInfo> >::type ChildCounterMap;

// Construction
public:
	EntitiesEntity();
	virtual ~EntitiesEntity();

// Attributes
public:

#ifdef OS_TODOCIP
	inline const EntityID & getEntityID() const;
#else
	// Restituisce l'oggetto base
	inline shared_ptr<ObjectsIRevisionable> getPrimary() const;
	inline EntityID getEntityID() const;
#endif

	// Restituisce l'oggetto corrente da utilizzare per il render dell'entit
	inline shared_ptr<ObjectsIRevisionable> getCurrent() const;
	// Restituisce l'ID dell'entit padre
	inline const EntityID & getParentID() const;
	// Restituisce l'ID della sezione di riferimento
	inline const EntityID & getSectionID() const;
	// Restituisce se l'entit  visible
	inline bool getVisible() const;
	// Restituisce la data di stabilit
	inline const DateTime & getStabilityDate() const;
	// Restituisce la data delle statistiche
	//inline const DateTime & getStatisticsDate() const;
	// Restituisce lo score
	//inline double getScore() const; // TOCLEAN_SNAPSHOT_SCORE
	// Restituisce il depth
	inline uint32 getDepth() const;
	// Restituisce il padre l'entit padre
    shared_ptr<EntitiesEntity> getParent(shared_ptr<IPortalDatabase> database) const;
	// Restituisce il tipo di oggetto rappresentato dall'entit
	PortalObjectType getObjectType() const;
	// Restituisce se l'entit  visibile (se non ha una corrente, o la corrente specifica che non  visibile)
	bool isVisible() const;

	DateTime getEntitySubmitDate() const;

	// Restituisce il puntatore al portale
	//shared_ptr<Portal> getPortal() const;

	// Restituisce gli id dei figli dell'oggetto di tipo specificato (non restituisce mai null, usare otUnknown per tutti i figli)
	shared_ptr<EntitiesEntities> getChilds(const shared_ptr<IPortalDatabase> &database, PortalObjectType type, const RangeUint32 &range, shared_ptr<DbSqlSelect> select) const;
	shared_ptr<EntitiesEntities> getChilds(const shared_ptr<IPortalDatabase> &database, const ObjectsTypes &types, const RangeUint32 &range, shared_ptr<DbSqlSelect> select) const;

	shared_ptr<EntitiesEntities> getChilds(const shared_ptr<IPortalDatabase> &database, PortalObjectType type, const RangeUint32 &range = RangeUint32(0, 0), CommonOrder order = coSubmitDateAsc) const;
	shared_ptr<EntitiesEntities> getChilds(const shared_ptr<IPortalDatabase> &database, const ObjectsTypes &types, const RangeUint32 &range = RangeUint32(0, 0), CommonOrder order = coSubmitDateAsc) const;

	// Restituisce le sezioni figlie dell'oggetto col componente specificato
	shared_ptr<EntitiesEntities> getChilds(const shared_ptr<IPortalDatabase> &database, const ExtensionsComponentID &component, const RangeUint32 &range = RangeUint32(0, 0), CommonOrder order = coPositionAsc) const;

	// Restituisce il numero totale di figli di un tipo specificato
	uint32 getChildsCount(const shared_ptr<IPortalDatabase> &database, PortalObjectType type, shared_ptr<DbSqlSelect> select = null) const;
	uint32 getChildsCount(const shared_ptr<IPortalDatabase> &database, const ObjectsTypes &types, shared_ptr<DbSqlSelect> select = null) const;

	// Genera lo statement di base per il caricamento dei figli dell'entit
	void getChildsStatement(const shared_ptr<IPortalDatabase> &database, PortalObjectType type, const RangeUint32 &range, bool count, bool includeSystem, bool includeInvisible, shared_ptr<DbSqlSelect> &statement) const;
	void getChildsStatement(const shared_ptr<IPortalDatabase> &database, const ObjectsTypes &types, const RangeUint32 &range, bool count, bool includeSystem, bool includeInvisible, shared_ptr<DbSqlSelect> &statement) const;

	// Restituisce la lista delle revisioni dell'oggetto
	//const StringList & getRevisions(const shared_ptr<IPortalDatabase> &database) const;

	bool allowChild(PortalObjectType type) const;
	bool allowChild(shared_ptr<EntitiesEntity> second) const;

	std::string getViewLink(const shared_ptr<Portal> portal, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>())) const;
	std::string getEditLink(const shared_ptr<Portal> portal, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>())) const;
	
private:
	static String getChildKey(PortalObjectType type);
	static String getChildsKey(const ObjectsTypes &types);

// Operations
public:
	// Carica l'entit
	//bool load(const shared_ptr<IPortalDatabase> &database, shared_ptr<ObjectsIRevisionable> object);
	bool load(const shared_ptr<IPortalDatabase> &database, const EntityID & id);

	// Svuota l'entità
	void clear();

	// Svuota la cache dei figli
	void clearChildsCache();

	// Esporta la revisione corrente dell'entit
	void exportXML(shared_ptr<XMLPortalExporter> exporter);

private:
	void _getSql(const shared_ptr<IPortalDatabase> &database, const ObjectsTypes &types, const RangeUint32 &range, bool count, shared_ptr<DbSqlSelect> select, String &sql) const;
	//bool _loadObject(const shared_ptr<IPortalDatabase> &database, shared_ptr<ObjectsIRevisionable> object);

protected:
	//entities::snapshot_profile_weak_ptr m_profile;
#ifndef OS_TODOCIP
	shared_ptr<ObjectsIRevisionable> m_primary;
#endif
	shared_ptr<ObjectsIRevisionable> m_current;
	EntityID m_parent;
	EntityID m_section;
	bool m_visible;
	PortalObjectType m_type;
	//double m_score; // TOCLEAN_SNAPSHOT_SCORE
	uint32 m_depth;
	DateTime m_stabilityDate;

	mutable boost::recursive_mutex m_cs;
	mutable ChildsCollection m_childs;
	mutable ChildCounterMap m_countersMap;
	//mutable StringList *m_revisions;
};

//////////////////////////////////////////////////////////////////////

//inline entities::snapshot_profile_ptr Entity::getSnapshotProfile() const { return m_profile.lock(); }
#ifdef OS_TODOCIP
	inline const EntityID & EntitiesEntity::getEntityID() const { return (m_current != null) ? m_current->getEntityID() : EntityID::EMPTY; }
#else
	inline shared_ptr<ObjectsIRevisionable> EntitiesEntity::getPrimary() const { return m_primary; }
	inline EntityID EntitiesEntity::getEntityID() const { return (m_primary != null) ? m_primary->getEntityID() : EntityID::EMPTY; }
#endif
inline shared_ptr<ObjectsIRevisionable> EntitiesEntity::getCurrent() const { return m_current; }
inline const EntityID& EntitiesEntity::getParentID() const { return m_parent; }
inline const EntityID& EntitiesEntity::getSectionID() const { return m_section; }
inline bool EntitiesEntity::getVisible() const { return m_visible; }
inline const DateTime & EntitiesEntity::getStabilityDate() const { return m_stabilityDate; }
//inline double EntitiesEntity::getScore() const { return m_score; } // TOCLEAN_SNAPSHOT_SCORE
inline uint32 EntitiesEntity::getDepth() const { return m_depth; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENTITIES_ENTITY_H
