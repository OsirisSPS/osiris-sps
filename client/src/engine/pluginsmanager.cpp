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
#include "pluginsmanager.h"

#include "engine.h"
#include "filesystem.h"
#include "lock.h"
#include "log.h"
#include "options.h"
#include "platformmanager.h"
#include "plugin.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SYSTEM(OS_NAMESPACE_NAME::PluginsManager)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

PluginsManager::PluginsManager()
{

}

PluginsManager::~PluginsManager()
{
	clearPlugins();	
}

shared_ptr<Plugin> PluginsManager::getPlugin(const String &id) const
{
	OS_LOCK(m_cs);

	Plugins::const_iterator i = m_plugins.find(id);
	if(i != m_plugins.end())
		return i->second;

	return nullptr;
}

String PluginsManager::getDataPath() const
{
	return utils::makeFolderPath(Options::instance()->getDataPath(), OS_STORE_PATH);
}

String PluginsManager::getSharePath() const
{
	return utils::makeFolderPath(Options::instance()->getSharePath(), OS_PLUGINS_PATH);
}

void PluginsManager::loadPlugin(const UniqueID &id, const String &path)
{
	shared_ptr<Plugin> plugin(OS_NEW Plugin());
	switch(plugin->load(id, path))
	{
	case Plugin::lrInvalid:
								break;

	case Plugin::lrFailed:		{
                                    String message = _S("Cannot load plugin '") + path + _S("'");
                                    String lastError = PlatformManager::instance()->getLastError();
                                    if(lastError.empty() == false)
                                    {
                                        message.append(_S(", last error: "));
                                        message.append(lastError);
                                    }

                                    OS_LOG_ERROR(message);
                                }

								break;

	case Plugin::lrSuccess:		if(plugin->initialize())
								{
									OS_LOCK(m_cs);
									OS_ASSERT(m_plugins.find(plugin->getID().toUTF16()) == m_plugins.end());
									m_plugins[plugin->getID().toUTF16()] = plugin;
								}
								else
								{
									OS_LOG_ERROR(_S("Plugin initialization error ('") + path + _S("')"));
								}

								break;
	}
}

void PluginsManager::loadPlugins()
{
	loadPlugins(getSharePath());
	loadPlugins(getDataPath());
}

void PluginsManager::loadPlugins(const String &path)
{
	if(FileSystem::instance()->directoryExists(path) == false)
		return;

	StringList plugins;
	if(FileSystem::instance()->getDirectories(path, plugins, false) == false)
		return;

	for(StringList::const_iterator i = plugins.begin(); i != plugins.end(); ++i)
	{
		UniqueID id((*i).to_ascii());
		if(id.validate(false) == false)
			continue;
				
		loadPlugin(id, utils::makeFolderPath(path, *i));
	}
}

void PluginsManager::clearPlugins()
{
	OS_LOCK(m_cs);
	m_plugins.clear();
}

void PluginsManager::unloadPlugins()
{
	OS_LOCK(m_cs);

	for(Plugins::const_iterator i = m_plugins.begin(); i != m_plugins.end(); i->second->terminate(), ++i);
	// N.B.: le plugin vanno rilasciate nel distruttore perchè è possibile che ci siano riferimenti di memoria
	// a quest'ultime che verrebbero corrotti se le plugin venissero deallocate qui.
	// Es.: una plugin che esporta un modulo registra il factory di quest'ultimo che viene rilasciato (e quindi deallocato)
	// successivamente alla chiamata a questa funzione.
}

String PluginsManager::getName() const
{
	return _S("plugins");
}

bool PluginsManager::start(bool recovery)
{
	loadPlugins();

	return true;
}

void PluginsManager::stop()
{
	unloadPlugins();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
