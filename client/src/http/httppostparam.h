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

#ifndef _OS_HTTP_HTTPPOSTPARAM_H
#define _OS_HTTP_HTTPPOSTPARAM_H

#include "base/object.h"
#include "boost/none.hpp"
#include "boost/variant.hpp"
#include "buffer.h"
#include "collections.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HttpExport HttpPostParam : public Object
{
// Construction
public:
    HttpPostParam();
	virtual ~HttpPostParam();

// Attributes
public:
	inline bool empty() const;

	inline bool isBuffer() const;
	inline bool isString() const;

	bool isTextPlain() const;

	const std::wstring & getString() const;
	void setString(const std::wstring &value);

	const Buffer * getBuffer() const;
	void setBuffer(const Buffer &value);
	
	inline const std::string & getContentType() const;
	inline void setContentType(const std::string &contentType);

	inline const std::string & getContentDisposition() const;
	inline void setContentDisposition(const std::string &contentDisposition);

	inline const std::wstring & getAttribute(const std::wstring &name) const;
	inline void setAttribute(const std::wstring &name, const std::wstring &value);

	const std::wstring & getName() const;
	void setName(const std::wstring &name);

	const std::wstring & getFilename() const;
	void setFilename(const std::wstring &filename);

// Operations
public:
	void clear();

private:
	boost::variant<boost::none_t, std::wstring, Buffer> m_value;
	std::string m_contentType;
	std::string m_contentDisposition;
	StringCollection<std::wstring> m_attributes;

public:
	static const std::wstring NAME;
	static const std::wstring FILENAME;
};

//////////////////////////////////////////////////////////////////////

inline bool HttpPostParam::empty() const { return boost::get<boost::none_t>(&m_value) != null; }

inline bool HttpPostParam::isBuffer() const { return boost::get<Buffer>(&m_value) != null; }
inline bool HttpPostParam::isString() const { return boost::get<std::wstring>(&m_value) != null; }

inline const std::string & HttpPostParam::getContentType() const { return m_contentType; }
inline void HttpPostParam::setContentType(const std::string &contentType) { m_contentType = contentType; }

inline const std::string & HttpPostParam::getContentDisposition() const { return m_contentDisposition; }
inline void HttpPostParam::setContentDisposition(const std::string &contentDisposition) { m_contentDisposition = contentDisposition; }

inline const std::wstring & HttpPostParam::getAttribute(const std::wstring &name) const { return m_attributes.get(name); }
inline void HttpPostParam::setAttribute(const std::wstring &name, const std::wstring &value) { m_attributes.set(name, value); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif //_OSIRIS_HTTP_HTTPPOSTPARAM_H
