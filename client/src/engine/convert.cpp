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
#include "convert.h"

#include "constants.h"
#include "conversions.h"
#include "datetime.h"
#include "unicode.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////
/*
bool Convert::toBool(const String &v)
{
	return v == _S("true");
}

byte Convert::toByte(const String &v)
{
	return static_cast<byte>(os_toi(v.c_str()));
}

int32 Convert::toInt32(const String &v)
{
	return os_toi(v.c_str());
}

uint32 Convert::toUint32(const String &v)
{
	return os_tol(v.c_str());
}

double Convert::toDouble(const String &v)
{
    return os_strtod(v.c_str(), nullptr);
}

String Convert::toString(bool v)
{
	// Nota: non modificare queste funzioni visto che vengono richiamate
	// per generare le firme digitali degli oggetti
	return v ? _S("true") : _S("false");
}

String Convert::toString(byte v)
{
	return String::format(_S("%d").c_str(), v);
}

String Convert::toString(int32 v)
{
	return String::format(_S("%d").c_str(), v);
}

String Convert::toString(uint32 v)
{
	return String::format(_S("%d").c_str(), v);
}

String Convert::toString(double v)
{
//	Nota: NON usare stringstream perch internamente fa probabilmente una conversione
//	usando %g e non %f, l'esempio sotto dimostra che una conversione con %g non porta
//	allo stesso risultato (v1 != v2)
//
//	v1 = 1147854380.848;
//
//	str = String::format("%g", v1);
//	v2 = Convert::toDouble(str);
//
//	str = String::format("%f", v1);
//	v2 = Convert::toDouble(str);

	return String::format(_S("%f").c_str(), v);
}
*/
String Convert::toSQL(bool v)
{
	// 1 e 0 dovrebbero essere pi portabili rispetto a true e false
	return v ? _S("1") : _S("0");
}

String Convert::toSQL(int32 v)
{
	return conversions::to_utf16(v);
}

String Convert::toSQL(uint32 v)
{
	return conversions::to_utf16(v);
}

String Convert::toSQL(double v)
{
	return conversions::to_utf16(v);
}

String Convert::toSQL(const String &v)
{
	String sql = v;
	sql.replace_all(_S("'"), _S("''"));
	return _S("'") + sql + _S("'");
}

String Convert::toSQL(const std::string &v)
{
	return toSQL(String(v));
}

String Convert::toSQL(const uchar *v)
{
	return toSQL(utils::safeString(v));
}

String Convert::toSQL(const DateTime &dt)
{
	return toSQL(dt.toString());
}

String Convert::toSQL(const ObjectID &v)
{
    return toSQL(v.toUTF16());
}

/*
String Convert::toSQL(const EntityID &v)
{
    return toSQL(v.toUTF16());
}
*/

uint32 Convert::toUint32(PortalObjectType v)
{
	return static_cast<uint32>(v);
}

PortalObjectType Convert::toObjectType(uint32 v)
{
	PortalObjectType type = static_cast<PortalObjectType>(v);
	switch(type)
	{
	case portalObjectTypeUnknown:
	case portalObjectTypeInstance:
	case portalObjectTypePost:
	case portalObjectTypeText:
	case portalObjectTypeUser:
	case portalObjectTypeReputation:
	case portalObjectTypeAvatar:
	case portalObjectTypeFile:
	case portalObjectTypeTag:
	case portalObjectTypeSection:
	case portalObjectTypeAttribute:
	case portalObjectTypePrivateMessage:
	case portalObjectTypePoll:
	case portalObjectTypePollOption:
	case portalObjectTypeVote:
	case portalObjectTypeModel:
	case portalObjectTypeCalendarEvent:
	case portalObjectTypePollVote:
	case portalObjectTypeAttachment:
											return type;

	default:								OS_ASSERTFALSE();
											break;
	}

	return portalObjectTypeUnknown;
}

/*
String Convert::toString(PortalType v)
{
	return conversions::to_utf16(static_cast<int32>(v));		// int32 per comprendere l'unknown
}

PortalType Convert::toPortalType(const String &v)
{
	int32 type = conversions::from_utf16<int32>(v);
	switch(type)
	{
	case portalTypeUnknown:
	case portalTypeAnarchic:
	case portalTypeMonarchic:
										return static_cast<PortalType>(type);

	default:							OS_ASSERTFALSE();
										break;
	}

	return portalTypeUnknown;
}
*/

String Convert::toString(PortalObjectType v)
{
	return conversions::to_utf16(static_cast<int32>(v));
}

PortalObjectType Convert::toObjectType(const String &v)
{
	int32 type = conversions::from_utf16<int32>(v);
	switch(type)
	{
	case portalObjectTypeUnknown:
	case portalObjectTypeInstance:
	case portalObjectTypePost:
	case portalObjectTypeText:
	case portalObjectTypeUser:
	case portalObjectTypeReputation:
	case portalObjectTypeAvatar:
	case portalObjectTypeFile:
	case portalObjectTypeTag:
	case portalObjectTypeSection:
	case portalObjectTypeAttribute:
	case portalObjectTypePrivateMessage:
	case portalObjectTypePoll:
	case portalObjectTypePollOption:
	case portalObjectTypeVote:
	case portalObjectTypeModel:
	case portalObjectTypeCalendarEvent:
	case portalObjectTypePollVote:
	case portalObjectTypeAttachment:
											return static_cast<PortalObjectType>(type);

	default:								OS_ASSERTFALSE();
											break;
	}

	return portalObjectTypeUnknown;
}

String Convert::toString(ObjectsReputationThreshold v)
{
	return conversions::to_utf16(static_cast<int32>(v));
}

ObjectsReputationThreshold Convert::toReputationThreshold(const String &v)
{
	return toReputationThreshold(conversions::from_utf16<int32>(v));
}

ObjectsReputationThreshold Convert::toReputationThreshold(const uint32 &v)
{
	switch(v)
	{
	case rtAll:
	case rtNegative:
	case rtNotNegative:
	case rtPositive:
	case rtInherit:
									return static_cast<ObjectsReputationThreshold>(v);

	default:						OS_ASSERTFALSE();
									break;
	}

	return rtAll;
}

String Convert::toString(DataViewMode v)
{
	return conversions::to_utf16(static_cast<uint32>(v));
}

DataViewMode Convert::toAccountViewMode(const String &v)
{
	return toAccountViewMode(conversions::from_utf16<uint32>(v));
}

DataViewMode Convert::toAccountViewMode(const uint32 &v)
{
	switch(v)
	{
	case dataViewModeNormal:
	case dataViewModeEditor:
									return static_cast<DataViewMode>(v);

	default:						OS_ASSERTFALSE();
									break;
	}

	return dataViewModeNormal;
}

String Convert::toString(ObjectsInstanceArea v)
{
	return conversions::to_utf16(static_cast<int32>(v));
}

ObjectsInstanceArea Convert::toInstanceArea(const String &v)
{
	int32 area = conversions::from_utf16<int32>(v);
	switch(area)
	{
	case iaUnknown:
	case iaLeft:
	case iaTop:
	case iaRight:
	case iaBottom:
	case iaContentBottom:
	case iaContentTop:
	case iaContent:
										return static_cast<ObjectsInstanceArea>(area);

	default:							OS_ASSERTFALSE();
										break;
	}

	return iaUnknown;
}

String Convert::toString(ObjectsInstanceInherit v)
{
	return conversions::to_utf16(static_cast<int32>(v));
}

ObjectsInstanceInherit Convert::toInstanceInherit(const String &v)
{
	int32 inherit = conversions::from_utf16<int32>(v);
	switch(inherit)
	{
	case iiNone:
	case iiParent:
	case iiRecursive:
										return static_cast<ObjectsInstanceInherit>(inherit);

	default:							OS_ASSERTFALSE();
										break;
	}

	return iiParent;
}

/*
String Convert::toString(PortalAccess v)
{
	return conversions::to_utf16(static_cast<int32>(v));		// int32 per comprendere l'unknown
}

PortalAccess Convert::toPortalAccess(const String &v)
{
	int32 type = conversions::from_utf16<int32>(v);
	switch(type)
	{
	case portalAccessUnknown:
	case portalAccessPublic:
	case portalAccessPrivate:
										return static_cast<PortalAccess>(type);

	default:							OS_ASSERTFALSE();
										break;
	}

	return portalAccessUnknown;
}
*/

uint16 Convert::toUint16(p2p::packets::PacketType v)
{
	return static_cast<uint16>(v);
}

p2p::packets::PacketType Convert::toPacketType(uint16 v)
{
	p2p::packets::PacketType type = static_cast<p2p::packets::PacketType>(v);
	switch(type)
	{
	case p2p::packets::ptUnknown:
	case p2p::packets::ptError:
	case p2p::packets::ptBusy:
	case p2p::packets::ptHello:
	case p2p::packets::ptTerminate:
	case p2p::packets::ptAuthenticationRequest:
	case p2p::packets::ptAuthenticationResponse:
	case p2p::packets::ptPortalRequest:
	case p2p::packets::ptPortalResponse:
	case p2p::packets::ptQueryRequest:
	case p2p::packets::ptQueryResponse:
	case p2p::packets::ptObjectsRequest:
	case p2p::packets::ptObjectsResponse:
												return type;

	default:									OS_ASSERTFALSE();
												break;
	}

	return p2p::packets::ptUnknown;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
