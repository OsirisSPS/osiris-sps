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
#include "httpurl.h"

#include "algorithms.h"
#include "constants.h"
#include "conversions.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const HttpUrl HttpUrl::EMPTY;

//////////////////////////////////////////////////////////////////////

HttpUrl::HttpUrl()
{
	clear();
}

HttpUrl::HttpUrl(const std::string &url)
{
	parse(url);
}

HttpUrl::HttpUrl(const HttpUrl &second)
{
	*this = second;
}

HttpUrl::~HttpUrl()
{
	
}

bool HttpUrl::empty() const
{
	return m_host.empty();
}

void HttpUrl::clear()
{
	m_username.clear();
	m_password.clear();
	m_host.clear();
	m_port = 0;
	m_path.clear();	
	m_params.clear();
	m_ssl = false;
}

bool HttpUrl::parse(const std::string &url)
{
	clear();

	bool hasProtocol = url.find(OS_HTTP_PROTOCOL_SEPARATOR) != std::string::npos;
	
	std::string protocol;
	if((hasProtocol == false) || algorithms::starts_with(url, OS_HTTP_HTTP_PROTOCOL))
	{
		protocol = OS_HTTP_HTTP_PROTOCOL;
		m_ssl = false;				
	}
	else if(algorithms::starts_with(url, OS_HTTP_HTTPS_PROTOCOL))
	{
		protocol = OS_HTTP_HTTPS_PROTOCOL;
		m_ssl = true;
	}
	else
	{
		return false;
	}

	if(hasProtocol)
		m_host = algorithms::mid(url, protocol.length());
	else
		m_host = url;

	size_t pos = m_host.find_first_of("/");
	if(pos == std::string::npos)
	{
		m_path = OS_HTTP_PATH_DEFAULT;
	}
	else
	{
		m_path = algorithms::mid(m_host, pos);
		m_host = algorithms::left(m_host, pos);		
	}

	pos = m_host.find("@");
	if(pos != std::string::npos)
	{
		std::string authentication = algorithms::left(m_host, pos);
		m_host = algorithms::mid(m_host, pos + 1);

		pos = authentication.find(":");
		if(pos != std::string::npos)
		{
			m_username = algorithms::left(authentication, pos);
			m_password = algorithms::mid(authentication, pos + 1);
		}
		else
		{
			m_username = authentication;
		}
	}

	pos = m_host.rfind(":");
	if(pos != std::string::npos)
	{
		m_port = conversions::from_string<uint16>(algorithms::mid(m_host, pos + 1));
		m_host = algorithms::left(m_host, pos);
	}
	else
	{
		if(m_ssl)
			m_port = OS_HTTP_HTTPS_PORT_DEFAULT;
		else
			m_port = OS_HTTP_HTTP_PORT_DEFAULT;
	}

	pos = m_path.find("?");
	if(pos != std::string::npos)
	{
		m_params = algorithms::mid(m_path, pos + 1);
		m_path = algorithms::left(m_path, pos);
	}	

	return true;
}

std::string HttpUrl::toString() const
{
	if(empty())
		return constants::empty_string;

	std::string result = m_ssl ? OS_HTTP_HTTPS_PROTOCOL : OS_HTTP_HTTP_PROTOCOL;
	
	if(m_username.empty() == false)
	{
		result.append(m_username);
		if(m_password.empty() == false)
		{
			result.append(":");
			result.append(m_password);
		}
		result.append("@");
	}
	
	result.append(m_host);

	uint16 defaultPort = m_ssl ? OS_HTTP_HTTPS_PORT_DEFAULT : OS_HTTP_HTTP_PORT_DEFAULT;
	if(m_port != defaultPort)
	{
		result.append(":");
		result.append(conversions::to_string(m_port));
	}
	
	result.append(m_path);

	if(m_params.empty() == false)
	{
		result.append("?");
		result.append(m_params);
	}

	return result;
}

HttpUrl & HttpUrl::operator=(const HttpUrl &second)
{
	m_username = second.m_username;
	m_password = second.m_password;
	m_host = second.m_host;
	m_port = second.m_port;
	m_path = second.m_path;
	m_params = second.m_params;
	m_ssl = second.m_ssl;

	return *this;
}

bool HttpUrl::operator==(const HttpUrl &second) const
{
	if(m_username != second.m_username)
		return false;

	if(m_password != second.m_password)
		return false;

	if(m_host != second.m_host)
		 return false;

	if(m_port != second.m_port)
		return false;

	if(m_path != second.m_path)
		return false;

	if(m_params != second.m_params)
		return false;

	if(m_ssl != second.m_ssl)
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
