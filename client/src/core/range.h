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

#ifndef _OS_CORE_RANGE_H
#define _OS_CORE_RANGE_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T, typename S = uint32>
class Range
{
	typedef T value_type;
	typedef S size_type;	

	typedef Range<value_type, size_type> range_type;

// Construction
public:
	Range(value_type start = 0, size_type size = 0);
	Range(const range_type &second);
	virtual ~Range();

// Attributes
public:
	inline value_type start() const;
	inline value_type end() const;
	inline size_type size() const;

	inline bool empty() const;

	inline void setStart(const value_type &start);
	inline void setSize(const size_type &size);

// Operators
public:
	bool operator ==(const range_type &second) const;
	bool operator !=(const range_type &second) const;

	range_type & operator =(const range_type &second);

public:
	static const range_type EMPTY;

protected:
	value_type m_start;
	size_type m_size;
};

//////////////////////////////////////////////////////////////////////

typedef Range <int32> RangeInt32;
typedef Range <uint32> RangeUint32;
typedef Range <double, double> RangeDouble;

//////////////////////////////////////////////////////////////////////

template <typename T, typename S>
const typename Range<T, S>::range_type Range<T, S>::EMPTY(0, 0);

//////////////////////////////////////////////////////////////////////

template <typename T, typename S>
inline typename Range<T, S>::value_type Range<T, S>::start() const { return m_start; }

template <typename T, typename S>
inline typename Range<T, S>::value_type Range<T, S>::end() const { return m_start + m_size; }

template <typename T, typename S>
inline typename Range<T, S>::size_type Range<T, S>::size() const { return m_size; }

template <typename T, typename S>
inline bool Range<T, S>::empty() const { return m_size == 0; }

template <typename T, typename S>
inline void Range<T, S>::setStart(const value_type &start) { m_start = start; }

template <typename T, typename S>
inline void Range<T, S>::setSize(const size_type &size) { m_size = size; }	

//////////////////////////////////////////////////////////////////////

template <typename T, typename S>
Range<T, S>::Range(value_type start, size_type size)
{
	m_start = start;
	m_size = size;
}

template <typename T, typename S>
Range<T, S>::Range(const range_type &second)
{
	*this = second;
}

template <typename T, typename S>
Range<T, S>::~Range()
{

}

template <typename T, typename S>
bool Range<T, S>::operator ==(const range_type &second) const
{
	return (m_start == second.m_start) && (m_size == second.m_size);
}

template <typename T, typename S>
bool Range<T, S>::operator !=(const range_type &second) const
{
	return (*this == second) == false;
}

template <typename T, typename S>
typename Range<T, S>::range_type & Range<T, S>::operator =(const range_type &second)
{
	m_start = second.m_start;
	m_size = second.m_size;

	return *this;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_RANGE_H
