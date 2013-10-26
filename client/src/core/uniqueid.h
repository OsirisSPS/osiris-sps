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

#ifndef _OS_CORE_UNIQUEID_H
#define _OS_CORE_UNIQUEID_H

#include "base/object.h"
#include "constants.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport UniqueID : public Object
{
	typedef Object BaseClass;

// Construction
public:
	UniqueID();
	UniqueID(const std::string &id);
	UniqueID(const UniqueID &second);
	virtual ~UniqueID();

// Attributes
public:
	inline const std::string & getString() const;
	inline bool empty() const;
	
// Operators
public:
	UniqueID & operator=(const std::string &id);
	
// Operations
public:
	bool assign(const std::string &id);
	void clear();

	bool validate(bool allow_null) const;

	inline const std::string & toAscii() const;
	std::wstring toWide() const;
	String toUTF16() const;

	static UniqueID generate();

private:
	static std::string standardiseID(const std::string &id);

public:
	static const uint32 SIZE;
	static const UniqueID EMPTY;

protected:
	std::string m_id;
};

//////////////////////////////////////////////////////////////////////

inline const std::string & UniqueID::getString() const { return m_id; }
inline const std::string & UniqueID::toAscii() const { return m_id; }
inline bool UniqueID::empty() const { return m_id.empty(); }

//////////////////////////////////////////////////////////////////////

inline bool operator ==(const UniqueID &first, const UniqueID &second)
{
	return first.getString() == second.getString();
}

inline bool operator !=(const UniqueID &first, const UniqueID &second)
{
	return first.getString() != second.getString();
}

inline bool operator <(const UniqueID &first, const UniqueID &second)
{
	return first.getString() < second.getString();
}

inline bool operator <=(const UniqueID &first, const UniqueID &second)
{
	return first.getString() <= second.getString();
}

inline bool operator >(const UniqueID &first, const UniqueID &second)
{
	return first.getString() > second.getString();
}

inline bool operator >=(const UniqueID &first, const UniqueID &second)
{
	return first.getString() >= second.getString();
}

//////////////////////////////////////////////////////////////////////

inline std::size_t hash_value(const UniqueID &v)
{
	return boost::hash<std::string>()(v.getString());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_UNIQUEID_H
