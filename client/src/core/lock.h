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

#ifndef _OS_CORE_LOCK_H
#define _OS_CORE_LOCK_H

#include "boost/thread/locks.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
inline boost::shared_ptr<typename T::scoped_lock> create_scoped_lock(T &mutex)
{
	return boost::shared_ptr<typename T::scoped_lock>(OS_NEW_T(typename T::scoped_lock)(mutex), os_delete_t());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/stringize.hpp"

//////////////////////////////////////////////////////////////////////

#define OS_LOCK(m)	BOOST_PP_CAT(OS_NAMESPACE_NAME::shared_ptr<void> __lock, OS_CURRENT_LINE)(OS_NAMESPACE_NAME::create_scoped_lock(m))

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_LOCK_H
