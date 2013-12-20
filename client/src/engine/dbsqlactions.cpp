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
#include "dbsqlactions.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DbSqlCreateTable::DbSqlCreateTable(shared_ptr<DbSchemaTable> table) : DbSqlICommand(ctCreateTable),
																		m_table(table)
{
	OS_ASSERT(m_table != nullptr);
}

DbSqlCreateTable::~DbSqlCreateTable()
{

}

//////////////////////////////////////////////////////////////////////

DbSqlRenameTable::DbSqlRenameTable(const String &oldTable, const String &newTable) : DbSqlICommand(ctRenameTable),
																		   m_oldTable(oldTable),
																		   m_newTable(newTable)
{

}

DbSqlRenameTable::~DbSqlRenameTable()
{

}

//////////////////////////////////////////////////////////////////////

DbSqlDropTable::DbSqlDropTable(const String &table) : DbSqlICommand(ctDropTable),
	  										m_table(table)
{

}

DbSqlDropTable::~DbSqlDropTable()
{

}

//////////////////////////////////////////////////////////////////////

DbSqlCreateField::DbSqlCreateField(shared_ptr<DbSchemaField> field) : DbSqlICommand(ctCreateField),
													m_field(field)
{

}

DbSqlCreateField::~DbSqlCreateField()
{

}

//////////////////////////////////////////////////////////////////////

DbSqlCreateIndex::DbSqlCreateIndex(shared_ptr<DbSchemaIndex> index) : DbSqlICommand(ctCreateIndex),
													m_index(index)
{

}

DbSqlCreateIndex::~DbSqlCreateIndex()
{

}

//////////////////////////////////////////////////////////////////////

DbSqlDropIndex::DbSqlDropIndex(shared_ptr<DbSchemaIndex> index) : DbSqlICommand(ctDropIndex),
												m_index(index)
{

}

DbSqlDropIndex::~DbSqlDropIndex()
{

}

//////////////////////////////////////////////////////////////////////

DbSqlRebuildTable::DbSqlRebuildTable(shared_ptr<DbSchemaTable> newTable, shared_ptr<DbSchemaTable> oldTable) :	DbSqlICommand(ctRebuildTable),
																										m_newTable(newTable),
																										m_oldTable(oldTable)
{

}

DbSqlRebuildTable::~DbSqlRebuildTable()
{

}

void DbSqlRebuildTable::importField(const String &newField, const String &oldField)
{
	m_importFields.push_back(StringPair(newField, oldField));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
