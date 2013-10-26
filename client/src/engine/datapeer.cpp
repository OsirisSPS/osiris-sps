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
#include "datapeer.h"

#include "convert.h"
#include "dbbinder.h"
#include "dbtables.h"
#include "networksystem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DataPeer::DataPeer() : RecordBase(dataRecordTypePeer),
			   port(0)
{

}

DataPeer::~DataPeer()
{

}

bool DataPeer::onRead(const DataTableRow &row)
{
	ip.read(row, DBTABLES::PEERS::IP);
	port.read(row, DBTABLES::PEERS::PORT);
	
	return true;
}

bool DataPeer::onWrite(DbSqlValues &values) const
{
	ip.write(values, DBTABLES::PEERS::IP);
	port.write(values, DBTABLES::PEERS::PORT);
	
	return true;
}

bool DataPeer::validate(shared_ptr<IPortalDatabase> database) const
{
	if(RecordBase::validate(database) == false)
		return false;

	if(NetworkSystem::instance()->validateIP(ip) == false)
		return false;

	return true;
}

String DataPeer::getTableName() const
{
	return DBTABLES::PEERS_TABLE;
}

String DataPeer::getRecordKeyName() const
{
	return DBTABLES::PEERS::IP;
}

String DataPeer::getRecordKeyValue() const
{
	return ip;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
