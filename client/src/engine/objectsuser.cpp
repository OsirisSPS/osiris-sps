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
#include "objectsuser.h"

#include "algorithms.h"
#include "convert.h"
#include "cryptmanager.h"
#include "entitiessnapshotmanager.h"
#include "objectsavatar.h"
#include "objectssigner.h"
#include "datauserstats.h"
#include "dbbinder.h"
#include "idbconnection.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "datatree.h"
#include "iideportalpage.h"
#include "options.h"
#include "osiriscommon.h"
#include "idesession.h"
#include "portalsportal.h"
#include "portalsprofile.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"
#include "xmlschema.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsUserDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsUser::VERSION = OS_PORTAL_OBJECT_USER_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsUser::ObjectsUser() : ObjectBase(portalObjectTypeUser, OS_PORTAL_OBJECT_USER_VERSION)
{
	authors_threshold = rtInherit;
	editors_threshold = rtInherit;
	gender = ugUnspecified;
	show_mark = true;
	show_avatar = true;
	time_offset = 0;
}

ObjectsUser::~ObjectsUser()
{

}

shared_ptr<ObjectsAvatar> ObjectsUser::getAvatar(const shared_ptr<IPortalDatabase> &db) const
{
	if(show_avatar == false)
		return null;

	OS_ASSERT(db != null);
	return db->getAvatar(getAvatarID());
}

ObjectID ObjectsUser::getAvatarID() const
{
	OS_ASSERT(id->empty() == false);
	return IPortalDatabase::getUserAvatarID(id);
}

bool ObjectsUser::hasAvatar(const shared_ptr<IPortalDatabase> &db) const
{
	return getAvatar(db) != null;
}

bool ObjectsUser::hasMark() const
{
	return show_mark && (mark->empty() == false);
}

/*
ReputationThreshold User::getAuthorsThreshold() const
{
	return Convert::toReputationThreshold(authors_threshold);
}

ReputationThreshold User::getEditorsThreshold() const
{
	return Convert::toReputationThreshold(editors_threshold);
}
*/

bool ObjectsUser::verify(const PortalID &portal) const
{
	// Verifica la firma con la propria chiave pubblica
	return ObjectBase::verify(public_key, portal);
}

const ObjectID & ObjectsUser::getAuthor() const
{
	// Nota: l'autore di un utente coincide con se stesso
	return id;
}

void ObjectsUser::getDependencies(const shared_ptr<IPortalDatabase> &db, list<ObjectID>::type &dependencies)
{
	ObjectBase::getDependencies(db, dependencies);

	if(hasAvatar(db))
		// Salva l'avatar nelle dipendenze
		dependencies.push_back(getAvatarID());
}

bool ObjectsUser::validate(shared_ptr<IPortalDatabase> database) const
{
	if(ObjectBase::validate(database) == false)
		return false;

	if(_validatePublicKey() == false)
		return false;

	// N.B.: per compatibilità con le versioni precedenti, non è possibile qui inserire check sull'username se non un trim o si rischia di invalidare utenti già registrati
	String parsedName = algorithms::trim_copy(name.get());
	if(parsedName.empty())
		return false;

	// La soglia di reputazione degli editor non pu essere superiore a quella degli autori
	// 0.14 : Tolto. Motivi: 1) Dar la possibilità di sperimentare; 2) c'è un valore "rtInherit" in quell'enum, che risulta alto, andrebbe calcolato per fare il confronto che ho commentato.
	// TOFIX: mi è capitato, e ci ho messo una vita a capire il perchè. Senza eseguirlo in debug, non avrei mai potuto, perchè manca un giro per segnalare cos'è successo: 
	//    la pagina di edit spara un semplice "impossibile aggiornare.". Lo so che andrebbe gestito a eccezioni, cerchiamo di sistemarli quando li incontriamo...	
	/*
	if(authors_threshold > editors_threshold)
		return false;
	*/

	if(join_date == DateTime::EMPTY)
		return false;

	// TODO: validazione della mail

	if(contacts->empty() == false)
	{
		shared_ptr<XMLSchema> schema(OS_NEW XMLSchema());
		schema->parseFile(utils::makeFilePath(utils::makeFolderPath(Options::instance()->getSharePath(),OS_SCHEMAS_PATH), OS_SCHEMA_CONTACTS));

		shared_ptr<XMLDocument> document(OS_NEW XMLDocument(schema));
		if(document->parseString(contacts) == false)
			return false;
	}

	return true;
}

LanguageResult ObjectsUser::acceptable(shared_ptr<IPortalDatabase> database) const
{
	LanguageResult result = ObjectBase::acceptable(database);
	if(result.empty() == false)
		return result;

	if(isSigned())
	{
		if(verify(database->getPortal()->getPortalID().getString()) == false)
			return LanguageResult("signature_invalid");
	}

	// Pov user always accepted. We can't risk that this user ban itself. // VERYVERYURGENT: Funziona qui, così? no... ma almeno che sia valida la firma devo verificarlo.
	if(database->getPortal()->getPovUserID() == id)
		return LanguageResult();

	return LanguageResult();
}

void ObjectsUser::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	ObjectBase::exportXML(exporter);

	shared_ptr<XMLNode> node = exporter->getRoot();

	node->setAttributeString(DBTABLES::USERS::NAME, name);
	node->setAttributeString(DBTABLES::USERS::DESCRIPTION, description);
	node->setAttributeDateTime(DBTABLES::USERS::JOIN_DATE, join_date);

	if(exporter->getMode() != XMLPortalExporter::emLite)
	{
		node->setAttributeDateTime(DBTABLES::USERS::BIRTH_DATE, birth_date);
		if(gender != static_cast<uint32>(ugUnspecified))
			node->setAttributeUint32(DBTABLES::USERS::GENDER, gender);
		node->setAttributeString(DBTABLES::USERS::LOCATION, location);
		node->setAttributeInt32(DBTABLES::USERS::TIME_OFFSET, time_offset);
		node->setAttributeString(DBTABLES::USERS::LANGUAGE, language);
		if(misc->empty() == false)
			node->setAttributeString(DBTABLES::USERS::MISC, misc);
		if(mark->empty() == false)
			node->setAttributeString(DBTABLES::USERS::MARK, mark);
		node->setAttributeBool(DBTABLES::USERS::SHOW_MARK, show_mark);
		node->setAttributeBool(DBTABLES::USERS::SHOW_AVATAR, show_avatar);
		if(email->empty() == false)
			node->setAttributeString(DBTABLES::USERS::EMAIL, email);
		if(website->empty() == false)
			node->setAttributeString(DBTABLES::USERS::WEBSITE, website);
		if(contacts->empty() == false)
			node->setAttributeString(DBTABLES::USERS::CONTACTS, contacts);
	}

	if(exporter->getMode() != XMLPortalExporter::emData)
	{
		node->setAttributeString(_S("view_href"), exporter->getPortal()->getUserLink(id));

		portals::profile_ptr profile = exporter->getPortal()->getProfile(exporter->getDatabase(), id);
		if(profile != null)
		{
			node->setAttributeString(_S("score_value"), profile->getScore().getStringValue());
			node->setAttributeString(_S("score_comment"), profile->getScore().getDescription());
		}

		if(exporter->getMode() == XMLPortalExporter::emFull)
		{
			shared_ptr<ObjectsAvatar> avatar = getAvatar(exporter->getDatabase());
			if(avatar != null)
			{
				node->setAttributeString(_S("avatar"), exporter->getPortal()->getAvatarLink(avatar->id, avatar->submit_date));
			}

			Buffer publicKeyBuffer = public_key;
			node->setAttributeString(_S("publickey"), publicKeyBuffer.toHex());
		}
	}

	if(exporter->getWithStats())
	{
		shared_ptr<DataIStatistics> stats = exporter->getDatabase()->loadStatistics<DataUserStats>(DBTABLES::USERS_STATS_TABLE, id->getString());
		if(stats != null)
			stats->exportXML(exporter);
	}
}

bool ObjectsUser::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(ObjectBase::onSign(signer) == false)
		return false;

	signer->add(public_key);
	signer->add(name);
	signer->add(description);
	signer->add(authors_threshold);
	signer->add(editors_threshold);
	signer->add(join_date);
	signer->add(birth_date);
	signer->add(gender);
	signer->add(location);
	signer->add(time_offset);
	signer->add(language);
	signer->add(misc);
	signer->add(mark);
	signer->add(show_mark);
	signer->add(show_avatar);
	signer->add(email);
	signer->add(website);
	signer->add(contacts);
	signer->add(options); // Razor only

	return true;
}

bool ObjectsUser::onRead(const ObjectID &id, const DataTree &data)
{
	if(ObjectBase::onRead(id, data) == false)
		return false;

	public_key.read(data, DBTABLES::USERS::PUBLIC_KEY);
	name.read(data, DBTABLES::USERS::NAME);
	description.read(data, DBTABLES::USERS::DESCRIPTION);
	authors_threshold.read(data, DBTABLES::USERS::AUTHORS_THRESHOLD);
	editors_threshold.read(data, DBTABLES::USERS::EDITORS_THRESHOLD);
	join_date.read(data, DBTABLES::USERS::JOIN_DATE);
	birth_date.read(data, DBTABLES::USERS::BIRTH_DATE);
	gender.read(data, DBTABLES::USERS::GENDER);
	location.read(data, DBTABLES::USERS::LOCATION);
	time_offset.read(data, DBTABLES::USERS::TIME_OFFSET);
	language.read(data, DBTABLES::USERS::LANGUAGE);
	misc.read(data, DBTABLES::USERS::MISC);
	mark.read(data, DBTABLES::USERS::MARK);
	show_mark.read(data, DBTABLES::USERS::SHOW_MARK);
	show_avatar.read(data, DBTABLES::USERS::SHOW_AVATAR);
	email.read(data, DBTABLES::USERS::EMAIL);
	website.read(data, DBTABLES::USERS::WEBSITE);
	contacts.read(data, DBTABLES::USERS::CONTACTS);
	options.read(data, DBTABLES::USERS::OPTIONS);

	return true;
}

bool ObjectsUser::onWrite(DataTree &data) const
{
	if(ObjectBase::onWrite(data) == false)
		return false;

	public_key.write(data, DBTABLES::USERS::PUBLIC_KEY);
	name.write(data, DBTABLES::USERS::NAME);
	description.write(data, DBTABLES::USERS::DESCRIPTION);
	authors_threshold.write(data, DBTABLES::USERS::AUTHORS_THRESHOLD);
	editors_threshold.write(data, DBTABLES::USERS::EDITORS_THRESHOLD);
	join_date.write(data, DBTABLES::USERS::JOIN_DATE);
	birth_date.write(data, DBTABLES::USERS::BIRTH_DATE);
	gender.write(data, DBTABLES::USERS::GENDER);
	location.write(data, DBTABLES::USERS::LOCATION);
	time_offset.write(data, DBTABLES::USERS::TIME_OFFSET);
	language.write(data, DBTABLES::USERS::LANGUAGE);
	misc.write(data, DBTABLES::USERS::MISC);
	mark.write(data, DBTABLES::USERS::MARK);
	show_mark.write(data, DBTABLES::USERS::SHOW_MARK);
	show_avatar.write(data, DBTABLES::USERS::SHOW_AVATAR);
	email.write(data, DBTABLES::USERS::EMAIL);
	website.write(data, DBTABLES::USERS::WEBSITE);
	contacts.write(data, DBTABLES::USERS::CONTACTS);
	options.write(data, DBTABLES::USERS::OPTIONS);

	return true;
}

bool ObjectsUser::onRead(const DataTableRow &row)
{
	if(ObjectBase::onRead(row) == false)
		return false;

	public_key.read(row, DBTABLES::USERS::PUBLIC_KEY);
	name.read(row, DBTABLES::USERS::NAME);
	description.read(row, DBTABLES::USERS::DESCRIPTION);
	authors_threshold.read(row, DBTABLES::USERS::AUTHORS_THRESHOLD);
	editors_threshold.read(row, DBTABLES::USERS::EDITORS_THRESHOLD);
	join_date.read(row, DBTABLES::USERS::JOIN_DATE);
	birth_date.read(row, DBTABLES::USERS::BIRTH_DATE);
	gender.read(row, DBTABLES::USERS::GENDER);
	location.read(row, DBTABLES::USERS::LOCATION);
	time_offset.read(row, DBTABLES::USERS::TIME_OFFSET);
	language.read(row, DBTABLES::USERS::LANGUAGE);
	misc.read(row, DBTABLES::USERS::MISC);
	mark.read(row, DBTABLES::USERS::MARK);
	show_mark.read(row, DBTABLES::USERS::SHOW_MARK);
	show_avatar.read(row, DBTABLES::USERS::SHOW_AVATAR);
	email.read(row, DBTABLES::USERS::EMAIL);
	website.read(row, DBTABLES::USERS::WEBSITE);
	contacts.read(row, DBTABLES::USERS::CONTACTS);
	options.read(row, DBTABLES::USERS::OPTIONS);

	return true;
}

bool ObjectsUser::onWrite(DbSqlValues &values) const
{
	if(ObjectBase::onWrite(values) == false)
		return false;

	public_key.write(values, DBTABLES::USERS::PUBLIC_KEY);
	name.write(values, DBTABLES::USERS::NAME);
	description.write(values, DBTABLES::USERS::DESCRIPTION);
	authors_threshold.write(values, DBTABLES::USERS::AUTHORS_THRESHOLD);
	editors_threshold.write(values, DBTABLES::USERS::EDITORS_THRESHOLD);
	join_date.write(values, DBTABLES::USERS::JOIN_DATE);
	birth_date.write(values, DBTABLES::USERS::BIRTH_DATE);
	gender.write(values, DBTABLES::USERS::GENDER);
	location.write(values, DBTABLES::USERS::LOCATION);
	time_offset.write(values, DBTABLES::USERS::TIME_OFFSET);
	language.write(values, DBTABLES::USERS::LANGUAGE);
	misc.write(values, DBTABLES::USERS::MISC);
	mark.write(values, DBTABLES::USERS::MARK);
	show_mark.write(values, DBTABLES::USERS::SHOW_MARK);
	show_avatar.write(values, DBTABLES::USERS::SHOW_AVATAR);
	email.write(values, DBTABLES::USERS::EMAIL);
	website.write(values, DBTABLES::USERS::WEBSITE);
	contacts.write(values, DBTABLES::USERS::CONTACTS);
	options.write(values, DBTABLES::USERS::OPTIONS);

	return true;
}

bool ObjectsUser::_validatePublicKey() const
{
	// La chiave pubblica deve essere valida
	if(public_key->empty())
		return false;

	// L'hash della chiave pubblica di un utente deve coincidere con l'hash dell'utente
	if(CryptManager::instance()->SHA(public_key->getData(), public_key->getSize()).toHex() != getHash())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsUserDescriptor::ObjectsUserDescriptor()
{

}

ObjectsUserDescriptor::~ObjectsUserDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsUserDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsUser());
}

String ObjectsUserDescriptor::getTableName()
{
	return DBTABLES::USERS_TABLE;
}

String ObjectsUserDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_USER_TYPENAME;
}

String ObjectsUserDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_USER_GROUPNAME;
}

bool ObjectsUserDescriptor::hasStatistics()
{
	return true;
}

void ObjectsUserDescriptor::createStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	DescriptorBase::createStatistics(db, object);

	shared_ptr<DataIStatistics> statistics(OS_NEW DataUserStats());
	statistics->reference = object->id->toUTF16();
	statistics->create_ZZZ(db);
}

void ObjectsUserDescriptor::computeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	DescriptorBase::computeStatistics(db, object);

	OS_ASSERT(object->isUser());
	shared_ptr<ObjectsUser> user = objects_user_cast(object);

	shared_ptr<DataUserStats> stats(OS_NEW DataUserStats());
	stats->reference = user->id->toUTF16();

	String userId = user->id->toUTF16();
	// Quanti oggetti creati da me
		// Versione 0.11:
		//stats->objects = db->value_of(String::format(_S("select count(*) from os_entries where revision='' and author='%S'"), userId.c_str()));
		// Con il cambio interpretazione primaria diventer:
		//select count(distinct revision) from os_entries where original_author = '%S'
		// Per ora (cambia che conta le primarie+revisioni, non solo primarie
	// N.B. CLODOURGENT così però non conta voti, reputazioni etc e l'utente stesso. DEVO valorizzare entity_author = author per questi.
#ifdef OS_TODOCIP
	uint32 revisionable = db->getConnection()->value_of(String::format(_S("select count(*) from os_entries where entity_author='%S'").c_str(), userId.c_str()));
	uint32 other = db->getConnection()->value_of(String::format(_S("select count(*) from os_entries where author='%S' and entity is null").c_str(), userId.c_str()));
	stats->objects = revisionable + other;
#else
		stats->objects = db->value_of(String::format(_S("select count(*) from os_entries where author='%S'").c_str(), userId.c_str()));
#endif
	// Quanti oggetti di altri sono stati revisionati da me  // N.B. non quante revisioni!
		// Versione 0.11:
		//stats->revisions = db->value_of(String::format(_S("select count(*) from os_entries t1 where t1.author!='%S' and exists(select id from os_entries t2 where t2.author='%S' and t2.revision=t1.id)"), userId.c_str(), userId.c_str()));
		// Con il cambio interpretazione primaria diventer:
		//select count(distinct revision) from os_entries where author = '%S' and author != original_author;
		// Per ora
#ifdef OS_TODOCIP
		stats->revisions = db->getConnection()->value_of(String::format(_S("select count(distinct entity) from os_entries where author='%S' and entity_author!='%S'").c_str(), userId.c_str(), userId.c_str()));
#else
		stats->revisions = db->getConnection()->value_of(String::format(_S("select count(distinct t1.revision) from os_entries t1, os_entries t2 where t1.author='%S' and t1.revision=t2.id and t2.author!='%S'").c_str(), userId.c_str(), userId.c_str()));
#endif

	// Numero utente
	stats->progressive_number = db->getConnection()->value_of(String::format(_S("select count(*) from os_users where join_date<='%S'").c_str(), user->join_date->toString().c_str()));

	db->updateRecord(stats);
}

void ObjectsUserDescriptor::removeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	DescriptorBase::removeStatistics(db, object);

	ObjectOrEntityID id;
	if(object)
		id = object->id->toUTF16();
	
	db->removeProfileStatistics(DBTABLES::USERS_STATS_TABLE, id);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
