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

#ifndef _OS_CORE_COLLECTIONS_H
#define _OS_CORE_COLLECTIONS_H

#include "algorithms.h"
#include "boost/checked_delete.hpp"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename K, typename T>
struct Collections
{
	typedef Collections<K, T> _Collections;

	typedef K key;
	typedef const key & key_ref;
	typedef T type;
	typedef type * pointer_type;

	class handler
	{
	public:
		handler() {}
		virtual ~handler() {}

	// Interface
	public:
		virtual void onInsert(key_ref k, pointer_type v) = 0;
		virtual void onRemove(key_ref k, pointer_type v) = 0;
		virtual void onClear() = 0;
	};
	typedef handler * handler_ptr;

	struct policies
	{
		class accessor : public handler
		{
			friend class simple;
			friend class smart;
			friend class ref;
			friend class shared;
		};
		inline static accessor * accessTo(handler *h) { return static_cast<accessor *>(h); }

		class basic_policy
		{
		public:
			typedef type * type_ptr;

			typedef typename unordered_map<key, type_ptr>::type collection_map;
			typedef typename collection_map::iterator map_iterator;
			typedef typename collection_map::const_iterator map_const_iterator;

			typedef typename deque<map_iterator>::type collection_array;
			typedef typename collection_array::iterator array_iterator;
			typedef typename collection_array::const_iterator array_const_iterator;

			typedef std::pair<map_iterator, bool> collection_pair;

			typedef typename collection_array::size_type size_type;
			typedef typename collection_map::value_type value_type;
		};

		class simple : public basic_policy
		{
        public:
		    typedef typename basic_policy::type_ptr type_ptr;

			typedef typename basic_policy::collection_map collection_map;
			typedef typename basic_policy::map_iterator map_iterator;
			typedef typename basic_policy::map_const_iterator map_const_iterator;

			typedef typename basic_policy::collection_array collection_array;
			typedef typename basic_policy::array_iterator array_iterator;
			typedef typename basic_policy::array_const_iterator array_const_iterator;

			typedef typename basic_policy::collection_pair collection_pair;

			typedef typename basic_policy::size_type size_type;
			typedef typename basic_policy::value_type value_type;

		public:
			inline static void insert(key_ref k, type_ptr v, collection_map &map, collection_array &array, handler *h, bool back)
			{
                collection_pair result = map.insert(value_type(k, v));
				if(result.second)
				{
					if(back)
						array.push_back(result.first);
					else
						array.push_front(result.first);

					if(h != null)
						accessTo(h)->onInsert(result.first->first, result.first->second);
				}
			}

			inline static void add(collection_map &map, collection_array &array, const collection_array &sarray, handler *h)
			{
				for(array_const_iterator i = sarray.begin(); i != sarray.end(); ++i)
				{
	                collection_pair result = map.insert(value_type((*i)->first, (*i)->second));
					if(result.second)
					{
						array.push_back(result.first);
						if(h != null)
							accessTo(h)->onInsert(result.first->first, result.first->second);
					}
				}
			}

			inline static bool remove(key_ref k, collection_map &map, collection_array &array, handler *h)
			{
				map_iterator i = map.find(k);
				if(i == map.end())
					return false;

				if(h != null)
					accessTo(h)->onRemove(i->first, i->second);

				utils::remove(array, i);
				map.erase(i);

				return true;
			}

			inline static void clear(collection_map &map, collection_array &array, handler *h)
			{
				if(h != null)
					accessTo(h)->onClear();

				array.clear();
				map.clear();
			}
		};

		class smart : public basic_policy
		{
        public:
		    typedef typename basic_policy::type_ptr type_ptr;

			typedef typename basic_policy::collection_map collection_map;
			typedef typename basic_policy::map_iterator map_iterator;
			typedef typename basic_policy::map_const_iterator map_const_iterator;

			typedef typename basic_policy::collection_array collection_array;
			typedef typename basic_policy::array_iterator array_iterator;
			typedef typename basic_policy::array_const_iterator array_const_iterator;

			typedef typename basic_policy::collection_pair collection_pair;

			typedef typename basic_policy::size_type size_type;
			typedef typename basic_policy::value_type value_type;

		public:
			inline static void insert(key_ref k, type_ptr v, collection_map &map, collection_array &array, handler *h, bool back)
			{
                collection_pair result = map.insert(value_type(k, v));
				if(result.second)
				{
					if(back)
						array.push_back(result.first);
					else
						array.push_front(result.first);

					if(h != null)
						accessTo(h)->onInsert(result.first->first, result.first->second);
				}
			}

			inline static void add(collection_map &map, collection_array &array, const collection_array &sarray, handler *h)
			{
				for(array_const_iterator i = sarray.begin(); i != sarray.end(); ++i)
				{
					collection_pair result = map.insert(value_type((*i)->first, new type(*((*i)->second))));
					if(result.second)
					{
						array.push_back(result.first);
						if(h != null)
							accessTo(h)->onInsert(result.first->first, result.first->second);
					}
				}
			}

			inline static bool remove(key_ref k, collection_map &map, collection_array &array, handler *h)
			{
				map_iterator i = map.find(k);
				if(i == map.end())
					return false;

				if(h != null)
					accessTo(h)->onRemove(i->first, i->second);

				boost::checked_delete(i->second);
				utils::remove(array, i);
				map.erase(i);

				return true;
			}

			inline static void clear(collection_map &map, collection_array &array, handler *h)
			{
				if(h != null)
					accessTo(h)->onClear();

				array.clear();

				while(map.empty() == false)
				{
					boost::checked_delete(map.begin()->second);
					map.erase(map.begin());
				}
			}
		};

		class shared
		{
		public:
			typedef shared_ptr<type> type_ptr;

			typedef typename unordered_map<key, type_ptr>::type collection_map;
			typedef typename collection_map::iterator map_iterator;
			typedef typename collection_map::const_iterator map_const_iterator;

			typedef typename deque<map_iterator>::type collection_array;
			typedef typename collection_array::iterator array_iterator;
			typedef typename collection_array::const_iterator array_const_iterator;

			typedef std::pair<map_iterator, bool> collection_pair;

			typedef typename collection_array::size_type size_type;
			typedef typename collection_map::value_type value_type;

		public:
			inline static void insert(key_ref k, type_ptr v, collection_map &map, collection_array &array, handler *h, bool back)
			{
                collection_pair result = map.insert(value_type(k, v));
				if(result.second)
				{
					if(back)
						array.push_back(result.first);
					else
						array.push_front(result.first);

					if(h != null)
						accessTo(h)->onInsert(result.first->first, result.first->second.get());
				}
			}

			inline static void add(collection_map &map, collection_array &array, const collection_array &sarray, handler *h)
			{
				for(array_const_iterator i = sarray.begin(); i != sarray.end(); ++i)
				{
	                collection_pair result = map.insert(value_type((*i)->first, (*i)->second));
					if(result.second)
					{
						array.push_back(result.first);
						if(h != null)
							accessTo(h)->onInsert(result.first->first, result.first->second.get());
					}
				}
			}

			inline static bool remove(key_ref k, collection_map &map, collection_array &array, handler *h)
			{
				map_iterator i = map.find(k);
				if(i == map.end())
					return false;

				if(h != null)
					accessTo(h)->onRemove(i->first, i->second.get());

				utils::remove(array, i);
				map.erase(i);

				return true;
			}

			inline static void clear(collection_map &map, collection_array &array, handler *h)
			{
				if(h != null)
					accessTo(h)->onClear();

				array.clear();
				map.clear();
			}
		};
	};

	template <typename P>
	class basic_collection
	{
	public:
		typedef P Policy;

		typedef typename P::type_ptr type_ptr;

		typedef typename _Collections::key key;
		typedef typename _Collections::key_ref key_ref;
		typedef typename _Collections::type type;
		typedef typename _Collections::pointer_type pointer_type;

		typedef typename Policy::collection_map collection_map;
		typedef typename Policy::map_iterator map_iterator;
		typedef typename Policy::map_const_iterator map_const_iterator;

		typedef typename Policy::collection_array collection_array;
		typedef typename Policy::array_iterator array_iterator;
		typedef typename Policy::array_const_iterator array_const_iterator;

		typedef typename Policy::size_type size_type;
		typedef typename Policy::value_type value_type;

		typedef basic_collection<P> self;

		template <typename AI, typename MI>
		class collection_iterator
		{
			typedef AI array_iterator_type;
			typedef MI map_iterator_type;

			typedef typename array_iterator_type::pointer array_pointer;
			typedef typename array_iterator_type::reference array_reference;

			typedef typename map_iterator_type::pointer map_pointer;
			typedef typename map_iterator_type::value_type map_value_type;
			typedef typename map_iterator_type::reference map_reference;

		public:
			collection_iterator(const array_iterator_type &i) : m_iterator(i) { }
			~collection_iterator() { }

		public:
			inline collection_iterator & operator++()	// preincrement
			{
				m_iterator++;
				return *this;
			}

			inline collection_iterator operator++(int)	// postincrement
			{
				collection_iterator tmp = *this;
				++(*this);
				return tmp;
			}

			inline collection_iterator & operator--()	// predecrement
			{
				m_iterator--;
				return *this;
			}

			inline collection_iterator operator--(int)	// postdecrement
			{
				collection_iterator tmp = *this;
				--(*this);
				return tmp;
			}

			map_value_type * operator->() const
			{
				return (*m_iterator).operator->();
			}

			map_reference operator*() const
			{
				return *(*m_iterator);
			}

			inline collection_iterator & operator=(const collection_iterator &right)
			{
				m_iterator = right.m_iterator;
				return *this;
			}

			inline bool operator==(const collection_iterator &right) const
			{
				return m_iterator == right.m_iterator;
			}

			inline bool operator!=(const collection_iterator &right) const
			{
				return m_iterator != right.m_iterator;
			}

		protected:
			array_iterator_type m_iterator;
		};

		typedef collection_iterator<array_const_iterator, map_const_iterator> iterator;

	// Construction
	public:
		basic_collection(handler_ptr handler = null) : m_handler(handler)
		{

		}

		basic_collection(const self &collection) : m_handler(null)
		{
			*this = collection;
		}

		virtual ~basic_collection()
		{
			clear();
		}

	// Attributes
	public:
		inline iterator begin() const { return iterator(m_array.begin()); }
		inline iterator end() const { return iterator(m_array.end()); }

		inline size_type size() const { return m_array.size(); }
		inline bool empty() const { return m_array.empty(); }

		inline type_ptr front() const { return get(begin()->first); }
		inline type_ptr back() const { return get((--end())->first); }

		inline iterator at(size_type index) const { OS_ASSERT(index < m_array.size()); return iterator(m_array.begin() + index); }

		inline key_ref key_at(size_type index) const { return m_array.at(index)->first; }
		inline type_ptr value_at(size_type index) const { return m_array.at(index)->second; }

		inline bool exists(key_ref k) const
		{
			return m_map.find(k) != m_map.end();
		}

		inline type_ptr get(key_ref k) const
		{
			typename collection_map::const_iterator mi = m_map.find(k);
			if(mi != m_map.end())
				return mi->second;

			return null;
		}

	// Operations
	public:
		bool push_front(key_ref k, type_ptr v)
		{
			return _insert(k, v, false);
		}

		bool push_back(key_ref k, type_ptr v)
		{
			return _insert(k, v, true);
		}

		bool remove(key k)
		{
			return Policy::remove(k, m_map, m_array, m_handler);
		}

		size_type remove(type_ptr v)
		{
			size_type count = 0;
			for(map_iterator i = m_map.begin(); i != m_map.end();)
			{
				map_iterator y = i;
				++i;
				if(y->second == v && remove(y->first))
					count++;
			}
			return count;
		}

		void clear()
		{
			Policy::clear(m_map, m_array, m_handler);
		}

		void pop_front()
		{
			if(empty() == false)
				remove(begin()->first);
		}

		void pop_back()
		{
			if(empty() == false)
				remove((--end())->first);
		}

		void add(const self &collection)
		{
			Policy::add(m_map, m_array, collection.m_array, m_handler);
		}

	private:
		bool _insert(key_ref k, type_ptr v, bool back)
		{
			OS_ASSERT(exists(k) == false);
			if(exists(k) == false)
			{
				Policy::insert(k, v, m_map, m_array, m_handler, back);
				return true;
			}

			return false;
		}

	// Operators
	public:
		self & operator =(const self &collection)
		{
			clear();
			add(collection);

			return *this;
		}

		type_ptr operator[](key_ref k) const
		{
			return get(k);
		}

	protected:
		collection_map m_map;
		collection_array m_array;
		handler_ptr m_handler;
	};

	typedef typename policies::simple simple_policy;
	typedef basic_collection<simple_policy> simple_collection;

	typedef typename policies::smart smart_policy;
	typedef basic_collection<smart_policy> smart_collection;

	typedef typename policies::shared shared_policy;
	typedef basic_collection<shared_policy> shared_collection;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
struct StringCollectionTraits
{

};

template <>
struct CoreExport StringCollectionTraits<std::string>
{
	static const std::string empty;
};

template <>
struct CoreExport StringCollectionTraits<std::wstring>
{
	static const std::wstring empty;
};

template <>
struct CoreExport StringCollectionTraits<String>
{
	static const String empty;
};

//////////////////////////////////////////////////////////////////////

template <typename S, typename T = StringCollectionTraits<S> >
class StringCollection : public Collections<S, S>::smart_collection
{
public:
    typedef T Traits;
	typedef typename Collections<S, S>::smart_collection CollectionBase;
	typedef typename CollectionBase::iterator iterator;

public:
	StringCollection()
	{

	}

	StringCollection(const StringCollection<S, T> &second) : CollectionBase(second)
	{

	}

	virtual ~StringCollection()
	{

	}

// Attributes
public:
	const S & value_at(typename CollectionBase::size_type index) const
	{
		S *value = CollectionBase::value_at(index);
		if(value != null)
			return *value;

		return Traits::empty;
	}

	const S & get(const S &key) const
	{
		S *value = CollectionBase::get(key);
		if(value != null)
			return *value;

		return Traits::empty;
	}

	void set(const S &key, const S &value)
	{
		S *current = CollectionBase::get(key);
		if(current != null)
			*current = value;
		else
			CollectionBase::push_back(key, new S(value));
	}

// Operations
public:
	void encode(S &str, const S &keys_separator, const S &values_separator) const
	{
		str.clear();
		for(iterator i = CollectionBase::begin(); i != CollectionBase::end(); ++i)
		{
			if(i != CollectionBase::begin())
				str += keys_separator;

			str += i->first + values_separator + *i->second;
		}
	}

	void decode(const S &str, const S &keys_separator, const S &values_separator, bool trim = true)
	{
		CollectionBase::clear();

		typename vector<S>::type keys;
		algorithms::split(str, keys, boost::algorithm::is_any_of(keys_separator.c_str()), true);

		for(typename vector<S>::type::iterator i = keys.begin(); i != keys.end(); ++i)
		{
			S &str = *i;

			// Cerca la posizione dell'operatore di assegnamento
			typename S::size_type pos = str.find(values_separator);
			// Verifica che la posizione sia valida
			if(pos != S::npos)
			{
				S name = algorithms::left(str, pos);
				S value = algorithms::mid(str, pos + values_separator.length());

				if(trim)
				{
					algorithms::trim(name);
					algorithms::trim(value);
				}

				// Salva il parametro corrente
				set(name, value);
			}
		}
	}
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_COLLECTIONS_H
