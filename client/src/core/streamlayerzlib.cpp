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
#include "streamlayerzlib.h"

#include "crypto++.h"
#include "log.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<StreamLayerZlib>
{
// Construction
public:
	pimpl(CompressionLevel level);
	virtual ~pimpl();

// Operations
public:
	uint32 read(const StreamLayerZlib *layer, void *v, uint32 size) const;
	uint32 write(const StreamLayerZlib *layer, const void *v, uint32 size);
	bool close();

private:
	bool ensureCompressor(const StreamLayerZlib *layer);
	bool ensureDecompressor(const StreamLayerZlib *layer, uint32 &size) const;

private:
	scoped_ptr<CryptoPP::ZlibCompressor> m_compressor;
	mutable scoped_ptr<CryptoPP::ZlibDecompressor> m_decompressor;
	CompressionLevel m_level;
};

//////////////////////////////////////////////////////////////////////

pimpl<StreamLayerZlib>::pimpl(CompressionLevel level) : m_level(level)
{

}

pimpl<StreamLayerZlib>::~pimpl()
{
	close();
}

uint32 pimpl<StreamLayerZlib>::read(const StreamLayerZlib *layer, void *v, uint32 size) const
{
	if(ensureDecompressor(layer, size) == false)
		return 0;

	OS_ASSERT(m_decompressor != null);

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

uint32 pimpl<StreamLayerZlib>::write(const StreamLayerZlib *layer, const void *v, uint32 size)
{
	if(ensureCompressor(layer) == false)
		return 0;

	try
	{
		OS_ASSERT(m_compressor != null);
		m_compressor->Put(static_cast<const byte *>(v), static_cast<unsigned int>(size));			
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
		return 0;
	}

	return size;
}	

bool pimpl<StreamLayerZlib>::close()
{
	try
	{
		if(m_compressor != null)
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

bool pimpl<StreamLayerZlib>::ensureCompressor(const StreamLayerZlib *layer)
{
	OS_ASSERT(layer != null);
	if(m_compressor != null)
		return true;

	try
	{
		shared_ptr<IStreamLayer> nextLayer = layer->getNext();
		if(nextLayer == null)
		{
			OS_ASSERTFALSE();
			return false;
		}

		m_compressor.reset(new CryptoPP::ZlibCompressor(new CryptoPP::StreamLayerSink(nextLayer), CryptoPP::deflateLevel<CryptoPP::ZlibCompressor>(m_level)));
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
		return false;
	}

	OS_ASSERT(m_compressor != null);	
	return true;
}

bool pimpl<StreamLayerZlib>::ensureDecompressor(const StreamLayerZlib *layer, uint32 &size) const
{
	try
	{
		shared_ptr<IStreamLayer> nextLayer = layer->getNext();
		if(nextLayer == null)
		{
			OS_ASSERTFALSE();
			return false;
		}

		if(m_decompressor == null)
			m_decompressor.reset(new CryptoPP::ZlibDecompressor());

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

StreamLayerZlib::StreamLayerZlib(CompressionLevel level) : m_impl(level)												
{

}

StreamLayerZlib::~StreamLayerZlib()
{
	
}

uint32 StreamLayerZlib::read(void *v, uint32 size) const
{
	return m_impl->read(this, v, size);
}

uint32 StreamLayerZlib::write(const void *v, uint32 size)
{
	return m_impl->write(this, v, size);
}

bool StreamLayerZlib::close()
{
	if(m_impl->close() == false)
		return false;

	return LayerBase::close();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
