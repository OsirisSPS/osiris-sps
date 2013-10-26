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

#ifndef _OS_CORE_MULTILOCK_H
#define _OS_CORE_MULTILOCK_H

#include "boost/preprocessor/repetition.hpp"
#include "pimpl_ptr.h"

//////////////////////////////////////////////////////////////////////

#define OS_MULTILOCK_ARITY	5

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport MultiLock : public boost::noncopyable
{
// Construction
public:
	#define OS_MULTILOCK_CTOR_DECL(z, n, _) MultiLock(BOOST_PP_ENUM_PARAMS(BOOST_PP_INC(n), boost::recursive_mutex &m), bool initially_locked = true);
	BOOST_PP_REPEAT(OS_MULTILOCK_ARITY, OS_MULTILOCK_CTOR_DECL, _)
	#undef OS_MULTILOCK_CTOR_DECL
	~MultiLock();

// Attributes
public:
	bool owns_lock() const;

// Operations
public:
	void lock();
	void unlock();

private:
	pimpl_of<MultiLock>::type m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/stringize.hpp"

//////////////////////////////////////////////////////////////////////

#define OS_MULTILOCK	BOOST_PP_CAT(integra::MultiLock __multilock, OS_CURRENT_LINE)

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_MULTILOCK_H
