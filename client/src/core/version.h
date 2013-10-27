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

#ifndef _OS_CORE_VERSION_H
#define _OS_CORE_VERSION_H

//////////////////////////////////////////////////////////////////////

#define OS_VERSION_NULL							0

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport Version
{
// Construction
public:
	Version(const uint16 &value = OS_VERSION_NULL);
	Version(uint8 minor, uint8 major);
	Version(const Version &second);
	~Version();

// Attributes
public:
	inline uint16 get() const;
	inline void set(uint16 value);
	inline void set(uint8 minor, uint8 major);

	inline uint8 getMinor() const;
	inline void setMinor(uint8 minor);

	inline uint8 getMajor() const;
	inline void setMajor(uint8 major);

	inline bool empty() const;

	std::string toString() const;
	bool fromString(const std::string &str);		

// Operations
public:
	void clear();

// Operators
public:
	Version & operator =(const Version &second);

	inline bool operator ==(const Version &second) const;
	inline bool operator !=(const Version &second) const;
	inline bool operator >(const Version &second) const;
	inline bool operator >=(const Version &second) const;
	inline bool operator <(const Version &second) const;
	inline bool operator <=(const Version &second) const;

public:
	static const Version EMPTY;

private:
	uint16 m_value;
};

//////////////////////////////////////////////////////////////////////

inline uint16 Version::get() const { return m_value; }
inline void Version::set(uint16 value) { m_value = value; }
inline void Version::set(uint8 minor, uint8 major) { m_value = OS_MAKEVERSION(minor, major); }

inline uint8 Version::getMinor() const { return OS_VERSION_MINOR(m_value); }
inline void Version::setMinor(uint8 minor) { set(minor, getMajor()); }

inline uint8 Version::getMajor() const { return OS_VERSION_MAJOR(m_value); }
inline void Version::setMajor(uint8 major) { set(getMinor(), major); }

inline bool Version::empty() const { return get() == OS_VERSION_NULL; }

inline bool Version::operator ==(const Version &second) const { return m_value == second.m_value; }
inline bool Version::operator !=(const Version &second) const { return m_value != second.m_value; }
inline bool Version::operator >(const Version &second) const { return m_value > second.m_value; }
inline bool Version::operator >=(const Version &second) const { return m_value >= second.m_value; }
inline bool Version::operator <(const Version &second) const { return m_value < second.m_value; }
inline bool Version::operator <=(const Version &second) const { return m_value <= second.m_value; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_VERSION_H
