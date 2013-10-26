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

#ifndef _IDBDRIVER_H
#define _IDBDRIVER_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IDbConnection;
class IDbOptions;

//////////////////////////////////////////////////////////////////////

class DatabaseExport IDbDriver : public Object
{
// Construction
public:
	IDbDriver();
	virtual ~IDbDriver();

// Interface
public:
	// Restituisce il nome del driver
	virtual std::string getName() const = 0;
	// Restituisce la descrizione del driver
	virtual std::string getDescription() const = 0;
	// Restituisce le opzioni del driver
	virtual shared_ptr<IDbOptions> createOptions() = 0;
	// Crea una nuova connessione
	virtual shared_ptr<IDbConnection> createConnection(const std::string &database, shared_ptr<IDbOptions> options) = 0;
	// Crea un nuovo database
	virtual bool createDatabase(const std::string &name, shared_ptr<IDbOptions> options) = 0;
	// Rimuove un database esistente
	virtual bool removeDatabase(const std::string &name, shared_ptr<IDbOptions> options) = 0;
	// Indica se un database esiste
	virtual bool databaseExists(const std::string &name, shared_ptr<IDbOptions> options) const = 0;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDBDRIVER_H
