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

#ifndef _MYSQL_PARAMETER_H
#define _MYSQL_PARAMETER_H

#include "dbiparameter.h"
#include "buffer.h"

//////////////////////////////////////////////////////////////////////

namespace mysql {

//////////////////////////////////////////////////////////////////////

class Parameter : public IDatabaseParameter
{
// Construction
public:
	Parameter(MYSQL_BIND *binder);
	virtual ~Parameter();

// Attributes
public:
	inline MYSQL_BIND * getBinder() const;

// IParameter interface
public:
	virtual void setBool(bool value);
	virtual void setByte(byte value);
	virtual void setInt32(int32 value);
	virtual void setUint32(uint32 value);
	virtual void setDouble(double value);
	virtual void setString(const String &value);
	virtual void setBlob(const void *data, uint32 size);
	virtual void setNull();

private:
	void _setValue(enum_field_types type, const void *data, uint32 size, bool is_null, bool is_unsigned);

private:
	MYSQL_BIND *m_binder;
	Buffer m_buffer;
	bool m_null;
};

//////////////////////////////////////////////////////////////////////

inline MYSQL_BIND * Parameter::getBinder() const { return m_binder; }

//////////////////////////////////////////////////////////////////////

} // mysql

//////////////////////////////////////////////////////////////////////

#endif // _MYSQL_PARAMETER_H
