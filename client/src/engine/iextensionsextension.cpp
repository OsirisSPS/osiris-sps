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
#include "iextensionsextension.h"

#include "idbconnection.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "options.h"
#include "portalsportal.h"
#include "uniqueid.h"
#include "utils.h"
#include "version.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IExtensionsExtension::IExtensionsExtension()
{

}

IExtensionsExtension::~IExtensionsExtension()
{

}

String IExtensionsExtension::getDataPath() const
{
	return utils::makeFolderPath(utils::makeFolderPath(Options::instance()->getDataPath(), OS_EXTENSIONS_EXTENSIONS_PATH), getID().toUTF16());
}

bool IExtensionsExtension::isExtensionRegistered(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> db, Version &version) const
{
	OS_ASSERT(portal != nullptr);

	OS_EXCEPT_IF(db == nullptr, "Invalid database");

	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::EXTENSIONS_TABLE));
	select->fields.add(DbSqlField(DBTABLES::EXTENSIONS::VERSION));
	select->where.add(DBTABLES::EXTENSIONS::ID, Convert::toSQL(getID().toUTF16()));
	select->limit.setCount(1);

	DataTable result;
	if(db->execute(select, result) == false)
		return false;

	if(result.rows() == 0)
		return false;

	version.fromString(static_cast<String>(*result[0][0]).to_ascii());
	return true;
}

bool IExtensionsExtension::registerExtension(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> db, const Version &version)
{
	OS_ASSERT(portal != nullptr);

	//shared_ptr<IPortalDatabase> db = portal->getDatabase();
	OS_EXCEPT_IF(db == nullptr, "Invalid database");

	DbSqlValues values;
	values.set(DBTABLES::EXTENSIONS::ID, getID().toUTF16());
	values.set(DBTABLES::EXTENSIONS::VERSION, static_cast<String>(version.toString()));

	String sql = db->getConnection()->prepare_insert(DBTABLES::EXTENSIONS_TABLE, values);
	return db->getConnection()->executeStatement(sql, values);
}

bool IExtensionsExtension::updateExtension(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> db, const Version &version)
{
	OS_ASSERT(portal != nullptr);

	//shared_ptr<IPortalDatabase> db = portal->getDatabase();
	OS_EXCEPT_IF(db == nullptr, "Invalid database");

	DbSqlValues values;
	values.set(DBTABLES::EXTENSIONS::VERSION, static_cast<String>(version.toString()));

	String sql = db->getConnection()->prepare_update(DBTABLES::EXTENSIONS_TABLE, values, DBTABLES::EXTENSIONS::ID, Convert::toSQL(getID().toUTF16()));
	return db->getConnection()->executeStatement(sql, values);
}

void IExtensionsExtension::firePortalLoaded(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> database)
{
	onPortalLoaded(portal, database);
}

void IExtensionsExtension::onPortalLoaded(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> database)
{

}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
