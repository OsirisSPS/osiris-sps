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

#ifndef _OS_CORE_ICACHE_H
#define _OS_CORE_ICACHE_H

#include "base/object.h"
#include "boost/thread/recursive_mutex.hpp"
#include "collections.h"
#include "lock.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename K, typename T, typename S, bool nullable = true>
class ICache : public Object
{
	typedef typename Collections<K, T>::shared_collection collection_type;

	typedef typename collection_type::key key;
	typedef typename collection_type::key_ref key_ref;

	typedef S source;
	typedef shared_ptr<T> pointer_type;

// Construction
public:
	ICache(uint32 limit);
	virtual ~ICache();

// Attributes
public:
	// Restituisce il numero di elementi attuali prensenti nella cache
	inline uint32 size() const;

	// Restituisce la dimensione massima della cache
	inline uint32 getLimit() const;
	// Imposta la dimensione massima della cache
	inline void setLimit(const uint32 &limit);

// Operations
public:
	// Restituisce l'oggetto specificato, se non esiste viene caricato
	pointer_type get(key_ref key, source s);

	// Restituisce true se l'oggetto specificato  presente nella cache
	bool exists(key_ref key);

	// Ricerca l'oggetto specificato (restituisce nullptr nel caso non esista)
	pointer_type find(key_ref key);

	// Rimuove un oggetto dalla cache
	void remove(key_ref key);

	// Svuota la cache
	void clear();

	// Inserisce un elemento nella cache
	void insert(key_ref key, pointer_type ptr);

private:
	void _resize();
	pointer_type _extract(key_ref key, source s);

// Interface
protected:
	virtual pointer_type load(key_ref key, source s) = 0;

protected:
	uint32 m_limit;
	mutable boost::recursive_mutex m_cs;			// boost::recursive_mutex per rendere la mappa thread-safe
	collection_type m_entries;
};

//////////////////////////////////////////////////////////////////////

template <typename K, typename T, typename S, bool nullable>
inline uint32 ICache <K, T, S, nullable>::size() const
{
	OS_LOCK(m_cs);
	return static_cast<uint32>(m_entries.size());
}

template <typename K, typename T, typename S, bool nullable>
inline uint32 ICache <K, T, S, nullable>::getLimit() const 
{
	OS_LOCK(m_cs);
	return m_limit; 
}

template <typename K, typename T, typename S, bool nullable>
inline void ICache <K, T, S, nullable>::setLimit(const uint32 &limit)
{
	OS_LOCK(m_cs);
	m_limit = std::max <uint32>(1, limit);
	_resize();
}

//////////////////////////////////////////////////////////////////////

template <typename K, typename T, typename S, bool nullable>
ICache <K, T, S, nullable>::ICache(uint32 limit)
{
	setLimit(limit);
}

template <typename K, typename T, typename S, bool nullable>
ICache <K, T, S, nullable>::~ICache()
{
	clear();
}

template <typename K, typename T, typename S, bool nullable>
typename ICache <K, T, S, nullable>::pointer_type ICache <K, T, S, nullable>::get(key_ref key, source s)
{
	return _extract(key, s);
}

template <typename K, typename T, typename S, bool nullable>
bool ICache <K, T, S, nullable>::exists(key_ref key)
{ 
	OS_LOCK(m_cs);
	return m_entries.exists(key);
}

template <typename K, typename T, typename S, bool nullable>
typename ICache <K, T, S, nullable>::pointer_type ICache <K, T, S, nullable>::find(key_ref key)
{
	OS_LOCK(m_cs);
	return m_entries.get(key);
}

template <typename K, typename T, typename S, bool nullable>
void ICache <K, T, S, nullable>::remove(key_ref key)
{
	OS_LOCK(m_cs);
	m_entries.remove(key);
}

template <typename K, typename T, typename S, bool nullable>
void ICache <K, T, S, nullable>::clear()
{
	OS_LOCK(m_cs);
	m_entries.clear();
}

template <typename K, typename T, typename S, bool nullable>
void ICache <K, T, S, nullable>::insert(key_ref key, pointer_type ptr)
{
	OS_LOCK(m_cs);
	m_entries.remove(key);
	m_entries.push_back(key, ptr);
	_resize();
}

template <typename K, typename T, typename S, bool nullable>
void ICache <K, T, S, nullable>::_resize()
{
	OS_LOCK(m_cs);
	utils::pop_front_until(m_entries, m_limit);
}

template <typename K, typename T, typename S, bool nullable>
typename ICache <K, T, S, nullable>::pointer_type ICache <K, T, S, nullable>::_extract(key_ref key, source s)
{	
	pointer_type ptr;
	if(exists(key))
		ptr = find(key);
	else
		ptr = load(key, s);
	
	if(nullable || ptr != nullptr)
		insert(key, ptr);

	return ptr;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_ICACHE_H
