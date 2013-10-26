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

#ifndef _ENTITIES_ENTITIES_H
#define _ENTITIES_ENTITIES_H

#include "base/object.h"
#include "entitiesentities.h"
#include "dbdb.h"
#include "ids.h"
#include "objectsobjects.h"
#include "range.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;

//////////////////////////////////////////////////////////////////////

class EngineExport EntitiesEntities : public Object
{
public:
	typedef list<EntityID>::type list_type;
	typedef list_type::value_type value_type;
	typedef list_type::iterator iterator;
	typedef list_type::const_iterator const_iterator;
	typedef list_type::reverse_iterator reverse_iterator;
	typedef list_type::const_reverse_iterator const_reverse_iterator;

// Construction
public:
	EntitiesEntities(shared_ptr<EntitiesEntity> parent, const String &sql);
	virtual ~EntitiesEntities();

// Attributes
public:
	inline shared_ptr<EntitiesEntity> getParent() const;
	inline const list_type & getObjects() const;
	inline const RangeUint32 & getRange() const;
	inline const String & getSql() const;

	inline iterator begin();
	inline const_iterator begin() const;
	inline iterator end();
	inline const_iterator end() const;

	inline reverse_iterator rbegin();
	inline const_reverse_iterator rbegin() const;
	inline reverse_iterator rend();
	inline const_reverse_iterator rend() const;

	inline bool empty() const;
	inline size_t size() const;

	shared_ptr<EntitiesEntity> get(shared_ptr<IPortalDatabase> database, const value_type &id) const;

// Operations
public:
	inline void push_back(const value_type &id);
	inline void push_front(const value_type &id);

protected:
	weak_ptr<EntitiesEntity> m_parent;		// Entit padre
	list_type m_objects;			// Elenco degli oggetti di riferimento
	RangeUint32 m_range;			// Range di riferimento degli oggetti
	String m_sql;					// Sql con cui sono state estratte le entit
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<EntitiesEntity> EntitiesEntities::getParent() const { return m_parent.lock(); }
inline const EntitiesEntities::list_type & EntitiesEntities::getObjects() const { return m_objects; }
inline const RangeUint32 & EntitiesEntities::getRange() const { return m_range; }
inline const String & EntitiesEntities::getSql() const { return m_sql; }

inline EntitiesEntities::iterator EntitiesEntities::begin() { return m_objects.begin(); }
inline EntitiesEntities::const_iterator EntitiesEntities::begin() const { return m_objects.begin(); }
inline EntitiesEntities::iterator EntitiesEntities::end() { return m_objects.end(); }
inline EntitiesEntities::const_iterator EntitiesEntities::end() const { return m_objects.end(); }

inline EntitiesEntities::reverse_iterator EntitiesEntities::rbegin() { return m_objects.rbegin(); }
inline EntitiesEntities::const_reverse_iterator EntitiesEntities::rbegin() const { return m_objects.rbegin(); }
inline EntitiesEntities::reverse_iterator EntitiesEntities::rend() { return m_objects.rend(); }
inline EntitiesEntities::const_reverse_iterator EntitiesEntities::rend() const { return m_objects.rend(); }

inline bool EntitiesEntities::empty() const { return m_objects.empty(); }
inline size_t EntitiesEntities::size() const { return m_objects.size(); }

inline void EntitiesEntities::push_back(const value_type &id) { m_objects.push_back(id); }
inline void EntitiesEntities::push_front(const value_type &id) { m_objects.push_front(id); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENTITIES_ENTITIES_H
