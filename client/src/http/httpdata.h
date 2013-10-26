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

#ifndef _OS_HTTP_HTTPDATA_H
#define _OS_HTTP_HTTPDATA_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;

//////////////////////////////////////////////////////////////////////

class HttpExport HttpData : public Object
{
// Construction
public:
    HttpData();
	virtual ~HttpData();

// Attributes
public:
	const byte * getData() const;
	uint32 getSize() const;

// Operations
public:
	void writeHeaderLine(const std::string &line);
	void writeHeaderParam(const std::string &paramName, const std::string &paramValue, bool check = true);	
	void writeHeaderLineTerminator();
	
	void write(const std::string &str);
	void write(const Buffer &buffer);
	void write(const byte *data, uint32 size);

private:
	scoped_ptr<Buffer> m_data;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif //_OSIRIS_HTTP_HTTPDATA_H
