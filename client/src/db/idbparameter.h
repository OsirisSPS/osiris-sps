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

#ifndef _IDBPARAMETER_H
#define _IDBPARAMETER_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;
class DataItem;

//////////////////////////////////////////////////////////////////////

class DatabaseExport IDbParameter : public Object
{
// Construction
public:
	IDbParameter();
	virtual ~IDbParameter();

// Interface
public:
	virtual void setBool(bool value) = 0;
	virtual void setByte(byte value) = 0;
	virtual void setInt32(int32 value) = 0;
	virtual void setUint32(uint32 value) = 0;
	virtual void setDouble(double value) = 0;
	virtual void setString(const String &value) = 0;
	virtual void setBlob(const void *data, uint32 size) = 0;
	virtual void setNull() = 0;

// Operations
public:
	void setBlob(const Buffer &buffer);
	void setValue(const DataItem &value);
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDBPARAMETER_H
