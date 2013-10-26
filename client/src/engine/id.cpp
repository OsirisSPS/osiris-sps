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
#include "id.h"

#include "algorithms.h"
#include "buffer.h"
#include "constants.h"
#include "conversions.h"
#include "cryptmanager.h"
#include "regexmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace conversions {

//////////////////////////////////////////////////////////////////////

template<>
ID::IDType toEnumeratedType(uint32 v)
{
	switch(v)
	{
	case ID::idUnknown:
	case ID::idPortal:
	case ID::idObject:
	case ID::idModule:
	case ID::idComponent:
							return static_cast<ID::IDType>(v);

	default:				OS_ASSERTFALSE();
							break;
	}

	return ID::idUnknown;
}

//////////////////////////////////////////////////////////////////////

} // conversions

//////////////////////////////////////////////////////////////////////

ID::Header::Header(IDType type)
{
	clear();

	m_bytes[3] = static_cast<byte>(type);
}

ID::Header::~Header()
{

}

ID::IDType ID::Header::getType() const
{
	return conversions::toEnumeratedType<IDType>(m_bytes[3]);
}

void ID::Header::clear()
{
	m_value = 0;	// inizializza m_bytes a 0
}

std::string ID::Header::encode() const
{
	std::string hex;
	if(CryptManager::instance()->encodeHex(m_bytes, sizeof(m_bytes), hex) == false)
	{
		OS_ASSERTFALSE();
		return constants::empty_string;
	}

	return hex;
}

bool ID::Header::decode(const std::string &header)
{
	OS_ASSERT(header.size() == ID::HEADER_SIZE);

	Buffer buffer;
	if(buffer.fromHex(header))
	{
		OS_ASSERT(buffer.getSize() == sizeof(m_bytes));
		buffer.seekToBegin();
		return buffer.read(&m_bytes, sizeof(m_bytes)) == sizeof(m_bytes);
	}

	return false;
}

//////////////////////////////////////////////////////////////////////

// Dimensioni in HEX
const uint32 ID::SIZE = 48;
const uint32 ID::HEADER_SIZE = 8;
const uint32 ID::HASH_SIZE = 40;

//////////////////////////////////////////////////////////////////////

ID::ID() : m_header(idUnknown)
{

}

ID::ID(const std::string &id) : m_header(idUnknown)
{
	assign(id);
}

ID::ID(const ID &second) : BaseClass(second),
                           m_header(idUnknown)
{
	assign(second.m_id);
}

ID::~ID()
{

}

std::wstring ID::toWide() const
{
	return String(getString()).to_wide();
}

String ID::toUTF16() const
{
    return String(getString());
}

std::string ID::getHash() const
{
	OS_ASSERT(CryptManager::SHA_HEX_DIGESTSIZE == HASH_SIZE);

	if(m_id.empty())
		return constants::empty_string;

	OS_ASSERT(algorithms::mid(m_id, HEADER_SIZE).size() == HASH_SIZE);
	return algorithms::mid(m_id, HEADER_SIZE);
}
/*
ID::operator const std::string &() const
{
	return getString();
}

ID::operator String() const
{
    return toUTF16();
}
*/
ID & ID::operator=(const std::string &id)
{
	assign(id);
	return *this;
}

bool ID::assign(const std::string &id)
{
	clear();

	if(id.empty())
		return true;

	if(id.size() != SIZE)
		return false;

	if(regex_match(id, RegexManager::instance()->getRegex(_S("[0-9A-F]*"), false)) == false)
	{
		OS_ASSERTFALSE();
		return false;
	}

	// N.B.: se si omette potrebbe essere falsata la validazione degli oggetti
	m_id = standardiseID(id);
	// Carica l'id
	if(m_header.decode(algorithms::left(m_id, HEADER_SIZE)) == false)
	{
		OS_ASSERTFALSE();

		clear();
		return false;
	}

	return true;
}

std::string ID::generateID(const Header &header, const std::string &hash)
{
	OS_ASSERT(hash.size() == HASH_SIZE);
	return standardiseID(header.encode() + hash);
}

std::string ID::standardiseID(const std::string &id)
{
	// Gli ID sono sempre maiuscoli
	return algorithms::to_upper_copy(id);
}

bool ID::validate(bool allow_null) const
{	
	if(empty())
		return allow_null;

	// Verifica che la tipologia di ID prevista coincida
	IDType validationType = getValidationType();
	if((validationType != idUnknown) && (validationType != getType()))
		return false;

	return true;
}

ID::IDType ID::getValidationType() const
{
	return idUnknown;
}

void ID::clear()
{
	m_header.clear();
	m_id.clear();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
