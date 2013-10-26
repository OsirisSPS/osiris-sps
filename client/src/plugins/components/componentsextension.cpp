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

#include "stdafx.h"
#include "componentsextension.h"

#include "iportaldatabase.h"
#include "dbdatabasessystem.h"
#include "dbschemaschema.h"
#include "extensionssystem.h"
#include "filesystem.h"
#include "osiriscommon.h"
#include "osirisplugins.h"
#include "pluginsmanager.h"
#include "portalsportal.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

extern OS_NAMESPACE_NAME::String g_componentsPath;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {

//////////////////////////////////////////////////////////////////////

ComponentsExtension::ComponentsExtension(const String &path)
{
	m_path = path;
}

ComponentsExtension::~ComponentsExtension()
{

}

String ComponentsExtension::getBasePath()
{
	return g_componentsPath;
}

String ComponentsExtension::getSchemaPath(const Version &version)
{
	return utils::makeFilePath(utils::makeFolderPath(getBasePath(), OS_COMPONENTS_DB_FOLDER), String(OS_COMPONENTS_SCHEMA) + _S("_") + version.toString() + _S(".xml"));
}

ExtensionID ComponentsExtension::getID() const
{
	return ExtensionID(OS_COMPONENTS_EXTENSION_ID.to_ascii());
}

String ComponentsExtension::getName() const
{
	return _S("Components extension");
}

String ComponentsExtension::getDescription() const
{
	return _S("");
}

String ComponentsExtension::getAuthor() const
{
	return _S("");
}

String ComponentsExtension::getPath() const
{
	return m_path;
}

bool ComponentsExtension::getInternal() const
{
	return true;
}

void ComponentsExtension::load()
{
	// TODO: registrare qui componenti/moduli	
}

void ComponentsExtension::unload()
{
	// TODO: deregistrare qui componenti/moduli	
}

void ComponentsExtension::onPortalLoaded(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> database)
{
	ExtensionBase::onPortalLoaded(portal, database);

	// ID Fix 0.13
	database->execute(_S("update os_extensions set id='F81C732A924C42CB32201B84D5372C9322770CDC' where id='00000005F81C732A924C42CB32201B84D5372C9322770CDC'"));

	Version version;
	if(isExtensionRegistered(portal, database, version))
	{
		// TODO: se la versione  inferiore a quella corrente deve effettuare l'aggiornamento

		// portal->getSnapshotManager()->resetAll(false);
	}
	else
	{
		installExtension(portal, database);
	}
}

bool ComponentsExtension::installExtension(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> db)
{
	// CLODOVERYURGENT: Perchè l'ho tolta da razor sta roba?
#ifndef OS_OSIRIS_VERSION_RAZOR
	shared_ptr<DbSchemaSchema> schema(OS_NEW DbSchemaSchema());
	if(schema->parse(getSchemaPath(OS_COMPONENTS_SCHEMA_VERSION)) == false)
		return false;

	//shared_ptr<IPortalDatabase> db = portal->getDatabase();
	OS_EXCEPT_IF(db == null || db->connected() == false, "Invalid portal database");

	if(DatabasesSystem::instance()->loadSchema(db->getConnection(), schema) == false)
		return false;
#endif

	return registerExtension(portal, db, OS_COMPONENTS_EXTENSION_VERSION());	
}

//////////////////////////////////////////////////////////////////////

} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
