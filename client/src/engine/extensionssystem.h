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

#ifndef _EXTENSIONSSYSTEM_H
#define _EXTENSIONSSYSTEM_H

#include "boost/thread/recursive_mutex.hpp"
#include "entitiesentities.h"
#include "extensionsextensions.h"
#include "ids.h"
#include "isystem.h"
#include "ordered_map.h"
#include "portalsportals.h"
#include "singleton.h"
#include "uniqueid.h"


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IExtensionsCodeProvider;
class IExtensionsComponent;
class IExtensionsExtension;
class IExtensionsModule;
class ExtensionsComponentEditor;
class ExtensionsComponentID;
class ExtensionsComponentViewer;
class ExtensionsModuleEditor;
class ExtensionsModuleID;
class ExtensionsModuleViewer;

class IPortalDatabase;
class Portal;

//////////////////////////////////////////////////////////////////////

class EngineExport ExtensionsSystem : public DynamicSingleton<ExtensionsSystem>,
									  public ISystem
{
public:
	typedef list<shared_ptr<IExtensionsCodeProvider> >::type CodeProviders;
	typedef unordered_map<String, shared_ptr<IExtensionsComponent> >::type Components;
	typedef unordered_map<String, shared_ptr<IExtensionsModule> >::type Modules;	
	typedef ordered_map<UniqueID, shared_ptr<IExtensionsExtension> > Extensions;

// Construction
public:
	ExtensionsSystem();
	virtual ~ExtensionsSystem();

// Attributes
public:
	inline const Components & getComponents() const;
	inline const Modules & getModules() const;
	inline const Extensions & getExtensions() const;

	shared_ptr<IExtensionsCodeProvider> getCodeProvider(const String &name) const;

	shared_ptr<IExtensionsComponent> getComponent(const ExtensionsComponentID &id) const;
	bool hasComponent(const ExtensionsComponentID &id) const;

	shared_ptr<IExtensionsExtension> getExtension(const ExtensionID &id) const;
	bool hasExtension(const ExtensionID &id) const;

	shared_ptr<IExtensionsModule> getModule(const ExtensionsModuleID &id) const;
	bool hasModule(const ExtensionsModuleID &id) const;

	String getDataPath() const;
	String getSharePath() const;

	inline int getKnownUpgradableCounter() const;
	inline void setKnownUpgradableCounter(int v);
	
// Operations
public:
	ExtensionsComponentID generateComponentID() const;
	ExtensionsModuleID generateModuleID() const;

	bool registerCodeProvider(shared_ptr<IExtensionsCodeProvider> codeProvider);
	void unregisterCodeProvider(shared_ptr<IExtensionsCodeProvider> codeProvider);
	
	bool registerComponent(shared_ptr<IExtensionsComponent> component);
	void unregisterComponent(shared_ptr<IExtensionsComponent> component);
	
	bool registerModule(shared_ptr<IExtensionsModule> module);
	void unregisterModule(shared_ptr<IExtensionsModule> module);

	void notifyPortalLoaded(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> database);

	virtual shared_ptr<ExtensionsComponentEditor> createComponentEditor(const ExtensionsComponentID &id, shared_ptr<EntitiesEntity> entity);
	virtual shared_ptr<ExtensionsComponentViewer> createComponentViewer(const ExtensionsComponentID &id, shared_ptr<EntitiesEntity> entity, bool force = true);

	virtual shared_ptr<ExtensionsModuleEditor> createModuleEditor(const ExtensionsModuleID &id);
	virtual shared_ptr<ExtensionsModuleViewer> createModuleViewer(const ExtensionsModuleID &id, bool ensure);

	bool loadExtension(shared_ptr<IExtensionsExtension> extension);
	void reloadExtensions();

	// Install an extension and load it.
	void install(const ExtensionID &id, const String &path, bool load = true);

	// Unload Uninstall an extension.
	void uninstall(const ExtensionID &id);

	// Browse the share folder to find ".osiris" package to automatically install.
	void installShare(bool recovery);

private:
	void loadExtensions();
	void loadExtensions(const String &path);
	void unloadExtensions();

	shared_ptr<ExtensionsModuleEditor> allocModuleEditor(const ExtensionsModuleID &id);
	shared_ptr<ExtensionsModuleViewer> allocModuleViewer(const ExtensionsModuleID &id);

	void clear();

// ISystem interface
public:
	virtual String getName() const;

	virtual bool start(bool recovery);
	virtual void stop();

protected:
	CodeProviders m_codeProviders;
	Components m_components;
	Modules m_modules;
	Extensions m_extensions;

	int m_knownUpgradableCounter;

	mutable boost::recursive_mutex m_cs;
};

//////////////////////////////////////////////////////////////////////

inline const ExtensionsSystem::Components & ExtensionsSystem::getComponents() const { return m_components; }
inline const ExtensionsSystem::Modules & ExtensionsSystem::getModules() const { return m_modules; }
inline const ExtensionsSystem::Extensions & ExtensionsSystem::getExtensions() const { return m_extensions; }

inline int ExtensionsSystem::getKnownUpgradableCounter() const { return m_knownUpgradableCounter; }
inline void ExtensionsSystem::setKnownUpgradableCounter(int v) { m_knownUpgradableCounter = v; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _EXTENSIONSSYSTEM_H
