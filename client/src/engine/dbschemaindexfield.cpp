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
#include "dbschemaindexfield.h"

#include "convert.h"
#include "dbschematable.h"
#include "dbsqlactions.h"
#include "platformmanager.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String DbSchemaIndexField::NAME = _S("name");
const String DbSchemaIndexField::ORDER = _S("order");

const String DbSchemaIndexField::ORDER_ASC = _S("asc");
const String DbSchemaIndexField::ORDER_DESC = _S("desc");

//////////////////////////////////////////////////////////////////////

DbSchemaIndexField::DbSchemaIndexField(shared_ptr<DbSchemaIndex> index) : m_index(index),
										  m_order(ifoDefault)
{

}

DbSchemaIndexField::~DbSchemaIndexField()
{

}

bool DbSchemaIndexField::parse(shared_ptr<XMLNode> nodeField)
{
	OS_ASSERT(nodeField != nullptr);
	if(nodeField == nullptr)
		return false;

	if(_parseName(nodeField->getAttributeString(NAME)) == false)
		return false;

	if(_parseOrder(nodeField->getAttributeString(ORDER)) == false)
		return false;

	return true;
}

bool DbSchemaIndexField::compare(shared_ptr<DbSchemaIndexField> second)
{
	OS_ASSERT(second != nullptr);

	if(getName() != second->getName())
		return false;

	if(getOrder() != second->getOrder())
		return false;

	return true;
}

bool DbSchemaIndexField::_parseName(const String &value)
{
	m_name = value;
	return m_name.empty() == false;
}

bool DbSchemaIndexField::_parseOrder(const String &value)
{
	if(value.empty())
	{
		m_order = ifoDefault;
		return true;
	}

	if(value.compare_no_case(ORDER_ASC))
	{
		m_order = ifoAsc;
		return true;
	}

	if(value.compare_no_case(ORDER_DESC))
	{
		m_order = ifoDesc;
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
