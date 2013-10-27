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
#include "p2pmachine.h"

#include "convert.h"
#include "datatable.h"
#include "dbdatabasessystem.h"
#include "idbconnection.h"
#include "idbparameter.h"
#include "idbstatement.h"
#include "dbsqlselect.h"
#include "engine.h"
#include "log.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

P2pMachine::P2pMachine()
{

}

P2pMachine::~P2pMachine()
{

}

bool P2pMachine::load(const String &id)
{
	try
	{
		shared_ptr<IDbConnection> connection = Engine::instance()->createSystemConnection();
		
		shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(_S("os_machines")));
		select->where.add(_S("id"), Convert::toSQL(id));
		select->limit.setCount(1);

		DataTable result;
		connection->query(select, result);

		if(result.rows() == 1)
		{
			DataTableRow row = result[0];

			DbValue<String> id;
			id.read(row, _S("id"));
			if(m_id.fromHex(id->to_ascii()) == false)
			{
				OS_ASSERTFALSE();
				return false;
			}

			DbValue<Buffer> public_key;
			public_key.read(row, _S("public_key"));
			m_publicKey = public_key;

			return true;
		}
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return false;
}

bool P2pMachine::store()
{
	if(validate() == false)
		return false;

	try
	{
		shared_ptr<IDbConnection> connection = Engine::instance()->createSystemConnection();
		
		DbSqlValues values;
		values.set(_S("id"), m_id.toHex());
		values.set(_S("public_key"), m_publicKey);

		shared_ptr<IDbStatement> statement = connection->_prepare(connection->prepare_insert(_S("os_machines"), values));
		for(DbSqlValues::iterator i = values.begin(); i != values.end(); i++)
		{
			statement->addParameter()->setValue(i->second);
		}

		statement->execute();
		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return false;
}

bool P2pMachine::update()
{
	if(validate() == false)
		return false;

	try
	{
		shared_ptr<IDbConnection> connection = Engine::instance()->createSystemConnection();
		
		DbSqlValues values;
		values.set(_S("id"), m_id.toHex());
		values.set(_S("public_key"), m_publicKey);

		shared_ptr<IDbStatement> statement = connection->_prepare(connection->prepare_update(_S("os_machines"), values, _S("id"), Convert::toSQL(m_id.toHex())));
		for(DbSqlValues::iterator i = values.begin(); i != values.end(); i++)
		{
			statement->addParameter()->setValue(i->second);
		}

		statement->execute();
		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return false;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
