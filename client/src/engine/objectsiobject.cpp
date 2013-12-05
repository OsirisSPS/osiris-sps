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
#include "objectsiobject.h"

#include "constants.h"
#include "conversions.h"
#include "datadata.h"
#include "dataistatistics.h"
#include "datatree.h"
#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "engine.h"
#include "entitiessnapshotmanager.h"
#include "isissystem.h"
#include "log.h"
#include "objectssigner.h"
#include "objectsdescriptor.h"
#include "objectsirevisionable.h"
#include "objectsuser.h"
#include "objectssystem.h"
#include "omlmanager.h"
#include "idesession.h"
#include "portalsoptionsshared.h"
#include "portalsportal.h"
#include "realtimestats.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ObjectsIObject::ObjectsIObject(PortalObjectType object_type, uint32 object_version) : RecordBase(dataRecordTypeObject),
																		m_objectType(object_type)
{
	OS_ASSERT(getDescriptor() != null);

	version = object_version;
}

ObjectsIObject::~ObjectsIObject()
{

}

bool ObjectsIObject::isVotable() const
{
	return IPortalDatabase::isVotable(getObjectType());
}

shared_ptr<ObjectsIDescriptor> ObjectsIObject::getDescriptor() const
{
	return ObjectsSystem::instance()->getDescriptor(getObjectType());
}

uint32 ObjectsIObject::getSize() const
{
	DataTree data;
	// Serializza l'oggetto
	if(write(data))
	{
		Buffer buff;
		// Salva il datatree in un buffer non compresso e non criptato
		if(data.write(buff, false, null))
            return buff.getSize();
	}

	return constants::nsize;
}

String ObjectsIObject::getHash() const // TOCLEAN
{
#ifdef OS_NOOBJECTID
	return id->toUTF16();
#else
	return id->getHash();
#endif
}

String ObjectsIObject::getSearchContent() const
{
	String searchContent;

	StringList values;
	enumerateSearchValues(values);

	for(StringList::const_iterator i = values.begin(); i != values.end(); ++i)
	{
		if(searchContent.empty() == false)
			searchContent.append(_S(" "));

		searchContent.append(*i);
	}

	return searchContent;
}

bool ObjectsIObject::allowParent(PortalObjectType type) const
{
	return ObjectsSystem::instance()->getDescriptor(type)->allowChild(getObjectType());
}

bool ObjectsIObject::allowChild(PortalObjectType type) const
{
	return getDescriptor()->allowChild(type);
}

bool ObjectsIObject::sign(const Buffer &private_key, const PortalID &portal)
{
	OS_EXCEPT_IF(portal.empty(), "invalid portal id");

	shared_ptr<ObjectsSigner> signer(OS_NEW ObjectsSigner(portal));

	// Genera il blocco ad firmare
	if(onSign(signer) == false)
		return false;

	Buffer buff;
	// Genera la firma del blocco
	if(signer->sign(private_key, buff) == false)
		return false;

	signature = buff;
	return true;
}

bool ObjectsIObject::verify(const Buffer &public_key, const PortalID &portal) const
{
	OS_EXCEPT_IF(portal.empty(), "invalid portal id");

	shared_ptr<ObjectsSigner> signer(OS_NEW ObjectsSigner(portal));
	// Ricostruisce il blocco da validare
	if(onSign(signer) == false)
		return false;

	OS_ASSERT(signature->empty() == false);
	// Verifica che il blocco ricostruito corrisponda alla firma dell'oggetto
	return signer->validate(public_key, signature);
}

bool ObjectsIObject::read(const ObjectID &id, const DataTree &data)
{
	return onRead(id, data);
}

bool ObjectsIObject::write(DataTree &data) const
{
	return onWrite(data);
}

bool ObjectsIObject::read(const DataTableRow &row)
{
	return RecordBase::read(row);
}

bool ObjectsIObject::write(DbSqlValues &values) const
{
	return RecordBase::write(values);
}

shared_ptr<ObjectsIObject> ObjectsIObject::clone() const
{
	DataTree data;
	if(write(data) == false)
		return null;

	shared_ptr<ObjectsIObject> object = getDescriptor()->createObject();
	if(object->read(id, data) == false)
		return null;

	return object;
}

LanguageResult ObjectsIObject::create(const shared_ptr<IPortalDatabase> &db, shared_ptr<IdeSession> session)
{
	if(session->isPortalGuest(db))
		return LanguageResult("not_logged");
	else
		return create(db, session->getPrivateKey());
}

LanguageResult ObjectsIObject::create(const shared_ptr<IPortalDatabase> &db, const Buffer &privateKey)
{
	LanguageResult result;

	try
	{
		if(onCreate(db, privateKey) == false)
			result = LanguageResult("create_failed");

		if(result.empty())
		{
			if(sign(privateKey, db->getPortal()->getPortalID()) == false)
				result = LanguageResult("cannot_sign");	
		}

		if(result.empty())
		{
			LanguageResult result = store(db);
			if(result.empty() == false)
				return result;	
		}
		else
		{
			OS_LOG_NOTICE(_S("Object ") + id->toUTF16() + _S(" not created: ") + String(result.getId()));
		}

		/*
		LanguageResult result = db->__insertObject(get_this_ptr<IObject>(), loggedUser->getPrivateKey());
		if(result.empty() == false)
			return result;
		*/
		
		// Richiama l'evento di creazione di un oggetto
		onObjectCreated(db, db->getPortal());
	}
	catch(std::exception &e)
	{		
		result.setNoTranslate(e.what());
	}

	return result;
}

LanguageResult ObjectsIObject::store(const shared_ptr<IPortalDatabase> &db)
{
	LanguageResult result;

	try
	{
		result = acceptable(db);
		if(result.empty())
			if(db->storeObject2(get_this_ptr<ObjectsIObject>()) == false)
				return LanguageResult("cannot_store");
		
		/*
		if(result.empty())
			OS_LOG_NOTICE(_S("Object ") + id->toUTF16() + _S(" stored."));
		else
			OS_LOG_NOTICE(_S("Object ") + id->toUTF16() + _S(" not stored: ") + String(result.getId()));
		*/
	}
	catch(std::exception &e)
	{
		result.setNoTranslate(std::string(e.what()));		
	}

	return result;
}

LanguageResult ObjectsIObject::acceptableForBadWords(const shared_ptr<IPortalDatabase> &database, const String& text) const
{	
	// TODO: l'opzione deve contere un elenco, e ogni voce controllata.
	String item = database->getPortal()->getOptionsShared()->getBadWords();

	if(item.empty())
		return LanguageResult();
		
	String::size_type pos = text.find(item);
	if(pos != String::npos)
		return LanguageResult("badwords", "item", item, "position", pos);

	return LanguageResult();
}

/*
bool IObject::isObjectPrimary() const
{
	return true;
}
*/

/*
bool IObject::isObjectVisible() const
{
	return true;
}
*/

/*
const ObjectID & IObject::getPrimary() const
{
	return id;
}
*/

void ObjectsIObject::ensureID(const shared_ptr<IPortalDatabase> &db)
{
}

void ObjectsIObject::getDependencies(const shared_ptr<IPortalDatabase> &db, list<ObjectID>::type &dependencies)
{

}

const String & ObjectsIObject::getTitle() const
{
	return String::EMPTY;
}

const String & ObjectsIObject::getDescription() const
{
	return String::EMPTY;
}

const String & ObjectsIObject::getContent() const
{
	return String::EMPTY;
}

void ObjectsIObject::enumerateSearchValues(StringList &values) const
{
	// <0.12 - Tolto perchè il titolo lo indicizzo a parte.
	// 0.13 - Rimesso, perchè Lucene di default non cerca anche negli altri campi. http://www.osiris-sps.org/forums/index.php?showtopic=1085
	// 0.13 08/08/2009 - Ri-tolto. Fonderlo nel contenuto porta a calcolar male la rilevanza. Forse sarebbe il caso di estrapolare anche la descrizione.
	/*
	const String &title = getTitle();
	if(title.empty() == false)
		values.push_back(title);
	*/

	const String &description = getDescription();
	if(description.empty() == false)
        values.push_back(description);

	const String &content = getContent();
	if(content.empty() == false)
		values.push_back(content);
}

void ObjectsIObject::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	shared_ptr<XMLNode> node = exporter->getRoot();

	node->setAttributeString(_S("type"), getDescriptor()->getTypeName());
		
	node->setAttributeString(DBTABLES::ID, id->toUTF16());
	node->setAttributeString(DBTABLES::POV, pov->toUTF16());
	if(submit_date != DateTime::EMPTY)
		node->setAttributeDateTime(DBTABLES::SUBMIT_DATE, submit_date);

	if(exporter->getMode() == XMLPortalExporter::emData)
	{
		// Non esporto la firma. Servirebbe solo per un'importazione da Osiris verso Osiris, e questo non è il formato adatto.
		// node->setAttributeBuffer(DBTABLES::SIGNATURE, signature);
	}
	else
	{
		

		if(exporter->getMode() == XMLPortalExporter::emLite)
		{
			// Il titolo, la descrizione e il contenuto vengono esportati solo in modalit "lite", altrimenti dovrebbero
			// essere le derivate ad esportarli come campi specifici

			String title = getTitle();
			// Formatta il titolo
			onExportTitle(title, exporter);
			if(title.empty() == false)
				node->setAttributeString(_S("title"), title);

			String description = getDescription();
			// Formatta la descrizione
			onExportDescription(description, exporter);
			if(description.empty() == false)
				node->setAttributeString(_S("description"), description);

			String content = getContent();
			// Formatta il contenuto
			onExportContent(content, exporter);
			if(content.empty() == false)
				node->setAttributeString(_S("content"), content);
		}
	}
}

void ObjectsIObject::onExportTitle(String &title, shared_ptr<XMLPortalExporter> exporter)
{
	// Nessuna operazione nella classe base
}

void ObjectsIObject::onExportDescription(String &description, shared_ptr<XMLPortalExporter> exporter)
{
	// Nessuna operazione nella classe base
}

void ObjectsIObject::onExportContent(String &content, shared_ptr<XMLPortalExporter> exporter)
{
	// Nessuna operazione nella classe base
}

void ObjectsIObject::onObjectCreated(const shared_ptr<IPortalDatabase> &db, shared_ptr<Portal> portal)
{
	// URGENT:  da sistemare, dovrebbe seguire la stessa logica per favorire la distribuzione anche quando l'oggetto viene aggiornato...
	// Dovrebbe essere il metodo IObject::create che inserire l'oggetto ad un rank inferiore per favorire la diffusione,
	// quando invece viene ricevuto via allineamento viene semplicemente inserito normalmente quindi il rank segue i soliti giri.
	// N.B.1: bisognerebbe seguire la stessa logica quando viene aggiornato un oggetto in locale (non via allineamento)
	// N.B.2: se un oggetto viene ricevuto via allineamento DEVE essere inserito al rank minimo corrente e non
	// ad un rank inferiore a quello minimo (altrimenti, se arrivano molti oggetti, il rank tende a 0), se invece
	// un oggetto viene creato in locale  importante cercare di diffonderlo il pi velocemente possibile

	// Commentato in seguito all'aggiunta dell'ordinamento per submit_date nell'allineamento in fase di estrazione
	// db->increaseEntryRank(id);
}

bool ObjectsIObject::validate(shared_ptr<IPortalDatabase> database) const
{
	if(RecordBase::validate(database) == false)
		return false;

	if(submit_date->isValid() == false)
		return false;

	return true;
}

LanguageResult ObjectsIObject::acceptable(shared_ptr<IPortalDatabase> database) const
{
	// Probabile che diversi controlli di "validate" li faccia più volte, perchè chiamata dalla acceptable e dalla "validateRecord".
	// Non è grave, perchè sta a me spostare controlli dalle "validate" alle "acceptable", fino a evitare le validate da IObject in giù.
	bool valid = validate(database);
	if(valid == false)
	{
		bool valid = validate(database); // PAZZO
		return LanguageResult("invalid");
	}

	if(getAuthor().empty())
		return LanguageResult("author_required");

	// Carica l'utente di riferimento
	if(isUser() == false)
	{
		shared_ptr<ObjectsUser> user = objects_user_cast(database->getPortal()->getObject(database, getAuthor()));
		if(user == null)
			return LanguageResult("author_unknown");

		if(isSigned())
		{
			// Verifica la firma digitale dell'oggetto sulla base del proprio autore
			if(verify(user->public_key, database->getPortal()->getPortalID().getString()) == false)
				return LanguageResult("signature_invalid");
		}
	}

	

	// Objects Max Size
	uint32 size = getSize();
	uint32 maxSize = database->getPortal()->getOptionsShared()->getObjectsMaxSize();
	if(size == 0 || size > maxSize)
		return LanguageResult("object_max_size", "size", size, "maxsize", maxSize);

	// Object in the future
	if(database->getPortal()->getOptionsShared()->getAllowObjectInFuture() == false)
	{
		DateTime now = IsisSystem::instance()->getInternetDateTime();
		if(now.isNull())
		{
			// ToFix, Clock maybe not available but can return available later...
			// return LanguageResult("no_clock_sync");
		}
		else
		{
			if(submit_date > now)
				return LanguageResult("object_in_future");
		}
	}

	// Object not signed
	if(database->getPortal()->getOptionsShared()->getAllowObjectUnsigned() == false)
	{
		if(isSigned() == false)
			return LanguageResult("signature_missing");
	}
		
	// Soglia autore
	{
		ObjectsReputationThreshold threshold = database->getPortal()->getOptionsShared()->getAuthorsReputationThreshold();
		
		ReputationsScore scoreDetail = database->getPortal()->getSnapshotManager()->computeReputation(database, getAuthor().toUTF16());
		bool valid = database->getPortal()->getSnapshotManager()->compareReputation(threshold, scoreDetail.getSafeValue());
		if(valid == false)
			return LanguageResult("under_author_threshold");		
	}

	return LanguageResult();
}

String ObjectsIObject::getTableName() const
{
	return getDescriptor()->getTableName();
}

String ObjectsIObject::getRecordKeyName() const
{
	return DBTABLES::ID;
}

String ObjectsIObject::getRecordKeyValue() const
{
	return id->toUTF16();
}


bool ObjectsIObject::onCreate(const shared_ptr<IPortalDatabase> &db, const Buffer &privateKey)
{
	ensureID(db);

	// Controlla se non  stato specificato un id valido
	if(id->empty())
	{
		// Genera un nuovo id per l'oggetto
		// TOCHECK: Perchè solo per i revisionabili? forse CLODOURGENT
		shared_ptr<ObjectsIRevisionable> revision = objects_revisionable_cast(get_this_ptr<ObjectsIObject>());
		if(revision != null)			
		{			
			id = ObjectID::generate();
		}
	}
	else
	{
		// Se l'ID  valorizzato dall'esterno devono coincidere il tipo e i flags

#ifndef OS_NOOBJECTID
		if(id->getObjectType() != getObjectType())
			return false;
#endif

		
	}

	// Controlla se non  stata specificata una data valida
	if(submit_date->isNull())
	{
		DateTime now = IsisSystem::instance()->getInternetDateTime();
		if(now.isNull())
			return false;

		// Salva la data dell'oggetto
		submit_date = now;
	}

	pov = db->getPortal()->getPovID();

	// Inizializza la firma (viene valorizzato in una fase successiva)
	signature.reset();

	return true;
}

bool ObjectsIObject::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	signer->add(id);
	signer->add(pov);
	signer->add(submit_date);
	signer->add(version);

	return true;
}

bool ObjectsIObject::onRead(const ObjectID &id, const DataTree &data)
{
	this->id = id;

	// Type not read, don't need.
	pov.read(data, DBTABLES::POV);
	submit_date.read(data, DBTABLES::SUBMIT_DATE);
	version.read(data, DBTABLES::VERSION);
	signature.read(data, DBTABLES::SIGNATURE);

	return true;
}

bool ObjectsIObject::onWrite(DataTree &data) const
{
	// Nota l'ID non viene serializzato in un datatree perch viene usato nome
	// di un nodo contenente i dati dell'oggetto (infatti nella read viene passato)
	
	DbValue<uint32> type = Convert::toUint32(getObjectType());
	
	type.write(data, DBTABLES::ENTRIES::TYPE);
	pov.write(data, DBTABLES::POV);
	submit_date.write(data, DBTABLES::SUBMIT_DATE);
	version.write(data, DBTABLES::VERSION);
	signature.write(data, DBTABLES::SIGNATURE);

	return true;
}

bool ObjectsIObject::onRead(const DataTableRow &row)
{
	id.read(row, DBTABLES::ID);
	pov.read(row, DBTABLES::POV);
	submit_date.read(row, DBTABLES::SUBMIT_DATE);
	version.read(row, DBTABLES::VERSION);
	signature.read(row, DBTABLES::SIGNATURE);

	return true;
}

bool ObjectsIObject::onWrite(DbSqlValues &values) const
{
	id.write(values, DBTABLES::ID);
	pov.write(values, DBTABLES::POV);
	submit_date.write(values, DBTABLES::SUBMIT_DATE);
	version.write(values, DBTABLES::VERSION);
	signature.write(values, DBTABLES::SIGNATURE);

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
