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

#ifndef _OS_CORE_NULLPTR_SUPPORT_H
#define _OS_CORE_NULLPTR_SUPPORT_H

//////////////////////////////////////////////////////////////////////

using boost::shared_ptr;
using boost::shared_array;
using boost::weak_ptr;

//////////////////////////////////////////////////////////////////////

class CoreExport null_ptr
{
public:
	null_ptr();
	~null_ptr();

	template <typename T>
	OS_FORCEINLINE operator shared_ptr<T>() const { return shared_ptr<T>(); }

	template <typename T>
	OS_FORCEINLINE operator shared_array<T>() const { return shared_array<T>(); }

	template <typename T>
	OS_FORCEINLINE operator T *() const { return 0; }
};

//////////////////////////////////////////////////////////////////////

extern CoreExport const null_ptr nullptr;

//////////////////////////////////////////////////////////////////////

template <typename T>
OS_FORCEINLINE bool operator==(const null_ptr &first, const T &second) { return second == 0; }

template <typename T>
OS_FORCEINLINE bool operator==(const T &first, const null_ptr &second) { return first == 0; }

template <typename T>
OS_FORCEINLINE bool operator!=(const null_ptr &first, const T &second) { return second != 0; }

template <typename T>
OS_FORCEINLINE bool operator!=(const T &first, const null_ptr &second) { return first != 0; }
/*
template <typename T>
OS_FORCEINLINE bool operator==(const null_ptr &first, const shared_ptr<T> &second) { return second == 0; }

template <typename T>
OS_FORCEINLINE bool operator==(const shared_ptr<T> &first, const null_ptr &second) { return first == 0; }

template <typename T>
OS_FORCEINLINE bool operator!=(const null_ptr &first, const shared_ptr<T> &second) { return second != 0; }

template <typename T>
OS_FORCEINLINE bool operator!=(const shared_ptr<T> &first, const null_ptr &second) { return first != 0; }

template <typename T>
OS_FORCEINLINE bool operator==(const null_ptr &first, const shared_array<T> &second) { return second == 0; }

template <typename T>
OS_FORCEINLINE bool operator==(const shared_array<T> &first, const null_ptr &second) { return first == 0; }

template <typename T>
OS_FORCEINLINE bool operator!=(const null_ptr &first, const shared_array<T> &second) { return second != 0; }

template <typename T>
OS_FORCEINLINE bool operator!=(const shared_array<T> &first, const null_ptr &second) { return first != 0; }

template <typename T, typename D>
OS_FORCEINLINE bool operator==(const null_ptr &first, const scoped_ptr<T, D> &second) { return second == 0; }

template <typename T, typename D>
OS_FORCEINLINE bool operator==(const scoped_ptr<T, D> &first, const null_ptr &second) { return first == 0; }

template <typename T, typename D>
OS_FORCEINLINE bool operator!=(const null_ptr &first, const scoped_ptr<T, D> &second) { return second != 0; }

template <typename T, typename D>
OS_FORCEINLINE bool operator!=(const scoped_ptr<T, D> &first, const null_ptr &second) { return first != 0; }

template <typename T, typename D>
OS_FORCEINLINE bool operator==(const null_ptr &first, const scoped_array<T, D> &second) { return second == 0; }

template <typename T, typename D>
OS_FORCEINLINE bool operator==(const scoped_array<T, D> &first, const null_ptr &second) { return first == 0; }

template <typename T, typename D>
OS_FORCEINLINE bool operator!=(const null_ptr &first, const scoped_array<T, D> &second) { return second != 0; }

template <typename T, typename D>
OS_FORCEINLINE bool operator!=(const scoped_array<T, D> &first, const null_ptr &second) { return first != 0; }
*/
//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_NULLPTR_SUPPORT_H
