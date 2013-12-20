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
#include "httpcontent.h"

#include "buffer.h"
#include "cryptmanager.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HttpContent::HttpContent() : m_content(OS_NEW Buffer()),
							 m_contentEncoding(httpContentEncodingNone),
							 m_transferEncoding(httpTransferEncodingNone),
							 m_contentLength(0),
							 m_waitingLength(0),
							 m_chunkStart(0),
							 m_chunkSize(0),
							 m_completed(false),
							 m_valid(true)
{

}

HttpContent::~HttpContent()
{

}

const Buffer & HttpContent::getContent() const
{
	OS_ASSERT(m_content != nullptr);
	return *m_content;
}

bool HttpContent::empty() const
{
	OS_ASSERT(m_content != nullptr);
	return m_content->empty();
}

void HttpContent::process(const byte *data, uint32 size)
{
	switch(m_transferEncoding)
	{
	case httpTransferEncodingNone:		processData(data, size);
										break;

	case httpTransferEncodingChunked:	processChunkedData(data, size);
										break;

	default:							OS_ASSERTFALSE();

										update(true, false);
										break;
	}
}

std::string HttpContent::toString() const
{
	std::string str;

	if(empty() == false)
	{
		OS_ASSERT(m_content != nullptr);
		str.assign(reinterpret_cast<const achar *>(m_content->getData()), m_content->getSize());
	}

	return str;
}

void HttpContent::processData(const byte *data, uint32 size)
{
	OS_ASSERT(m_content != nullptr);
	m_content->write(data, size);

	uint32 availableContent = m_content->getSize();
	if(availableContent < m_contentLength)
		m_waitingLength = m_contentLength - availableContent;
	else
		m_waitingLength = 0;

	update(m_waitingLength == 0, true);
}

void HttpContent::processChunkedData(const byte *data, uint32 size)
{
	if(m_buffer == nullptr)
		m_buffer.reset(OS_NEW Buffer());

	OS_ASSERT(m_buffer != nullptr);
	m_buffer->write(data, size);

	const byte *bufferData = m_buffer->getData();
	uint32 bufferSize = m_buffer->getSize();

	while(m_chunkStart < bufferSize)
	{
		if(m_waitingLength > 0)
		{
			uint32 chunkEnd = m_chunkStart + m_chunkSize;
			if(chunkEnd < bufferSize)
			{
				m_waitingLength = 0;
			}
			else
			{
				m_waitingLength = chunkEnd - bufferSize;
			}

			if(m_waitingLength == 0)
			{
				OS_ASSERT(m_content != nullptr);
				m_content->write(bufferData + m_chunkStart, m_chunkSize);

				m_chunkStart += m_chunkSize;
				m_waitingLength = 0;
			}
			else
			{
				update(false, true);
				return;
			}
		}

		bool chunkReceived = false;

		astring chunk;
		for(uint32 i = m_chunkStart; i < bufferSize; i++)
		{
			chunkReceived = false;

			byte data = bufferData[i];
			if(data == '\n')
			{
				m_chunkStart = i + 1;
				chunkReceived = true;

				// N.B.: se il chunk è vuoto procede con la lettura (potrebbero ancora essere disponibili dei dati)
				if(chunk.empty() == false)
					break;
			}
			else
			{
				if(data != '\r')
					chunk += static_cast<achar>(data);
			}
		}

		if(chunkReceived == false || chunk.empty())
		{
			update(false, true);
			return;
		}

		if(sscanf(chunk.c_str(), "%x", &m_chunkSize) == 1)
		{
			if(m_chunkSize == 0)
			{
				update(true, true);
				return;
			}
			else
			{
				m_waitingLength = m_chunkSize;
			}
		}
		else
		{
			OS_ASSERTFALSE();

			update(true, false);
			return;
		}
	}
}

void HttpContent::update(bool completed, bool valid)
{
	OS_ASSERT(m_content != nullptr);

	m_completed = completed;
	m_valid = valid;

	if(m_completed)
	{
		if(m_valid)
		{
			switch(m_contentEncoding)
			{
			case httpContentEncodingNone:
												break;

			case httpContentEncodingGzip:		{
													shared_ptr<Buffer> decompressedContent(OS_NEW Buffer());
													if(CryptManager::instance()->decompress(compressorTypeGzip, m_content->getData(), m_content->getSize(), *decompressedContent))
													{
														m_content = decompressedContent;
													}
													else
													{
														m_valid = false;
														m_content->clear();
													}
												}

												break;

			case httpContentEncodingDeflate:	{
													shared_ptr<Buffer> decompressedContent(OS_NEW Buffer());
													if(CryptManager::instance()->decompress(compressorTypeDeflate, m_content->getData(), m_content->getSize(), *decompressedContent))
													{
														m_content = decompressedContent;
													}
													else
													{
														m_valid = false;
														m_content->clear();
													}
												}

												break;

			default:							OS_ASSERTFALSE();
												break;
			}
		}
		else
		{
			m_content->clear();
		}

		// Resetta il buffer una volta completata la ricezione
		m_buffer.reset();
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
