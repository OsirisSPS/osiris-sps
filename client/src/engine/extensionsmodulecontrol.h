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

#ifndef _EXTENSIONSMODULECONTROL_H
#define _EXTENSIONSMODULECONTROL_H

#include "iextensionsmodule.h"
#include "ideportalcontrol.h"
#include "htmldiv.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLDocument;

//////////////////////////////////////////////////////////////////////

class EngineExport ExtensionsModuleControl : public IPortalPageControl<HtmlDiv>
{
	typedef IPortalPageControl<HtmlDiv> ControlBase;

// Construction
public:
	ExtensionsModuleControl();
	virtual ~ExtensionsModuleControl();

// Attributes:
public:
	inline const shared_ptr<IExtensionsModule> getModule() const;
	inline const String & getTitle() const;
	inline const EntityID & getInstance() const;
	inline shared_ptr<XMLDocument> getModuleDocument() const;

// Operations
public:
	virtual bool init(shared_ptr<IExtensionsModule> module, const String &title, const EntityID &instance, const String &xml);	

protected:
	shared_ptr<IExtensionsModule> m_module;
	String m_title;
	EntityID m_instance;
	shared_ptr<XMLDocument> m_moduleDocument;
};

//////////////////////////////////////////////////////////////////////

inline const shared_ptr<IExtensionsModule> ExtensionsModuleControl::getModule () const { return m_module; }
inline const String & ExtensionsModuleControl::getTitle() const { return m_title; }
inline const EntityID & ExtensionsModuleControl::getInstance() const { return m_instance; }
inline shared_ptr<XMLDocument> ExtensionsModuleControl::getModuleDocument() const { return m_moduleDocument; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _EXTENSIONSMODULECONTROL_H
