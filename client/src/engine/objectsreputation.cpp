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
#include "objectsreputation.h"

#include "objectssigner.h"
#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "datatree.h"
#include "osiriscommon.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsReputationDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsReputation::VERSION = OS_PORTAL_OBJECT_REPUTATION_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsReputation::ObjectsReputation() : EntryBase(portalObjectTypeReputation, VERSION)
{
	score = 0;
}

ObjectsReputation::~ObjectsReputation()
{

}

void ObjectsReputation::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	EntryBase::exportXML(exporter);

	if(exporter->getMode() == XMLPortalExporter::emData)
	{
		shared_ptr<XMLNode> node = exporter->getRoot();

		node->setAttributeString(DBTABLES::REPUTATIONS::REFERENCE, reference->toUTF16());
		node->setAttributeDouble(DBTABLES::REPUTATIONS::SCORE, score);
		node->setAttributeString(DBTABLES::REPUTATIONS::DESCRIPTION, description);
		node->setAttributeBool(DBTABLES::REPUTATIONS::FOLLOW, follow);
	}
}

void ObjectsReputation::ensureID(const shared_ptr<IPortalDatabase> &db)
{
	id = db->getUserReputationID(author, reference);
}

void ObjectsReputation::getDependencies(const shared_ptr<IPortalDatabase> &db, list<ObjectID>::type &dependencies)
{
	EntryBase::getDependencies(db, dependencies);

	dependencies.push_back(reference);
}

bool ObjectsReputation::validate(shared_ptr<IPortalDatabase> database) const
{
	if(EntryBase::validate(database) == false)
		return false;

	return true;
}

LanguageResult ObjectsReputation::acceptable(shared_ptr<IPortalDatabase> database) const
{
	LanguageResult result = EntryBase::acceptable(database);
	if(result.empty() == false)
		return result;

	if(reference->empty())
		return LanguageResult("reference_required");

	if(IPortalDatabase::getUserReputationID(getAuthor(), reference) != id)
		return LanguageResult("id_mismatch");

	if(getAuthor() == reference)
		return LanguageResult("autovalutation");

	if(score < OS_REPUTATION_MIN || score > OS_REPUTATION_MAX)
		return LanguageResult("score_outofrange");
	
	return LanguageResult();
}

bool ObjectsReputation::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(EntryBase::onSign(signer) == false)
		return false;

	signer->add(reference);
	signer->add(score);
	signer->add(description);
	signer->add(follow);

	return true;
}

bool ObjectsReputation::onRead(const ObjectID &id, const DataTree &data)
{
	if(EntryBase::onRead(id, data) == false)
		return false;

	reference.read(data, DBTABLES::REPUTATIONS::REFERENCE);
	score.read(data, DBTABLES::REPUTATIONS::SCORE);
	description.read(data, DBTABLES::REPUTATIONS::DESCRIPTION);
	follow.read(data, DBTABLES::REPUTATIONS::FOLLOW);

	return true;
}

bool ObjectsReputation::onWrite(DataTree &data) const
{
	if(EntryBase::onWrite(data) == false)
		return false;

	reference.write(data, DBTABLES::REPUTATIONS::REFERENCE);
	score.write(data, DBTABLES::REPUTATIONS::SCORE);
	description.write(data, DBTABLES::REPUTATIONS::DESCRIPTION);
	follow.write(data, DBTABLES::REPUTATIONS::FOLLOW);

	return true;
}

bool ObjectsReputation::onRead(const DataTableRow &row)
{
	if(EntryBase::onRead(row) == false)
		return false;

	reference.read(row, DBTABLES::REPUTATIONS::REFERENCE);
	score.read(row, DBTABLES::REPUTATIONS::SCORE);
	description.read(row, DBTABLES::REPUTATIONS::DESCRIPTION);
	follow.read(row, DBTABLES::REPUTATIONS::FOLLOW);

	return true;
}

bool ObjectsReputation::onWrite(DbSqlValues &values) const
{
	if(EntryBase::onWrite(values) == false)
		return false;

	reference.write(values, DBTABLES::REPUTATIONS::REFERENCE);
	score.write(values, DBTABLES::REPUTATIONS::SCORE);
	description.write(values, DBTABLES::REPUTATIONS::DESCRIPTION);
	follow.write(values, DBTABLES::REPUTATIONS::FOLLOW);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsReputationDescriptor::ObjectsReputationDescriptor()
{

}

ObjectsReputationDescriptor::~ObjectsReputationDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsReputationDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsReputation());
}

String ObjectsReputationDescriptor::getTableName()
{
	return DBTABLES::REPUTATIONS_TABLE;
}

String ObjectsReputationDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_REPUTATION_TYPENAME;
}

String ObjectsReputationDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_REPUTATION_GROUPNAME;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
