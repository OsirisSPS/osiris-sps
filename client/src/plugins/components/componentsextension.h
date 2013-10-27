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

#ifndef OS_COMPONENTS_COMPONENTSEXTENSION_H
#define OS_COMPONENTS_COMPONENTSEXTENSION_H

#include "iextensionsextension.h"

#include "osiriscommon.h"

//////////////////////////////////////////////////////////////////////

#ifdef OS_OSIRIS_VERSION_RAZOR	
	#define OS_COMPONENTS_EXTENSION_VERSION()		OS_MAKEVERSION(0, 1)
#else
	#define OS_COMPONENTS_EXTENSION_VERSION()		OS_MAKEVERSION(1, 0)
#endif

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {

//////////////////////////////////////////////////////////////////////

class ComponentsExtension : public IExtensionsExtension
{
	typedef IExtensionsExtension ExtensionBase;

// Construction
public:
	ComponentsExtension(const String &path);
	virtual ~ComponentsExtension();

// Attributes
public:
	static String getBasePath();
	static String getSchemaPath(const Version &version);

// Operations
private:
	bool installExtension(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> db);

// IExtension interface
public:
	virtual ExtensionID getID() const;
	virtual String getName() const;
	virtual String getDescription() const;	
	virtual String getAuthor() const;
	virtual String getPath() const;
	virtual bool getInternal() const;

	virtual void load();
	virtual void unload();

// IExtension overrides
public:
	virtual void onPortalLoaded(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> database);

private:
	String m_path;
};

//////////////////////////////////////////////////////////////////////

} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // OS_COMPONENTS_COMPONENTSEXTENSION_H
