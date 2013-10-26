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

#ifndef _ENGINE_OBJECTSDESCRIPTOR_H
#define _ENGINE_OBJECTSDESCRIPTOR_H

#include "datadata.h"
#include "dbdb.h"
#include "objectsdescriptorclass.h"
#include "ideide.h"
#include "ids.h"
#include "entitiesentities.h"
#include "portalsportals.h"
#include "searchsearch.h"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;
class ISearchOptions;
//class ObjectID;

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsIDescriptor
{
	OS_DECLARE_DESCRIPTOR_INTERFACE()

// Enumerated types
public:
	enum OrderMode
	{
		omSubmitDate	= 0,
		omPosition		= 1,
	};

// Construction
public:
	ObjectsIDescriptor();
	virtual ~ObjectsIDescriptor();

// Attributes
public:
	bool allowChild(PortalObjectType type) const;

	// Restituisce la dimensione massima di un oggetto (default OS_PORTAL_OBJECT_MAXSIZE)
	//virtual uint32 getMaxObjectSize() const;

	virtual bool objectExists(shared_ptr<IPortalDatabase> db, const ObjectID &id);

	// Restituisce le tipologie di figli ammesse dall'oggetto
	virtual void getAllowedChilds(ObjectsTypes &allowed_childs) const;

	virtual OrderMode getOrderMode() const;

// Operations
public:
	virtual shared_ptr<ObjectsIObject> loadObject(shared_ptr<IPortalDatabase> db, const ObjectID &id);

// Interface
public:
	virtual shared_ptr<ObjectsIObject> createObject() = 0;
	virtual String getTableName() = 0;

	virtual String getTypeName() = 0;
	virtual String getGroupName() = 0;

// Overridables
public:
	// Restituisce se estratto da una ricerca, pu essere visualizzato il padre al posto suo.
	virtual bool isGroupable();

	// Restituisce se l'oggetto ha delle statistiche da computare
	virtual bool hasStatistics();

	// Restituisce i parametri di ricerca per l'oggetto
	virtual shared_ptr<ISearchOptions> getSearchOptions();

	// Restituisce il controllo di editazione dell'oggetto
	virtual ide::edit::editor_ptr createEditControl(shared_ptr<EntitiesEntity> entity = null, shared_ptr<EntitiesEntity> parent = null);
	// Restituisce il controllo di vista dell'oggetto
	virtual ide::view::viewer_ptr createViewControl(shared_ptr<EntitiesEntity> entity = null);

	// Crea il record di statistiche sull'oggetto
	virtual void createStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object);
	// Computa le statistiche dell'oggetto
	virtual void computeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object);
	// Rimuove le statistiche di un profilo (se object != null rimuove solo le statistiche sull'oggetto, altrimenti tutte le statistiche del profilo)
	virtual void removeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object = null);
		
};

//////////////////////////////////////////////////////////////////////

template <typename T, typename B = ObjectsIDescriptor>
class ObjectsDescriptor : public DynamicSingleton<T>,
				   public B
{
	typedef B DescriptorBase;

// Construction
public:
	ObjectsDescriptor();
	virtual ~ObjectsDescriptor();
};

//////////////////////////////////////////////////////////////////////

template <typename T, typename B>
ObjectsDescriptor<T, B>::ObjectsDescriptor()
{

}

template <typename T, typename B>
ObjectsDescriptor<T, B>::~ObjectsDescriptor()
{

}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_OBJECTSDESCRIPTOR_H
