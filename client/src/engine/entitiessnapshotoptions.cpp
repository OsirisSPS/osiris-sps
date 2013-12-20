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
#include "entitiessnapshotoptions.h"

#include "conversions.h"
#include "datetime.h"
#include "dbdatabasessystem.h"
#include "idbdriver.h"
#include "idboptions.h"
#include "isisendpoint.h"
#include "lock.h"
#include "log.h"

#include "option.h"
#include "options.h"
#include "osiriscommon.h"
#include "portalsportalssystem.h"
#include "xmlattribute.h"
#include "xmlattributes.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"
#include "xmlschema.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace entities {

//////////////////////////////////////////////////////////////////////

const String SnapshotOptions::NAME = _S("name");
const String SnapshotOptions::DESCRIPTION = _S("description");

//////////////////////////////////////////////////////////////////////

const String SnapshotOptions::options::name = _S("name");
const String SnapshotOptions::options::description = _S("description");

//////////////////////////////////////////////////////////////////////

SnapshotOptions::SnapshotOptions(const PortalID &portalID) : m_portalID(portalID)
{
	m_snapshotOptions.ensureOption(options::name, String::EMPTY, false);
	m_snapshotOptions.ensureOption(options::description, String::EMPTY, false);

}

SnapshotOptions::~SnapshotOptions()
{

}

String SnapshotOptions::getName() const
{
	return m_snapshotOptions.getOption(options::name);
}

void SnapshotOptions::setName(const String &name)
{
	m_snapshotOptions.setOption(options::name, name);
}

String SnapshotOptions::getDescription() const
{
	return m_snapshotOptions.getOption(options::description);
}

void SnapshotOptions::setDescription(const String &description)
{
	m_snapshotOptions.setOption(options::description, description);
}


void SnapshotOptions::clear()
{
	m_snapshotOptions.toDefault();
}

bool SnapshotOptions::readFile(const String &filename)
{
	shared_ptr<XMLSchema> schema(OS_NEW XMLSchema());
	schema->parseFile(utils::makeFilePath(utils::makeFolderPath(Options::instance()->getSharePath(),OS_SCHEMAS_PATH), OS_SCHEMA_OPTIONS));

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument(schema));
	if(document->parseFile(filename) == false)
		return false;

	return _readXML(document);
}

bool SnapshotOptions::writeFile(const String &filename) const
{
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	if(_writeXML(document) == false)
		return false;

	return document->writeFile(filename);
}

bool SnapshotOptions::validate() const
{
	if(getName().empty())
		return false;

	if(getDescription().empty())
		return false;

	return true;
}

void SnapshotOptions::unload()
{
}

bool SnapshotOptions::_readXML(shared_ptr<XMLDocument> document)
{
	clear();

	shared_ptr<XMLNode> root = document->getRoot();
	if(root == nullptr)
		return false;

	m_snapshotOptions.parseXMLNode(root);

	return validate();
}

bool SnapshotOptions::_writeXML(shared_ptr<XMLDocument> document) const
{
	if(validate() == false)
		return false;

	document = m_snapshotOptions.toXML(false);

	return true;
}

//////////////////////////////////////////////////////////////////////

} // entities
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
