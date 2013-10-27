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

#ifndef _OS_ENGINE_PLUGINMODULE_H
#define _OS_ENGINE_PLUGINMODULE_H

#include "base/object.h"
#include "boost/noncopyable.hpp"
#include "osirisplugins.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DynLib;
class Plugin;
class Version;

//////////////////////////////////////////////////////////////////////

class EngineExport PluginModule : public boost::noncopyable,
								  public Object
{
// Construction
public:
	PluginModule(shared_ptr<Plugin> plugin);
	virtual ~PluginModule();

// Attributes
public:
	inline shared_ptr<Plugin> getPlugin() const;
	inline shared_ptr<DynLib> getLibrary() const;

// Operations
public:
	bool load(const String &filename);
	void unload();

	bool initialize();
	bool terminate();

protected:
	os_plugin_interface *m_interface;
	weak_ptr<Plugin> m_plugin;
	shared_ptr<DynLib> m_lib;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<Plugin> PluginModule::getPlugin() const { return m_plugin.lock(); }
inline shared_ptr<DynLib> PluginModule::getLibrary() const { return m_lib; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_PLUGINMODULE_H
