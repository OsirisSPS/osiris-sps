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

#ifndef _IEXTENSIONSCOMPONENT_H
#define _IEXTENSIONSCOMPONENT_H

#include "dbdb.h"
#include "entitiesentities.h"
#include "extensionsextensions.h"
#include "ideide.h"
#include "base/object.h"
#include "ids.h"
#include "objectsobjects.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ExtensionsComponentEditor;
class ExtensionsComponentID;
class ExtensionsComponentViewer;
class IPortalDatabase;
class XMLPortalExporter;
class XMLDocument;

//////////////////////////////////////////////////////////////////////

class EngineExport IExtensionsComponent : public Object
{
// Construction
public:
	IExtensionsComponent();
	virtual ~IExtensionsComponent();

// Interface
public:
	virtual ExtensionsComponentID getID() = 0;
	virtual String getName() = 0;

	virtual shared_ptr<ExtensionsComponentEditor> createEditor(shared_ptr<EntitiesEntity> entity) = 0;
	virtual shared_ptr<ExtensionsComponentViewer> createViewer(shared_ptr<EntitiesEntity> entity) = 0;

// Overridables
public:
	virtual void exportXML(const EntityID &id, shared_ptr<XMLPortalExporter> exporter);
	virtual void initXML(shared_ptr<XMLDocument> document);

	virtual void createStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object);
	virtual void computeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object);
	virtual void removeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object = nullptr);

	virtual bool removeProfileData(shared_ptr<IPortalDatabase> db);
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IEXTENSIONSCOMPONENT_H
