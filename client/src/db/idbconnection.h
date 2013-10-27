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

#ifndef _IDBCONNECTION_H
#define _IDBCONNECTION_H

#include "base/object.h"
#include "dbdb.h"
#include "dbsqlvalues.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DataTable;
class IDbDriver;
class IDbOptions;
class IDbResult;
class IDbStatement;

//////////////////////////////////////////////////////////////////////

class DatabaseExport IDbConnection : public Object
{
// Construction
public:
	IDbConnection(const String &database, shared_ptr<IDbOptions> options, shared_ptr<IDbDriver> driver);
	virtual ~IDbConnection();

// Attributes
public:
	// Restituisce il database di riferimento
	inline const String & getDatabase() const;
	// Restitusce le opzioni della connessione
	inline shared_ptr<IDbOptions> getOptions() const;
	// Restituisce il driver della connessione
	inline shared_ptr<IDbDriver> getDriver() const;

// Interface
public:
	// Apre la connessione
	virtual void open() = 0;
	// Chiude la connessione
	virtual void close() = 0;
	// Apre una transazione
	virtual void beginTransaction() = 0;
	// Committa la transazione corrente
	virtual void commit() = 0;
	// Annulla la transazione corrente
	virtual void rollback() = 0;
	// Esegue un comando sql e restituisce il numero di record affetti
	virtual uint32 _execute(const String &sql) = 0;
	// Prepara l'esecuzione di una query
	virtual shared_ptr<IDbStatement> _prepare(const String &sql) = 0;
	// Esegue una query e valorizza il risultato in caso di successo
	virtual shared_ptr<IDbResult> _query(const String &sql) = 0;
	// Genera l'sql relativo al comando specificato
	virtual void _parse(shared_ptr<DbSqlICommand> command, String &sql) = 0;
	// Effettua l'analisi delle tabelle
	virtual void analyze() = 0;
	// Ricostruisce gli indici
	virtual void reindex() = 0;
	// Compatta il database
	virtual void vacuum() = 0;

// Operations
public:
	void logError(const String &sql, std::exception &e) const;
	uint32 execute(const String &sql);
	shared_ptr<IDbResult> query(const String &sql);
	bool executeStatement(const String &sql, const DbSqlValues &values);
		
	uint32 execute(shared_ptr<DbSqlICommand> command);
	shared_ptr<IDbResult> query(shared_ptr<DbSqlICommand> select);		
	bool query(shared_ptr<DbSqlICommand> select, DataTable &table);
	bool query(const String &sql, DataTable &table);	
	
	uint32 value_of(const String &sql);
	uint32 value_of(shared_ptr<DbSqlICommand> command);
	

// Overridables
public:
	virtual String escape(const String &name) const;

	// TODO: temp
	virtual String prepare_insert(const String &table, const DbSqlValues &values) const;
	virtual String prepare_update(const String &table, const DbSqlValues &values, const String &field, const String &value) const;
	virtual String prepare_update(const String &table, const DbSqlValues &values, const String &f1, const String &v1, const String &f2, const String &v2) const;
	virtual String sql_remove(const String &table, const String &field, const String &value) const;
	virtual String sql_remove(const String &table, const String &f1, const String &v1, const String &f2, const String &v2) const;

protected:
	String m_database;
	shared_ptr<IDbOptions> m_options;
	weak_ptr<IDbDriver> m_driver;
};

//////////////////////////////////////////////////////////////////////

inline const String & IDbConnection::getDatabase() const { return m_database; }
inline shared_ptr<IDbOptions> IDbConnection::getOptions() const { return m_options; }
inline shared_ptr<IDbDriver> IDbConnection::getDriver() const { return m_driver.lock(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDBCONNECTION_H
