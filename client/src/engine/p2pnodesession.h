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

#ifndef _OS_P2P_NODESESSION_H
#define _OS_P2P_NODESESSION_H

#include "base/object.h"
#include "buffer.h"
#include "cryptkey.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CryptKeyAgreement;
class IPAddress;
class P2PConnection;

//////////////////////////////////////////////////////////////////////

namespace p2p {

//////////////////////////////////////////////////////////////////////

class EngineExport NodeSession : public Object
{
// Construction
public:
	NodeSession();
	virtual ~NodeSession();

// Attributes
public:
    inline bool getBusy() const;
	inline void setBusy(bool busy);

protected:
	bool m_busy;								// Indica se il nodo remoto è sovraccarico
};

//////////////////////////////////////////////////////////////////////

inline bool NodeSession::getBusy() const { return m_busy; }
inline void NodeSession::setBusy(bool busy) { m_busy = busy; }

//////////////////////////////////////////////////////////////////////

class EngineExport LocalSession : public NodeSession
{
public:
	enum DerivationType
	{
		dtNone					= 0,
		dtNextPacketSended		= 1,
		dtNextPacketReceived	= 2,
	};

// Construction
public:
	LocalSession();
	virtual ~LocalSession();

// Attribute
public:
	inline const CryptKey & getKey() const;

	inline bool getHelloSended() const;
	inline void setHelloSended(bool sended);

	const Buffer & getModulus() const;
	const Buffer & getGenerator() const;
	const Buffer & getPublicKey() const;

// Operations
public:
	void addKeyDerivation(DerivationType type, const String &derivation);
	void addKeyDerivation(DerivationType type, const Buffer &derivation);

	void applyDerivation(DerivationType type);

	shared_ptr<CryptKeyAgreement> initDH();
	shared_ptr<CryptKeyAgreement> initDH(const Buffer &modulus, const Buffer &generator);

	bool agree(const Buffer &publicKey);
	void updateIV();

	// Cripta i dati con la chiave di sessione tramite AES
	bool encryptData(void *data, uint32 size, Buffer &encrypted);
	// Decripta i dati con la chiave di sessione tramite AES
	bool decryptData(void *data, uint32 size, Buffer &decrypted);

	bool signKey(const Buffer &privateKey, Buffer &signature);

protected:
	shared_ptr<CryptKeyAgreement> m_keyAgreement;	// DH
	CryptKey m_key;									// Chiave di sessione con cui criptare i dati
	uint32 m_iv;									// Indice del pacchetto corrente
	DerivationType m_derivationType;				// Tipologia di derivazione della chiave di sessione
	Buffer m_derivationKey;							// Buffer di derivazione della chiave
	bool m_helloSended;
};

//////////////////////////////////////////////////////////////////////

inline const CryptKey & LocalSession::getKey() const { return m_key; }

inline bool LocalSession::getHelloSended() const { return m_helloSended; }
inline void LocalSession::setHelloSended(bool sended) { m_helloSended = sended; }

//////////////////////////////////////////////////////////////////////

class EngineExport RemoteSession : public NodeSession
{
	friend class OS_NAMESPACE_NAME::P2PConnection;

// Construction
public:
	RemoteSession();
	virtual ~RemoteSession();

// Attributes
public:
	inline shared_ptr<IPAddress> getAddress() const;

	inline bool getValidable() const;
	inline void setValidable(bool validable);

	inline bool getError() const;
	inline void setError(bool Error);

// Operations
public:
	bool verifySignature(const Buffer &public_key, const CryptKey &key) const;

protected:
	shared_ptr<IPAddress> m_address;		// Indirizzo del nodo
	Buffer m_id;							// ID del nodo
	Buffer m_signature;						// Firma digitale della chiave di sessione
	bool m_known;							// Indica se il nodo è un nodo conosciuto
	bool m_validable;						// Indica se la connessione remota è validabile
	bool m_error;							// Indica se si è verificato un errore
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IPAddress> RemoteSession::getAddress() const { return m_address; }

inline bool RemoteSession::getValidable() const { return m_validable; }
inline void RemoteSession::setValidable(bool validable) { m_validable = validable; }

inline bool RemoteSession::getError() const { return m_error; }
inline void RemoteSession::setError(bool error) { m_error = error; }

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_P2P_NODESESSION_H
