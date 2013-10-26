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

#ifndef _OS_ENGINE_ID_H
#define _OS_ENGINE_ID_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

// N.B.: non modificare mai questi valori

#define OS_IDTYPE_UNKNOWN						0
#define OS_IDTYPE_PORTAL						1
#define OS_IDTYPE_OBJECT						2
#define OS_IDTYPE_MODULE						3
#define OS_IDTYPE_COMPONENT						4

//////////////////////////////////////////////////////////////////////

/*
	ID osiris:
				4 bytes: header
					bytes[0] = specifico per la tipologia
					bytes[1] = specifico per la tipologia
					bytes[2] = specifico per la tipologia
					bytes[3] = riservato, contiene la tipologia di id

				20 bytes: hash (SHA) dell'oggetto
*/

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ID : public Object
{
	typedef Object BaseClass;

public:
	enum IDType
	{
		idUnknown	= OS_IDTYPE_UNKNOWN,
		idPortal	= OS_IDTYPE_PORTAL,
		idObject	= OS_IDTYPE_OBJECT,
		idModule	= OS_IDTYPE_MODULE,
		idComponent = OS_IDTYPE_COMPONENT,
	};

	class EngineExport Header
	{
		friend class ID;

	// Construction
	public:
		Header(IDType type);
		~Header();

	// Attributes
	public:
		IDType getType() const;

		inline byte getFirst() const;
		inline void setFirst(byte value);

		inline byte getSecond() const;
		inline void setSecond(byte value);

		inline byte getThird() const;
		inline void setThird(byte value);

		inline byte getFourth() const;

		inline uint32 getValue() const;

	// Operations
	private:
		void clear();

		std::string encode() const;
		bool decode(const std::string &header);

	private:
		union
		{
			byte m_bytes[4];
			uint32 m_value;
		};
	};

// Construction
public:
	ID();
	ID(const std::string &id);
	ID(const ID &second);
	virtual ~ID();

// Attributes
public:
	inline const Header & getHeader() const;

	inline const std::string & getString() const;
	
	inline IDType getType() const;

	inline bool empty() const;
	
	std::wstring toWide() const;
	String toUTF16() const;

	std::string getHash() const;

// Operators
public:
	ID & operator=(const std::string &id);

// Operations
public:
	bool assign(const std::string &id);
	void clear();

// Overridables
public:
	virtual bool validate(bool allow_null) const;

protected:
	virtual IDType getValidationType() const;

protected:
	static std::string generateID(const Header &header, const std::string &hash);
	static std::string standardiseID(const std::string &id);

public:
	static const uint32 SIZE;
	static const uint32 HEADER_SIZE;
	static const uint32 HASH_SIZE;

protected:
	Header m_header;
	std::string m_id;
};

//////////////////////////////////////////////////////////////////////

inline byte ID::Header::getFirst() const { return m_bytes[0]; }
inline void ID::Header::setFirst(byte value) { m_bytes[0] = value; }

inline byte ID::Header::getSecond() const { return m_bytes[1]; }
inline void ID::Header::setSecond(byte value) { m_bytes[1] = value; }

inline byte ID::Header::getThird() const { return m_bytes[2]; }
inline void ID::Header::setThird(byte value) { m_bytes[2] = value; }

inline byte ID::Header::getFourth() const { return m_bytes[3]; }

inline uint32 ID::Header::getValue() const { return m_value; }

//////////////////////////////////////////////////////////////////////

inline const ID::Header & ID::getHeader() const { return m_header; }
inline const std::string & ID::getString() const { return m_id; }

inline ID::IDType ID::getType() const { return m_header.getType(); }

inline bool ID::empty() const { return m_id.empty(); }

//////////////////////////////////////////////////////////////////////

inline bool operator ==(const ID &first, const ID &second)
{
	return first.getString() == second.getString();
}

inline bool operator !=(const ID &first, const ID &second)
{
	return first.getString() != second.getString();
}

inline bool operator <(const ID &first, const ID &second)
{
	return first.getString() < second.getString();
}

inline bool operator <=(const ID &first, const ID &second)
{
	return first.getString() <= second.getString();
}

inline bool operator >(const ID &first, const ID &second)
{
	return first.getString() > second.getString();
}

inline bool operator >=(const ID &first, const ID &second)
{
	return first.getString() >= second.getString();
}

//////////////////////////////////////////////////////////////////////

inline std::size_t hash_value(const ID &v)
{
	return boost::hash<std::string>()(v.getString());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_ID_H
