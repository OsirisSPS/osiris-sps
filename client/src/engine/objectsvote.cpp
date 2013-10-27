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
#include "objectsvote.h"

#include "objectssigner.h"
#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "datatree.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsVoteDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsVote::VERSION = OS_PORTAL_OBJECT_VOTE_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsVote::ObjectsVote() : EntryBase(portalObjectTypeVote, VERSION)
{

}

ObjectsVote::~ObjectsVote()
{

}

bool ObjectsVote::isNull() const
{
	return score.isNull();
}

void ObjectsVote::setNull()
{
	score.setNull();
}

void ObjectsVote::ensureID(const shared_ptr<IPortalDatabase> &db)
{
	id = db->getUserVoteID(author, reference);
}

void ObjectsVote::getDependencies(const shared_ptr<IPortalDatabase> &db, list<ObjectID>::type &dependencies)
{
	EntryBase::getDependencies(db, dependencies);

#ifndef OS_TODOCIP
	dependencies.push_back(reference->toObjectID());
#endif
}

bool ObjectsVote::validate(shared_ptr<IPortalDatabase> database) const
{
	if(EntryBase::validate(database) == false)
		return false;


	// TOFIX_VALIDATING
	/*
	if(IPortalDatabase::isVotable(reference->getObjectType()) == false)
		return false;

    if(reference->validateRevisionable(false, true) == false)
		return false;
	*/

	

	return true;
}

LanguageResult ObjectsVote::acceptable(shared_ptr<IPortalDatabase> database) const
{
	LanguageResult result = EntryBase::acceptable(database);
	if(result.empty() == false)
		return result;

	if(IPortalDatabase::getUserVoteID(getAuthor(), reference) != id)
		return LanguageResult("id_mismatch");

	if(isNull() == false)
	{
		uint32 v = score;
		if(v < OS_PORTAL_OBJECT_VOTE_MIN || v > OS_PORTAL_OBJECT_VOTE_MAX)
			return LanguageResult("score_outofrange");
	}
	
	return LanguageResult();
}

void ObjectsVote::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	EntryBase::exportXML(exporter);

	if(exporter->getMode() == XMLPortalExporter::emData)
	{
		exporter->getRoot()->setAttributeString(DBTABLES::VOTES::REFERENCE, reference->toUTF16());
		exporter->getRoot()->setAttributeUint32(DBTABLES::VOTES::SCORE, score);
	}
}

bool ObjectsVote::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(EntryBase::onSign(signer) == false)
		return false;

	signer->add(reference);
	signer->add(score);

	return true;
}

bool ObjectsVote::onRead(const ObjectID &id, const DataTree &data)
{
	if(EntryBase::onRead(id, data) == false)
		return false;

	reference.read(data, DBTABLES::VOTES::REFERENCE);
	score.read(data, DBTABLES::VOTES::SCORE);

	return true;
}

bool ObjectsVote::onWrite(DataTree &data) const
{
	if(EntryBase::onWrite(data) == false)
		return false;

	reference.write(data, DBTABLES::VOTES::REFERENCE);
	score.write(data, DBTABLES::VOTES::SCORE);

	return true;
}

bool ObjectsVote::onRead(const DataTableRow &row)
{
	if(EntryBase::onRead(row) == false)
		return false;

	reference.read(row, DBTABLES::VOTES::REFERENCE);
	score.read(row, DBTABLES::VOTES::SCORE);

	return true;
}

bool ObjectsVote::onWrite(DbSqlValues &values) const
{
	if(EntryBase::onWrite(values) == false)
		return false;

	reference.write(values, DBTABLES::VOTES::REFERENCE);
	score.write(values, DBTABLES::VOTES::SCORE);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsVoteDescriptor::ObjectsVoteDescriptor()
{

}

ObjectsVoteDescriptor::~ObjectsVoteDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsVoteDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsVote());
}

String ObjectsVoteDescriptor::getTableName()
{
	return DBTABLES::VOTES_TABLE;
}

String ObjectsVoteDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_VOTE_TYPENAME;
}

String ObjectsVoteDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_VOTE_GROUPNAME;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
