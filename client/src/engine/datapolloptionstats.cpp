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
#include "datapolloptionstats.h"

#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DataPollOptionStats::DataPollOptionStats()
{
	votes = 0;
	votes_average = 0;
}

DataPollOptionStats::~DataPollOptionStats()
{

}

String DataPollOptionStats::getTableName() const
{
	return DBTABLES::POLLS_OPTIONS_STATS_TABLE;
}

void DataPollOptionStats::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	StatisticsBase::exportXML(exporter);

	shared_ptr<XMLNode> node = exporter->getNodeStats();
	node->setAttributeUint32(DBTABLES::POLLS_OPTIONS_STATS::VOTES, votes);
	node->setAttributeDouble(DBTABLES::POLLS_OPTIONS_STATS::VOTES_AVERAGE, votes_average);
}

bool DataPollOptionStats::onRead(const DataTableRow &row)
{
	if(StatisticsBase::onRead(row) == false)
		return false;

	votes.read(row, DBTABLES::POLLS_OPTIONS_STATS::VOTES);
	votes_average.read(row, DBTABLES::POLLS_OPTIONS_STATS::VOTES_AVERAGE);

	return true;
}

bool DataPollOptionStats::onWrite(DbSqlValues &values) const
{
	if(StatisticsBase::onWrite(values) == false)
		return false;

	votes.write(values, DBTABLES::POLLS_OPTIONS_STATS::VOTES);
	votes_average.write(values, DBTABLES::POLLS_OPTIONS_STATS::VOTES_AVERAGE);

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
