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

#include "dbdatabasessystem.h"
#include "driver.h"
#include "osirisplugins.h"
#include "plugin.h"
#include "server.h"
#include "thirdpartylibrariesreporter.h"
#include "thirdpartylibrary.h"

/////////////////////////////////////////////////////////////////////////////

OS_MODULE_ENTRY_POINT()

//////////////////////////////////////////////////////////////////////

namespace mysql {

//////////////////////////////////////////////////////////////////////

static bool pluginInitialize(Plugin &plugin)
{
	if(Server::create()->start(plugin.getPath(), OS_MYSQL_DATA) == false)
		return false;

	if(DatabasesSystem::instance()->addDriver(shared_ptr<IDatabaseDriver>(OS_NEW Driver(plugin.getPath()))) == false)
		return false;

	shared_ptr<ThirdPartyLibrary> mysqlLibrary = ThirdPartyLibrariesReporter::instance()->addLibrary();
	OS_ASSERT(mysqlLibrary != nullptr);
	mysqlLibrary->setName("Mysql");
    mysqlLibrary->setUrl("http://www.mysql.com");
    mysqlLibrary->setLicense("GPL"); 

	return true;
}

static bool pluginTerminate()
{
	Server::destroy();
	
	return true;
}

//////////////////////////////////////////////////////////////////////

} // mysql

//////////////////////////////////////////////////////////////////////

OS_PLUGIN_DECLARE(pluginInitialize, pluginTerminate)

//////////////////////////////////////////////////////////////////////
