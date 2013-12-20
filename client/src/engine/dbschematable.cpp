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
#include "dbschematable.h"

#include "dbschemafield.h"
#include "dbschemaindex.h"
#include "dbsqlactions.h"
#include "log.h"
#include "platformmanager.h"
#include "xmlnode.h"
#include "xmlnodes.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String DbSchemaTable::NAME = _S("name");
const String DbSchemaTable::OLD_NAME = _S("old_name");
const String DbSchemaTable::FIELDS = _S("fields");
const String DbSchemaTable::FIELD = _S("field");
const String DbSchemaTable::INDEXES = _S("indexes");
const String DbSchemaTable::INDEX = _S("index");

//////////////////////////////////////////////////////////////////////

DbSchemaTable::DbSchemaTable(shared_ptr<DbSchemaSchema> schema) : m_schema(schema)
{

}

DbSchemaTable::~DbSchemaTable()
{
	clear();
}

shared_ptr<DbSchemaField> DbSchemaTable::getField(const String &name, bool checkOldName) const
{
	for(DbSchemaFields::const_iterator i = m_fields.begin(); i != m_fields.end(); ++i)
	{
		if((*i)->getName().compare_no_case(name))
			return *i;

		if(checkOldName && (*i)->getOldName().compare_no_case(name))
			return *i;
	}

	return nullptr;
}

shared_ptr<DbSchemaIndex> DbSchemaTable::getIndex(const String &name) const
{
	for(DbSchemaIndexes::const_iterator i = m_indexes.begin(); i != m_indexes.end(); ++i)
	{
		if((*i)->getName().compare_no_case(name))
			return *i;
	}

	return nullptr;
}

bool DbSchemaTable::parse(shared_ptr<XMLNode> table)
{
	OS_ASSERT(table != nullptr);

	m_name = table->getAttributeString(NAME);
	if(m_name.empty())
		return false;

	m_oldname = table->getAttributeString(OLD_NAME);
	if(m_oldname.empty() == false && m_oldname == m_name)
		return false;

	shared_ptr<XMLNode> nodeFields = table->getNode(FIELDS);
	if(nodeFields != nullptr && _parseFields(nodeFields) == false)
		return false;

	shared_ptr<XMLNode> nodeIndexes = table->getNode(INDEXES);
	if(nodeIndexes != nullptr && _parseIndexes(nodeIndexes) == false)
		return false;

	return true;
}

bool DbSchemaTable::generateDifferences(shared_ptr<DbSchemaTable> new_table, DbSqlCommands &commands)
{
	// N.B.: per compatibilità con i driver supportati le modifiche consentite a livello di tabella
	// sono la rinominazione della tabella e l'aggiunta di un campo IN CODA alla tabella, qualunque altra modifica
	// come l'alterazione o la rimozione di un campo comporta la ricostruzione dell'intera tabella

	shared_ptr<DbSqlRenameTable> commandRenameTable;
	if(getName() != new_table->getName())
	{
		OS_TRACE(String::format(_S("table renamed: oldname='%S', newname='%S'\n").c_str(), getName().c_str(), new_table->getName().c_str()).to_ascii());
		commandRenameTable.reset(OS_NEW DbSqlRenameTable(getName(), new_table->getName()));
	}

	bool rebuildTable = false;
	shared_ptr<DbSqlRebuildTable> commandRebuildTable(OS_NEW DbSqlRebuildTable(new_table, get_this_ptr()));

	// Scorre tutti i campi correnti e controlla se sono state eliminati nella nuova versione
    for(DbSchemaFields::const_iterator cf = m_fields.begin(); cf != m_fields.end(); ++cf)
	{
		shared_ptr<DbSchemaField> current_field = *cf;
		// Controlla se nella nuova tabella non figura il campo corrente
		if(new_table->hasField(current_field->getName(), true) == false)
		{
			rebuildTable = true;
			OS_TRACE(String::format(_S("field '%S' removed from table '%S'\n").c_str(), current_field->getName().c_str(), getName().c_str()).to_ascii());
		}
	}

	uint32 fieldsCount = getFieldsCount();
	uint32 currentField = 0;

	DbSqlCommands newFields;

	const DbSchemaFields &new_fields = new_table->getFields();
	// Scorre i campi della nuova tabella
	for(DbSchemaFields::const_iterator nf = new_fields.begin(); nf != new_fields.end(); ++nf)
	{
		shared_ptr<DbSchemaField> new_field = *nf;
		shared_ptr<DbSchemaField> old_field = nullptr;

		// Controlla se il campo è ancora presente
		if(hasField(new_field->getName()))
		{
			old_field = getField(new_field->getName());
		}
		else
		{
			// Controlla se il campo è stato rinominato
			if(new_field->wasRenamed())
			{
				old_field = getField(new_field->getOldName());
				if(old_field == nullptr)
				{
					OS_LOG_ERROR(String::format(_S("field in table '%S' has been renamed but wasn't not found in the old table: oldname='%S', newname='%S'\n").c_str(), getName().c_str(), new_field->getOldName().c_str(), new_field->getName().c_str()));
					return false;
				}

				rebuildTable = true;
			}
			else
			{
				OS_TRACE(String::format(_S("field '%S' added in table '%S'\n").c_str(), new_field->getName().c_str(), new_field->getTableName().c_str()).to_ascii());

				if(currentField < fieldsCount)
				{
					// N.B.: non tutti i driver supportato l'aggiunta di un nuovo campo che non sia alla fine della tabella,
					// perciò, per compatibilità, è necessaria la ricostruzione della tabella in tale circostanza
					rebuildTable = true;
				}
				else
				{
					shared_ptr<DbSqlICommand> createField(OS_NEW DbSqlCreateField(new_field));
					newFields.push_back(createField);
				}
			}
		}

		// Se è stato trovo il vecchio campo lo allinea con quello nuovo
		if(old_field != nullptr)
		{
			commandRebuildTable->importField(new_field->getName(), old_field->getName());

			if(old_field->compare(new_field) == false)
			{
				OS_TRACE(String::format(_S("field '%S' was changed in table '%S'\n").c_str(), new_field->getName().c_str(), new_field->getTableName().c_str()).to_ascii());
				rebuildTable = true;
			}
		}

		currentField++;
	}

	// Controlla se è necessario ricostruire la tabella
	if(rebuildTable)
	{
		// N.B.: se la tabella viene ricostruita non è necessario accodare il comando di rinominazione se esiste (è implicito)
		commands.push_back(commandRebuildTable);
	}
	else
	{
		// Nel caso ci siano solo nuovi campi un'eventuale rinominazione va effettuata in precedenza perchè
		// i nuovi campi sono basati sulla nuova tabella
		if(commandRenameTable != nullptr)
			commands.push_back(commandRenameTable);

		// Esporta i comandi di creazione dei nuovi campi
		while(newFields.empty() == false)
		{
			commands.push_back(newFields.front());
			newFields.pop_front();
		}

		// Effettua il controllo sugli indici solo nel caso in cui non si debba ricostruire la tabella e dopo aver accodato le modifiche sui campi

		// Scorre tutti gli indici correnti e controlla se sono stati eliminati nella nuova versione
	    for(DbSchemaIndexes::const_iterator ci = m_indexes.begin(); ci != m_indexes.end(); ++ci)
		{
			shared_ptr<DbSchemaIndex> current_index = *ci;
			// Controlla se nella nuova tabella non figura l'indice corrente
			if(new_table->hasIndex(current_index->getName()) == false)
			{
				OS_TRACE(String::format(_S("index '%S' removed from table '%S'\n").c_str(), current_index->getName().c_str(), getName().c_str()).to_ascii());
				shared_ptr<DbSqlICommand> commandDropIndex(OS_NEW DbSqlDropIndex(current_index));			
				commands.push_back(commandDropIndex);
			}
		}

		// Scorre gli indici della nuova tabella
		const DbSchemaIndexes &new_indexes = new_table->getIndexes();
		for(DbSchemaIndexes::const_iterator ni = new_indexes.begin(); ni != new_indexes.end(); ++ni)
		{
			shared_ptr<DbSchemaIndex> new_index = *ni;
			
			// Controlla se l'indice è ancora presente
			if(hasIndex(new_index->getName()))
			{
				shared_ptr<DbSchemaIndex> old_index = getIndex(new_index->getName());
				OS_ASSERT(old_index != nullptr);

				if(old_index->compare(new_index) == false)
				{
					OS_TRACE(String::format(_S("index '%S' was changed in table '%S'\n").c_str(), new_index->getName().c_str(), new_index->getTableName().c_str()).to_ascii());

					// Ricostruisce l'indice

					shared_ptr<DbSqlICommand> commandDropIndex(OS_NEW DbSqlDropIndex(old_index));			
					commands.push_back(commandDropIndex);

					shared_ptr<DbSqlICommand> commandCreateIndex(OS_NEW DbSqlCreateIndex(new_index));			
					commands.push_back(commandCreateIndex);
				}
			}
			else
			{
				OS_TRACE(String::format(_S("index '%S' added in table '%S'\n").c_str(), new_index->getName().c_str(), new_index->getTableName().c_str()).to_ascii());

				shared_ptr<DbSqlICommand> commandCreateIndex(OS_NEW DbSqlCreateIndex(new_index));			
				commands.push_back(commandCreateIndex);
			}
		}
	}

	return true;
}

void DbSchemaTable::_addField(shared_ptr<DbSchemaField> field)
{
	if(field == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	OS_EXCEPT_IF(hasField(field->getName()), String::format(_S("Database schema error: duplicated field '%S'").c_str(), field->getName().c_str()).to_ascii());
	m_fields.push_back(field);
}

void DbSchemaTable::_addIndex(shared_ptr<DbSchemaIndex> index)
{
	if(index == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	OS_EXCEPT_IF(hasIndex(index->getName()), String::format(_S("Database schema error: duplicated index '%S'").c_str(), index->getName().c_str()).to_ascii());
	m_indexes.push_back(index);
}

bool DbSchemaTable::_parseFields(shared_ptr<XMLNode> nodeFields)
{
	OS_ASSERT(nodeFields != nullptr);
	if(nodeFields == nullptr)
		return false;

	shared_ptr<XMLNodes> nodeChilds = nodeFields->getNodes();
	for(XMLNodes::const_iterator i = nodeChilds->begin(); i != nodeChilds->end(); ++i)
	{
		if(_parseField(*i) == false)
			return false;
	}

	return true;
}

bool DbSchemaTable::_parseField(shared_ptr<XMLNode> nodeField)
{
	shared_ptr<DbSchemaField> table_field(OS_NEW DbSchemaField(get_this_ptr()));
	if(table_field->parse(nodeField) == false)
		return false;

	_addField(table_field);
	return true;
}

bool DbSchemaTable::_parseIndexes(shared_ptr<XMLNode> nodeIndexes)
{
	OS_ASSERT(nodeIndexes != nullptr);
	if(nodeIndexes == nullptr)
		return false;

	shared_ptr<XMLNodes> nodeChilds = nodeIndexes->getNodes();
	for(XMLNodes::const_iterator i = nodeChilds->begin(); i != nodeChilds->end(); ++i)
	{
		if(_parseIndex(*i) == false)
			return false;
	}

	return true;
}

bool DbSchemaTable::_parseIndex(shared_ptr<XMLNode> nodeIndex)
{
	shared_ptr<DbSchemaIndex> table_index(OS_NEW DbSchemaIndex(get_this_ptr()));
	if(table_index->parse(nodeIndex) == false)
		return false;

	_addIndex(table_index);
	return true;
}

void DbSchemaTable::clear()
{
	m_name.clear();
	m_fields.clear();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
