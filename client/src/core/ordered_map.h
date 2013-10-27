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

#ifndef _OS_CORE_ORDERED_MAP_H
#define _OS_CORE_ORDERED_MAP_H

#include "boost/operators.hpp"
#include "sealed.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename K, typename V>
class ordered_map : public boost::equality_comparable<ordered_map<K, V> >,
				    Sealed<ordered_map<K, V> >
{
// Internal typedefs
public:
	typedef ordered_map<K, V> type;

	typedef K key_type;
	typedef V mapped_type;

	typedef std::pair<const key_type, mapped_type> value_type;	

	typedef typename unordered_map<key_type, mapped_type>::type container_map;
	typedef typename container_map::iterator map_iterator;
	typedef typename container_map::const_iterator map_const_iterator;

	typedef typename list<map_iterator>::type container_list;		// N.B.: non usare vector/deque per mantenere immutati gli iteratori nella mappa di associazione
	typedef typename container_list::iterator list_iterator;
	typedef typename container_list::const_iterator list_const_iterator;

	typedef typename unordered_map<const key_type *, list_iterator>::type container_assoc;

	typedef typename container_map::size_type size_type;
	
	template <typename LI, typename MI>
	class container_iterator
	{
	public:
		typedef LI list_iterator_type;
		typedef MI map_iterator_type;

		typedef typename map_iterator_type::iterator_category iterator_category;
		typedef typename map_iterator_type::value_type value_type;
		typedef typename map_iterator_type::difference_type difference_type;	
		typedef typename map_iterator_type::pointer pointer;
		typedef typename map_iterator_type::reference reference;

	public:
		container_iterator()
		{

		}

		container_iterator(const list_iterator_type &i) : m_iterator(i)
		{

		}

		// Supporto per la conversione da "iterator" a "const_iterator"
		template <typename U>
		container_iterator(const U &v) : m_iterator(static_cast<const list_iterator_type &>(v.iterator()))
		{

		}

		container_iterator(const std::pair<container_iterator, bool> &v) : m_iterator(v.first.iterator())
		{

		}

		container_iterator(const container_iterator &second) : m_iterator(second.m_iterator)
		{

		}

		~container_iterator()
		{

		}

	public:
		inline const list_iterator_type & iterator() const
		{
			return m_iterator;
		}

		inline container_iterator & operator++()		// preincrement
		{
			m_iterator++;
			return *this;
		}

		inline container_iterator operator++(int)		// postincrement
		{
			container_iterator tmp = *this;
			++(*this);
			return tmp;
		}

		inline container_iterator & operator--()		// predecrement
		{
			m_iterator--;
			return *this;
		}

		inline container_iterator operator--(int)		// postdecrement
		{
			container_iterator tmp = *this;
			--(*this);
			return tmp;
		}

		inline value_type * operator->() const
		{
			return (*m_iterator).operator->();
		}

		inline reference operator*() const
		{
			return *(*m_iterator);
		}

		inline container_iterator & operator=(const container_iterator &second)
		{
			m_iterator = second.m_iterator;
			return *this;
		}

		inline bool operator==(const container_iterator &second) const
		{
			return m_iterator == second.m_iterator;
		}

		inline bool operator!=(const container_iterator &second) const
		{
			return m_iterator != second.m_iterator;
		}

	protected:
		list_iterator_type m_iterator;
	};

	typedef container_iterator<list_iterator, map_iterator> iterator;
	typedef container_iterator<list_const_iterator, map_const_iterator> const_iterator;

// Construction
public:
	ordered_map()
	{

	}

	ordered_map(const type &second)
	{
		*this = second;
	}

	~ordered_map()
	{
		clear();
	}

// Attributes
public:
	inline iterator begin()
	{
		return iterator(m_list.begin());
	}

	inline const_iterator begin() const
	{
		return const_iterator(m_list.begin());
	}

	inline iterator end()
	{
		return iterator(m_list.end());
	}

	inline const_iterator end() const
	{
		return const_iterator(m_list.end());
	}

	inline size_type size() const
	{
		return m_list.size();
	}

	inline bool empty() const
	{
		return m_list.empty();
	}

	inline mapped_type & get(const key_type &k)
	{
		map_iterator i = m_map.find(k);
		if(i != m_map.end())
			return i->second;

		return push_back(k, mapped_type()).first->second;
	}

	inline bool set(const key_type &k, const mapped_type &v)
	{
		map_iterator i = m_map.find(k);
		if(i == m_map.end())
			return push_back(k, v).second;

		i->second = v;
		return true;
	}

	inline bool exists(const key_type &k) const
	{
		return m_map.find(k) != m_map.end();
	}

// Operations
public:
	inline std::pair<iterator, bool> push_front(const key_type &k, const mapped_type &v)
	{
		return insert(begin(), value_type(k, v));
	}

	inline std::pair<iterator, bool> push_back(const key_type &k, const mapped_type &v)
	{
		return insert(end(), value_type(k, v));
	}

	inline std::pair<iterator, bool> insert(const value_type &v)
	{
		return insert(end(), v);
	}

	std::pair<iterator, bool> insert(iterator pos, const value_type &v)
	{
		std::pair<map_iterator, bool> result = m_map.insert(v);
		if(result.second == false)
		{
			if(result.first != m_map.end())
			{
				typename container_assoc::iterator current = m_assoc.find(&result.first->first);
				OS_ASSERT(current != m_assoc.end());
				return std::make_pair(iterator(current->second), false);
			}

			OS_ASSERTFALSE();
			return std::make_pair(end(), false);
		}
	
		list_iterator p = m_list.insert(pos.iterator(), result.first);
		m_assoc[&result.first->first] = p;	
		return std::make_pair(iterator(p), true);
	}

	size_type count(const key_type &k) const
	{
		return m_map.count(k);
	}

	bool erase(const key_type &k)
	{
		map_iterator mi = m_map.find(k);
		if(mi == m_map.end())
			return false;

		typename container_assoc::iterator ai = m_assoc.find(&mi->first);
		OS_ASSERT(ai != m_assoc.end());

		m_list.erase(ai->second);
		m_assoc.erase(ai);
		m_map.erase(mi);

		return true;
	}

	bool erase(iterator i)
	{
		OS_ASSERT(i != end());
		return erase(i->first);
	}

	void erase(iterator first, iterator last)
	{	
		if(first == begin() && last == end())
		{	
			clear();			
		}
		else
		{
			while(first != last)
				erase(first++);			
		}
	}

	iterator find(const key_type &k)
	{
		map_iterator mi = m_map.find(k);
		if(mi == m_map.end())
			return iterator(m_list.end());

		typename container_assoc::iterator ai = m_assoc.find(&mi->first);
		OS_ASSERT(ai != m_assoc.end());
		return iterator(ai->second);
	}

	const_iterator find(const key_type &k) const
	{
		map_const_iterator mi = m_map.find(k);
		if(mi == m_map.end())
			return const_iterator(m_list.end());

		typename container_assoc::const_iterator ai = m_assoc.find(&mi->first);
		OS_ASSERT(ai != m_assoc.end());

		return static_cast<const_iterator>(iterator(ai->second));
	}

	void clear()
	{
		m_assoc.clear();
		m_list.clear();
		m_map.clear();
	}

	void pop_front()
	{
		OS_ASSERT(empty() == false);
		erase(begin()->first);
	}

	void pop_back()
	{
		OS_ASSERT(empty() == false);
		erase((--end())->first);
	}

	void append(const type &second)
	{
		for(const_iterator i = second.begin(); i != second.end(); ++i)
		{
			push_back(i->first, i->second);
		}
	}

// Operators
public:
	type & operator =(const type &container)
	{
		clear();
		append(container);

		return *this;
	}

	bool operator ==(const type &second) const
	{
		if(size() != second.size())
			return false;

		for(const_iterator i1 = begin(), i2 = second.begin(); i1 != end(); ++i1, ++i2)
		{
			if(i1->first != i2->first)
				return false;

			if(i1->second != i2->second)
				return false;
		}

		return true;
	}

	inline mapped_type & operator[](const key_type &k)
	{
		return get(k);
	}

protected:
	container_map m_map;
	container_list m_list;
	container_assoc m_assoc;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_ORDERED_MAP_H
