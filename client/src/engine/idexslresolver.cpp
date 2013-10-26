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
#include "idexslresolver.h"

#include "extensionsextension.h"
#include "extensionssystem.h"
#include "filesystem.h"
#include "id.h"
#include "ideide.h"
#include "idesystem.h"
#include "ideskin.h"
#include "options.h"
#include "osiriscommon.h"
#include "plugin.h"
#include "uniqueid.h"
#include "pluginsmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String IdeXSLResolver::HTDOCS_PATH("http://www.osiris-sps.org/htdocs/");
const String IdeXSLResolver::EXTENSIONS_PATH("http://www.osiris-sps.org/extensions/");
const String IdeXSLResolver::PLUGINS_PATH("http://www.osiris-sps.org/plugins/");

//////////////////////////////////////////////////////////////////////

IdeXSLResolver::IdeXSLResolver(shared_ptr<IdeSkin> skin) : m_skin(skin)
{

}

IdeXSLResolver::~IdeXSLResolver()
{

}

/*
bool IdeXSLResolver::resolveSkinPath(shared_ptr<IdeSkin> skin, const String &path, String &resolved)
{
	OS_ASSERT(skin != null);

    // Calcola il path fisico del file
	resolved = skin->getResourcePath(path);
	return resolved.empty() == false;
}

bool IdeXSLResolver::resolveSkinsPath(const UniqueID &id, const String &path, String &resolved)
{
	shared_ptr<IdeSkin> skin = IdeSystem::instance()->getSkin(id.toUTF16());
	if(skin == null)
	{
	    OS_ASSERTFALSE();
	    return false;
	}

	return resolveSkinPath(skin, path, resolved);
}
*/

bool IdeXSLResolver::resolveHtdocsPath(const String &path, String &resolved)
{
	OS_ASSERT(m_skin);

	resolved = m_skin->getResourcePath(path);
	return (resolved.empty() == false);	
}

bool IdeXSLResolver::resolvePluginsPath(const UniqueID &id, const String &path, String &resolved)
{
	shared_ptr<Plugin> plugin = PluginsManager::instance()->getPlugin(id.toUTF16());
	if(plugin == null)
		return false;

	resolved = utils::makeFilePath(plugin->getPath(), path);
	if(FileSystem::instance()->exists(resolved))
        return true;

    resolved.clear();
	return false;
}

bool IdeXSLResolver::resolveExtensionsPath(const ExtensionID &id, const String &path, String &resolved)
{
	shared_ptr<IExtensionsExtension> extension = ExtensionsSystem::instance()->getExtension(id);
	if(extension == null)
		return false;

	resolved = utils::makeFilePath(extension->getPath(), path);
	if(FileSystem::instance()->exists(resolved))
        return true;

    resolved.clear();
	return false;
}

bool IdeXSLResolver::resolve(const String &uri, String &resolved)
{
	OS_ASSERT(resolved.empty());

	/*
	if(uri.starts_with(SKIN_PATH, false))
	{
		if(resolveSkinPath(m_skin, uri.mid(SKIN_PATH.size()), resolved))
			return true;
	}
	else if(uri.starts_with(SKINS_PATH, false))
	{
		size_t pos = SKINS_PATH.size();
		UniqueID id(uri.find_next(_S("/"), pos).to_ascii());
		if(id.validate(false))
		{
			if(resolveSkinsPath(id, uri.mid(pos), resolved))
				return true;
		}
	}
	*/
	if(uri.starts_with(HTDOCS_PATH, false))
	{
		if(resolveHtdocsPath(uri.mid(HTDOCS_PATH.size()), resolved))
				return true;
	}
	else if(uri.starts_with(EXTENSIONS_PATH, false))
	{
		size_t pos = EXTENSIONS_PATH.size();
		ExtensionID id(uri.find_next(_S("/"), pos).to_ascii());
		if(id.validate(false))
		{
			if(resolveExtensionsPath(id, uri.mid(pos), resolved))
				return true;
		}
	}
	else if(uri.starts_with(PLUGINS_PATH, false))
	{
		size_t pos = PLUGINS_PATH.size();
		UniqueID id(uri.find_next(_S("/"), pos).to_ascii());
		if(id.validate(false))
		{
			if(resolvePluginsPath(id, uri.mid(pos), resolved))
				return true;
		}
	}

	OS_ASSERTFALSE();
	OS_EXCEPTION("Invalid xsl uri '" + uri.to_ascii() + "'");

	return false;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
