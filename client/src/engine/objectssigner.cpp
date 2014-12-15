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
#include "objectssigner.h"

#include "cryptmanager.h"
#include "datetime.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

const std::string ObjectsSigner::FIELD_DELIMITER = ";";

//////////////////////////////////////////////////////////////////////

ObjectsSigner::ObjectsSigner()
{

}

ObjectsSigner::~ObjectsSigner()
{

}

bool ObjectsSigner::sign(const Buffer &private_key, Buffer &signature)
{
	return CryptManager::instance()->rsaSign(private_key, m_block.getData(), m_block.getSize(), signature);
}

bool ObjectsSigner::validate(const Buffer &public_key, const Buffer &signature) const
{
	return CryptManager::instance()->rsaVerify(public_key, m_block.getData(), m_block.getSize(), signature);
}

template <>
void EngineExport ObjectsSigner::add<bool>(const bool &value)
{
	add(&value, sizeof(bool));
}

template <>
void EngineExport ObjectsSigner::add<int32>(const int32 &value)
{
	add(&value, sizeof(int32));
}

template <>
void EngineExport ObjectsSigner::add<uint32>(const uint32 &value)
{
	add(&value, sizeof(uint32));
}

template <>
void EngineExport ObjectsSigner::add<double>(const double &value)
{
	add(&value, sizeof(double));
}

template <>
void EngineExport ObjectsSigner::add<std::string>(const std::string &value)
{
	add(value.data(), value.size()); // Razor added
}

// TOCLEAN, serve ancora?
template <>
void EngineExport ObjectsSigner::add<String>(const String &value)
{
	// Nota: la stringa deve rimanere immutata, pertanto non bisogna
	// richiamare il metodo writeString (che scriverebbe prima la dimensione
	// e poi il buffer), ma va fatta una scrittura semplice



	//add(value.buffer(), static_cast<uint32>(value.buffer_size()));

	add<std::string>(value.to_utf8());
	
	//add(value.buffer(), static_cast<uint32>(value.buffer_size()));
}

template <>
void EngineExport ObjectsSigner::add<Buffer>(const Buffer &value)
{
	// Nota: il buffer deve rimanere immutato, pertanto non bisogna
	// richiamare il metodo writeBuffer (che scriverebbe prima la dimensione
	// e poi il buffer), ma va fatta una scrittura semplice

	add(value.getData(), value.getSize());
}

template <>
void EngineExport ObjectsSigner::add<DateTime>(const DateTime &value)
{
	add(value.toString());
}



template <>
void EngineExport ObjectsSigner::add<UniqueID>(const UniqueID &value)
{
#ifdef OS_NEWSIGNATURE
	add(value.getString());
#else
	add(value.toUTF16());	// Retrocompatibilità con il formato degli ID
#endif
}

#ifdef OS_DEBUGID

template <>
void EngineExport ObjectsSigner::add<ObjectID>(const ObjectID &value)
{
#ifdef OS_NEWSIGNATURE
	add(value.getString());
#else
	add(value.toUTF16());	// Retrocompatibilità con il formato degli ID
#endif
}

template <>
void EngineExport ObjectsSigner::add<ExtensionsComponentID>(const ExtensionsComponentID &value)
{
#ifdef OS_NEWSIGNATURE
	add(value.getString());
#else
	add(value.toUTF16());	// Retrocompatibilità con il formato degli ID
#endif	
}

template <>
void EngineExport ObjectsSigner::add<ExtensionsModuleID>(const ExtensionsModuleID &value)
{
#ifdef OS_NEWSIGNATURE
	add(value.getString());
#else
	add(value.toUTF16());	// Retrocompatibilità con il formato degli ID
#endif
}



template <>
void EngineExport ObjectsSigner::add<EntityID>(const EntityID &value)
{
#ifdef OS_NEWSIGNATURE
	add(value.getString());
#else
	add(value.toUTF16());	// Retrocompatibilità con il formato degli ID
#endif
}

template <>
void EngineExport ObjectsSigner::add<PovID>(const PovID &value)
{
#ifdef OS_NEWSIGNATURE
	add(value.getString());
#else
	add(value.toUTF16());	// Retrocompatibilità con il formato degli ID
#endif
}

template <>
void EngineExport ObjectsSigner::add<PortalID>(const PortalID &value)
{
#ifdef OS_NEWSIGNATURE
	add(value.getString());
#else
	add(value.toUTF16());	// Retrocompatibilità con il formato degli ID
#endif
}


#endif

void ObjectsSigner::add(const void *data, uint32 size)
{
	//toclean
	{
		Buffer b;
		b.write(data,size);
		m_debug += b.toHex() + ",";
	}
	m_block.write(data, size);	
#ifdef OS_NEWSIGNATURE
	uint32 s = FIELD_DELIMITER.size();
	m_block.write(FIELD_DELIMITER.c_str(), FIELD_DELIMITER.size());
#endif
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
