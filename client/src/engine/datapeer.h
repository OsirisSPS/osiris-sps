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

#ifndef _ENGINE_DATAPEER_H
#define _ENGINE_DATAPEER_H

#include "dbvalue.h"
#include "datairecord.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport DataPeer : public DataIRecord
{
	typedef DataIRecord RecordBase;

// Construction
public:
	DataPeer();
	virtual ~DataPeer();

// IRecord interface
protected:
	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

// IRecord overrides
public:
	virtual bool validate(shared_ptr<IPortalDatabase> database) const;
	virtual String getTableName() const;
	virtual String getRecordKeyName() const;
	virtual String getRecordKeyValue() const; 

public:
	DbValue<String> ip;
	DbValue<uint32> port;	
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<DataPeer> peer_cast(shared_ptr<DataIRecord> record) { return boost::dynamic_pointer_cast<DataPeer>(record); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_DATAPEER_H
