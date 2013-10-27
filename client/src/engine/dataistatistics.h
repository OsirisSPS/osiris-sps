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

#ifndef _ENGINE_DATAISTATISTICS_H
#define _ENGINE_DATAISTATISTICS_H

#include "dbvalue.h"
#include "entitiesentities.h"
#include "ideide.h"
#include "datairecord.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLPortalExporter;

//////////////////////////////////////////////////////////////////////

class EngineExport DataIStatistics : public DataIRecord
{
	typedef DataIRecord RecordBase;

// Construction
public:
	DataIStatistics();
	virtual ~DataIStatistics();

// Operations
public:
	virtual bool create_ZZZ(shared_ptr<IPortalDatabase> db);

// IRecord overrides
public:
	virtual bool validate(shared_ptr<IPortalDatabase> database) const;
	virtual String getTableName() const;
	virtual String getRecordKeyName() const;
	virtual String getRecordKeyValue() const;

// IRecord interface
protected:
	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

// Overridables
public:
	virtual void exportXML(shared_ptr<XMLPortalExporter> exporter);

public:
	DbValue<ObjectOrEntityID> reference;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<DataIStatistics> statistics_cast(shared_ptr<DataIRecord> record) { return boost::dynamic_pointer_cast<DataIStatistics>(record); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_DATAISTATISTICS_H
