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
#include "extensionssystem.h"

#include "archivesmanager.h"
#include "cryptmanager.h"
#include "engine.h"
#include "ihttpserver.h"
#include "log.h"
#include "buffer.h"
#include "filesystem.h"
#include "lock.h"
#include "path.h"
#include "extensionsextension.h"
#include "iextensionscodeprovider.h"
#include "iextensionscomponent.h"
#include "iextensionsmodule.h"
#include "extensionsinvalidcomponent.h"
#include "extensionsinvalidmodule.h"
#include "notificationsmanager.h"
#include "options.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SYSTEM(OS_NAMESPACE_NAME::ExtensionsSystem)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ExtensionsSystem::ExtensionsSystem()
{
	m_knownUpgradableCounter = -1;
}

ExtensionsSystem::~ExtensionsSystem()
{
	clear();
}

shared_ptr<IExtensionsCodeProvider> ExtensionsSystem::getCodeProvider(const String &name) const
{
	OS_LOCK(m_cs);

	for(CodeProviders::const_iterator i = m_codeProviders.begin(); i != m_codeProviders.end(); ++i)
	{
		if((*i)->getName().compare_no_case(name))
			return *i;
	}

	return nullptr;
}

shared_ptr<IExtensionsComponent> ExtensionsSystem::getComponent(const ExtensionsComponentID &id) const
{
	OS_LOCK(m_cs);

	Components::const_iterator i = m_components.find(id.toUTF16());
	return i != m_components.end() ? i->second : nullptr;
}

bool ExtensionsSystem::hasComponent(const ExtensionsComponentID &id) const
{
	OS_LOCK(m_cs);

	return getComponent(id) != nullptr;
}

shared_ptr<IExtensionsExtension> ExtensionsSystem::getExtension(const ExtensionID &id) const
{
	OS_LOCK(m_cs);

	Extensions::const_iterator i = m_extensions.find(id);
	if(i != m_extensions.end())
		return i->second;

	return nullptr;
}

bool ExtensionsSystem::hasExtension(const ExtensionID &id) const
{
	OS_LOCK(m_cs);

	return getExtension(id) != nullptr;
}

shared_ptr<IExtensionsModule> ExtensionsSystem::getModule(const ExtensionsModuleID &id) const
{
	OS_LOCK(m_cs);

	Modules::const_iterator i = m_modules.find(id.toUTF16());
	return i != m_modules.end() ? i->second : nullptr;
}

bool ExtensionsSystem::hasModule(const ExtensionsModuleID &id) const
{
	OS_LOCK(m_cs);

	return getModule(id) != nullptr;
}

String ExtensionsSystem::getDataPath() const
{
	return utils::makeFolderPath(Options::instance()->getDataPath(), OS_EXTENSIONS_EXTENSIONS_PATH);
}

String ExtensionsSystem::getSharePath() const
{
	return utils::makeFolderPath(Options::instance()->getSharePath(), OS_EXTENSIONS_EXTENSIONS_PATH);
}

ExtensionsComponentID ExtensionsSystem::generateComponentID() const
{
	//return ExtensionsComponentID::generate(CryptManager::instance()->randomSHA().toHex());
	return ExtensionsComponentID::generate();
}

ExtensionsModuleID ExtensionsSystem::generateModuleID() const
{
	//return ExtensionsModuleID::generate(CryptManager::instance()->randomSHA().toHex());
	return ExtensionsModuleID::generate();
}

bool ExtensionsSystem::registerCodeProvider(shared_ptr<IExtensionsCodeProvider> codeProvider)
{
	OS_LOCK(m_cs);

	if(codeProvider == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	String name = codeProvider->getName();
	if(getCodeProvider(name) != nullptr)
	{
		OS_LOG_ERROR(_S("Code provider '") + name + _S(" 'already registered"));
		return false;
	}

	m_codeProviders.push_back(codeProvider);
	return true;
}

void ExtensionsSystem::unregisterCodeProvider(shared_ptr<IExtensionsCodeProvider> codeProvider)
{
	OS_LOCK(m_cs);

	utils::remove(m_codeProviders, codeProvider);
}

bool ExtensionsSystem::registerComponent(shared_ptr<IExtensionsComponent> component)
{
	OS_LOCK(m_cs);

	if(component == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	String name = component->getName();

	ExtensionsComponentID id = component->getID();
	// TOFIX_VALIDATING
	/*
	if(id.validate(false) == false || id.getType() != ID::idComponent)
	{
		OS_LOG_ERROR(_S("Invalid component id: ") + name);
		return false;
	}
	*/

	shared_ptr<IExtensionsComponent> existing = getComponent(id);
	if(existing != nullptr)
	{
		OS_LOG_ERROR(_S("Duplicated component: ") + existing->getName() + _S(", ") + name);
		OS_ASSERTFALSE();
		return false;
	}

	m_components[id.toUTF16()] = component;
	return true;
}

void ExtensionsSystem::unregisterComponent(shared_ptr<IExtensionsComponent> component)
{
	OS_LOCK(m_cs);

	if(component == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	Components::iterator i = m_components.find(component->getID().toUTF16());
	if(i == m_components.end())
	{
		OS_ASSERTFALSE();
		return;
	}

	m_components.erase(i);
}

bool ExtensionsSystem::registerModule(shared_ptr<IExtensionsModule> module)
{
	OS_LOCK(m_cs);

	if(module == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	String name = module->getName();

	ExtensionsModuleID id = module->getID();
	// TOFIX_VALIDATING
	/*
	if(id.validate(false) == false || id.getType() != ID::idModule)
	{
		OS_LOG_ERROR(_S("Invalid module id: ") + name);
		return false;
	}
	*/

//#ifndef OS_DEBUG
	shared_ptr<IExtensionsModule> existing = getModule(id);
	if(existing != nullptr)
	{
		OS_LOG_ERROR(_S("Duplicated module: ") + existing->getName() + _S(", ") + name);
		return false;
	}
//#endif // OS_DEBUG

	m_modules[id.toUTF16()] = module;
	return true;
}

void ExtensionsSystem::unregisterModule(shared_ptr<IExtensionsModule> module)
{
	OS_LOCK(m_cs);

	if(module == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	Modules::iterator i = m_modules.find(module->getID().toUTF16());
	if(i == m_modules.end())
	{
		OS_ASSERTFALSE();
		return;
	}

	m_modules.erase(i);
}

void ExtensionsSystem::notifyPortalLoaded(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> database)
{
	OS_LOCK(m_cs);

	for(Extensions::iterator i = m_extensions.begin(); i != m_extensions.end(); ++i)
	{
		i->second->firePortalLoaded(portal, database);
	}
}

shared_ptr<ExtensionsComponentEditor> ExtensionsSystem::createComponentEditor(const ExtensionsComponentID &id, shared_ptr<EntitiesEntity> entity)
{
	OS_LOCK(m_cs);

	shared_ptr<IExtensionsComponent> component = getComponent(id);
	return component != nullptr ? component->createEditor(entity) : nullptr;
}

shared_ptr<ExtensionsComponentViewer> ExtensionsSystem::createComponentViewer(const ExtensionsComponentID &id, shared_ptr<EntitiesEntity> entity, bool force)
{
	OS_LOCK(m_cs);

	shared_ptr<IExtensionsComponent> component = getComponent(id);
	if(component != nullptr)
		return component->createViewer(entity);

	if(force)
		return shared_ptr<ExtensionsComponentViewer>(OS_NEW ExtensionsInvalidComponent(id));

	return nullptr;
}

shared_ptr<ExtensionsModuleEditor> ExtensionsSystem::createModuleEditor(const ExtensionsModuleID &id)
{
	OS_LOCK(m_cs);

	return allocModuleEditor(id);
}

shared_ptr<ExtensionsModuleViewer> ExtensionsSystem::createModuleViewer(const ExtensionsModuleID &id, bool ensure)
{
	OS_LOCK(m_cs);

	shared_ptr<ExtensionsModuleViewer> viewer = allocModuleViewer(id);
	if(viewer == nullptr && ensure)
		viewer.reset(OS_NEW ExtensionsInvalidModule(id));

	return viewer;
}

bool ExtensionsSystem::loadExtension(shared_ptr<IExtensionsExtension> extension)
{
	OS_LOCK(m_cs);

	if(extension == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	String name = extension->getName();

	ExtensionID id = extension->getID();
	if(id.validate(false) == false)
	{
		OS_LOG_ERROR(_S("Invalid extension id: ") + name);
		return false;
	}

	shared_ptr<IExtensionsExtension> existing = getExtension(id);
	if(existing != nullptr)
	{
		OS_LOG_ERROR(_S("Duplicated extension: ") + existing->getName() + _S(", ") + name);
		return false;
	}

	extension->load();
	m_extensions.push_back(id, extension);

	return true;
}

void ExtensionsSystem::reloadExtensions()
{
	OS_LOCK(m_cs);

	unloadExtensions();
	loadExtensions();
}

void ExtensionsSystem::install(const ExtensionID &id, const String &path, bool load)
{
	// TODO: If the uninstall fail (for example, if some file in the directory are in use), the install fail.
	OS_LOCK(m_cs);

	if(id.validate(false) == false)
	{
		OS_LOG_ERROR(_S("Invalid extension id: ") + id.getString());
		return;
	}

	if(hasExtension(id))
		uninstall(id);

	String outPath = utils::makeFolderPath(getDataPath(), id.getString());

	ArchivesManager::instance()->extract(path, outPath);

	if(load)
	{
		shared_ptr<ExtensionsExtension> extension(OS_NEW ExtensionsExtension());
		if(extension->init(id, outPath))
			loadExtension(extension);
	}

	setKnownUpgradableCounter(-1);
}

void ExtensionsSystem::uninstall(const ExtensionID &id)
{
	OS_LOCK(m_cs);

	if(id.validate(false) == false)
	{
		OS_LOG_ERROR(_S("Invalid extension id: ") + id.getString());
		return;
	}

	shared_ptr<ExtensionsExtension> extension = boost::dynamic_pointer_cast<ExtensionsExtension>(ExtensionsSystem::instance()->getExtension(id));
	if(extension)
	{
		extension->unload();
	}

		String outPath = utils::makeFolderPath(getDataPath(), id.getString());

	FileSystem::instance()->removeDirectory(outPath);

	m_extensions.erase(id);

	setKnownUpgradableCounter(-1);
}

void ExtensionsSystem::installShare(bool recovery)
{
	OS_LOCK(m_cs);

	bool developingRepository = FileSystem::instance()->fileExists(utils::makeFilePath(getDataPath(), _S("git.readme")));

	bool force = recovery;

	if(Options::instance()->isLastVersionCurrent() == false)
		force = true;

	StringList extensions;
	if(FileSystem::instance()->getFiles(getSharePath(), extensions, false) == false)
		return;

	int index = 0;
	for(StringList::const_iterator i = extensions.begin(); i != extensions.end(); ++i)
	{
		index++;
		String path = *i;
		String fullPath = utils::makeFilePath(getSharePath(), path);

		path.replace_all(_S(".osiris"),String::EMPTY);

		bool required = false;
		if( (path == OS_EXTENSIONS_CORE) || // Core
			(path == OS_EXTENSIONS_SKINS) || // Skins
			(path == OS_EXTENSIONS_ADVERTISING) || // Advertising
			(path == OS_EXTENSIONS_HTMLSNIPPET) || // Html Snippet
			(path == OS_EXTENSIONS_LANGUAGE_ENGLISH) || // English
			(path == OS_EXTENSIONS_LANGUAGE_INVARIANT) )  // Invariant
			required = true;

		ExtensionID id = path.to_ascii();

		bool needInstall = force;
		if( (FileSystem::instance()->directoryExists(utils::makeFolderPath(getDataPath(), path)) == false) && required)
			needInstall = true;

		if( (needInstall) && (developingRepository) && (FileSystem::instance()->directoryExists(utils::makeFolderPath(getDataPath(), path)) == true) )
		{
            NotificationsManager::instance()->notify(String::format(_S("Skipped (GIT-versioning) default extensions, %d/%d").c_str(), index, extensions.size()));
            needInstall = false; // Force doesn't matter
		}

		if(needInstall)
		{
			NotificationsManager::instance()->notify(String::format(_S("Installing default extensions, %d/%d").c_str(), index, extensions.size()));

			install(id, fullPath, false);
		}
	}
}

void ExtensionsSystem::loadExtensions()
{
	OS_LOCK(m_cs);

	loadExtensions(getDataPath());
}

void ExtensionsSystem::loadExtensions(const String &path)
{
	OS_LOCK(m_cs);

	if(FileSystem::instance()->directoryExists(path) == false)
		return;

	StringList extensions;
	if(FileSystem::instance()->getDirectories(path, extensions, false) == false)
		return;

	for(StringList::const_iterator i = extensions.begin(); i != extensions.end(); ++i)
	{
		ExtensionID id((*i).to_ascii());
		if(id.validate(false) == false)
			continue;	// Il nome della directory deve coincidere come standard con l'ID, pertanto salta tutti quelle non conformi

		if(id.getString() == "138B613D055759C619D5F4EFD9FDB978387E97CB") // Osiris Core 0.X - FORCOMPATIBILITY - Only Osiris Core 1.X must be loaded.
			continue;

		if(id.getString() == "E781E8B0E5F2CFE0FA74CC520756692EE2FB35DD") // Old Limpid Skin
			continue;

		shared_ptr<ExtensionsExtension> extension(OS_NEW ExtensionsExtension());
		if(extension->init(id, utils::makeFolderPath(path, *i)))
			loadExtension(extension);
	}
}

void ExtensionsSystem::unloadExtensions()
{
	OS_LOCK(m_cs);

	// TODO: le extensions andrebbero scandite in ordine inverso (attualmente ordered_map non supporta reverse_iterator)

	for(Extensions::const_iterator i = m_extensions.begin(); i != m_extensions.end(); i->second->unload(), ++i);
	m_extensions.clear();
}

shared_ptr<ExtensionsModuleEditor> ExtensionsSystem::allocModuleEditor(const ExtensionsModuleID &id)
{
	OS_LOCK(m_cs);

	shared_ptr<IExtensionsModule> module = getModule(id);
	return module != nullptr ? module->createEditor() : nullptr;
}

shared_ptr<ExtensionsModuleViewer> ExtensionsSystem::allocModuleViewer(const ExtensionsModuleID &id)
{
	OS_LOCK(m_cs);

	shared_ptr<IExtensionsModule> module = getModule(id);
	return module != nullptr ? module->createViewer() : nullptr;
}

void ExtensionsSystem::clear()
{
	OS_LOCK(m_cs);

	m_components.clear();
	m_modules.clear();
	m_extensions.clear();
	m_codeProviders.clear();
}

String ExtensionsSystem::getName() const
{
	return _S("extensions");
}

bool ExtensionsSystem::start(bool recovery)
{
	OS_LOCK(m_cs);

	installShare(recovery);

	loadExtensions();

	return true;
}

void ExtensionsSystem::stop()
{
	OS_LOCK(m_cs);

	unloadExtensions();
	clear();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
