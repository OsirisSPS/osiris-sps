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

#ifndef _OS_CORE_CRYPTMANAGER_H
#define _OS_CORE_CRYPTMANAGER_H

#include "crypt.h"
#include "locked.h"
#include "pimpl_ptr.h"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;
class CryptKey;
class IStream;

//////////////////////////////////////////////////////////////////////

class CoreExport CryptManager : public StaticSingleton<CryptManager>
{
// Construction
protected:
	CryptManager();

public:
	virtual ~CryptManager();

// Attributes
public:
	Locked<CryptoPP::AutoSeededRandomPool>::unique getRandomPool() const;

// Operations
public:
	uint32 CRC32(const void *data, size_t size) const;

	Buffer MD5(const void *data, size_t size) const;
	Buffer SHA(const void *data, size_t size) const;
	Buffer SHA256(const void *data, size_t size) const;
	Buffer SHA512(const void *data, size_t size) const;

	Buffer randomMD5() const;
	Buffer randomSHA() const;

	bool aesEncrypt(const CryptKey &key, const void *data, size_t size, Buffer &encrypted, bool saveHeader = true);
	bool aesDecrypt(const CryptKey &key, const void *data, size_t size, Buffer &decrypted, bool loadHeader = true);

	// Genera una coppia di chiavi
	bool rsaGenerateKeys(RsaType type, Buffer &privateKey, Buffer &publicKey);
	// Genera la chiave pubblica sulla base di una privata
	bool rsaGetPublicKey(const Buffer &privateKey, Buffer &publicKey);
	// Cripta con l'rsa
	bool rsaEncrypt(const Buffer &public_key, const void *data, size_t size, Buffer &encrypted);
	// Decripta con l'rsa
	bool rsaDecrypt(const Buffer &private_key, const void *data, size_t size, Buffer &decrypted);
	// Cripta con l'rsa inversa
	bool rrsaEncrypt(const Buffer &private_key, const void *data, size_t size, Buffer &encrypted);
	// Decripta con l'rsa inversa
	bool rrsaDecrypt(const Buffer &public_key, const void *data, size_t size, Buffer &decrypted);

	// Firma un blocco con la chiave privata specificata
	bool rsaSign(const Buffer &private_key, const void *data, size_t size, Buffer &signature);
	// Indica se il blocco è stato firmato con la chiave privata relativa a quella pubblica specificata
	bool rsaVerify(const Buffer &public_key, const void *data, size_t size, const Buffer &signature);
	// Indica se le due chiavi corrispondono
	bool rsaCheckKeys(const Buffer &privateKey, const Buffer &publicKey);

	CompressorType detectCompressor(const void *data, size_t size);
	CompressorType detectCompressor(shared_ptr<IStream> stream);

	bool matchCompressor(const void *data, size_t size, CompressorType compressor);
	bool matchCompressor(shared_ptr<IStream> stream, CompressorType compressor);

	bool compress(CompressorType compressor, const void *data, size_t size, Buffer &compressed, CompressionLevel level = compressionLevelHigh);
	bool decompress(CompressorType decompressor, const void *data, size_t size, Buffer &decompressed);

	bool encodeBase64(const void *data, size_t size, String &encoded);
	bool encodeBase64(const void *data, size_t size, std::string &encoded);
	bool decodeBase64(const std::string &input, Buffer &output);

	bool encodeHex(const void *data, size_t size, String &encoded);
	bool encodeHex(const void *data, size_t size, std::string &encoded);
	bool decodeHex(const std::string &encoded, Buffer &decoded);

	void randomBlock(void *data, size_t size) const;

private:
	// Se reversed = true -> key = privateKey, reversed = false -> key = publicKey
	bool _rsaEncrypt(const Buffer &key, const void *data, size_t size, Buffer &encrypted, bool reversed);
	// Se reversed = true -> key = publicKey, reversed = false -> key = privateKey
	bool _rsaDecrypt(const Buffer &key, const void *data, size_t size, Buffer &decrypted, bool reversed);

public:
	static const uint32 MD5_DIGESTSIZE;
	static const uint32 SHA_DIGESTSIZE;
	static const uint32 SHA256_DIGESTSIZE;
	static const uint32 SHA512_DIGESTSIZE;
	static const uint32 MD5_HEX_DIGESTSIZE;
	static const uint32 SHA_HEX_DIGESTSIZE;
	static const uint32 SHA256_HEX_DIGESTSIZE;
	static const uint32 SHA512_HEX_DIGESTSIZE;

private:
	pimpl_of<CryptManager>::type m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_CRYPTMANAGER_H
