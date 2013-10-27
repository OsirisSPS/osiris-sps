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

#ifndef _DB_SQL_CONDITION_H
#define _DB_SQL_CONDITION_H

#include "dbsqlfield.h"
#include "dbsqlicommand.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DatabaseExport DbSqlCondition
{
// Enumerated types
public:
	enum Flags
	{
		cfEqual		= (uint32) 1 << 0,
		cfDifferent = (uint32) 1 << 1,
		cfMinor		= (uint32) 1 << 2,
		cfMajor		= (uint32) 1 << 3,
		cfLike		= (uint32) 1 << 4,
		cfIn		= (uint32) 1 << 5,
		cfAnd		= (uint32) 1 << 6,		// In relazione alla condizione precedente
		cfOr		= (uint32) 1 << 7,		// In relazione alla condizione precedente
	};

// Construction
public:
	DbSqlCondition();
	DbSqlCondition(const DbSqlField &field, const String &value, uint32 flags = cfEqual | cfAnd);
	DbSqlCondition(const DbSqlField &field, shared_ptr<DbSqlICommand> command, uint32 flags = cfEqual | cfAnd);
	DbSqlCondition(const DbSqlField &field, const StringList &values, uint32 flags = cfIn | cfAnd);
	DbSqlCondition(const DbSqlCondition &second);
	virtual ~DbSqlCondition();

// Attributes
public:
	inline const DbSqlField & getField() const;
	inline void setField(const DbSqlField &field);

	inline const String & getValue() const;
	inline void setValue(const String &value);

	inline shared_ptr<DbSqlICommand> getCommand() const;
	inline void setCommand(shared_ptr<DbSqlICommand> command);
	inline bool hasCommand() const;

	inline uint32 getFlags() const;
	inline void setFlags(uint32 flags);

	inline bool hasEqual() const;
	inline bool hasDifferent() const;
	inline bool hasMinor() const;
	inline bool hasMajor() const;
	inline bool hasLike() const;
	inline bool hasIn() const;

	inline bool isIn() const;
	inline bool isAnd() const;
	inline bool isOr() const;

// Operators
public:
	DbSqlCondition & operator =(const DbSqlCondition &second);

	bool operator ==(const DbSqlCondition &second);
	bool operator !=(const DbSqlCondition &second);

protected:
	DbSqlField m_field;
	String m_conditionValue;
	shared_ptr<DbSqlICommand> m_conditionCommand;
	uint32 m_flags;
};

//////////////////////////////////////////////////////////////////////

typedef DbSqlCollection<DbSqlCondition> DbSqlConditions;

//////////////////////////////////////////////////////////////////////

inline const DbSqlField & DbSqlCondition::getField() const { return m_field; }
inline void DbSqlCondition::setField(const DbSqlField &field) { m_field = field; }

inline const String & DbSqlCondition::getValue() const { return m_conditionValue; }
inline void DbSqlCondition::setValue(const String &value) { m_conditionValue = value; m_conditionCommand.reset(); }

inline shared_ptr<DbSqlICommand> DbSqlCondition::getCommand() const { return m_conditionCommand; }
inline void DbSqlCondition::setCommand(shared_ptr<DbSqlICommand> command) { m_conditionCommand = command; m_conditionValue.clear(); }
inline bool DbSqlCondition::hasCommand() const { return m_conditionCommand != null; }

inline uint32 DbSqlCondition::getFlags() const { return m_flags; }
inline void DbSqlCondition::setFlags(uint32 flags) { m_flags = flags; }

inline bool DbSqlCondition::hasEqual() const { return (m_flags & cfEqual) == cfEqual; }
inline bool DbSqlCondition::hasDifferent() const { return (m_flags & cfDifferent) == cfDifferent; }
inline bool DbSqlCondition::hasMinor() const { return (m_flags & cfMinor) == cfMinor; }
inline bool DbSqlCondition::hasMajor() const { return (m_flags & cfMajor) == cfMajor; }
inline bool DbSqlCondition::hasLike() const { return (m_flags & cfLike) == cfLike; }
inline bool DbSqlCondition::hasIn() const { return (m_flags & cfIn) == cfIn; }

inline bool DbSqlCondition::isIn() const { return (m_flags & cfIn) == cfIn; }
inline bool DbSqlCondition::isAnd() const { return (m_flags & cfAnd) == cfAnd; }
inline bool DbSqlCondition::isOr() const { return (m_flags & cfAnd) == cfAnd; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_SQL_CONDITION_H
