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
#include "objectsientry.h"

#include "cryptmanager.h"
#include "objectssigner.h"
#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "iideportalpage.h"
#include "datatree.h"
#include "objectsuser.h"
#include "idesession.h"
#include "portalsportal.h"
#include "realtimestats.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ObjectsIEntry::ObjectsIEntry(PortalObjectType object_type, uint32 object_version) : ObjectBase(object_type, object_version)
{

}

ObjectsIEntry::~ObjectsIEntry()
{

}

const ObjectID & ObjectsIEntry::getAuthor() const
{
	return author;
}

bool ObjectsIEntry::validate(shared_ptr<IPortalDatabase> database) const // TOCLEAN all ::validate?
{
	if(ObjectBase::validate(database) == false)
		return false;
		
	return true;
}

LanguageResult ObjectsIEntry::acceptable(shared_ptr<IPortalDatabase> database) const
{
	LanguageResult result = ObjectBase::acceptable(database);
	if(result.empty() == false)
		return result;

	if(author->empty())
		return LanguageResult("author_required");

	return result;
}

void ObjectsIEntry::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	ObjectBase::exportXML(exporter);

	if(exporter->getMode() == XMLPortalExporter::emData)
	{
		shared_ptr<XMLNode> node = exporter->getRoot();
		node->setAttributeString(_S("author"), author->toUTF16());
	}
	else
	{
		exportAuthor(exporter);
	}
}

void ObjectsIEntry::exportAuthor(shared_ptr<XMLPortalExporter> exporter)
{
	shared_ptr<ObjectsUser> authorObj = objects_user_cast(exporter->getPage()->getObject(author));
	if(authorObj == nullptr)
		return;

	shared_ptr<XMLPortalExporter> authorExporter(OS_NEW XMLPortalExporter(exporter->getNode(DBTABLES::AUTHOR), exporter->getPage(), exporter->getMode()));
	authorObj->exportXML(authorExporter);
}

bool ObjectsIEntry::onCreate(const shared_ptr<IPortalDatabase> &db, const Buffer &privateKey)
{
	if(ObjectBase::onCreate(db, privateKey) == false)
		return false;

	if(author->empty())
		return false;

	shared_ptr<ObjectsUser> user = objects_user_cast(db->getPortal()->getObject(db, author));
	if(user == nullptr)
		return false;

	// TODOCIP, TOCLEAN: Servono ancora le validateRecord?
	// Assicura che l'utente sia valido
	if(db->validateRecord(user) == false)
		return false;
	
	// Verifica che la chiave privata corrisponda a quella dell'utente
	if(CryptManager::instance()->rsaCheckKeys(privateKey, user->public_key) == false)
		return false;

	return true;
}

bool ObjectsIEntry::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(ObjectBase::onSign(signer) == false)
		return false;

	signer->add(author);

	return true;
}

bool ObjectsIEntry::onRead(const ObjectID &id, const DataTree &data)
{
	if(ObjectBase::onRead(id, data) == false)
		return false;

	author.read(data, DBTABLES::AUTHOR);

	return true;
}

bool ObjectsIEntry::onWrite(DataTree &data) const
{
	if(ObjectBase::onWrite(data) == false)
		return false;

	author.write(data, DBTABLES::AUTHOR);

	return true;
}

bool ObjectsIEntry::onRead(const DataTableRow &row)
{
	if(ObjectBase::onRead(row) == false)
		return false;

	author.read(row, DBTABLES::AUTHOR);

	return true;
}

bool ObjectsIEntry::onWrite(DbSqlValues &values) const
{
	if(ObjectBase::onWrite(values) == false)
		return false;

	author.write(values, DBTABLES::AUTHOR);

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
