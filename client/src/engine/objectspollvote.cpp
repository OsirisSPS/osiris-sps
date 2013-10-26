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
#include "objectspollvote.h"

#include "objectssigner.h"
#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "datatree.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsPollVoteDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsPollVote::VERSION = OS_PORTAL_OBJECT_POLLVOTE_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsPollVote::ObjectsPollVote() : EntryBase(portalObjectTypePollVote, VERSION)
{

}

ObjectsPollVote::~ObjectsPollVote()
{

}

bool ObjectsPollVote::isNull() const
{
	return score.isNull();
}

bool ObjectsPollVote::isReferencedTo(const EntityID &reference)
{
	if(isNull())
		return false;

	return this->reference == reference;
}

void ObjectsPollVote::setNull()
{
	reference.setNull();
	score.setNull();
}

void ObjectsPollVote::ensureID(const shared_ptr<IPortalDatabase> &db)
{
	id = db->getUserPollVoteID(author, reference);
}

void ObjectsPollVote::getDependencies(const shared_ptr<IPortalDatabase> &db, list<ObjectID>::type &dependencies)
{
	EntryBase::getDependencies(db, dependencies);

#ifndef OS_TODOCIP
	dependencies.push_back(parent->toObjectID());

	if(reference->empty() == false)		// Il riferimento  opzionale
		dependencies.push_back(reference->toObjectID());
#endif
}

bool ObjectsPollVote::validate(shared_ptr<IPortalDatabase> database) const
{
	if(EntryBase::validate(database) == false)
		return false;


	// TOFIX_VALIDATING
	/*
	if(parent->getObjectType() != portalObjectTypePoll)
		return false;

	if(parent->validateRevisionable(false, true) == false)
		return false;

	if(reference->validateRevisionable(true, true) == false)
		return false;
	*/

	

	return true;
}

LanguageResult ObjectsPollVote::acceptable(shared_ptr<IPortalDatabase> database) const
{
	LanguageResult result = EntryBase::acceptable(database);
	if(result.empty() == false)
		return result;

	if(IPortalDatabase::getUserPollVoteID(getAuthor(), parent) != id)
		return LanguageResult("id_mismatch");

	if(isNull() == false)
	{
		uint32 v = score;
		if(v < OS_PORTAL_OBJECT_VOTE_MIN || v > OS_PORTAL_OBJECT_VOTE_MAX)
			return LanguageResult("score_outofrange");
	}
	
	return LanguageResult();
}

void ObjectsPollVote::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	EntryBase::exportXML(exporter);

	if(exporter->getMode() == XMLPortalExporter::emData)
	{
		exporter->getRoot()->setAttributeString(DBTABLES::POLLS_VOTES::PARENT, parent->toUTF16());
		exporter->getRoot()->setAttributeString(DBTABLES::POLLS_VOTES::REFERENCE, reference->toUTF16());
		exporter->getRoot()->setAttributeUint32(DBTABLES::POLLS_VOTES::SCORE, score);
	} 
}

bool ObjectsPollVote::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(EntryBase::onSign(signer) == false)
		return false;

	signer->add(parent);
	signer->add(reference);
	signer->add(score);

	return true;
}

bool ObjectsPollVote::onRead(const ObjectID &id, const DataTree &data)
{
	if(EntryBase::onRead(id, data) == false)
		return false;

	parent.read(data, DBTABLES::POLLS_VOTES::PARENT);
	reference.read(data, DBTABLES::POLLS_VOTES::REFERENCE);
	score.read(data, DBTABLES::POLLS_VOTES::SCORE);

	return true;
}

bool ObjectsPollVote::onWrite(DataTree &data) const
{
	if(EntryBase::onWrite(data) == false)
		return false;

	parent.write(data, DBTABLES::POLLS_VOTES::PARENT);
	reference.write(data, DBTABLES::POLLS_VOTES::REFERENCE);
	score.write(data, DBTABLES::POLLS_VOTES::SCORE);

	return true;
}

bool ObjectsPollVote::onRead(const DataTableRow &row)
{
	if(EntryBase::onRead(row) == false)
		return false;

	parent.read(row, DBTABLES::POLLS_VOTES::PARENT);
	reference.read(row, DBTABLES::POLLS_VOTES::REFERENCE);
	score.read(row, DBTABLES::POLLS_VOTES::SCORE);

	return true;
}

bool ObjectsPollVote::onWrite(DbSqlValues &values) const
{
	if(EntryBase::onWrite(values) == false)
		return false;

	parent.write(values, DBTABLES::POLLS_VOTES::PARENT);
	reference.write(values, DBTABLES::POLLS_VOTES::REFERENCE);
	score.write(values, DBTABLES::POLLS_VOTES::SCORE);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsPollVoteDescriptor::ObjectsPollVoteDescriptor()
{

}

ObjectsPollVoteDescriptor::~ObjectsPollVoteDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsPollVoteDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsPollVote());
}

String ObjectsPollVoteDescriptor::getTableName()
{
	return DBTABLES::POLLS_VOTES_TABLE;
}

String ObjectsPollVoteDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_POLLVOTE_TYPENAME;
}

String ObjectsPollVoteDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_POLLVOTE_GROUPNAME;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
