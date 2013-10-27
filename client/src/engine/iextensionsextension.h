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

#ifndef _IEXTENSIONSEXTENSION_H
#define _IEXTENSIONSEXTENSION_H

#include "enable_this_ptr.h"
#include "extensionsextensions.h"
#include "base/object.h"
#include "dbdb.h"
#include "ids.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;
class Portal;
class Version;

//////////////////////////////////////////////////////////////////////

class EngineExport IExtensionsExtension : public Object,
								public enable_this_ptr<IExtensionsExtension>
{
// Construction
public:
	IExtensionsExtension();
	virtual ~IExtensionsExtension();

// Attributes
public:
	// Restituisce il path riservato all'extension per il salvataggio dei dati
	String getDataPath() const;
	// Restituisce true se l'extension è registrata nel portale specificato
	bool isExtensionRegistered(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> db, Version &version) const;

// Operations
public:
	// Registra l'extension all'interno di un portale
	bool registerExtension(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> db, const Version &version);
	// Aggiorna la versione dell'extension in un portale
	bool updateExtension(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> db, const Version &version);

	void firePortalLoaded(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> database);

// Interface
public:
	virtual ExtensionID getID() const = 0;
	virtual String getName() const = 0;
	virtual String getDescription() const = 0;	
	virtual String getAuthor() const = 0;
	virtual String getPath() const = 0;
	virtual bool getInternal() const = 0; // Se true, non deve comparire nell'elenco addons.

	virtual void load() = 0;
	virtual void unload() = 0;

// Overridables
protected:
	virtual void onPortalLoaded(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> database);
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IEXTENSIONSEXTENSION_H
