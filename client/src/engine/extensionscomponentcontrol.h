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

#ifndef _EXTENSIONSCOMPONENTCONTROL_H
#define _EXTENSIONSCOMPONENTCONTROL_H

#include "iextensionscomponent.h"
#include "ideportalcontrol.h"

#include "htmldiv.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLDocument;

//////////////////////////////////////////////////////////////////////

class EngineExport ExtensionsComponentControl : public IPortalPageControl<HtmlDiv>
{
	typedef IPortalPageControl<HtmlDiv> ControlBase;

// Construction
public:
	ExtensionsComponentControl();
	virtual ~ExtensionsComponentControl();

// Attributes:
public:
	inline const shared_ptr<IExtensionsComponent> getComponent() const;
	inline const EntityID & getEntityID() const;
	inline shared_ptr<XMLDocument> getComponentDocument() const;

	shared_ptr<EntitiesEntity> getEntity(const shared_ptr<IPortalDatabase> &database) const;

// Operations
public:
	virtual bool init(shared_ptr<IExtensionsComponent> component, const EntityID &section, const String &xml);	

protected:
	shared_ptr<IExtensionsComponent> m_component;
	EntityID m_entityID;
	shared_ptr<XMLDocument> m_componentDocument;
};

//////////////////////////////////////////////////////////////////////

inline const shared_ptr<IExtensionsComponent> ExtensionsComponentControl::getComponent () const { return m_component; }
inline const EntityID & ExtensionsComponentControl::getEntityID() const { return m_entityID; }
inline shared_ptr<XMLDocument> ExtensionsComponentControl::getComponentDocument() const { return m_componentDocument; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _EXTENSIONSCOMPONENTCONTROL_H
