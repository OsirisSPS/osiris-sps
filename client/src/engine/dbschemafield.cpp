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
#include "dbschemafield.h"

#include "convert.h"
#include "conversions.h"
#include "dbschematable.h"
#include "dbsqlactions.h"
#include "platformmanager.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String DbSchemaField::NAME = _S("name");
const String DbSchemaField::OLD_NAME = _S("old_name");
const String DbSchemaField::TYPE = _S("type");
const String DbSchemaField::SIZE = _S("size");
const String DbSchemaField::ALLOW_NULL = _S("nullptr");
const String DbSchemaField::DEFAULT = _S("default");
const String DbSchemaField::PRIMARY = _S("primary");

const String DbSchemaField::TYPE_BOOL = _S("bool");
const String DbSchemaField::TYPE_INT = _S("int");
const String DbSchemaField::TYPE_UINT = _S("uint");
const String DbSchemaField::TYPE_DOUBLE = _S("double");
const String DbSchemaField::TYPE_VARCHAR = _S("varchar");
const String DbSchemaField::TYPE_TEXT = _S("text");		
const String DbSchemaField::TYPE_BLOB = _S("blob");		

//////////////////////////////////////////////////////////////////////

DbSchemaField::DbSchemaField(shared_ptr<DbSchemaTable> table) : m_table(table)
{
	m_size = 0;
	m_null = false;	
	m_primary = false;
}

DbSchemaField::~DbSchemaField()
{

}

const String & DbSchemaField::getTableName() const
{
	shared_ptr<DbSchemaTable> table = getTable();
	if(table != nullptr)
		return table->getName();

	return String::EMPTY;
}

bool DbSchemaField::parse(shared_ptr<XMLNode> nodeField)
{
	OS_ASSERT(nodeField != nullptr);
	if(nodeField == nullptr)
		return false;

	if(_parseName(nodeField->getAttributeString(NAME)) == false)
		return false;

	if(_parseOldName(nodeField->getAttributeString(OLD_NAME)) == false)
		return false;

	if(_parseType(nodeField->getAttributeString(TYPE)) == false)
		return false;

	if(_parseSize(nodeField->getAttributeString(SIZE)) == false)
		return false;

	if(_parseNull(nodeField->getAttributeString(ALLOW_NULL)) == false)
		return false;

	if(_parseDefault(nodeField->getAttributeString(DEFAULT)) == false)
		return false;

	if(_parsePrimary(nodeField->getAttributeString(PRIMARY)) == false)
		return false;

	return true;
}

bool DbSchemaField::compare(shared_ptr<DbSchemaField> second)
{
	if(getName() != second->getName())
		return false;
	if(getType() != second->getType())
		return false;
	if(getSize() != second->getSize())
		return false;
	if(getNull() != second->getNull())
		return false;
	if(getDefault() != second->getDefault())
		return false;
	if(getPrimary() != second->getPrimary())
		return false;

	return true;
}

bool DbSchemaField::_parseName(const String &value)
{
	m_name = value;
	return m_name.empty() == false;
}

bool DbSchemaField::_parseOldName(const String &value)
{
	OS_ASSERT(m_name.empty() == false);

	m_oldname = value;
	if(m_oldname.empty())
		return true;

	return m_oldname != m_name;
}

bool DbSchemaField::_parseType(const String &value)
{
	if(value == TYPE_BOOL)
	{
		m_type = ftBool;
		return true;
	}

	if(value == TYPE_INT)
	{
		m_type = ftInt;
		return true;
	}

	if(value == TYPE_UINT)
	{
		m_type = ftUint;
		return true;
	}

	if(value == TYPE_DOUBLE)
	{
		m_type = ftDouble;
		return true;
	}

	if(value == TYPE_VARCHAR)
	{
		m_type = ftVarchar;
		return true;
	}

	if(value == TYPE_TEXT)
	{
		m_type = ftText;
		return true;
	}

	if(value == TYPE_BLOB)
	{
		m_type = ftBlob;
		return true;
	}

	m_type = ftUnknown;
	return false;
}

bool DbSchemaField::_parseSize(const String &value)
{
	if(value.empty())
		m_size = 0;
	else
		m_size = conversions::from_utf16<uint32>(value);

	return true;
}

bool DbSchemaField::_parseNull(const String &value)
{
	if(value.empty())
		m_null = false;
	else
		m_null = conversions::from_utf16<bool>(value);

	// CLODOURGENT
	// Il problema qui è che entity, pov e altri durante la conversione sarebbero vuoti...
	m_null = true;

	return true;
}

bool DbSchemaField::_parseDefault(const String &value)
{
	m_default = value;
	return true;
}

bool DbSchemaField::_parsePrimary(const String &value)
{
	if(value.empty())
		m_primary = false;
	else
        m_primary = conversions::from_utf16<bool>(value);

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
