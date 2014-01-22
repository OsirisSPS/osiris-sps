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

#ifndef _OS_CORE_CRYPTOPP_H
#define _OS_CORE_CRYPTOPP_H

#include "buffer.h"
#include "crypt.h"
#include "istreamlayer.h"

//////////////////////////////////////////////////////////////////////

#if OS_COMPILER	== OS_COMPILER_MSVC
	#pragma warning(disable : 4267)		// 'conversion from <type> to <type>, possible loss of data'
#endif

#ifdef IS_LITTLE_ENDIAN
	#undef IS_LITTLE_ENDIAN
#endif

// Disabilita i warnings sugli algoritmi riconosciuti come non sicuri (es: arc4, md2, md4, md5)
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK	1

//////////////////////////////////////////////////////////////////////

#include "crypto++/aes.h"
#include "crypto++/base64.h"
#include "crypto++/blowfish.h"
#include "crypto++/dh.h"
#include "crypto++/files.h"
#include "crypto++/fltrimpl.h"
#include "crypto++/gzip.h"
#include "crypto++/hex.h"
#include "crypto++/hmac.h"
#include "crypto++/md4.h"
#include "crypto++/md5.h"
#include "crypto++/modes.h"
#include "crypto++/osrng.h"
#include "crypto++/randpool.h"
#include "crypto++/rsa.h"
#include "crypto++/sha.h"
#include "crypto++/zlib.h"

//////////////////////////////////////////////////////////////////////

#if OS_COMPILER	== OS_COMPILER_MSVC
	#pragma warning(default : 4267)
#endif

//////////////////////////////////////////////////////////////////////

namespace CryptoPP {

//////////////////////////////////////////////////////////////////////

typedef StringSource BytesSource;

typedef RSAES_OAEP_SHA_Encryptor RSAEncryptor;
typedef RSAES_OAEP_SHA_Decryptor RSADecryptor;

typedef RSASS<PKCS1v15, SHA>::Signer RSASigner;
typedef RSASS<PKCS1v15, SHA>::Verifier RSAVerifier;

//////////////////////////////////////////////////////////////////////

class BufferSink : public Bufferless<Sink>
{
// Construction
public:
	BufferSink(OS_NAMESPACE_NAME::Buffer &buffer) : m_buffer(&buffer) 
	{
	
	}

	virtual ~BufferSink() 
	{
	
	}

// BufferedTransformation Overrides
public:
	virtual size_t Put2(const byte *begin, size_t length, int messageEnd, bool blocking)
	{
		if(length > 0)
		{
			OS_NAMESPACE_NAME::uint32 size = static_cast<OS_NAMESPACE_NAME::uint32>(length);
			if(m_buffer->write(begin, size) != size)
				throw Exception(Exception::IO_ERROR, "buffer write error");
		}
		
		return 0;
	}

private:
	OS_NAMESPACE_NAME::Buffer *m_buffer;
};

//////////////////////////////////////////////////////////////////////

class BufferSource : public BytesSource
{
	typedef BytesSource SourceBase;

// Construction
public:
	BufferSource(OS_NAMESPACE_NAME::Buffer &buffer, bool pumpAll = true) : SourceBase(static_cast<const byte *>(buffer.getData()), static_cast<unsigned int>(buffer.getSize()), pumpAll) 
	{
	
	}

	virtual ~BufferSource() 
	{ 
	
	}
};

//////////////////////////////////////////////////////////////////////

class StreamLayerSink : public Sink
{
// Construction
public:
	StreamLayerSink(OS_NAMESPACE_NAME::shared_ptr<OS_NAMESPACE_NAME::IStreamLayer> layer) : m_layer(layer)
	{
		OS_ASSERT(layer != nullptr);
	}

	virtual ~StreamLayerSink() 
	{
	
	}

// BufferedTransformation Interface
public:
	virtual size_t Put2(const byte *begin, size_t length, int messageEnd, bool blocking)
	{
		if(length > 0)
		{
			if(m_layer->write(begin, static_cast<OS_NAMESPACE_NAME::uint32>(length)) == false)
				throw Exception(Exception::IO_ERROR, "layer write error");
		}
		
		return 0;
	}

	virtual bool IsolatedFlush(bool hardFlush, bool blocking)
	{
		m_layer->flush();

		return false;
	}

private:
	OS_NAMESPACE_NAME::shared_ptr<OS_NAMESPACE_NAME::IStreamLayer> m_layer;
};

//////////////////////////////////////////////////////////////////////

class PEMStripper : public Unflushable<Filter>
{
public:
	PEMStripper(BufferedTransformation *attachment = NULL) : m_dashcount(0)
    {
        Detach(attachment);
    }

    virtual size_t Put2(const byte *begin, size_t length, int messageEnd, bool blocking)
    {
		FILTER_BEGIN;

        while(m_inputPosition < length)
        {
            if(static_cast<char>(begin[m_inputPosition++]) == '-') 
				m_dashcount++;

            if(m_dashcount)
            {
				// Lo standard PEM è composto da 5 '-' + <tag> + 5 '-' es:
				/*
				-----BEGIN PUBLIC KEY-----		
				<public key>
				-----END PUBLIC KEY-----		

				-----BEGIN RSA PRIVATE KEY-----
				<private key>
				-----END RSA PRIVATE KEY-----

				-----BEGIN CERTIFICATE-----
				<certificate key>
				-----END CERTIFICATE-----
				*/

                if(m_dashcount == 10) 
					m_dashcount = 0;

                continue;
            } 
			else 
			{
                FILTER_OUTPUT(1, begin + m_inputPosition - 1, 1, 0);
            }
        }

        if(messageEnd)
        {
            FILTER_OUTPUT(2, 0, 0, messageEnd);
        }

        FILTER_END_NO_MESSAGE_END;
    }

private:
    int m_dashcount;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
inline static unsigned int deflateLevel(OS_NAMESPACE_NAME::CompressionLevel level)
{
	OS_ASSERT(level != OS_NAMESPACE_NAME::compressionLevelNone);

	switch(level)
	{
	case OS_NAMESPACE_NAME::compressionLevelLow:	return T::MIN_DEFLATE_LEVEL;
	case OS_NAMESPACE_NAME::compressionLevelNormal:	return T::DEFAULT_DEFLATE_LEVEL;
	case OS_NAMESPACE_NAME::compressionLevelHigh:	return T::MAX_DEFLATE_LEVEL;

	default:										OS_ASSERTFALSE();
													break;
	}
	
	return T::DEFAULT_DEFLATE_LEVEL;
}

//////////////////////////////////////////////////////////////////////

} // CryptoPP

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_CRYPTOPP_H
