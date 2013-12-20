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

#ifndef _OS_CORE_GROWABLECACHE_H
#define _OS_CORE_GROWABLECACHE_H

#include "base/object.h"
#include "boost/none.hpp"
#include "boost/optional.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T, typename C = typename list<shared_ptr<T> >::type>
class IGrowableCache : public Object
{
public:
	typedef C Entries;

// Construction
public:
	IGrowableCache(boost::optional<uint32> optimalSize = boost::none);
	virtual ~IGrowableCache();

// Attributes
public:
    inline boost::optional<uint32> getOptimalSize() const;
    inline void setOptimalSize(boost::optional<uint32> optimalSize);

	// Indica se la cache è vuota
	inline bool empty() const;
	// Restituisce il numero di elementi presenti nella cache
	inline uint32 size() const;

// Operations
public:
	// Svuota la cache
	void clear();
	// Estrae il primo elemento disponibile nella cache, se necessario crea un nuovo elemento
	shared_ptr<T> peekEntry();

private:
	shared_ptr<T> peekAvailableEntry();

// Interface
protected:
	virtual shared_ptr<T> createEntry() = 0;

protected:
	boost::optional<uint32> m_optimalSize;
	Entries m_entries;
};

////////////////////////////////////////////////////////////////////////////

template <typename T, typename C>
IGrowableCache<T, C>::IGrowableCache(boost::optional<uint32> optimalSize)
{
    setOptimalSize(optimalSize);
}

template <typename T, typename C>
IGrowableCache<T, C>::~IGrowableCache()
{

}

template <typename T, typename C>
inline boost::optional<uint32> IGrowableCache<T, C>::getOptimalSize() const
{
    return m_optimalSize;
}

template <typename T, typename C>
inline void IGrowableCache<T, C>::setOptimalSize(boost::optional<uint32> optimalSize)
{
    OS_ASSERT(optimalSize == boost::none || optimalSize.get() > 0);
    m_optimalSize = optimalSize;
}

template <typename T, typename C>
inline bool IGrowableCache<T, C>::empty() const
{
	return static_cast<uint32>(m_entries.empty());
}

template <typename T, typename C>
inline uint32 IGrowableCache<T, C>::size() const
{
	return static_cast<uint32>(m_entries.size());
}

template <typename T, typename C>
void IGrowableCache<T, C>::clear()
{
	m_entries.clear();
}

template <typename T, typename C>
shared_ptr<T> IGrowableCache<T, C>::peekEntry()
{
	shared_ptr<T> entry = peekAvailableEntry();
	if(entry != nullptr)
		return entry;

	entry = createEntry();
	if(entry != nullptr)
		m_entries.push_back(entry);

	return entry;
}

template <typename T, typename C>
shared_ptr<T> IGrowableCache<T, C>::peekAvailableEntry()
{
	shared_ptr<T> entry;
	for(typename Entries::iterator i = m_entries.begin(); i != m_entries.end();)
	{
		typename Entries::iterator current = i;
		++i;

		// Controlla se l'entry corrente è disponibile per l'utilizzo
		if((*current).unique())
		{
			// Controlla se non è stata trovata ancora un'entry disponibile
			if(entry == nullptr)
			{
				// Salva l'entry disponibile
				entry = *current;
			}
			else
			{
				// Se l'entry è stata trovata cerca di rimuovere ulteriori entries per mantenere la dimensione predefinita
				m_entries.erase(current);
			}

			if(m_optimalSize == boost::none || m_optimalSize.get() >= static_cast<uint32>(m_entries.size()))
			{
				OS_ASSERT(entry != nullptr);
				break;
			}
		}
	}

	return entry;
}

////////////////////////////////////////////////////////////////////////////

template <typename T, typename C = typename IGrowableCache<T>::Entries>
class GrowableCache : public IGrowableCache<T, C>
{
	typedef IGrowableCache<T> CacheBase;

// Construction
public:
	GrowableCache(boost::optional<uint32> optimalSize = boost::none);
	virtual ~GrowableCache();

// IGrowableCache interface
protected:
	virtual shared_ptr<T> createEntry();
};

//////////////////////////////////////////////////////////////////////

template <typename T, typename C>
GrowableCache<T, C>::GrowableCache(boost::optional<uint32> optimalSize) : CacheBase(optimalSize)
{

}

template <typename T, typename C>
GrowableCache<T, C>::~GrowableCache()
{

}

template <typename T, typename C>
shared_ptr<T> GrowableCache<T, C>::createEntry()
{
	return shared_ptr<T>(OS_NEW_T(T), os_delete_t());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_GROWABLECACHE_H
