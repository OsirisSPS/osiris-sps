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

#ifndef _IEXTENSIONSMODULE_H
#define _IEXTENSIONSMODULE_H

#include "base/object.h"
#include "objectsobjects.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ExtensionsModuleEditor;
class ExtensionsModuleID;
class ExtensionsModuleViewer;
class XMLDocument;
class XMLPortalExporter;

//////////////////////////////////////////////////////////////////////

class EngineExport IExtensionsModule : public Object
{
// Construction
public:
	IExtensionsModule();
	virtual ~IExtensionsModule();

// Attributes
public:
	inline bool getCacheContent() const;
	inline void setCacheContent(bool cache);

	inline const String & getDefaultStyle() const;
	inline void setDefaultStyle(const String &style);

// Overridables
public:
	virtual void exportXML(const EntityID &id, shared_ptr<XMLPortalExporter> exporter);
	virtual void initXML(shared_ptr<XMLDocument> document);
	
// Interface
public:
	virtual ExtensionsModuleID getID() = 0;
	virtual String getName() = 0;

	virtual shared_ptr<ExtensionsModuleEditor> createEditor() = 0;
	virtual shared_ptr<ExtensionsModuleViewer> createViewer() = 0;

protected:
	bool m_cacheContent;
	String m_defaultStyle;
};

//////////////////////////////////////////////////////////////////////

inline bool IExtensionsModule::getCacheContent() const { return m_cacheContent; }
inline void IExtensionsModule::setCacheContent(bool cache) { m_cacheContent = cache; }

inline const String & IExtensionsModule::getDefaultStyle() const { return m_defaultStyle; }
inline void IExtensionsModule::setDefaultStyle(const String &style) { m_defaultStyle = style; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IEXTENSIONSMODULE_H
