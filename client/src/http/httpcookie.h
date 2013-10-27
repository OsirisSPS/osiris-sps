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

#ifndef _OS_HTTP_HTTPCOOKIE_H
#define _OS_HTTP_HTTPCOOKIE_H

#include "base/object.h"
#include "boost/date_time/posix_time/ptime.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HttpExport HttpCookie : public Object
{
// Construction
public:
    HttpCookie(const std::string &name, const std::string &value, const boost::posix_time::ptime &expires, const std::string &domain, const std::string &path, bool secure);
	virtual ~HttpCookie();

// Attributes
public:
	inline const std::string & getName() const;
	inline const std::string & getValue() const;
	inline const boost::posix_time::ptime & getExpires() const;
	inline const std::string & getDomain() const;
	inline const std::string & getPath() const;
	inline bool getSecure() const;

protected:
	std::string m_name;
	std::string m_value;
	boost::posix_time::ptime m_expires;
	std::string m_domain;
	std::string m_path;
	bool m_secure;

public:
	static const boost::posix_time::ptime MAX_EXPIRES_DATE;
};

//////////////////////////////////////////////////////////////////////

inline const std::string & HttpCookie::getName() const { return m_name; }
inline const std::string & HttpCookie::getValue() const { return m_value; }
inline const boost::posix_time::ptime & HttpCookie::getExpires() const { return m_expires; }
inline const std::string & HttpCookie::getDomain() const { return m_domain; }
inline const std::string & HttpCookie::getPath() const { return m_path; }
inline bool HttpCookie::getSecure() const { return m_secure; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif //_OSIRIS_HTTP_HTTPCOOKIE_H
