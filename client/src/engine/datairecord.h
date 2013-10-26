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

#ifndef _ENGINE_DATAIRECORD_H
#define _ENGINE_DATAIRECORD_H

#include "base/object.h"
#include "datadata.h"
#include "enable_this_ptr.h"
#include "languageresult.h"
#include "dbdb.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DataTableRow;
class IDbConnection;
class IPortalDatabase;

//////////////////////////////////////////////////////////////////////

class EngineExport DataIRecord : public enable_this_ptr<DataIRecord>,
									public Object
{
	typedef Object BaseClass;

// Construction
public:
	DataIRecord(DataRecordType record_type);
	virtual ~DataIRecord();

// Attributes
public:
	inline DataRecordType getRecordType() const;

	inline bool isAccount() const;
	inline bool isEntry() const;
	inline bool isObject() const;
	inline bool isPeer() const;
	inline bool isStatistics() const;
	inline bool isLocalMessage() const;

// Operations
public:
	// Carica la struttura da una riga di una tabella
	bool read(const DataTableRow &row);
	// Salva la struttura in una struttura VALUES
	bool write(DbSqlValues &values) const;
	// Database insert/update/delete
	bool insert(shared_ptr<IDbConnection> connection);	
	bool update(shared_ptr<IDbConnection> connection);
	bool remove(shared_ptr<IDbConnection> connection);

// Overridables
public:
	// Valida il record
	virtual bool validate(shared_ptr<IPortalDatabase> database) const;
	// Is acceptable? Under "validate" only check about syntax, under "acceptable" all other kind of check.
	//virtual LanguageResult acceptable(shared_ptr<IPortalDatabase> database) const;
	// Table name
	virtual String getTableName() const = 0;
	// Unique Key Field, for update/delete on database
	virtual String getRecordKeyName() const = 0;
	// Unique Key Value, String only expected.
	virtual String getRecordKeyValue() const = 0; 
	// Second Key Field, optional
	virtual String getRecordKey2Name() const;
	virtual String getRecordKey2Value() const; 

// Interface
protected:
	virtual bool onRead(const DataTableRow &row) = 0;
	virtual bool onWrite(DbSqlValues &values) const = 0;

protected:
	const DataRecordType m_recordType;
};

//////////////////////////////////////////////////////////////////////

inline DataRecordType DataIRecord::getRecordType() const { return m_recordType; }

inline bool DataIRecord::isAccount() const { return getRecordType() == dataRecordTypeAccount; }
inline bool DataIRecord::isEntry() const { return getRecordType() == dataRecordTypeEntry; }
inline bool DataIRecord::isObject() const { return getRecordType() == dataRecordTypeObject; }
inline bool DataIRecord::isPeer() const { return getRecordType() == dataRecordTypePeer; }
inline bool DataIRecord::isStatistics() const { return getRecordType() == dataRecordTypeStatistics; }
inline bool DataIRecord::isLocalMessage() const { return getRecordType() == dataRecordTypeLocalMessage; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_DATAIRECORD_H
