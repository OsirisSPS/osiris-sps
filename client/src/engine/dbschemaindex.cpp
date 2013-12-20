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
#include "dbschemaindex.h"

#include "convert.h"
#include "conversions.h"
#include "dbschematable.h"
#include "dbschemaindexfield.h"
#include "dbsqlactions.h"
#include "platformmanager.h"
#include "xmlnode.h"
#include "xmlnodes.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String DbSchemaIndex::NAME = _S("name");
const String DbSchemaIndex::UNIQUE = _S("unique");
const String DbSchemaIndex::FIELDS = _S("fields");
const String DbSchemaIndex::FIELD = _S("field");

//////////////////////////////////////////////////////////////////////
 
DbSchemaIndex::DbSchemaIndex(shared_ptr<DbSchemaTable> table) : m_table(table)
{
	m_unique = false;
}

DbSchemaIndex::~DbSchemaIndex()
{

}

shared_ptr<DbSchemaIndexField> DbSchemaIndex::getField(const String &name) const
{
	for(DbSchemaIndexFields::const_iterator i = m_fields.begin(); i != m_fields.end(); ++i)
	{
		if((*i)->getName().compare_no_case(name))
			return *i;
	}

	return nullptr;
}

const String & DbSchemaIndex::getTableName() const
{
	shared_ptr<DbSchemaTable> table = getTable();
	if(table != nullptr)
		return table->getName();

	return String::EMPTY;
}

bool DbSchemaIndex::parse(shared_ptr<XMLNode> nodeIndex)
{
	OS_ASSERT(nodeIndex != nullptr);
	if(nodeIndex == nullptr)
		return false;

	if(_parseName(nodeIndex->getAttributeString(NAME)) == false)
		return false;

	if(_parseUnique(nodeIndex->getAttributeString(UNIQUE)) == false)
		return false;

	if(_parseFields(nodeIndex) == false)
		return false;

	// Controlla se l'indice non ha un nome fisso
	if(m_name.empty())
	{
		// Genera in automatico il nome dell'indice
		m_name = _generateIndexName(getTable(), m_fields);
		OS_ASSERT(m_name.empty() == false);
	}

	return true;
}

bool DbSchemaIndex::compare(shared_ptr<DbSchemaIndex> second)
{
	OS_ASSERT(second != nullptr);

	// Controlla se le proprietà dell'indice sono diverse

	if(getName() != second->getName())
		return false;

	if(getUnique() != second->getUnique())
		return false;

	// Controlla se il numero di campi dell'indice è diverso

	uint32 fields = getFieldsCount();
	if(fields != second->getFieldsCount())
		return false;

	// Controlla se i campi dell'indice sono diversi

	for(uint32 i = 0; i < fields; i++)
	{
		if(m_fields[i]->compare(second->m_fields[i]) == false)
			return false;
	}

	return true;
}

void DbSchemaIndex::_addField(shared_ptr<DbSchemaIndexField> field)
{
	if(field == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	OS_EXCEPT_IF(getTable()->hasField(field->getName()) == false, String::format(_S("Database schema error: field '%S' not found in table '%S'").c_str(), field->getName().c_str(), getTable()->getName().c_str()).to_ascii());
	OS_EXCEPT_IF(hasField(field->getName()), String::format(_S("Database schema error: duplicated index field '%S'").c_str(), field->getName().c_str()).to_ascii());

	m_fields.push_back(field);
}

bool DbSchemaIndex::_parseName(const String &value)
{
	m_name = value;
	return true;	// Il nome dell'indice è opzionale
}

bool DbSchemaIndex::_parseUnique(const String &value)
{
	if(value.empty())
		m_unique = false;
	else
        m_unique = conversions::from_utf16<bool>(value);

	return true;
}

bool DbSchemaIndex::_parseFields(shared_ptr<XMLNode> nodeIndex)
{
	OS_ASSERT(nodeIndex != nullptr);
	if(nodeIndex == nullptr)
		return false;

	shared_ptr<XMLNodes> nodeChilds = nodeIndex->getNodes();
	for(XMLNodes::const_iterator i = nodeChilds->begin(); i != nodeChilds->end(); ++i)
	{
		if(_parseField(*i) == false)
			return false;
	}

	return true;
}

bool DbSchemaIndex::_parseField(shared_ptr<XMLNode> nodeField)
{
	shared_ptr<DbSchemaIndexField> indexField(OS_NEW DbSchemaIndexField(get_this_ptr()));
	if(indexField->parse(nodeField) == false)
		return false;

	_addField(indexField);
	return true;
}

String DbSchemaIndex::_generateIndexName(shared_ptr<DbSchemaTable> table, const DbSchemaIndexFields &fields)
{
	if(table == nullptr || fields.empty())
	{
		OS_ASSERTFALSE();
		return String::EMPTY;
	}
	
	String name = table->getName() + _S("_index");	
	for(DbSchemaIndexFields::const_iterator i = fields.begin(); i != fields.end(); ++i)
	{
		name.append(_S("_"));
		name.append((*i)->getName());		
	}

	OS_ASSERT(name.empty() == false);
	return name;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
