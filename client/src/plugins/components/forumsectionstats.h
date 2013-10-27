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

#ifndef _DATA_SECTIONSTATS_H
#define _DATA_SECTIONSTATS_H

#include "dataistatistics.h"
#include "osiriscommon.h"

//////////////////////////////////////////////////////////////////////

// TODO: da sistemare

#ifdef OS_OSIRIS_VERSION_RAZOR
#define OS_FORUMS_SECTION_STATS_TABLE					_S("os_sections_stats")
#else
#define OS_FORUMS_SECTION_STATS_TABLE					_S("os_forums_section_stats")
#endif

#define OS_FORUMS_SECTION_STATS_TOPICS					_S("topics")
#define OS_FORUMS_SECTION_STATS_POSTS					_S("posts")
#define OS_FORUMS_SECTION_STATS_LAST_TOPIC				_S("last_topic")
#define OS_FORUMS_SECTION_STATS_LAST_MESSAGE			_S("last_message")
#define OS_FORUMS_SECTION_STATS_LAST_MESSAGE_DATE		_S("last_message_date")

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace forum {

//////////////////////////////////////////////////////////////////////

class SectionStats : public DataIStatistics
{
	typedef DataIStatistics StatisticsBase;

// Construction
public:
	SectionStats();
	virtual ~SectionStats();

// IStatistics interface
public:
	virtual String getTableName() const;

// IStatistics overrides
public:
	virtual void exportXML(shared_ptr<XMLPortalExporter> exporter);

protected:
	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:
	DbValue<uint32> topics;
	DbValue<uint32> posts;
	DbValue<EntityID> last_topic;
	DbValue<EntityID> last_message;
	DbValue<DateTime> last_message_date;
};

//////////////////////////////////////////////////////////////////////

} // forum
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DATA_SECTIONSTATS_H
