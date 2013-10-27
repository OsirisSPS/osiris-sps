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
#include "cryptkeyagreement.h"

#include "boost/thread/recursive_mutex.hpp"
#include "buffer.h"
#include "crypto++.h"
#include "cryptkey.h"
#include "cryptmanager.h"
#include "locked.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<CryptKeyAgreement>
{
// Construction
public:
	pimpl();
	virtual ~pimpl();
	
// Attributes
public:
	inline const Buffer & getModulus() const;
	inline const Buffer & getGenerator() const;

	inline const Buffer & getPublicKey() const;
	inline const Buffer & getPrivateKey() const;

// Operations
public:
	bool init(uint32 size);
	bool init(const Buffer &modulus, const Buffer &generator);

	bool agree(const Buffer &publicKey, CryptKey &agreedKey);

private:
	static Buffer integerToBuffer(const CryptoPP::Integer &v);
	static CryptoPP::Integer bufferToInteger(const Buffer &v);

	bool createKeyPair(CryptoPP::AutoSeededRandomPool &randomPool);

	void clear();

private:
	scoped_ptr<CryptoPP::DH> m_dh;
	Buffer m_modulus;
	Buffer m_generator;
	Buffer m_publicKey;
	Buffer m_privateKey;
};

//////////////////////////////////////////////////////////////////////

pimpl<CryptKeyAgreement>::pimpl()
{

}

pimpl<CryptKeyAgreement>::~pimpl()
{

}

inline const Buffer & pimpl<CryptKeyAgreement>::getModulus() const
{
	return m_modulus;
}

inline const Buffer & pimpl<CryptKeyAgreement>::getGenerator() const
{
	return m_generator;
}

inline const Buffer & pimpl<CryptKeyAgreement>::getPublicKey() const
{
	return m_publicKey;
}

inline const Buffer & pimpl<CryptKeyAgreement>::getPrivateKey() const
{
	return m_privateKey;
}


bool pimpl<CryptKeyAgreement>::init(uint32 size)
{
	Locked<CryptoPP::AutoSeededRandomPool>::unique randomPool = CryptManager::instance()->getRandomPool();

	clear();		

	try
	{
		m_dh.reset(new CryptoPP::DH(static_cast<CryptoPP::RandomNumberGenerator &>(*randomPool), size));
	}
	catch(std::exception &)
	{
		return false;	
	}

	return createKeyPair(*randomPool);
}

bool pimpl<CryptKeyAgreement>::init(const Buffer &modulus, const Buffer &generator)
{
	Locked<CryptoPP::AutoSeededRandomPool>::unique randomPool = CryptManager::instance()->getRandomPool();

	clear();		

	try
	{
		m_dh.reset(new CryptoPP::DH(bufferToInteger(modulus), bufferToInteger(generator)));
	}
	catch(std::exception &)
	{
		return false;	
	}

	return createKeyPair(*randomPool);
}

bool pimpl<CryptKeyAgreement>::agree(const Buffer &publicKey, CryptKey &agreedKey)
{
	try
	{
		if(m_dh == null)
		{
			OS_ASSERTFALSE();
			return false;
		}

		Buffer agreedValue;
		agreedValue.reserve(m_dh->AgreedValueLength());

		OS_ASSERT(m_privateKey.empty() == false);
		OS_ASSERT(publicKey.empty() == false);
		if(m_dh->Agree(agreedValue.getData(), m_privateKey.getData(), publicKey.getData(), true) == false)
			return false;

		agreedKey.generateKey(agreedValue.getData(), agreedValue.getSize());
	
		return true;
	}
	catch(std::exception &)
	{
		
	}

	return false;
}

Buffer pimpl<CryptKeyAgreement>::integerToBuffer(const CryptoPP::Integer &v)
{
	uint32 length = v.ByteCount();
	Buffer buff(length);

	for(uint32 i = 0; i < length; i++)
	{
		byte b = v.GetByte(i);
		buff.write(&b, sizeof(byte));
	}

	return buff;
}

CryptoPP::Integer pimpl<CryptKeyAgreement>::bufferToInteger(const Buffer &v)
{
	uint32 length = v.getSize();
	CryptoPP::Integer integer;

	for(uint32 i = 0; i < length; i++)
	{
		 integer.SetByte(i, v.getAt(i));
	}

	return integer;
}

bool pimpl<CryptKeyAgreement>::createKeyPair(CryptoPP::AutoSeededRandomPool &randomPool)
{
	try
	{
		OS_ASSERT(m_dh != null);

		OS_ASSERT(m_publicKey.empty());
		m_publicKey.reserve(m_dh->PublicKeyLength());

		OS_ASSERT(m_privateKey.empty());
		m_privateKey.reserve(m_dh->PrivateKeyLength());
		
		m_dh->GenerateKeyPair(randomPool, m_privateKey.getData(), m_publicKey.getData());

		OS_ASSERT(m_modulus.empty());
		m_modulus = integerToBuffer(m_dh->GetGroupParameters().GetModulus());

		OS_ASSERT(m_generator.empty());
		m_generator = integerToBuffer(m_dh->GetGroupParameters().GetGenerator());

		return true;
	}
	catch(std::exception &)
	{
		
	}

	return false;
}

void pimpl<CryptKeyAgreement>::clear()
{
	m_dh.reset();
	m_modulus.clear();
	m_generator.clear();
	m_publicKey.clear();
	m_privateKey.clear();
}

//////////////////////////////////////////////////////////////////////

CryptKeyAgreement::CryptKeyAgreement()
{
	
}

CryptKeyAgreement::~CryptKeyAgreement()
{

}

const Buffer & CryptKeyAgreement::getModulus() const
{
	return m_impl->getModulus();
}

const Buffer & CryptKeyAgreement::getGenerator() const
{
	return m_impl->getGenerator();
}

const Buffer & CryptKeyAgreement::getPublicKey() const
{
	return m_impl->getPublicKey();
}

const Buffer & CryptKeyAgreement::getPrivateKey() const
{
	return m_impl->getPrivateKey();
}

bool CryptKeyAgreement::init(uint32 size)
{
	return m_impl->init(size);
}

bool CryptKeyAgreement::init(const Buffer &modulus, const Buffer &generator)
{
	return m_impl->init(modulus, generator);
}

bool CryptKeyAgreement::agree(const Buffer &publicKey, CryptKey &agreedKey)
{
	return m_impl->agree(publicKey, agreedKey);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
