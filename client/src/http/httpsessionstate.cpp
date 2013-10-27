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
#include "buffer.h"
#include "cryptmanager.h"
#include "httpsessionstate.h"

#include "lock.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HttpSessionState::HttpSessionState(const String &id, const TimeDuration &duration) : m_id(id),
																					 m_duration(duration)
{
	m_token = CryptManager::instance()->randomSHA().toHex();
}

HttpSessionState::~HttpSessionState()
{

}

bool HttpSessionState::isElapsed() const
{
	OS_LOCK(m_cs);
	return m_elapse < DateTime::now();
}

bool HttpSessionState::hasToken(const std::string token) const
{
	for(StringList::const_iterator i = m_tokens.begin(); i != m_tokens.end(); ++i)
	{
		if((*i) == token)
			return true;
	}
	return false;
}

void HttpSessionState::validate()
{
	OS_LOCK(m_cs);
	m_elapse = DateTime::now() + m_duration;	
}

std::string HttpSessionState::createToken()
{
	std::string newToken = CryptManager::instance()->randomSHA().toHex();
	m_tokens.push_back(newToken);
	for(;m_tokens.size() > 100;)
		m_tokens.pop_front();
	return newToken;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
