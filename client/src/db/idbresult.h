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

#ifndef _IDBRESULT_H
#define _IDBRESULT_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DataItem;
class DataTable;
class DataTableRow;

//////////////////////////////////////////////////////////////////////

class DatabaseExport IDbResult : public Object
{
// Construction
public:
	IDbResult();
	virtual ~IDbResult();

// Operations
public:
	// Prepara una tabella per il bind dei records
	void init(DataTable &table);
	// Effettua il bind del record corrente con una riga di una tabella precedentemente inizializzata
	void bind(DataTableRow &row);
	// Effettua il bind di tutti i record con una tabella
	void bind(DataTable &table);

// Interface
public:
	// Restituisce il numero di colonne
	virtual uint32 columns() = 0;
	// Restituisce il nome di una colonna
	virtual String columnName(uint32 index) = 0;

	// Indica se si  raggiunta la posizione finale
	virtual bool end() = 0;

	// Si posiziona sul record successivo
	virtual void moveNext() = 0;

	// Legge il valore della colonna specificata del record corrente
	virtual void get(uint32 index, DataItem &value) = 0;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDBRESULT_H
