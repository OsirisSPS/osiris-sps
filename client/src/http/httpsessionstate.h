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

#ifndef _OS_HTTP_HTTPSESSIONSTATE_H
#define _OS_HTTP_HTTPSESSIONSTATE_H

#include "base/object.h"
#include "boost/thread/recursive_mutex.hpp"
#include "collections.h"
#include "datetime.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HttpExport HttpSessionState : public Object
{
// Construction
public:
    HttpSessionState(const String &id, const TimeDuration &duration);
	virtual ~HttpSessionState();

// Attributes
public:
	inline const String & getID() const;

	inline const String & get(const String &key) const;
	inline void set(const String &key, const String &value);

	inline const TimeDuration & getDuration() const;
	inline void setDuration(const TimeDuration &duration);

	bool isElapsed() const;

	inline std::string getToken() const;
	bool hasToken(const std::string token) const;

// Operations
public:
	void validate();

	// We use token for checking that different url calling are done in sequence.
	// For example, avoid an url that install an extension without pass throught confirm dialog.
	std::string createToken();

protected:
	mutable boost::recursive_mutex m_cs;
	String m_id;
	StringCollection<String> m_data;
	DateTime m_elapse;
	TimeDuration m_duration;

	// Session have an unique, random hash. It's used for validation between http request, to avoid an injected url that, for example, set an option.
	std::string m_token;
	// Another method for validation between http request. For example, it's used to avoid url that request extension install, without real user confirmation.
	StringList m_tokens;
};

//////////////////////////////////////////////////////////////////////

inline const String & HttpSessionState::getID() const { return m_id; }

inline const String & HttpSessionState::get(const String &key) const { return m_data.get(key); }
inline void HttpSessionState::set(const String &key, const String &value) { m_data.set(key, value); }

inline const TimeDuration & HttpSessionState::getDuration() const { return m_duration; }
inline void HttpSessionState::setDuration(const TimeDuration &duration) { m_duration = duration; }

inline std::string HttpSessionState::getToken() const { return m_token; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif //_OSIRIS_HTTP_HTTPSESSIONSTATE_H
