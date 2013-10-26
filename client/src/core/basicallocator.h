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

#ifndef _OS_CORE_BASICALLOCATOR_H
#define _OS_CORE_BASICALLOCATOR_H

#include <limits>

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
struct AllocatorTraits
{
	typedef T value_type;
	typedef T & reference;
	typedef const T & const_reference;
	typedef T * pointer;
	typedef const T * const_pointer;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
class BasicAllocator
{
public:
	typedef AllocatorTraits<T> Traits;
	typedef typename Traits::value_type value_type;
	typedef typename Traits::reference reference;
	typedef typename Traits::const_reference const_reference;
	typedef typename Traits::pointer pointer;
	typedef typename Traits::const_pointer const_pointer;
	typedef typename Traits::size_type size_type;
	typedef typename Traits::difference_type difference_type;

// Construction
public:
	BasicAllocator();
	BasicAllocator(const BasicAllocator<T> &second);
	template <typename U>
	BasicAllocator(const BasicAllocator<U> &second);
	virtual ~BasicAllocator();

// Attributes
public:
	inline static pointer address(reference value);
	inline static const_pointer address(const_reference value);

	inline static size_type max_size();	
	
// Operators
public:
	inline bool operator ==(const BasicAllocator &second) const;
	inline bool operator !=(const BasicAllocator &second) const;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
BasicAllocator<T>::BasicAllocator()
{

}

template <typename T>
BasicAllocator<T>::BasicAllocator(const BasicAllocator<T> &second)
{

}

template <typename T>
template <typename U>
BasicAllocator<T>::BasicAllocator(const BasicAllocator<U> &second)
{

}

template <typename T>
BasicAllocator<T>::~BasicAllocator()
{

}

template <typename T>
inline typename BasicAllocator<T>::pointer BasicAllocator<T>::address(reference value)
{
	return &value;
}

template <typename T>
inline typename BasicAllocator<T>::const_pointer BasicAllocator<T>::address(const_reference value)
{
	return &value;
}

template <typename T>
inline typename BasicAllocator<T>::size_type BasicAllocator<T>::max_size()
{
	return std::numeric_limits<size_type>::max() / sizeof(T);
}

template <typename T>
inline bool BasicAllocator<T>::operator ==(const BasicAllocator &second) const
{
	// Tutte le funzioni a livello di allocatori sono statiche per cui due istanze di allocatori sono sempre uguali (N.B.: alcune classi effettuano il controllo di ugualianza sugli allocatori, pertanto l'operatore di confronto è necessario)
	return true;
}

template <typename T>
inline bool BasicAllocator<T>::operator !=(const BasicAllocator &second) const
{
	// Vedi note operatore ==
	return false;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_BASICALLOCATOR_H
