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

#ifndef _OS_HTTP_HTTPCONTENT_H
#define _OS_HTTP_HTTPCONTENT_H

#include "base/object.h"
#include "http.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;

//////////////////////////////////////////////////////////////////////

class HttpExport HttpContent : public Object
{
// Construction
public:
    HttpContent();
	virtual ~HttpContent();

// Attributes
public:
	inline bool getCompleted() const;
	inline bool getValid() const;

	inline HttpContentEncoding getContentEncoding() const;
	inline void setContentEncoding(HttpContentEncoding contentEncoding);

	inline const std::string & getContentType() const;
	inline void setContentType(const std::string &contentType);

	inline HttpTransferEncoding getTransferEncoding() const;
	inline void setTransferEncoding(HttpTransferEncoding transferEncoding);

	inline uint32 getContentLength() const;
	inline void setContentLength(uint32 contentLength);	

	inline shared_ptr<Buffer> getBufferPtr() const;
	inline shared_ptr<Buffer> getContentPtr() const;

	const Buffer & getContent() const;
	bool empty() const;

// Operations
public:
	void process(const byte *data, uint32 size);

	std::string toString() const;

private:
	void processData(const byte *data, uint32 size);
	void processChunkedData(const byte *data, uint32 size);

	void update(bool completed, bool parse);

private:
	shared_ptr<Buffer> m_buffer;
	shared_ptr<Buffer> m_content;
	HttpContentEncoding m_contentEncoding;
	std::string m_contentType;
	HttpTransferEncoding m_transferEncoding;
	uint32 m_contentLength;
	uint32 m_waitingLength;
	uint32 m_chunkStart;
	uint32 m_chunkSize;	
	bool m_completed;
	bool m_valid;
};

//////////////////////////////////////////////////////////////////////

inline bool HttpContent::getCompleted() const { return m_completed; }
inline bool HttpContent::getValid() const { return m_valid; }

inline HttpContentEncoding HttpContent::getContentEncoding() const { return m_contentEncoding; }
inline void HttpContent::setContentEncoding(HttpContentEncoding contentEncoding) { m_contentEncoding = contentEncoding; }

inline const std::string & HttpContent::getContentType() const { return m_contentType; }
inline void HttpContent::setContentType(const std::string &contentType) { m_contentType = contentType; }

inline HttpTransferEncoding HttpContent::getTransferEncoding() const { return m_transferEncoding; }
inline void HttpContent::setTransferEncoding(HttpTransferEncoding transferEncoding) { m_transferEncoding = transferEncoding; }

inline uint32 HttpContent::getContentLength() const { return m_contentLength; }
inline void HttpContent::setContentLength(uint32 contentLength) { m_contentLength = contentLength; }

inline shared_ptr<Buffer> HttpContent::getBufferPtr() const { return m_buffer; }
inline shared_ptr<Buffer> HttpContent::getContentPtr() const { return m_content; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif //_OSIRIS_HTTP_HTTPCONTENT_H
