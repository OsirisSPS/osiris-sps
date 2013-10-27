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

#ifndef _OS_CORE_UNORDEREDWRAPPERS_H
#define _OS_CORE_UNORDEREDWRAPPERS_H

#include "noninstantiable.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T, typename H = boost::hash<T>, typename P = std::equal_to<T>, typename A = StdAllocator<T> >
struct unordered_set : public noninstantiable
{
	typedef boost::unordered_set<T, H, P, A> type;
};

template <typename T, typename H = boost::hash<T>, typename P = std::equal_to<T>, typename A = StdAllocator<T> >
struct unordered_multiset : public noninstantiable
{
	typedef boost::unordered_multiset<T, H, P, A> type;
};

template <typename K, typename V, typename H = boost::hash<K>, typename P = std::equal_to<K>, typename A = StdAllocator<std::pair<const K, V> > >
struct unordered_map : public noninstantiable
{	
	typedef boost::unordered_map<K, V, H, P, A> type;
};

template <typename K, typename V, typename H = boost::hash<K>, typename P = std::equal_to<K>, typename A = StdAllocator<std::pair<const K, V> > >
struct unordered_multimap : public noninstantiable
{	
	typedef boost::unordered_multimap<K, V, H, P, A> type;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_UNORDEREDWRAPPERS_H
