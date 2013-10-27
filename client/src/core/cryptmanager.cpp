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
#include "cryptmanager.h"

#include "crypto++.h"
#include "cryptkey.h"
#include "datetime.h"
#include "istream.h"
#include "log.h"
#include "platformmanager.h"
#include "realtimestats.h"

//////////////////////////////////////////////////////////////////////

// N.B.: questi valori sono estratti da Crypto++ in quanto "interni" e non accessibili (Inflator::MaxPrestreamHeaderSize è virtuale privata, versione Crypto++ 5.5.2)

#define OS_COMPRESSOR_ZLIB_MAX_HEADER_SIZE			2
#define OS_COMPRESSOR_GZIP_MAX_HEADER_SIZE			1024

#define OS_CRYPT_RANDOMBLOCK_SIZE					128

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const uint32 CryptManager::MD5_DIGESTSIZE = CryptoPP::Weak::MD5::DIGESTSIZE;
const uint32 CryptManager::SHA_DIGESTSIZE = CryptoPP::SHA::DIGESTSIZE;
const uint32 CryptManager::SHA256_DIGESTSIZE = CryptoPP::SHA256::DIGESTSIZE;
const uint32 CryptManager::SHA512_DIGESTSIZE = CryptoPP::SHA512::DIGESTSIZE;
const uint32 CryptManager::MD5_HEX_DIGESTSIZE = 2*CryptoPP::Weak::MD5::DIGESTSIZE;
const uint32 CryptManager::SHA_HEX_DIGESTSIZE = 2*CryptoPP::SHA::DIGESTSIZE;
const uint32 CryptManager::SHA256_HEX_DIGESTSIZE = 2*CryptoPP::SHA256::DIGESTSIZE;
const uint32 CryptManager::SHA512_HEX_DIGESTSIZE = 2*CryptoPP::SHA512::DIGESTSIZE;

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<CryptManager>
{
// Construction
public:
	pimpl();
	virtual ~pimpl();

// Attributes
public:
	inline Locked<CryptoPP::AutoSeededRandomPool>::unique getRandomPool() const;

// Operations
public:
	template <typename T>
	bool detectCompressor(const void *data, size_t size) const;
	template <typename T>
	bool detectCompressor(uint32 maxHeaderSize, shared_ptr<IStream> stream) const;

	template <typename T>
	Buffer hash_of(const void *data, size_t size) const;

	void randomBlock(void *data, size_t size) const;

	template <typename T>
	bool cipherEncrypt(const CryptKey &key, const void *data, size_t size, Buffer &encrypted, bool saveHeader) const;

	template <typename T>
	bool cipherDecrypt(const CryptKey &key, const void *data, size_t size, Buffer &decrypted, bool loadHeader) const;

	template <typename T>
	Buffer random_hash() const;

	bool deflateCompress(const void *data, size_t size, Buffer &compressed, CompressionLevel level);
	bool deflateDecompress(const void *data, size_t size, Buffer &decompressed);

	bool zlibCompress(const void *data, size_t size, Buffer &compressed, CompressionLevel level);
	bool zlibDecompress(const void *data, size_t size, Buffer &decompressed);

	bool gzipCompress(const void *data, size_t size, Buffer &compressed, CompressionLevel level);
	bool gzipDecompress(const void *data, size_t size, Buffer &decompressed);

private:
	mutable boost::recursive_mutex m_cs;
	scoped_ptr<CryptoPP::AutoSeededRandomPool> m_randomPool;
};

//////////////////////////////////////////////////////////////////////

pimpl<CryptManager>::pimpl() : m_randomPool(new CryptoPP::AutoSeededRandomPool())
{
	// Nota: crypto++ effettua delle inizializzazioni nel costruttore di AutoSeededRandomPool (in particolare
	// sotto Windows richiama CryptAcquireContext, ma non è thread-safe visto che più thread possono richiamare la funzione
	// randomBlock contemporaneamente in fase di inizializzazione), pertanto viene creato un oggetto AutoSeededRandomPool
	// nel costruttore per effettuare tali inizializzazioni
}

pimpl<CryptManager>::~pimpl()
{

}

inline Locked<CryptoPP::AutoSeededRandomPool>::unique pimpl<CryptManager>::getRandomPool() const
{
	return Locked<CryptoPP::AutoSeededRandomPool>::unique(m_randomPool.get(), m_cs);
}

template <typename T>
bool pimpl<CryptManager>::detectCompressor(const void *data, size_t size) const
{
	OS_ASSERT(data != null);

	try
	{
		T decompressor;
		decompressor.Put(static_cast<const byte *>(data), static_cast<unsigned int>(size));
		decompressor.MessageEnd();
	}
	catch(typename T::UnexpectedEndErr &)
	{
		// In questo caso l'header è stato correttamente processato, ma essendo previsto un body viene lanciata un'eccezione di tipo UnexpectedEndErr
	}
	catch(typename T::Err &)
	{
		return false;
	}

	return true;
}

template <typename T>
bool pimpl<CryptManager>::detectCompressor(uint32 maxHeaderSize, shared_ptr<IStream> stream) const
{
	OS_ASSERT(stream != null);
	OS_ASSERT(maxHeaderSize > 0);

	scoped_array<byte, os_deallocate_t> buffer(OS_ALLOCATE_T(byte, maxHeaderSize));
	uint32 count = stream->peek(buffer.get(), maxHeaderSize);
	if(count == 0)
	{
		OS_ASSERTFALSE();
		return false;
	}

	return detectCompressor<T>(buffer.get(), count);
}

template <typename T>
Buffer pimpl<CryptManager>::hash_of(const void *data, size_t size) const
{
	Buffer buffer(T::DIGESTSIZE);
	T transformer;
	CryptoPP::BytesSource(static_cast<const byte *>(data), static_cast<unsigned int>(size), true, new CryptoPP::HashFilter(transformer, new CryptoPP::BufferSink(buffer)));

	return buffer;
}

void pimpl<CryptManager>::randomBlock(void *data, size_t size) const
{
    OS_ASSERT(data != null);

	OS_LOCK(m_cs);
    m_randomPool->GenerateBlock(reinterpret_cast<byte *>(data), static_cast<unsigned int>(size));
}

template <typename T>
bool pimpl<CryptManager>::cipherEncrypt(const CryptKey &key, const void *data, size_t size, Buffer &encrypted, bool saveHeader) const
{
	try
	{
		OS_EXCEPT_IF(key.getIV().empty() == false && key.getIV().getSize() != static_cast<uint32>(T::BLOCKSIZE), "Invalid iv length");

		// Nota: è stata scelta la modalità CTR perchè consente un IV nullo (in quel caso il contatore è considerato 0),
		// se si cambia (si usa ad esempio il CBC) bisogna necessariamente avere in IV valido

		typename CryptoPP::CTR_Mode<T>::Encryption encryptor(key.getKey().getData(), key.getKey().getSize(), key.getIV().getData());

		encrypted.clear();
		CryptoPP::StreamTransformationFilter filter(encryptor, new CryptoPP::BufferSink(encrypted));

		if(saveHeader)
		{
			// Genera l'HMAC
			Buffer header = key.generateHMAC(data, static_cast<uint32>(size));
			// Salva l'HMAC come header del blocco
			filter.Put(header.getData(), header.getSize());
		}

		filter.Put(static_cast<const byte *>(data), static_cast<unsigned int>(size));
		filter.MessageEnd();

		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
	}

	return false;
}

template <typename T>
bool pimpl<CryptManager>::cipherDecrypt(const CryptKey &key, const void *data, size_t size, Buffer &decrypted, bool loadHeader) const
{
	try
	{
		OS_EXCEPT_IF(key.getIV().empty() == false && key.getIV().getSize() != static_cast<uint32>(T::BLOCKSIZE), "Invalid iv length");

		typename CryptoPP::CTR_Mode<T>::Decryption decryptor(key.getKey().getData(), key.getKey().getSize(), key.getIV().getData());

		decrypted.clear();
		CryptoPP::BytesSource source(static_cast<const byte *>(data), static_cast<unsigned int>(size), false, new CryptoPP::StreamTransformationFilter(decryptor, new CryptoPP::BufferSink(decrypted)));

		scoped_ptr<Buffer> header;
		if(loadHeader)
		{
			// Estra dal blocco criptato l'header
			source.Pump(CryptKey::getHMACSize());
			OS_ASSERT(decrypted.getSize() == CryptKey::getHMACSize());

			// Salva l'header resettando il blocco decriptato (l'header non va restituito)
			header.reset(OS_NEW Buffer(decrypted));
			decrypted.clear();
		}

		source.PumpAll();

		if(loadHeader)
		{
			// Genera l'hmac sul blocco decriptato e verifica che coincida con l'header
			if(key.generateHMAC(decrypted.getData(), decrypted.getSize()) != *header)
			{
				decrypted.clear();
				return false;
			}
		}

		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
	}

	return false;
}

template <typename T>
Buffer pimpl<CryptManager>::random_hash() const
{
	Buffer buffer(OS_CRYPT_RANDOMBLOCK_SIZE);
	buffer.reserve(OS_CRYPT_RANDOMBLOCK_SIZE);
	randomBlock(buffer.getData(), buffer.getSize());

	return hash_of<T>(buffer.getData(), buffer.getSize());
}

bool pimpl<CryptManager>::deflateCompress(const void *data, size_t size, Buffer &compressed, CompressionLevel level)
{
	try
	{
		compressed.clear();

		CryptoPP::Deflator compressor(null, CryptoPP::deflateLevel<CryptoPP::Deflator>(level));
		compressor.Put(static_cast<const byte *>(data), static_cast<unsigned int>(size));
		compressor.MessageEnd();

		compressed.grow(static_cast<uint32>(compressor.MaxRetrievable()));
		compressor.Get(compressed.getData(), compressed.getSize());

		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
	}

	return false;
}

bool pimpl<CryptManager>::deflateDecompress(const void *data, size_t size, Buffer &decompressed)
{
	try
	{
		decompressed.clear();

		CryptoPP::Inflator decompressor;
		decompressor.Put(static_cast<const byte *>(data), static_cast<unsigned int>(size));
		decompressor.MessageEnd();

		decompressed.grow(static_cast<uint32>(decompressor.MaxRetrievable()));
		decompressor.Get(decompressed.getData(), decompressed.getSize());

		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
	}

	return false;
}

bool pimpl<CryptManager>::zlibCompress(const void *data, size_t size, Buffer &compressed, CompressionLevel level)
{
	try
	{
		compressed.clear();

		CryptoPP::ZlibCompressor compressor(null, CryptoPP::deflateLevel<CryptoPP::ZlibCompressor>(level));
		compressor.Put(static_cast<const byte *>(data), static_cast<unsigned int>(size));
		compressor.MessageEnd();

		compressed.grow(static_cast<uint32>(compressor.MaxRetrievable()));
		compressor.Get(compressed.getData(), compressed.getSize());

		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
	}

	return false;
}

bool pimpl<CryptManager>::zlibDecompress(const void *data, size_t size, Buffer &decompressed)
{
	try
	{
		decompressed.clear();

		CryptoPP::ZlibDecompressor decompressor;
		decompressor.Put(static_cast<const byte *>(data), static_cast<unsigned int>(size));
		decompressor.MessageEnd();

		decompressed.grow(static_cast<uint32>(decompressor.MaxRetrievable()));
		decompressor.Get(decompressed.getData(), decompressed.getSize());

		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
	}

	return false;
}

bool pimpl<CryptManager>::gzipCompress(const void *data, size_t size, Buffer &compressed, CompressionLevel level)
{
	static boost::recursive_mutex m_CS; // TOFIX, no multithread.
	OS_LOCK(m_CS);
	
	try
	{
		compressed.clear();

		CryptoPP::Gzip compressor(new CryptoPP::BufferSink(compressed), CryptoPP::deflateLevel<CryptoPP::Gzip>(level));
		compressor.Put(static_cast<const byte *>(data), static_cast<unsigned int>(size));
		compressor.MessageEnd();

		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
	}

	return false;
}

bool pimpl<CryptManager>::gzipDecompress(const void *data, size_t size, Buffer &decompressed)
{
	try
	{
		decompressed.clear();

		CryptoPP::BytesSource source(static_cast<const byte *>(data), static_cast<unsigned int>(size), true, new CryptoPP::Gunzip(new CryptoPP::BufferSink(decompressed)));

		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
	}

	return false;
}

//////////////////////////////////////////////////////////////////////

CryptManager::CryptManager()
{

}

CryptManager::~CryptManager()
{

}

Locked<CryptoPP::AutoSeededRandomPool>::unique CryptManager::getRandomPool() const
{
	return m_impl->getRandomPool();
}

uint32 CryptManager::CRC32(const void *data, size_t size) const
{
	OS_ASSERT(data != null);
	OS_ASSERT(size > 0);

	if((data == null) || (size == 0))
	{
		OS_ASSERTFALSE();
		return 0;	// TODO: corretto?
	}

	uint32 value = 0;

	CryptoPP::CRC32 crc;
	OS_STATIC_ASSERT(CryptoPP::CRC32::DIGESTSIZE == sizeof(uint32));

	CryptoPP::SecByteBlock digest(crc.DigestSize());

	crc.Update(static_cast<const byte *>(data), size);
	crc.Final(digest);
	memcpy(&value, digest, sizeof(uint32));

	return value;
}

Buffer CryptManager::randomMD5() const
{
	return m_impl->random_hash<CryptoPP::Weak::MD5>();
}

Buffer CryptManager::randomSHA() const
{
	return m_impl->random_hash<CryptoPP::SHA>();
}

Buffer CryptManager::MD5(const void *data, size_t size) const
{
	return m_impl->hash_of<CryptoPP::Weak::MD5>(data, size);
}

Buffer CryptManager::SHA(const void *data, size_t size) const
{
	return m_impl->hash_of<CryptoPP::SHA>(data, size);
}

Buffer CryptManager::SHA256(const void *data, size_t size) const
{
	return m_impl->hash_of<CryptoPP::SHA256>(data, size);
}

Buffer CryptManager::SHA512(const void *data, size_t size) const
{
	return m_impl->hash_of<CryptoPP::SHA512>(data, size);
}

bool CryptManager::aesEncrypt(const CryptKey &key, const void *data, size_t size, Buffer &encrypted, bool saveHeader)
{
	return m_impl->cipherEncrypt<CryptoPP::AES>(key, data, size, encrypted, saveHeader);
}

bool CryptManager::aesDecrypt(const CryptKey &key, const void *data, size_t size, Buffer &decrypted, bool loadHeader)
{
	return m_impl->cipherDecrypt<CryptoPP::AES>(key, data, size, decrypted, loadHeader);
}

bool CryptManager::rsaGenerateKeys(RsaType type, Buffer &privateKey, Buffer &publicKey)
{
	bool result = false;

	try
	{
		privateKey.clear();
		scoped_ptr<CryptoPP::BufferSink> privateKeySink(new CryptoPP::BufferSink(privateKey));

		publicKey.clear();
		scoped_ptr<CryptoPP::BufferSink> publicKeySink(new CryptoPP::BufferSink(publicKey));

		Locked<CryptoPP::AutoSeededRandomPool>::unique randomPool = getRandomPool();

		CryptoPP::RSADecryptor decryptor(*randomPool, static_cast<unsigned int>(type));
		decryptor.DEREncode(*privateKeySink);
		privateKeySink->MessageEnd();

		CryptoPP::RSAEncryptor encryptor(decryptor);
		encryptor.DEREncode(*publicKeySink);
		publicKeySink->MessageEnd();

		result = true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
	}

	return result;
}

bool CryptManager::rsaGetPublicKey(const Buffer &privateKey, Buffer &publicKey)
{
	bool result = false;

	try
	{
		publicKey.clear();
		scoped_ptr<CryptoPP::BufferSink> publicKeySink(new CryptoPP::BufferSink(publicKey));

		scoped_ptr<CryptoPP::Source> privateKeySource(new CryptoPP::BytesSource(privateKey.getData(), privateKey.getSize(), true));
		CryptoPP::RSADecryptor decryptor(*privateKeySource);

		CryptoPP::RSAEncryptor encryptor(decryptor);
		encryptor.DEREncode(*publicKeySink);
		publicKeySink->MessageEnd();

		result = true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
	}

	return result;
}

bool CryptManager::rsaEncrypt(const Buffer &public_key, const void *data, size_t size, Buffer &encrypted)
{
	return _rsaEncrypt(public_key, data, size, encrypted, false);
}

bool CryptManager::rsaDecrypt(const Buffer &private_key, const void *data, size_t size, Buffer &decrypted)
{
	return _rsaDecrypt(private_key, data, size, decrypted, false);
}

bool CryptManager::rrsaEncrypt(const Buffer &private_key, const void *data, size_t size, Buffer &encrypted)
{
	return _rsaEncrypt(private_key, data, size, encrypted, true);
}

bool CryptManager::rrsaDecrypt(const Buffer &public_key, const void *data, size_t size, Buffer &decrypted)
{
	return _rsaDecrypt(public_key, data, size, decrypted, true);
}

bool CryptManager::rsaSign(const Buffer &private_key, const void *data, size_t size, Buffer &signature)
{
	RealtimeStatsScopeTimer RSS(_S("Activity"), _S("RSA Signature"));

	bool result = false;

	try
	{
		scoped_ptr<CryptoPP::Source> privateKeySource(new CryptoPP::BytesSource(private_key.getData(), private_key.getSize(), true));
		CryptoPP::RSASigner signer(*privateKeySource);

		signature.clear();
		Locked<CryptoPP::AutoSeededRandomPool>::unique randomPool = getRandomPool();
		CryptoPP::BytesSource(reinterpret_cast<const byte *>(data), static_cast<unsigned int>(size), true, new CryptoPP::SignerFilter(*randomPool, signer, new CryptoPP::BufferSink(signature)));

		result = true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
	}

	return result;
}

bool CryptManager::rsaVerify(const Buffer &public_key, const void *data, size_t size, const Buffer &signature)
{
	bool result = false;

	try
	{
		scoped_ptr<CryptoPP::Source> publicKeySource(new CryptoPP::BytesSource(public_key.getData(), public_key.getSize(), true));

		CryptoPP::RSAVerifier verifier(*publicKeySource);
		CryptoPP::BytesSource signatureString(signature.getData(), signature.getSize(), true);

		if(signatureString.MaxRetrievable() != verifier.SignatureLength())
		{
			result = false;
		}
		else
		{
			CryptoPP::SecByteBlock signatureBlock(verifier.SignatureLength());

			signatureString.Get(signatureBlock, static_cast<unsigned int>(signatureBlock.size()));

			CryptoPP::VerifierFilter *pVerifierFilter = new CryptoPP::VerifierFilter(verifier);
			pVerifierFilter->Put(signatureBlock, verifier.SignatureLength());
			CryptoPP::BytesSource source(reinterpret_cast<const byte *>(data), static_cast<unsigned int>(size), true, pVerifierFilter); // Creare una variabile temporanea per tenere pVerifierFilter valido all'interno dello scope
			result = pVerifierFilter->GetLastResult();
		}
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
	}

	return result;
}

bool CryptManager::rsaCheckKeys(const Buffer &privateKey, const Buffer &publicKey)
{
	// TODO: per effettuare il controllo viene ora generata e controllata la firma utilizzando
	// le chiavi specificate, bisognerebbe però vedere se esiste un metodo più veloce e "ortodosso"...

	static const String checker = _S("checker");

	Buffer signature;
	if(rsaSign(privateKey, checker.buffer(), checker.buffer_size(), signature) == false)
		return false;

	return rsaVerify(publicKey, checker.buffer(), checker.buffer_size(), signature);
}

CompressorType CryptManager::detectCompressor(const void *data, size_t size)
{
	if(data == null)
	{
		OS_ASSERTFALSE();
		return compressorTypeUnknown;
	}

	// CryptoPP::Inflator è headerless e quindi non può essere determinato

	if(m_impl->detectCompressor<CryptoPP::ZlibDecompressor>(data, size))
		return compressorTypeZlib;

	if(m_impl->detectCompressor<CryptoPP::Gunzip>(data, size))
		return compressorTypeGzip;

	return compressorTypeUnknown;
}

CompressorType CryptManager::detectCompressor(shared_ptr<IStream> stream)
{
	if(stream == null)
	{
		OS_ASSERTFALSE();
		return compressorTypeUnknown;
	}

	// CryptoPP::Inflator è headerless e quindi non può essere determinato

	if(m_impl->detectCompressor<CryptoPP::ZlibDecompressor>(OS_COMPRESSOR_ZLIB_MAX_HEADER_SIZE, stream))
		return compressorTypeZlib;

	if(m_impl->detectCompressor<CryptoPP::Gunzip>(OS_COMPRESSOR_GZIP_MAX_HEADER_SIZE, stream))
		return compressorTypeGzip;

	return compressorTypeUnknown;
}

bool CryptManager::matchCompressor(const void *data, size_t size, CompressorType compressor)
{
	if(data == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	switch(compressor)
	{
	case compressorTypeDeflate:		// Headerless
									break;

	case compressorTypeZlib:		return m_impl->detectCompressor<CryptoPP::ZlibDecompressor>(data, size);

	case compressorTypeGzip:		return m_impl->detectCompressor<CryptoPP::Gunzip>(data, size);

	default:						OS_ASSERTFALSE();
									break;
	}

	return false;
}

bool CryptManager::matchCompressor(shared_ptr<IStream> stream, CompressorType compressor)
{
	if(stream == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	switch(compressor)
	{
	case compressorTypeDeflate:		// Headerless
									break;

	case compressorTypeZlib:		return m_impl->detectCompressor<CryptoPP::ZlibDecompressor>(OS_COMPRESSOR_ZLIB_MAX_HEADER_SIZE, stream);

	case compressorTypeGzip:		return m_impl->detectCompressor<CryptoPP::Gunzip>(OS_COMPRESSOR_GZIP_MAX_HEADER_SIZE, stream);

	default:						OS_ASSERTFALSE();
									break;
	}

	return false;
}

bool CryptManager::compress(CompressorType compressor, const void *data, size_t size, Buffer &compressed, CompressionLevel level)
{
	switch(compressor)
	{
	case compressorTypeDeflate:		return m_impl->deflateCompress(data, size, compressed, level);

	case compressorTypeZlib:		return m_impl->zlibCompress(data, size, compressed, level);

	case compressorTypeGzip:		return m_impl->gzipCompress(data, size, compressed, level);

	default:						OS_ASSERTFALSE();
									break;
	}

	return false;
}

bool CryptManager::decompress(CompressorType decompressor, const void *data, size_t size, Buffer &decompressed)
{
	switch(decompressor)
	{
	case compressorTypeDeflate:		return m_impl->deflateDecompress(data, size, decompressed);

	case compressorTypeZlib:		return m_impl->zlibDecompress(data, size, decompressed);

	case compressorTypeGzip:		return m_impl->gzipDecompress(data, size, decompressed);

	default:						OS_ASSERTFALSE();
									break;
	}

	return false;
}

bool CryptManager::encodeBase64(const void *data, size_t size, String &encoded)
{
    encoded.clear();

    std::string tmp;
    if(encodeBase64(data, size, tmp) == false)
        return false;

    encoded = tmp;
    return true;
}

bool CryptManager::encodeBase64(const void *data, size_t size, std::string &encoded)
{
   try
	{
		encoded.clear();
		CryptoPP::BytesSource source(reinterpret_cast<const byte *>(data), static_cast<unsigned int>(size), true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(encoded), false));

		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
    }
    catch(...)
    {
        OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
    }

	return false;
}

bool CryptManager::decodeBase64(const std::string &input, Buffer &output)
{
	try
	{
		output.clear();
		CryptoPP::StringSource source(input, true, new CryptoPP::Base64Decoder(new CryptoPP::BufferSink(output)));
		output.seekToBegin();

		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
    }
    catch(...)
    {
        OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
    }

	return false;
}

bool CryptManager::encodeHex(const void *data, size_t size, String &encoded)
{
    encoded.clear();

    std::string tmp;
    if(encodeHex(data, size, tmp) == false)
        return false;

    encoded = tmp;
    return true;
}

bool CryptManager::encodeHex(const void *data, size_t size, std::string &encoded)
{
	try
	{
		encoded.clear();
		CryptoPP::BytesSource source(reinterpret_cast<const byte *>(data), static_cast<unsigned int>(size), true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(encoded)));

		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
    }
    catch(...)
    {
        OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
    }

	return false;
}

bool CryptManager::decodeHex(const std::string &encoded, Buffer &decoded)
{
	try
	{
		decoded.clear();
		CryptoPP::StringSource source(encoded, true, new CryptoPP::HexDecoder(new CryptoPP::BufferSink(decoded)));
		decoded.seekToBegin();

		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
    }
    catch(...)
    {
        OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
    }

	return false;
}

void CryptManager::randomBlock(void *data, size_t size) const
{
	m_impl->randomBlock(data, size);
}

bool CryptManager::_rsaEncrypt(const Buffer &key, const void *data, size_t size, Buffer &encrypted, bool reversed)
{
	bool result = false;

	try
	{
		scoped_ptr<CryptoPP::Source> keySource(new CryptoPP::BytesSource(key.getData(), key.getSize(), true));

		scoped_ptr<CryptoPP::PK_Encryptor> encryptor;
		if(reversed)
			encryptor.reset(new CryptoPP::RSAReverseEncryptor(*keySource));
		else
			encryptor.reset(new CryptoPP::RSAEncryptor(*keySource));

		encrypted.clear();
		Locked<CryptoPP::AutoSeededRandomPool>::unique randomPool = getRandomPool();
		CryptoPP::BytesSource(reinterpret_cast<const byte *>(data), static_cast<unsigned int>(size), true, new CryptoPP::PK_EncryptorFilter(*randomPool, *encryptor, new CryptoPP::BufferSink(encrypted)));

		result = true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
	}

	return result;
}

bool CryptManager::_rsaDecrypt(const Buffer &key, const void *data, size_t size, Buffer &decrypted, bool reversed)
{
	bool result = false;

	try
	{
		scoped_ptr<CryptoPP::Source> keySource(new CryptoPP::BytesSource(key.getData(), key.getSize(), true));

		scoped_ptr<CryptoPP::PK_Decryptor> decryptor;
		if(reversed)
			decryptor.reset(new CryptoPP::RSAReverseDecryptor(*keySource));
		else
			decryptor.reset(new CryptoPP::RSADecryptor(*keySource));

		decrypted.clear();
		Locked<CryptoPP::AutoSeededRandomPool>::unique randomPool = getRandomPool();
		CryptoPP::BytesSource(reinterpret_cast<const byte *>(data), static_cast<unsigned int>(size), true, new CryptoPP::PK_DecryptorFilter(*randomPool, *decryptor, new CryptoPP::BufferSink(decrypted)));

		result = true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(crypto));
	}

	return result;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
