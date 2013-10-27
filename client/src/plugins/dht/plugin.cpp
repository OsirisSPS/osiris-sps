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

#include "dhtmanager.h"
#include "osirisplugins.h"
#include "plugin.h"

//////////////////////////////////////////////////////////////////////

OS_MODULE_ENTRY_POINT()

//////////////////////////////////////////////////////////////////////

using namespace OS_NAMESPACE_NAME;

//////////////////////////////////////////////////////////////////////

shared_ptr<DHTManager> g_dht;

//////////////////////////////////////////////////////////////////////

static bool pluginInitialize(Plugin &plugin)
{
	g_dht.reset(OS_NEW DHTManager(plugin.getDataPath()));
	return g_dht->start();
}

static bool pluginTerminate()
{
	if(g_dht != null)
	{
		g_dht->stop();
		g_dht.reset();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_PLUGIN_DECLARE(pluginInitialize, pluginTerminate)

//////////////////////////////////////////////////////////////////////
