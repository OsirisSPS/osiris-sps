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

#include "stdafx.h"
#include "idbparameter.h"

#include "buffer.h"
#include "dataitem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IDbParameter::IDbParameter()
{

}

IDbParameter::~IDbParameter()
{

}

void IDbParameter::setBlob(const Buffer &buffer)
{
	setBlob(buffer.getData(), buffer.getSize());
}

void IDbParameter::setValue(const DataItem &value)
{
	switch(value.getType())
	{
	case DataItem::vt_null:			setNull();
									break;

	case DataItem::vt_bool:			setBool(static_cast<bool>(value));
									break;

	case DataItem::vt_byte:			setByte(static_cast<byte>(value));
									break;

	case DataItem::vt_int32:		setInt32(static_cast<int32>(value));
									break;

	case DataItem::vt_uint32:		setUint32(static_cast<uint32>(value));
									break;

	case DataItem::vt_double:		setDouble(static_cast<double>(value));
									break;

	case DataItem::vt_string:		setString(static_cast<String>(value));
									break;

	case DataItem::vt_buffer:		setBlob(value.get<Buffer>());
									break;

	case DataItem::vt_int64:
	case DataItem::vt_uint64:
	case DataItem::vt_datatree:
	default:
									OS_EXCEPTION("Unsupported DataItem type");
									break;
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
