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
#include "plugin.h"

#include "filesystem.h"
#include "lock.h"
#include "log.h"
#include "notificationsmanager.h"
#include "options.h"
#include "osiriscommon.h"
#include "pluginmodule.h"
#include "pluginsmanager.h"
#include "utils.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"
#include "xmlschema.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

Plugin::Plugin()
{

}

Plugin::~Plugin()
{
	unload();
}

UniqueID Plugin::getID() const
{
	OS_LOCK(m_cs);
	return m_id;
}

String Plugin::getPath() const
{
	OS_LOCK(m_cs);
	return m_path;
}

String Plugin::getDataPath() const
{
	return utils::makeFolderPath(Options::instance()->getDataPath(), utils::makeFolderPath(OS_STORE_PATH, getID().toUTF16()));
}

String Plugin::getName() const
{
	OS_LOCK(m_cs);
	return m_name;
}

String Plugin::getAuthor() const
{
	OS_LOCK(m_cs);
	return m_author;
}

String Plugin::getDescription() const
{
	OS_LOCK(m_cs);
	return m_description;
}

Version Plugin::getVersion() const
{
	return m_version;
}

String Plugin::getIcon() const
{
	return m_icon;
}

String Plugin::getLogo() const
{
	return m_logo;
}

Plugin::LoadResult Plugin::load(const UniqueID &id, const String &path)
{
	unload();

	OS_LOCK(m_cs);

	m_path = path;

	//String filename = utils::makeFilePath(path, id.toUTF16() + ".xml"); // 0.13
	String filename = utils::makeFilePath(path, OS_MANIFESTXML); // 0.14
	if(FileSystem::instance()->fileExists(filename) == false)
		return lrInvalid;

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument(XMLSchema::fromFile(utils::makeFilePath(utils::makeFolderPath(Options::instance()->getSharePath(),OS_SCHEMAS_PATH), OS_PLUGINS_PLUGIN_SCHEMA))));
	if(document->parseFile(filename) == false)
		return lrFailed;

	shared_ptr<XMLNode> root = document->getRoot();

	m_id = id;
	m_name = root->getAttributeString(_S("name"));
	m_author = root->getAttributeString(_S("author"));
	m_description = root->getAttributeString(_S("description"));	
	if(m_version.fromString(root->getAttributeString(_S("version")).to_ascii()) == false)
		return lrInvalid;
	m_icon = root->getAttributeString(_S("icon"));
	m_logo = root->getAttributeString(_S("logo"));

	NotificationsManager::instance()->notify(_S("Loading plugin: ") + m_name);

	shared_ptr<XMLNode> nodeModules = document->getRoot()->getNode(_S("modules"));
	if(nodeModules != nullptr)
	{
		shared_ptr<XMLNodes> modules = nodeModules->getNodes();
		for(XMLNodes::const_iterator i = modules->begin(); i != modules->end(); ++i)
		{
			String modulePath = utils::makeFilePath(Options::instance()->getRootPath(), (*i)->getAttributeString(_S("path")));
			shared_ptr<PluginModule> module(OS_NEW PluginModule(get_this_ptr()));
			if(module->load(modulePath))
			{
				m_modules.push_back(module);
			}
			else
			{
				OS_LOG_ERROR(_S("Cannot load plugin module '") + modulePath + _S("'"));
				return lrFailed;
			}
		}
	}

	return lrSuccess;
}

void Plugin::unload()
{
	OS_LOCK(m_cs);

	for(Modules::iterator i = m_modules.begin(); i != m_modules.end(); ++i)
	{
		(*i)->unload();
	}

	m_modules.clear();
	m_path.clear();
	m_id.clear();
}

bool Plugin::initialize()
{
	OS_LOCK(m_cs);

	for(Modules::iterator i = m_modules.begin(); i != m_modules.end(); ++i)
	{
		if((*i)->initialize() == false)
			return false;
	}

	return true;
}

bool Plugin::terminate()
{
	OS_LOCK(m_cs);

	for(Modules::iterator i = m_modules.begin(); i != m_modules.end(); ++i)
	{
		if((*i)->terminate() == false)
			return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
