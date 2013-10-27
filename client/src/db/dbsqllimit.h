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

#ifndef _DB_SQL_LIMIT_H
#define _DB_SQL_LIMIT_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DatabaseExport DbSqlLimit
{
// Construction
public:
	DbSqlLimit(const uint32 &offset = 0, const uint32 &count = 0);
	virtual ~DbSqlLimit();

// Attributes
public:
	inline bool exists() const;
	inline bool hasOffset() const;

	inline const uint32 & getCount() const;
	inline void setCount(const uint32 &count);

	inline const uint32 & getOffset() const;
	inline void setOffset(const uint32 &offset);

	inline void setRange(const uint32 &offset, const uint32 &count);

// Operations
public:
	void clear();

protected:
	uint32 m_count;
	uint32 m_offset;
};

//////////////////////////////////////////////////////////////////////

inline bool DbSqlLimit::exists() const { return m_count != 0; }
inline bool DbSqlLimit::hasOffset() const { return m_offset != 0; }

inline const uint32 & DbSqlLimit::getCount() const { return m_count; }
inline void DbSqlLimit::setCount(const uint32 &count) { m_count = count; }

inline const uint32 & DbSqlLimit::getOffset() const { return m_offset; }
inline void DbSqlLimit::setOffset(const uint32 &offset) { m_offset = offset; }

inline void DbSqlLimit::setRange(const uint32 &offset, const uint32 &count) { m_offset = offset; m_count = count; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_SQL_LIMIT_H
