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

#ifndef _COMPONENTS_MODULES_FEED_H
#define _COMPONENTS_MODULES_FEED_H

#include "iextensionsmodule.h"
#include "objectsobjects.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace modules {

//////////////////////////////////////////////////////////////////////

class Feed : public IExtensionsModule
{
	typedef IExtensionsModule ModuleBase;

// Construction
public:
	Feed();
	virtual ~Feed();

// IModule overrides
public:
	virtual void initXML(shared_ptr<XMLDocument> document);

// IModule interface
public:
	virtual ExtensionsModuleID getID();
	virtual String getName();
	
	virtual shared_ptr<ExtensionsModuleEditor> createEditor();
	virtual shared_ptr<ExtensionsModuleViewer> createViewer();
};

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _COMPONENTS_MODULES_FEED_H
