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

#ifndef _MYSQL_SERVER_H
#define _MYSQL_SERVER_H

#include "singleton.h"

//////////////////////////////////////////////////////////////////////

namespace mysql {

//////////////////////////////////////////////////////////////////////

class Server : public DynamicSingleton<Server>
{
// Construction
public:
	Server();
	virtual ~Server();

// Operations
public:
	bool start(const String &base_path, const String &data_path);
	void stop();

private:
	char *m_options[3];
	char m_base_path[OS_MAX_PATH];
	char m_data_path[OS_MAX_PATH];
};

//////////////////////////////////////////////////////////////////////

} // mysql

//////////////////////////////////////////////////////////////////////

#endif // _MYSQL_SERVER_H
