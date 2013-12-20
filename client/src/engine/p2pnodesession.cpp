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
#include "p2pnodesession.h"

#include "cryptmanager.h"
#include "cryptkeyagreement.h"
#include "ipaddress.h"
#include "p2pp2psystem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {

//////////////////////////////////////////////////////////////////////

NodeSession::NodeSession()
{
	m_busy = false;
}

NodeSession::~NodeSession()
{

}

//////////////////////////////////////////////////////////////////////

LocalSession::LocalSession()
{
	m_iv = 0;
	m_derivationType = dtNone;
	m_helloSended = false;
}

LocalSession::~LocalSession()
{

}

const Buffer & LocalSession::getModulus() const
{
	if(m_keyAgreement != nullptr)
		return m_keyAgreement->getModulus();

	return Buffer::EMPTY;
}

const Buffer & LocalSession::getGenerator() const
{
	if(m_keyAgreement != nullptr)
		return m_keyAgreement->getGenerator();

	return Buffer::EMPTY;
}

const Buffer & LocalSession::getPublicKey() const
{
	if(m_keyAgreement != nullptr)
		return m_keyAgreement->getPublicKey();

	return Buffer::EMPTY;
}

void LocalSession::addKeyDerivation(DerivationType type, const String &derivation)
{
	Buffer buffer;
	buffer.write(derivation.buffer(), static_cast<uint32>(derivation.buffer_size()));

	return addKeyDerivation(type, buffer);
}

void LocalSession::addKeyDerivation(DerivationType type, const Buffer &derivation)
{
	OS_ASSERT(type != dtNone);
	OS_ASSERT(derivation.empty() == false);

	m_derivationType = type;
	m_derivationKey = derivation;
}

void LocalSession::applyDerivation(DerivationType type)
{
	OS_ASSERT(type != dtNone);

	if(m_derivationType == dtNone)
		return;

	if(m_derivationType == type)
	{
		m_key.deriveKey(m_derivationKey.getData(), m_derivationKey.getSize());
		m_derivationKey.clear();

		m_derivationType = dtNone;
	}
}

shared_ptr<CryptKeyAgreement> LocalSession::initDH()
{
	m_keyAgreement = P2PSystem::instance()->peekKey();
	return m_keyAgreement;
}

shared_ptr<CryptKeyAgreement> LocalSession::initDH(const Buffer &modulus, const Buffer &generator)
{
	m_keyAgreement.reset(OS_NEW CryptKeyAgreement());
	if(m_keyAgreement->init(modulus, generator) == false)
		m_keyAgreement.reset();

	return m_keyAgreement;
}

bool LocalSession::agree(const Buffer &publicKey)
{
	if(m_keyAgreement == nullptr)
		return false;

	return m_keyAgreement->agree(publicKey, m_key);
}

void LocalSession::updateIV()
{
	m_iv++;
}

bool LocalSession::encryptData(void *data, uint32 size, Buffer &encrypted)
{
	OS_ASSERT(size > 0);

	// Aggiorna l'iv
	m_key.generateIV(m_iv);
	// Cripta il buffer del pacchetto con la chiave locale
	if(CryptManager::instance()->aesEncrypt(m_key, data, size, encrypted, false) == false)
		return false;

	encrypted.seekToBegin();
	OS_ASSERT(encrypted.getSize() == size);

	return true;
}

bool LocalSession::decryptData(void *data, uint32 size, Buffer &decrypted)
{
	OS_ASSERT(size > 0);

	// Aggiorna l'iv
	m_key.generateIV(m_iv);
	// Decripta il buffer del pacchetto con la chiave remota
	if(CryptManager::instance()->aesDecrypt(m_key, data, size, decrypted, false) == false)
		return false;

	decrypted.seekToBegin();
	OS_ASSERT(decrypted.getSize() == size);

	return true;
}

bool LocalSession::signKey(const Buffer &privateKey, Buffer &signature)
{
	const Buffer &key = m_key.getKey();
	OS_ASSERT(key.empty() == false);
	// Firma la chiave di sessione con la chiave specificata
	return CryptManager::instance()->rsaSign(privateKey, key.getData(), key.getSize(), signature);
}

//////////////////////////////////////////////////////////////////////

RemoteSession::RemoteSession() : m_address(OS_NEW IPAddress())
{
	m_known = false;
	m_validable = false;
	m_error = false;
}

RemoteSession::~RemoteSession()
{

}

bool RemoteSession::verifySignature(const Buffer &public_key, const CryptKey &key) const
{
	const Buffer &data = key.getKey();
	return CryptManager::instance()->rsaVerify(public_key, data.getData(), data.getSize(), m_signature);
}

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
