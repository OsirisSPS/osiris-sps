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
#include "datapollstats.h"

#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DataPollStats::DataPollStats()
{
	poll_votes = 0;
}

DataPollStats::~DataPollStats()
{

}

String DataPollStats::getTableName() const
{
	return DBTABLES::POLLS_STATS_TABLE;
}

void DataPollStats::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	StatisticsBase::exportXML(exporter);

	shared_ptr<XMLNode> node = exporter->getNodeStats();
	node->setAttributeUint32(DBTABLES::POLLS_STATS::POLL_VOTES, poll_votes);
}

bool DataPollStats::onRead(const DataTableRow &row)
{
	if(StatisticsBase::onRead(row) == false)
		return false;

	poll_votes.read(row, DBTABLES::POLLS_STATS::POLL_VOTES);

	return true;
}

bool DataPollStats::onWrite(DbSqlValues &values) const
{
	if(StatisticsBase::onWrite(values) == false)
		return false;

	poll_votes.write(values, DBTABLES::POLLS_STATS::POLL_VOTES);

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
