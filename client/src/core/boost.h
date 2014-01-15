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

#ifndef _OS_CORE_BOOST_H
#define _OS_CORE_BOOST_H

// N.B.: enable_this_ptr_support.h va incluso prima di shared_ptr.hpp altrimenti l'aggancio verrebbe oscurato dalla funzione sp_enable_shared_from_this(...) definita in quest'ultimo file
#include "enable_this_ptr_support.h"

//////////////////////////////////////////////////////////////////////

#define BOOST_SIGNALS_NO_DEPRECATION_WARNING

//////////////////////////////////////////////////////////////////////

#if OS_COMPILER	== OS_COMPILER_MSVC
	#pragma warning(disable : 4267)
#endif

//////////////////////////////////////////////////////////////////////

#include "boost/assert.hpp"
#include "boost/optional/optional_fwd.hpp"
#include "boost/regex/v4/regex_fwd.hpp"
#include "boost/shared_array.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/static_assert.hpp"
#include "boost/unordered/unordered_map.hpp"
#include "boost/unordered/unordered_set.hpp"
#include "boost/weak_ptr.hpp"

//////////////////////////////////////////////////////////////////////

#if OS_COMPILER	== OS_COMPILER_MSVC
	#pragma warning(default : 4267)
#endif

//////////////////////////////////////////////////////////////////////

namespace boost {

//////////////////////////////////////////////////////////////////////

class any;
class mutex;
class recursive_mutex;
class thread;

struct xtime;

//////////////////////////////////////////////////////////////////////

namespace posix_time
{
	class time_duration;
	class ptime;
}

//////////////////////////////////////////////////////////////////////

} // boost

//////////////////////////////////////////////////////////////////////

// boost::scoped_ptr e boost::scoped_array non supportano "deleters" custom

#include "scoped_ptr.h"
#include "scoped_array.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

using boost::shared_ptr;
using boost::shared_array;
using boost::weak_ptr;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#ifndef OS_SUPPORT_CPP0X
	#include "nullptr_support.h"
#endif

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_BOOST_H
