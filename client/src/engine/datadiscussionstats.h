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

#ifndef _ENGINE_DATADISCUSSIONSTATS_H
#define _ENGINE_DATADISCUSSIONSTATS_H

#include "dataistatistics.h"
#include "ids.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport DataDiscussionStats : public DataIStatistics
{
	typedef DataIStatistics StatisticsBase;

// Construction
public:
	DataDiscussionStats();
	virtual ~DataDiscussionStats();

// IRecord interface
public:
	virtual String getTableName() const;

// IStatistics overrides
public:
	virtual void exportXML(shared_ptr<XMLPortalExporter> exporter);

protected:
	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:
	DbValue<EntityID> parent;
	DbValue<uint32> posts;
	DbValue<uint32> votes;
	DbValue<double> votes_average;
	DbValue<EntityID> last_entry;
	DbValue<DateTime> last_entry_date;
	DbValue<bool> pinned;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<DataDiscussionStats> discussion_stats_cast(shared_ptr<DataIStatistics> statistic) { return boost::dynamic_pointer_cast<DataDiscussionStats>(statistic); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_DATADISCUSSIONSTATS_H
