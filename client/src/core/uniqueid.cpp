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
#include "uniqueid.h"

#include "algorithms.h"
#include "buffer.h"
#include "cryptmanager.h"
#include "regexmanager.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const uint32 UniqueID::SIZE = 40;	// Dimensione in HEX
const UniqueID UniqueID::EMPTY;

//////////////////////////////////////////////////////////////////////

UniqueID::UniqueID()
{

}

UniqueID::UniqueID(const std::string &id)
{
	assign(id);
}

UniqueID::UniqueID(const UniqueID &second) : BaseClass(second)
{
	assign(second.m_id);
}

UniqueID::~UniqueID()
{

}

UniqueID & UniqueID::operator=(const std::string &id)
{
	assign(id);
	return *this;
}

bool UniqueID::assign(const std::string &id)
{
	clear();

	std::string nid = standardiseID(id);

	if(nid.empty())
		return true;

	// TOCLEAN: Questo c'è per evitare di toccare tutti gli ID delle extension/component, usate dalla 0.X. Abolibile.
	if(nid.size() == 48)
	{
		nid = nid.substr(8);
	}

	if(nid.size() != SIZE)
		return false;

	if(regex_match(nid, RegexManager::instance()->getRegex(_S("[0-9A-F]*"), false)) == false)
	{
		//OS_ASSERTFALSE();
		return false;
	}

	m_id = nid;
	return true;
}

std::string UniqueID::standardiseID(const std::string &id)
{
	return algorithms::to_upper_copy(id);
}

void UniqueID::clear()
{
	m_id.clear();
}

bool UniqueID::validate(bool allow_null) const
{
	if(empty())
		return allow_null;

	return true;
}

std::wstring UniqueID::toWide() const
{
	return utils::atow(getString());
}

String UniqueID::toUTF16() const
{
    return String(getString());
}

UniqueID UniqueID::generate()
{
	UniqueID id(CryptManager::instance()->randomSHA().toHex());
	OS_ASSERT(id.validate(false));
	return id;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
