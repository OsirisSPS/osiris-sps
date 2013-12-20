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
#include "streamlayergzip.h"

#include "crypto++.h"
#include "log.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<StreamLayerGzip>
{
// Construction
public:
	pimpl(CompressionLevel level);
	virtual ~pimpl();

// Operations
public:	
	uint32 read(const StreamLayerGzip *layer, void *v, uint32 size) const;
	uint32 write(const StreamLayerGzip *layer, const void *v, uint32 size);
	bool close();

private:
	bool ensureCompressor(const StreamLayerGzip *layer);
	bool ensureDecompressor(const StreamLayerGzip *layer, uint32 &size) const;
	
private:
	scoped_ptr<CryptoPP::Gzip> m_compressor;
	mutable scoped_ptr<CryptoPP::Gunzip> m_decompressor;
	CompressionLevel m_level;
};

//////////////////////////////////////////////////////////////////////

pimpl<StreamLayerGzip>::pimpl(CompressionLevel level) : m_level(level)
{

}

pimpl<StreamLayerGzip>::~pimpl()
{
	close();
}

uint32 pimpl<StreamLayerGzip>::read(const StreamLayerGzip *layer, void *v, uint32 size) const
{
	if(ensureDecompressor(layer, size) == false)
		return 0;

	OS_ASSERT(m_decompressor != nullptr);

	try
	{
		OS_ASSERT(static_cast<uint32>(m_decompressor->MaxRetrievable()) >= size);		
		m_decompressor->Get(reinterpret_cast<byte *>(v), size);			
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
		return 0;
	}

	return size;
}

uint32 pimpl<StreamLayerGzip>::write(const StreamLayerGzip *layer, const void *v, uint32 size)
{
	if(ensureCompressor(layer) == false)
		return 0;

	OS_ASSERT(m_compressor != nullptr);

	try
	{		
		m_compressor->Put(static_cast<const byte *>(v), static_cast<unsigned int>(size));			
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
		return 0;
	}

	return size;
}	

bool pimpl<StreamLayerGzip>::close()
{
	try
	{
		if(m_compressor != nullptr)
		{
			m_compressor->MessageEnd();
			m_compressor.reset();			
		}		
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
		return false;
	}

	return true;
}

bool pimpl<StreamLayerGzip>::ensureCompressor(const StreamLayerGzip *layer)
{
	OS_ASSERT(layer != nullptr);
	if(m_compressor != nullptr)
		return true;

	try
	{
		shared_ptr<IStreamLayer> nextLayer = layer->getNext();
		if(nextLayer == nullptr)
		{
			OS_ASSERTFALSE();
			return false;
		}

		m_compressor.reset(new CryptoPP::Gzip(new CryptoPP::StreamLayerSink(nextLayer), CryptoPP::deflateLevel<CryptoPP::Gzip>(m_level)));
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
		return false;
	}

	return true;
}

bool pimpl<StreamLayerGzip>::ensureDecompressor(const StreamLayerGzip *layer, uint32 &size) const
{
	try
	{
		shared_ptr<IStreamLayer> nextLayer = layer->getNext();
		if(nextLayer == nullptr)
		{
			OS_ASSERTFALSE();
			return false;
		}

		if(m_decompressor == nullptr)
			m_decompressor.reset(new CryptoPP::Gunzip());

		while(static_cast<uint32>(m_decompressor->MaxRetrievable()) < size)
		{
			byte tmp = 0;
			if(nextLayer->read(&tmp, sizeof(byte)) != sizeof(byte))
			{
				if(nextLayer->available() == 0)
					m_decompressor->MessageEnd();

				size = std::min<uint32>(size, static_cast<uint32>(m_decompressor->MaxRetrievable()));
				return true;
			}
	
			m_decompressor->Put(tmp);
		}		
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
		return false;
	}

	return true;	
}

//////////////////////////////////////////////////////////////////////

StreamLayerGzip::StreamLayerGzip(CompressionLevel level) : m_impl(level)												
{

}

StreamLayerGzip::~StreamLayerGzip()
{
	
}

uint32 StreamLayerGzip::read(void *v, uint32 size) const
{
	return m_impl->read(this, v, size);
}

uint32 StreamLayerGzip::write(const void *v, uint32 size)
{
	return m_impl->write(this, v, size);
}

bool StreamLayerGzip::close()
{
	if(m_impl->close() == false)
		return false;

	return LayerBase::close();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
