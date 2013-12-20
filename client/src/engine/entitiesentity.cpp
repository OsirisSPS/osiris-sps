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
#include "entitiesentity.h"

#include "timerperformance.h"
#include "constants.h"
#include "convert.h"
#include "conversions.h"
#include "iportaldatabase.h"
#include "idbconnection.h"
#include "idesession.h"
#include "dbtables.h"
#include "objectsdescriptor.h"
#include "objectsirevisionable.h"
#include "objectssystem.h"
#include "objectsuser.h"
#include "dbsqlselect.h"
#include "entitiesentitychilds.h"
#include "entitiessnapshotmanager.h"
#include "dataitem.h"
#include "objectssection.h"
#include "osirislink.h"
#include "portalsaccount.h"
#include "portalsportal.h"
#include "lock.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
 
//////////////////////////////////////////////////////////////////////

// Tanti controlli sarebbero abolibili.

EntitiesEntity::ChildsInfo::ChildsInfo(uint32 c, const String &s) : count(c),
															sql(s)
{

}

//////////////////////////////////////////////////////////////////////

EntitiesEntity::EntitiesEntity()
{
	//m_revisions = nullptr;

	clear();
}

EntitiesEntity::~EntitiesEntity()
{
	clear();
}

//bool EntitiesEntity::_loadObject(const shared_ptr<IPortalDatabase> &database, shared_ptr<ObjectsIRevisionable> object)
bool EntitiesEntity::load(const shared_ptr<IPortalDatabase> &database, const EntityID & id)
{
	OS_TIMER_PERFORMANCE(TP, _S("Entity::_loadObject"));
	OS_LOCK(m_cs);

#ifndef OS_TODOCIP
	shared_ptr<ObjectsIRevisionable> object = objects_revisionable_cast(database->getPortal()->getObject(database, id.toUTF16()));
	m_primary = object;
	//OS_ASSERT(getEntityID() == id);
#endif

	if(database->getPortal()->getSnapshotManager()->m_enableEnsureLoadingEntity)
		database->getPortal()->getSnapshotManager()->ensure(database, id);

	DataTable result;
	// TOCLEAN_SNAPSHOT_SCORE
	//String sql = String::format(_S("select current,visible,score,depth,parent,section,stability_date from os_snapshot_objects where reference='%S'").c_str(), m_primary->id->toUTF16().c_str());
	String sql = String::format(_S("select type,current,visible,depth,parent,section,stability_date from os_snapshot_objects where entity='%S'").c_str(), id.toUTF16().c_str());
	database->execute(sql, result);
	if(!result.hasRow(0))
	{
		clear();
		return false;
	}
	else
	{
		ObjectID currentID = static_cast<String>(result.get(0,_S("current"))).to_ascii();
		m_type = Convert::toObjectType(static_cast<uint32>(result.get(0,_S("type"))));
		m_visible = result.get(0,_S("visible"));
		//m_score = *result.get(0,_S("score")); // TOCLEAN_SNAPSHOT_SCORE
		m_depth = result.get(0,_S("depth"));
		m_parent = static_cast<String>(result.get(0,_S("parent"))).to_ascii();
		m_section = static_cast<String>(result.get(0,_S("section"))).to_ascii();
		m_stabilityDate = String(result.get(0,_S("stability_date")));

		if(id == ObjectsSystem::instance()->getRootID())
			//m_current = objects_revisionable_cast(database->getPortal()->getObject(database, ObjectsSystem::instance()->getRootID().toObjectID()));
			m_current = ObjectsSection::getRootSection();
		else
		{		
#ifdef OS_TODOCIP
			/*
			if(currentID.empty())
				return false;
			m_current = objects_revisionable_cast(database->getPortal()->getObject(database, currentID));
			if(m_current == nullptr)
				return false;				
			*/

			if(currentID.empty() == false)
				m_current = objects_revisionable_cast(database->getPortal()->getObject(database, currentID));
#else
			if(currentID.empty())
			{
				m_current.reset();
				// OS_ASSERT(m_parent == ObjectsSystem::instance()->getSkippedID());
				// No, dato che la getEntity non fa + l'ensure, può succedere che carica un oggetto non stabilizzato.
				m_visible = false;
			}
			else
				m_current = objects_revisionable_cast(database->getPortal()->getObject(database, currentID));
#endif
		}

		return true;
	}
}

void EntitiesEntity::clear()
{
	OS_LOCK(m_cs);

#ifndef OS_TODOCIP
	m_primary.reset();
#endif
	m_type = portalObjectTypeUnknown;
	m_current.reset();
	m_visible = false;
	//m_score = 0; TOCLEAN_SNAPSHOT_SCORE
	m_depth = 0;
	m_parent = ObjectsSystem::instance()->getSkippedID();
	m_section = ObjectsSystem::instance()->getSkippedID();
	m_stabilityDate = DateTime::EMPTY;

	m_childs.clear();
	m_countersMap.clear();

	//OS_DELETE_T(m_revisions);
	//m_revisions = nullptr;
}

void EntitiesEntity::clearChildsCache()
{
	OS_LOCK(m_cs);

	m_childs.clear();
}

shared_ptr<EntitiesEntity> EntitiesEntity::getParent(shared_ptr<IPortalDatabase> database) const
{
	// Se  la root, non ha padre.
	if(m_current == nullptr)
		return nullptr;

	if(getEntityID() == ObjectsSystem::instance()->getRootID())
		return nullptr;

	return database->getPortal()->getSnapshotManager()->getEntity(database, m_parent);
}

PortalObjectType EntitiesEntity::getObjectType() const
{
	return m_current != nullptr ? m_current->getObjectType() : portalObjectTypeUnknown;
}

bool EntitiesEntity::isVisible() const
{
	return ( (m_current != nullptr) && (m_current->isVisible()) );
}

DateTime EntitiesEntity::getEntitySubmitDate() const
{

#ifdef OS_TODOCIP
	return m_current->entity_date;
#else
	return m_primary->submit_date;
#endif
}

shared_ptr<EntitiesEntities> EntitiesEntity::getChilds(const shared_ptr<IPortalDatabase> &database, PortalObjectType type, const RangeUint32 &range, shared_ptr<DbSqlSelect> select) const
{
	return getChilds(database, ObjectsTypes(1, type), range, select);
}

shared_ptr<EntitiesEntities> EntitiesEntity::getChilds(const shared_ptr<IPortalDatabase> &database, const ObjectsTypes &types, const RangeUint32 &range, shared_ptr<DbSqlSelect> select) const
{
	OS_ASSERT(select != nullptr);
	OS_ASSERT(select->count == false);

	OS_LOCK(m_cs);

	String sql;
	// Genera l'sql di caricamento dei figli
	_getSql(database, types, range, false, select, sql);

	String childsKey = getChildsKey(types);

	shared_ptr<EntitiesEntities> childs = m_childs.get(childsKey);
	// Controlla se i figli della tipologia specificata sono gi stati caricati in precedenza
	if(childs != nullptr)
	{
		if(childs->getRange() == range && childs->getSql() == sql)
			return childs;	// Se il range in cui sono stati caricati i figli e l'sql di estrazione corrispondono non serve effettuare nuovamente il caricamento...

		// Nota: se il range richiesto non coincide con quello attuale o l'sql  differente bisogna ricaricare i figli
		// Anche se il range attuale comprendesse quello richiesto bisogna comunque ricaricare i figli altrimenti dall'esterno,
		// una volta richiesto un range, scorrendo i figli non si avrebbe la certezza che siano nel range richiesto (a meno di complicare i vari giri)

		m_childs.remove(childsKey);
	}

	// Inizializza la lista dei figli prima di aprire il database (la connessione potrebbe fallire, ma la lista deve essere comunque creata)
	childs.reset(OS_NEW EntitiesEntities(const_cast <EntitiesEntity *>(this)->get_this_ptr(), sql));
	m_childs.push_back(childsKey, childs);

	DataTable result;
	database->execute(sql, result);
	for(uint32 i = 0; i < result.rows(); i++)
	{
		String child_id = result.get(i, DBTABLES::ENTITY);
		childs->push_back(child_id.to_ascii());
	}

	return childs;
}

shared_ptr<EntitiesEntities> EntitiesEntity::getChilds(const shared_ptr<IPortalDatabase> &database, PortalObjectType type, const RangeUint32 &range, CommonOrder order) const
{
	return getChilds(database, ObjectsTypes(1, type), range, order);
}

shared_ptr<EntitiesEntities> EntitiesEntity::getChilds(const shared_ptr<IPortalDatabase> &database, const ObjectsTypes &types, const RangeUint32 &range, CommonOrder order) const
{
	shared_ptr<DbSqlSelect> select;
	getChildsStatement(database, types, range, false, false, false, select);

	switch(order)
	{
	case coSubmitDateAsc:		select->orderBy.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::SUBMIT_DATE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), DbSqlField::foAsc);
								break;

	case coSubmitDateDesc:		select->orderBy.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::SUBMIT_DATE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), DbSqlField::foDesc);
								break;

	case coPositionAsc:			select->orderBy.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::POSITION, DBTABLES::SNAPSHOT_OBJECTS_TABLE), DbSqlField::foAsc);
								break;

	case coPositionDesc:		select->orderBy.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::POSITION, DBTABLES::SNAPSHOT_OBJECTS_TABLE), DbSqlField::foDesc);
								break;

	default:					OS_EXCEPTION("Invalid order");
								break;
	}

	return getChilds(database, types, range, select);
}

shared_ptr<EntitiesEntities> EntitiesEntity::getChilds(const shared_ptr<IPortalDatabase> &database, const ExtensionsComponentID &component, const RangeUint32 &range, CommonOrder order) const
{
	OS_TIMER_PERFORMANCE(TP, _S("Entity::getChilds"));
	shared_ptr<DbSqlSelect> select;
	getChildsStatement(database, portalObjectTypeSection, range, false, false, false, select);
	// Aggiunge alla select la tabella delle sezioni
	select->tables.add(DbSqlTable(DBTABLES::SECTIONS_TABLE));
	// Crea la join tra la tabella di cache e quella delle sezioni
	select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::CURRENT, DBTABLES::SNAPSHOT_OBJECTS_TABLE), DbSqlField(DBTABLES::SECTIONS::ID, DBTABLES::SECTIONS_TABLE));
	// Seleziona il componente specificato
	select->where.add(DbSqlField(DBTABLES::SECTIONS::COMPONENT, DBTABLES::SECTIONS_TABLE), Convert::toSQL(component.toUTF16()));

	switch(order)
	{
	case coSubmitDateAsc:		select->orderBy.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::SUBMIT_DATE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), DbSqlField::foAsc);
								break;

	case coSubmitDateDesc:		select->orderBy.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::SUBMIT_DATE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), DbSqlField::foDesc);
								break;

	case coPositionAsc:			select->orderBy.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::POSITION, DBTABLES::SNAPSHOT_OBJECTS_TABLE), DbSqlField::foAsc);
								break;

	case coPositionDesc:		select->orderBy.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::POSITION, DBTABLES::SNAPSHOT_OBJECTS_TABLE), DbSqlField::foDesc);
								break;

	default:					OS_EXCEPTION("Invalid order");
								break;
	}

	return getChilds(database, portalObjectTypeSection, range, select);
}

uint32 EntitiesEntity::getChildsCount(const shared_ptr<IPortalDatabase> &database, PortalObjectType type, shared_ptr<DbSqlSelect> select) const
{
	return getChildsCount(database, ObjectsTypes(1, type), select);
}

uint32 EntitiesEntity::getChildsCount(const shared_ptr<IPortalDatabase> &database, const ObjectsTypes &types, shared_ptr<DbSqlSelect> select) const
{
	OS_ASSERT(select == nullptr || select->count);		// Se  stata specificata una select valida dovrebbe essere di conteggio

	OS_LOCK(m_cs);

	String sql;
	// Genera l'sql di caricamento dei figli
	_getSql(database, types, RangeUint32::EMPTY, true, select, sql);

	String childsKey = getChildsKey(types);

	ChildCounterMap::iterator i = m_countersMap.find(childsKey);
	// Controlla se il totale  gi stato caricato in precedenza
	if(i != m_countersMap.end())
	{
		// Verifica che l'sql utilizzata in precedenza coincida con quella richiesta
		if(i->second->sql == sql)
		{
			return i->second->count;
		}
		else
		{
			// Resetta il calcolo precedente
			m_countersMap.erase(i);
		}
	}

	uint32 count = 0;

	DataTable result;
	if(database->execute(sql, result))
		count = *result[0][0];

	m_countersMap[childsKey] = shared_ptr<ChildsInfo>(OS_NEW_T(ChildsInfo(count, sql)), os_delete_t());
	return count;
}

void EntitiesEntity::getChildsStatement(const shared_ptr<IPortalDatabase> &database, PortalObjectType type, const RangeUint32 &range, bool count, bool includeSystem, bool includeInvisible, shared_ptr<DbSqlSelect> &statement) const
{
	 getChildsStatement(database, ObjectsTypes(1, type), range, count, includeSystem, includeInvisible, statement);
}

void EntitiesEntity::getChildsStatement(const shared_ptr<IPortalDatabase> &database, const ObjectsTypes &types, const RangeUint32 &range, bool count, bool includeSystem, bool includeInvisible, shared_ptr<DbSqlSelect> &statement) const
{
	OS_LOCK(m_cs);

	// Nota: qui bisogna specificare sempre la tabella perch dall'esterno la select pu essere messa in join con altre tabelle

	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::SNAPSHOT_OBJECTS_TABLE));
	if(count)
		select->count = true;
	else
		select->fields.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::ENTITY, DBTABLES::SNAPSHOT_OBJECTS_TABLE));

	database->getPortal()->getSnapshotManager()->ensureChilds(database, getEntityID());
	select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::PARENT, DBTABLES::SNAPSHOT_OBJECTS_TABLE), Convert::toSQL(getEntityID()));

	// Genera il filtro sui tipi di figli
	StringList childsTypes;
	for(ObjectsTypes::const_iterator i = types.begin(); i != types.end(); ++i)
	{
		if((*i) != portalObjectTypeUnknown)
			childsTypes.push_back(Convert::toSQL(static_cast<uint32>(*i)));
	}

	// Se  stato specificato un filtro sul tipo di figli lo applica
	if(childsTypes.empty() == false)
	{
		select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::TYPE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), childsTypes);
	}

	// Includi le sezioni di sistema (virtuali)?
	// Sono figlie della root, per cui la condizione ha senso solo in quel caso
	if(getEntityID() == ObjectsSystem::instance()->getRootID())
	{
		if(includeSystem == true)
		{
			// Salta la root stessa
			select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::ENTITY, DBTABLES::SNAPSHOT_OBJECTS_TABLE), Convert::toSQL(ObjectsSystem::instance()->getRootID()), DbSqlCondition::cfDifferent | DbSqlCondition::cfAnd);
		}
		else
		{
			// VERYURGENT: Le classi statement non hanno una "not in"... poco male, ma userebbe meglio l'indice.
			select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::INSERT_DATE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), Convert::toSQL(DateTime::EMPTY), DbSqlCondition::cfDifferent | DbSqlCondition::cfAnd);
		}
	}

	if(includeInvisible == false)
		select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::VISIBLE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), Convert::toSQL(true));

	uint32 size = range.size();
	if(size != 0)
		select->limit.setCount(size);

	uint32 start = range.start();
	if(start != 0)
		select->limit.setOffset(start);

	statement = select;
}

/*
const StringList & EntitiesEntity::getRevisions(const shared_ptr<IPortalDatabase> &database) const
{
	OS_LOCK(m_cs);

	if(m_revisions == nullptr)
	{
		m_revisions = OS_NEW_T(StringList);

		if(m_primary != nullptr) // OS_TODOCIP: da togliere?
		{
			String entityID = getEntityID().toUTF16();
#ifdef OS_TODOCIP
			String sql = String::format(_S("select id from %S where entity_id='%S' order by submit_date desc").c_str(), m_primary->getDescriptor()->getTableName().c_str(), entityID.c_str());
#else
			String sql = String::format(_S("select id from %S where revision='%S' order by submit_date desc").c_str(), m_primary->getDescriptor()->getTableName().c_str(), entityID.c_str());
#endif

			DataTable result;
			if(database->execute(sql, result))
			{
				for(uint32 i = 0; i < result.rows(); i++)
				{
					String id = *result.get(i, _S("id"));
					m_revisions->push_back(id);
				}
			}
		}
	}

	return *m_revisions;
}
*/

bool EntitiesEntity::allowChild(PortalObjectType type) const
{
	return (m_current != nullptr) ? m_current->allowChild(type) : false;
}

bool EntitiesEntity::allowChild(shared_ptr<EntitiesEntity> second) const
{
	OS_ASSERT(second != nullptr);
	return allowChild(second->getObjectType());
} 

std::string EntitiesEntity::getViewLink(const shared_ptr<Portal> portal, const ordered_map<std::wstring, std::wstring> &params) const
{
	return portal->getViewObjectLink(getEntityID(), params);
}

std::string EntitiesEntity::getEditLink(const shared_ptr<Portal> portal, const ordered_map<std::wstring, std::wstring> &params) const
{
	return portal->getEditObjectLink(getEntityID(), params);
}

String EntitiesEntity::getChildKey(PortalObjectType type)
{
	return getChildsKey(ObjectsTypes(1, type));
}

String EntitiesEntity::getChildsKey(const ObjectsTypes &types)
{
	OS_ASSERT(types.empty() == false);

	String key;
	for(ObjectsTypes::const_iterator i = types.begin(); i != types.end(); ++i)
	{
		if(i != types.begin())
			key.append(_S("-"));

		key.append(conversions::to_utf16(static_cast<uint32>(*i)));
	}

	return key;
}

void EntitiesEntity::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	if(m_current == nullptr)
		return;

	m_current->exportXML(exporter);

	// "showable"  diverso dal "visible" esportato dalla corrente.
	// Un oggetto potrebbe essere invisibile per diversi motivi, ad esempio se il padre  invisibile.
	exporter->getRoot()->setAttributeBool(_S("showable"), m_visible);
	exporter->getRoot()->setAttributeUint32(_S("depth"), m_depth);
	exporter->getRoot()->setAttributeString(_S("section"), m_section.toUTF16());

	/* URGENT da testare
	// Se la sezione non  lui stesso
	if(m_section != getPrimaryID())
	{
		entity_ptr section = getLoggedUser()->getEntity(m_section);
		if(section != nullptr)
		{
			shared_ptr<XMLNode> nodeSection = exporter->getRoot()->addChild(_S("section"));
			shared_ptr<XMLPortalExporter> exporterSection(OS_NEW XMLExporter(nodeSection, getPage(), false));
			section->exportXML(exporterSection);
		}
	}
	*/

	//if(exporter->getMode() == XMLPortalExporter::emFull)
	{
		String id = getEntityID().toUTF16();
		String portal = exporter->getPortal()->getPortalID().toUTF16();
		String title = exporter->getRoot()->getAttributeString(_S("title"));
		if(title.empty())
			title = id;

		/*
		String omlCodeView;
		if(id != String::EMPTY)
			omlCodeView = String::format(_S("[iurl=\"osiris://|url|portals/view|id=%S&portal=%S\"]%S[/iurl]").c_str(), id.c_str(), portal.c_str(), title.c_str());
		else
			omlCodeView = String::format(_S("[iurl=\"osiris://|url|portals/view|portal=%S\"]%S[/iurl]").c_str(), portal.c_str(), title.c_str());
		exporter->getRoot()->setAttributeString(_S("oml_code2"),omlCodeView);
		*/

		OsirisLink link;
		link.setParam("type", _W("url"));
		link.setParam("portal", portal.to_wide());
		if(id != String::EMPTY)
			link.setParam("url",_W("/portals/view?id=") + id.to_wide());
		else
			link.setParam("url",_W("/portals/view"));

		exporter->getRoot()->setAttributeString(_S("oml_code"),_S("[iurl=\"") + link.generate() + _S("\"]") + title + _S("[/iurl]"));	
		
	}
}

void EntitiesEntity::_getSql(const shared_ptr<IPortalDatabase> &database, const ObjectsTypes &types, const RangeUint32 &range, bool count, shared_ptr<DbSqlSelect> select, String &sql) const
{
	shared_ptr<DbSqlSelect> statement = select;
	// Controlla se non  stato specificato alcun statement
	if(statement == nullptr)
		// Carica lo statement di default
		getChildsStatement(database, types, range, count, false, false, statement);

	// Genera l'sql di caricamento dei figli
	database->getConnection()->_parse(statement, sql);
}



//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
