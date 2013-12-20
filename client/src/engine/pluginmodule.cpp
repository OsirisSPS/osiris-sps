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
#include "pluginmodule.h"

#include "boost/preprocessor/stringize.hpp"
#include "dynlib.h"
#include "version.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

PluginModule::PluginModule(shared_ptr<Plugin> plugin) : m_plugin(plugin),
														m_interface(nullptr)
{
	OS_ASSERT(plugin != nullptr);
}

PluginModule::~PluginModule()
{	
	unload();
}

bool PluginModule::load(const String &filename)
{
	unload();
	
	shared_ptr<DynLib> lib(OS_NEW DynLib());
	if(lib->load(filename))
	{
		PluginInterfaceCallback callback = reinterpret_cast<PluginInterfaceCallback>(lib->findProcedure(BOOST_PP_STRINGIZE(OS_PLUGIN_ENTRYPOINT)));
		if(callback != nullptr)
		{
			m_interface = callback();
			if(m_interface != nullptr)
			{
				m_lib = lib;
				return true;
			}
		}		
	}

	return false;
}

void PluginModule::unload()
{
	// Non richiamare il metodo unload visto che la dll potrebbe essere referenziata da qualcun altro (nel caso non lo sia il distruttore la dealloca in automatico)	
	m_lib = nullptr;
	m_interface = nullptr;
}

bool PluginModule::initialize()
{
	shared_ptr<Plugin> plugin = getPlugin();
	if(plugin == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	OS_ASSERT(m_interface != nullptr);
	return (m_interface != nullptr && m_interface->initialize != nullptr) ? m_interface->initialize(*plugin) : false;
}

bool PluginModule::terminate()
{
	OS_ASSERT(m_interface != nullptr);
	return (m_interface != nullptr && m_interface->terminate != nullptr) ? m_interface->terminate() : false;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
