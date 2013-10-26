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

#ifndef _CONVERT_H
#define _CONVERT_H

#include "datadata.h"
#include "dbvalue.h"
#include "p2ppacketspackets.h"
#include "objectsobjects.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport Convert
{
// Operations
public:
	/*
	static bool toBool(const String &v);
	static byte toByte(const String &v);
	static int32 toInt32(const String &v);
	static uint32 toUint32(const String &v);
	static double toDouble(const String &v);

	static String toString(bool v);
	static String toString(byte v);
	static String toString(int32 v);
	static String toString(uint32 v);
	static String toString(double v);
	*/

	template <typename T>
	static String toSQL(const DbValue<T> &v);

	static String toSQL(bool v);
	static String toSQL(int32 v);
	static String toSQL(uint32 v);
	static String toSQL(double v);
	static String toSQL(const String &v);
	static String toSQL(const std::string &v);
	static String toSQL(const uchar *v);
	static String toSQL(const DateTime &dt);
	static String toSQL(const ObjectID &v);
	//static String toSQL(const EntityID &v);

	static uint32 toUint32(PortalObjectType v);
	static PortalObjectType toObjectType(uint32 v);

	//static String toString(PortalType v);
	//static PortalType toPortalType(const String &v);

	static String toString(PortalObjectType v);
	static PortalObjectType toObjectType(const String &v);

	static String toString(ObjectsReputationThreshold v);
	static ObjectsReputationThreshold toReputationThreshold(const String &v);
	static ObjectsReputationThreshold toReputationThreshold(const uint32 &v);

	static String toString(DataViewMode v);
	static DataViewMode toAccountViewMode(const String &v);
	static DataViewMode toAccountViewMode(const uint32 &v);

	static String toString(ObjectsInstanceArea v);
	static ObjectsInstanceArea toInstanceArea(const String &v);

	static String toString(ObjectsInstanceInherit v);
	static ObjectsInstanceInherit toInstanceInherit(const String &v);

	/*
	static String toString(PortalAccess v);
	static PortalAccess toPortalAccess(const String &v);
	*/

	static uint16 toUint16(p2p::packets::PacketType v);
	static p2p::packets::PacketType toPacketType(uint16 v);
};

//////////////////////////////////////////////////////////////////////

template <typename T>
String Convert::toSQL(const DbValue<T> &v)
{
	return toSQL(v.get());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _CONVERT_H
