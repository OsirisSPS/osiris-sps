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

#ifndef _OS_CORE_ENABLE_THIS_PTR_SUPPORT_H
#define _OS_CORE_ENABLE_THIS_PTR_SUPPORT_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T> class enable_this_ptr;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

// Integrazione enable_this_ptr con boost::shared_ptr

namespace boost {

//////////////////////////////////////////////////////////////////////

template<class T> class shared_ptr;

//////////////////////////////////////////////////////////////////////

namespace detail {

//////////////////////////////////////////////////////////////////////

template <typename X, typename Y, typename T> 
inline void sp_enable_shared_from_this(boost::shared_ptr<X> *ppx, Y const *py, OS_NAMESPACE_NAME::enable_this_ptr<T> const *pe)
{
    if(pe != 0)
        pe->internal_accept_shared_owner(ppx, const_cast<Y *>(py));    
}

//////////////////////////////////////////////////////////////////////

} // detail
} // boost

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_ENABLE_THIS_PTR_SUPPORT_H
