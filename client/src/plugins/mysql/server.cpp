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
#include "server.h"

#include "log.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SINGLETON(mysql::Server)

//////////////////////////////////////////////////////////////////////

namespace mysql {

//////////////////////////////////////////////////////////////////////

Server::Server()
{
	OS_ZEROMEMORY(m_base_path, OS_MAX_PATH);
	OS_ZEROMEMORY(m_data_path, OS_MAX_PATH);
}

Server::~Server()
{
	stop();
}

bool Server::start(const String &base_path, const String &data_path)
{
	try
	{
	#ifndef OS_DEBUG
		strcpy(m_base_path, std::string("--basedir=" + base_path.to_ascii()).c_str());
		strcpy(m_data_path, std::string("--datadir=" + data_path.to_ascii()).c_str());

		m_options[0] = "mysql_server";
		m_options[1] = m_base_path;
		m_options[2] = m_data_path;

		return mysql_server_init(OS_COUNTOF(m_options), m_options, null) == MYSQL_OK;
	#else
		return true;
	#endif // OS_DEBUG
	}
	catch(...)
	{
		OS_LOG_ERROR(_S("Cannot start mysql server"));
	}

	return false;
}

void Server::stop()
{
#ifndef OS_DEBUG
	mysql_server_end();
#endif // OS_DEBUG
}

//////////////////////////////////////////////////////////////////////

} // mysql

//////////////////////////////////////////////////////////////////////
