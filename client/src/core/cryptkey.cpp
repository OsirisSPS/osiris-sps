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
#include "cryptkey.h"

#include "buffer.h"
#include "crypto++.h"
#include "cryptmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<CryptKey>
{
// Construction
public:
	pimpl();
	virtual ~pimpl();

// Attributes
public:
	inline const Buffer & getKey() const;
	inline const Buffer & getIV() const;

	static inline uint32 getHMACSize();

// Operations
public:
	const Buffer & updateKey(const void *data, uint32 size, bool reset);
	const Buffer & updateIV(const void *data, uint32 size);

	Buffer generateHMAC(const void *data, uint32 size) const;

private:
	Buffer m_key;
	Buffer m_iv;
};

//////////////////////////////////////////////////////////////////////

pimpl<CryptKey>::pimpl()
{

}

pimpl<CryptKey>::~pimpl()
{

}

inline const Buffer & pimpl<CryptKey>::getKey() const 
{ 
	return m_key; 
}

inline const Buffer & pimpl<CryptKey>::getIV() const 
{ 
	return m_iv; 
}

inline uint32 pimpl<CryptKey>::getHMACSize()
{
	return CryptoPP::HMAC<CryptoPP::SHA>::DIGESTSIZE;
}

const Buffer & pimpl<CryptKey>::updateKey(const void *data, uint32 size, bool reset)
{
	OS_EXCEPT_IF(data == nullptr || size == 0, "Invalid key");

	// N.B.: NON modificare il metodo di generazione/derivazione di una chiave altrimenti
	// si falsano tutti i dati codificati in precedenza

	if(reset)
		m_key.clear();

	CryptoPP::SHA256 sha256;
	if(m_key.empty() == false)
	{
		sha256.Update(m_key.getData(), m_key.getSize());
		m_key.clear();
	}

	sha256.Update(static_cast<const byte *>(data), size);

	m_key.reserve(sha256.DigestSize());
	sha256.Final(m_key.getData());

	return m_key;
}

const Buffer & pimpl<CryptKey>::updateIV(const void *data, uint32 size)
{
	OS_EXCEPT_IF(data == nullptr || size == 0, "Invalid iv");

	// N.B.: NON modificare il metodo di calcolo di un IV a partire da un buffer altrimenti
	// si falsano tutti i dati codificati in precedenza

	m_iv.clear();
	m_iv = CryptManager::instance()->MD5(data, size);
	return m_iv;
}

Buffer pimpl<CryptKey>::generateHMAC(const void *data, uint32 size) const
{
	OS_ASSERT(data != nullptr && size > 0);

    Buffer hmac;
	hmac.reserve(CryptoPP::HMAC<CryptoPP::SHA>::DIGESTSIZE);
	CryptoPP::HMAC<CryptoPP::SHA>(m_key.getData(), m_key.getSize()).CalculateDigest(hmac.getData(), static_cast<const byte *>(data), static_cast<unsigned int>(size));
	
	OS_ASSERT(hmac.getSize() == getHMACSize());
	return hmac;
}

//////////////////////////////////////////////////////////////////////

CryptKey::CryptKey()
{

}

CryptKey::CryptKey(const std::string &password, const std::string &salt)
{
	generateKey(password, salt);
}

CryptKey::CryptKey(const void *data, uint32 size)
{
	generateKey(data, size);
}

CryptKey::~CryptKey()
{
	
}

const Buffer & CryptKey::getKey() const 
{ 
	return m_impl->getKey(); 
}

const Buffer & CryptKey::getIV() const 
{ 
	return m_impl->getIV(); 
}

uint32 CryptKey::getHMACSize()
{
	return pimpl<CryptKey>::getHMACSize();
}

const Buffer & CryptKey::generateKey(const std::string &password, const std::string &salt)
{
	OS_EXCEPT_IF(password.empty(), "Invalid password");

	// Nota: modificando il metodo di calcolo di una chiave a partire da una password
	// falsa ovviamente tutti i dati codificati in precedenza con quella password

	std::string block = salt + password;
	return m_impl->updateKey(block.data(), static_cast<uint32>(block.size()), true);
}

const Buffer & CryptKey::generateKey(const void *data, uint32 size)
{
	return m_impl->updateKey(data, size, true);
}

const Buffer & CryptKey::deriveKey(const std::string &v)
{
	return m_impl->updateKey(v.data(), static_cast<uint32>(v.size()), false);
}

const Buffer & CryptKey::deriveKey(const void *data, uint32 size)
{
	return m_impl->updateKey(data, size, false);
}

const Buffer & CryptKey::generateIV(const uint32 &v)
{
	return m_impl->updateIV(&v, sizeof(uint32));
}

const Buffer & CryptKey::generateIV(const std::string &v)
{
	return m_impl->updateIV(v.data(), static_cast<uint32>(v.size()));
}

const Buffer & CryptKey::generateIV(const Buffer &v)
{
	return m_impl->updateIV(v.getData(), v.getSize());
}

Buffer CryptKey::generateHMAC(const void *data, uint32 size) const
{
	return m_impl->generateHMAC(data, size);
}

bool CryptKey::operator==(const CryptKey &second) const
{
	if(getKey() != second.getKey())
		return false;

	if(getIV() != second.getIV())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
