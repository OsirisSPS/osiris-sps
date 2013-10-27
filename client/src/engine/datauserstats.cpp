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
#include "datauserstats.h"

#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DataUserStats::DataUserStats()
{
	objects = 0;
	revisions = 0;
}

DataUserStats::~DataUserStats()
{

}

String DataUserStats::getTableName() const
{
	return DBTABLES::USERS_STATS_TABLE;
}

void DataUserStats::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	StatisticsBase::exportXML(exporter);

	shared_ptr<XMLNode> node = exporter->getNodeStats();
	node->setAttributeUint32(DBTABLES::USERS_STATS::OBJECTS, objects);
	node->setAttributeUint32(DBTABLES::USERS_STATS::REVISIONS, revisions);
	if(progressive_number.isNull() == false)
		node->setAttributeUint32(DBTABLES::USERS_STATS::PROGRESSIVE_NUMBER, progressive_number);
}

bool DataUserStats::onRead(const DataTableRow &row)
{
	if(StatisticsBase::onRead(row) == false)
		return false;

	objects.read(row, DBTABLES::USERS_STATS::OBJECTS);
	revisions.read(row, DBTABLES::USERS_STATS::REVISIONS);
	progressive_number.read(row, DBTABLES::USERS_STATS::PROGRESSIVE_NUMBER);

	return true;
}

bool DataUserStats::onWrite(DbSqlValues &values) const
{
	if(StatisticsBase::onWrite(values) == false)
		return false;

	objects.write(values, DBTABLES::USERS_STATS::OBJECTS);
	revisions.write(values, DBTABLES::USERS_STATS::REVISIONS);
	progressive_number.write(values, DBTABLES::USERS_STATS::PROGRESSIVE_NUMBER);

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
