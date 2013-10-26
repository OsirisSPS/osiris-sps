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

#ifndef _OS_ENGINE_PLUGIN_H
#define _OS_ENGINE_PLUGIN_H

#include "base/object.h"
#include "boost/noncopyable.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "enable_this_ptr.h"
#include "uniqueid.h"
#include "version.h"

//////////////////////////////////////////////////////////////////////

#define OS_PLUGINS_PLUGIN_SCHEMA				_S("plugin.xsd")

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class PluginModule;

//////////////////////////////////////////////////////////////////////

class EngineExport Plugin : public boost::noncopyable,
							public enable_this_ptr<Plugin>,
							public Object
{
	typedef Object BaseClass;

	typedef list<shared_ptr<PluginModule> >::type Modules;	

public:
	enum LoadResult
	{
		lrInvalid	= 0,
		lrFailed	= 1,
		lrSuccess	= 2,
	};

// Construction
public:
	Plugin();
	virtual ~Plugin();

// Attributes
public:
	UniqueID getID() const;
	// Restituisce il path di installazione della plugin
	String getPath() const;
	// Restituisce il path riservato alla plugin per il salvataggio dei dati
	String getDataPath() const;
	String getName() const;
	String getAuthor() const;	
	String getDescription() const;
	Version getVersion() const;
	String getIcon() const;
	String getLogo() const;
	
	
// Operations
public:
	LoadResult load(const UniqueID &id, const String &path);
	void unload();

	bool initialize();
	bool terminate();

protected:
	mutable boost::recursive_mutex m_cs;
	UniqueID m_id;
	String m_path;
	String m_name;
	String m_author;
	String m_description;
	Version m_version;
	String m_icon;
	String m_logo;	
	Modules m_modules;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_PLUGIN_H
