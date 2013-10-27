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
#include "dbvalue.h"

#include "constants.h"
#include "conversions.h"
#include "buffer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
EngineExport const bool & get_default<bool>()
{
	return constants::bool_false;
}

template <>
EngineExport const int32 & get_default<int32>()
{
	return constants::int32_zero;
}

template <>
EngineExport const uint32 & get_default<uint32>()
{
	return constants::uint32_zero;
}

template <>
EngineExport const double & get_default<double>()
{
	return constants::double_zero;
}

template <>
EngineExport const String & get_default<String>()
{
	return String::EMPTY;
}

template <>
EngineExport const Buffer & get_default<Buffer>()
{
	return Buffer::EMPTY;
}

template <>
EngineExport const DateTime & get_default<DateTime>()
{
	return DateTime::EMPTY;
}



#ifndef OS_DEBUGID
template <>
EngineExport const UniqueID & get_default<UniqueID>()
{
	return UniqueID::EMPTY;
}

#else

template <>
EngineExport const ObjectID & get_default<ObjectID>()
{
	return ObjectID::EMPTY;
}

template <>
EngineExport const EntityID & get_default<EntityID>()
{
	return EntityID::EMPTY;
}

template <> 
EngineExport const PovID & get_default<PovID>()
{
	return PovID::EMPTY;
}

template <>
EngineExport const ExtensionsComponentID & get_default<ExtensionsComponentID>()
{
	return ExtensionsComponentID::EMPTY;
}

template <>
EngineExport const ExtensionsModuleID & get_default<ExtensionsModuleID>()
{
	return ExtensionsModuleID::EMPTY;
}
#endif

template <>
EngineExport String export_to_xml<bool>(const bool &value)
{
	return conversions::to_utf16(value);
}

template <>
EngineExport bool import_from_xml<bool>(const String &value)
{
	return conversions::from_utf16<bool>(value);
}

template <>
EngineExport String export_to_xml<String>(const String &value)
{
	return static_cast<String>(value);
}

template <>
EngineExport String import_from_xml<String>(const String &value)
{
	return value;
}

template <>
EngineExport String export_to_xml<uint32>(const uint32 &value)
{
	return conversions::to_utf16(value);
}

template <>
EngineExport uint32 import_from_xml<uint32>(const String &value)
{
	return conversions::from_utf16<uint32>(value);
}

template <>
EngineExport String export_to_xml<DateTime>(const DateTime &value)
{
	return value.toString();
}

template <>
EngineExport DateTime import_from_xml<DateTime>(const String &value)
{
	return DateTime(value);
}

template <>
EngineExport String export_to_xml<Buffer>(const Buffer &value)
{
	return value.toHex();
}

template <>
EngineExport Buffer import_from_xml<Buffer>(const String &value)
{
	Buffer buffer;
	buffer.fromHex(value.to_ascii());
	return buffer;
}

template <>
EngineExport String export_to_xml<ObjectID>(const ObjectID &value)
{
	return value.toUTF16();
}

template <>
EngineExport ObjectID import_from_xml<ObjectID>(const String &value)
{
	return ObjectID(value.to_ascii());
}

#ifdef OS_ENTITYID
template <>
EngineExport String export_to_xml<EntityID>(const EntityID &value)
{
	return value.toUTF16();
}

template <>
EngineExport EntityID import_from_xml<EntityID>(const String &value)
{
	return EntityID(value.to_ascii());
}
#endif

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
