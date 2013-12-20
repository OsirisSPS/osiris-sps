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
#include "dbschemaschema.h"

#include "convert.h"
#include "dbschematable.h"
#include "dbsqlactions.h"
#include "dbsqlicommand.h"
#include "options.h"
#include "osiriscommon.h"
#include "platformmanager.h"
#include "xmlattributes.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"
#include "xmlschema.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String DbSchemaSchema::SCHEMA = _S("schema");
const String DbSchemaSchema::TABLES = _S("tables");
const String DbSchemaSchema::TABLE = _S("table");
const String DbSchemaSchema::VERSION = _S("version");
const String DbSchemaSchema::PREV_VERSION = _S("prev_version");

//////////////////////////////////////////////////////////////////////

DbSchemaSchema::DbSchemaSchema()
{

}

DbSchemaSchema::~DbSchemaSchema()
{
	clear();
}

shared_ptr<DbSchemaTable> DbSchemaSchema::getTable(const String &name, bool checkOldName) const
{
	for(DbSchemaTables::const_iterator i = m_tables.begin(); i != m_tables.end(); ++i)
	{
		if((*i)->getName().compare_no_case(name))
			return *i;

		if(checkOldName && (*i)->getOldName().compare_no_case(name))
			return *i;
	}

	return nullptr;
}

bool DbSchemaSchema::parse(const String &filename)
{
	shared_ptr<XMLSchema> schema(OS_NEW XMLSchema());
	schema->parseFile(utils::makeFilePath(utils::makeFolderPath(Options::instance()->getSharePath(),OS_SCHEMAS_PATH), OS_SCHEMA_DB));

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument(schema));
	if(document->parseFile(filename) == false)
		return false;

	shared_ptr<XMLNode> nodeRoot = document->getRoot();
	if(nodeRoot == nullptr)
		return false;

	return _parseSchema(nodeRoot);
}

bool DbSchemaSchema::generateDifferences(shared_ptr<DbSchemaSchema> new_schema, DbSqlCommands &commands)
{
	commands.clear();

	// Effettua il confronto solo se lo schema specificato  una nuova versione
	if(getVersion() != new_schema->getPrevVersion())
		return false;

	// Scorre tutte le tabelle correnti e controlla se sono state eliminate nella nuova versione
    for(DbSchemaTables::const_iterator ct = m_tables.begin(); ct != m_tables.end(); ++ct)
	{
		shared_ptr<DbSchemaTable> current_table = *ct;
		// Controlla se nel nuovo schema non figura la tabella corrente
		if(new_schema->hasTable(current_table->getName(), true) == false)
		{
			OS_TRACE(String::format(_S("table '%S' dropped\n").c_str(), current_table->getName().c_str()).to_ascii());

			shared_ptr<DbSqlICommand> dropTable(OS_NEW DbSqlDropTable(current_table->getName()));
			commands.push_back(dropTable);
		}
	}

	const DbSchemaTables &new_tables = new_schema->getTables();
	// Scorre le tabelle del nuovo schema
	for(DbSchemaTables::const_iterator nt = new_tables.begin(); nt != new_tables.end(); ++nt)
	{
		shared_ptr<DbSchemaTable> new_table = *nt;
		shared_ptr<DbSchemaTable> old_table = nullptr;

		// Controlla se la tabella  ancora presente
		if(hasTable(new_table->getName()))
		{
			old_table = getTable(new_table->getName());
		}
		else
		{
			// Controlla se la tabella  stata rinominata
			if(new_table->wasRenamed())
			{
				old_table = getTable(new_table->getOldName());
				if(old_table == nullptr)
				{
					OS_TRACE(String::format(_S("table renamed but not found: oldname='%S', newname='%S'\n").c_str(), new_table->getOldName().c_str(), new_table->getName().c_str()).to_ascii());
					return false;
				}
			}
			else
			{
				OS_TRACE(String::format(_S("table added: '%S'\n").c_str(), new_table->getName().c_str()).to_ascii());

				shared_ptr<DbSqlICommand> createTable(OS_NEW DbSqlCreateTable(new_table));
				commands.push_back(createTable);
			}
		}

		// Se  stata trova la vecchia tabella la allinea a quella nuova
		if(old_table != nullptr && old_table->generateDifferences(new_table, commands) == false)
			return false;
	}

	return true;
}

void DbSchemaSchema::_addTable(shared_ptr<DbSchemaTable> table)
{
	OS_EXCEPT_IF(hasTable(table->getName()), String::format(_S("Database schema error: duplicated table '%S'").c_str(), table->getName().c_str()).to_ascii());
	m_tables.push_back(table);
}

bool DbSchemaSchema::_parseSchema(shared_ptr<XMLNode> schema)
{
	clear();

	// Carica le opzioni dello schema
	_parseOptions(schema);

	shared_ptr<XMLNode> tables = schema->getNode(TABLES);
	if(tables == nullptr)
		return false;

	// Carica le tabelle dello schema
	return _parseTables(tables);
}

bool DbSchemaSchema::_parseOptions(shared_ptr<XMLNode> schema)
{
	if(m_version.fromString(schema->getAttributeString(VERSION).to_ascii()) == false)
		return false;

	if(schema->getAttributes()->exists(PREV_VERSION))
	{
		if(m_prevVersion.fromString(schema->getAttributeString(PREV_VERSION).to_ascii()) == false)
			return false;

		if(m_prevVersion >= m_version)
			return false;
	}

	return true;
}

bool DbSchemaSchema::_parseTables(shared_ptr<XMLNode> tables)
{
	shared_ptr<XMLNodes> childs = tables->getNodes();
	for(XMLNodes::const_iterator i = childs->begin(); i != childs->end(); ++i)
	{
		if(_parseTable(*i) == false)
			return false;
	}

	return true;
}

bool DbSchemaSchema::_parseTable(shared_ptr<XMLNode> table)
{
	shared_ptr<DbSchemaTable> schema_table(OS_NEW DbSchemaTable(get_this_ptr()));
	if(schema_table->parse(table) == false)
		return false;

	_addTable(schema_table);
	return true;
}

void DbSchemaSchema::clear()
{
	m_version.clear();
	m_prevVersion.clear();
	m_tables.clear();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
