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

#ifndef _OS_CORE_CRYPTKEY_H
#define _OS_CORE_CRYPTKEY_H

#include "boost/noncopyable.hpp"
#include "boost/operators.hpp"
#include "constants.h"
#include "pimpl_ptr.h"
#include "sealed.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;

//////////////////////////////////////////////////////////////////////

class CoreExport CryptKey : public boost::noncopyable,
							public boost::equality_comparable<CryptKey>,
							Sealed<CryptKey>
{
// Construction
public:
	CryptKey();
	CryptKey(const std::string &password, const std::string &salt = constants::empty_string);
	CryptKey(const void *data, uint32 size);
	~CryptKey();

// Attributes
public:
	const Buffer & getKey() const;
	const Buffer & getIV() const;

	static uint32 getHMACSize();

// Operations
public:
	// Genera una chiave a 256 bit dalla password specificata
	const Buffer & generateKey(const std::string &password, const std::string &salt = constants::empty_string);
	const Buffer & generateKey(const void *data, uint32 size);
	const Buffer & generateKeyOld(const String &password, const String &salt = String::EMPTY);

	// Deriva dalla chiave corrente un'altra chiave a 256 bit
	const Buffer & deriveKey(const std::string &v);
	const Buffer & deriveKey(const void *data, uint32 size);

	// Genera un iv (16 bytes) da un intero
	const Buffer & generateIV(const uint32 &v);
	// Genera un iv (16 bytes) da una stringa
	const Buffer & generateIV(const std::string &v);
	// Genera un iv (16 bytes) da un buffer
	const Buffer & generateIV(const Buffer &v);

	// Genera un HMAC sulla base della chiave corrente
	Buffer generateHMAC(const void *data, uint32 size) const;

// Operators
public:
	bool operator==(const CryptKey &second) const;
	
protected:
	pimpl_of<CryptKey>::type m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_CRYPTKEY_H
