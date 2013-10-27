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

#ifndef _OS_CORE_STDWRAPPERS_H
#define _OS_CORE_STDWRAPPERS_H

#include "noninstantiable.h"
#include "stdallocator.h"

//////////////////////////////////////////////////////////////////////

namespace std {

//////////////////////////////////////////////////////////////////////

typedef basic_string<OS_NAMESPACE_NAME::uchar> ustring;

//////////////////////////////////////////////////////////////////////

} // std

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

// basic_string wrappers

template <typename T, typename C = std::char_traits<T>, typename A = StdAllocator<T> >
struct basic_string
{
	typedef std::basic_string<T, C, A> type;
};

typedef basic_string<char>::type astring;
typedef basic_string<wchar_t>::type wstring;
typedef basic_string<uchar>::type ustring;

typedef astring string;

//////////////////////////////////////////////////////////////////////

// collections wrappers

template <typename T, typename A = StdAllocator<T> >
struct deque : public noninstantiable
{
	typedef std::deque<T, A> type;	
};

template <typename T, typename A = StdAllocator<T> >
struct vector : public noninstantiable
{
	typedef std::vector<T, A> type;	
};

template <typename T, typename A = StdAllocator<T> >
struct list : public noninstantiable
{
	typedef std::list<T, A> type;	
};

template <typename T, typename P = std::less<T>, typename A = StdAllocator<T> >
struct set : public noninstantiable
{
	typedef std::set<T, P, A> type;	
};

template <typename T, typename P = std::less<T>, typename A = StdAllocator<T> >
struct multiset : public noninstantiable
{
	typedef std::multiset<T, P, A> type;	
};

template <typename K, typename V, typename P = std::less<K>, typename A = StdAllocator<std::pair<const K, V> > >
struct map : public noninstantiable
{
	typedef std::map<K, V, P, A> type;
};

template <typename K, typename V, typename P = std::less<K>, typename A = StdAllocator<std::pair<const K, V> > >
struct multimap : public noninstantiable
{
	typedef std::multimap<K, V, P, A> type;
};

template <typename T, typename C = typename deque<T>::type>
struct stack : public noninstantiable
{
	typedef std::stack<T, C> type;
};

template <typename T, typename C = typename deque<T>::type>
struct queue : public noninstantiable
{
	typedef std::queue<T, C> type;
};

template <typename T, typename C = typename vector<T>::type, typename P = std::less<typename C::value_type> >
struct priority_queue : public noninstantiable
{
	typedef std::priority_queue<T, C> type;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_STDWRAPPERS_H
