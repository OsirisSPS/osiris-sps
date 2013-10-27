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

#ifndef _OS_CORE_HASH_H
#define _OS_CORE_HASH_H

#include "base/object.h"
#include "boost/operators.hpp"
#include "crypt.h"
#include "pimpl_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;

//////////////////////////////////////////////////////////////////////

class CoreExport Hash : public boost::equality_comparable<Hash>,
						public Object
{
	typedef Object BaseClass;

// Construction
public:
	Hash();
	Hash(const Buffer &buffer, HashType type);
	Hash(const Hash &second);
	virtual ~Hash();

// Attributes
public:
	const Buffer & getBuffer() const;
	HashType getType() const;

	bool empty() const;
	bool isValid() const;

// Operations
public:
	void assign(const Buffer &buffer, HashType type = hashTypeUnknown);
	void clear();

	std::string toHex() const;
	bool fromHex(const std::string &str, HashType type = hashTypeUnknown);

	std::string toBase64() const;
	bool fromBase64(const std::string &str, HashType type = hashTypeUnknown);

// Operators
public:
	Hash & operator =(const Hash &second);
	bool operator ==(const Hash &second) const;

public:
	static const Hash EMPTY;

private:
	pimpl_of<Hash>::type m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_HASH_H
