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
#include "objectsirevisionable.h"

#include "objectssigner.h"
#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "entitiessnapshotmanager.h"
#include "iideportalpage.h"
#include "datatree.h"
#include "objectsuser.h"
#include "objectssystem.h"
#include "idesession.h"
#include "portalsportal.h"
#include "portalsoptionsshared.h"
#include "realtimestats.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ObjectsIRevisionable::ObjectsIRevisionable(PortalObjectType object_type, uint32 object_version) : EntryBase(object_type, object_version)
{
	parent = EntityID::EMPTY;
#ifdef OS_TODOCIP
	entity = EntityID::EMPTY;
	entity_author = ObjectID::EMPTY;
	entity_date = DateTime::EMPTY;
	entity_parent = EntityID::EMPTY;
	entity_signature.reset();
#else
	revision = EntityID::EMPTY;
#endif
	visible = true;
	position = OS_PORTAL_OBJECT_POSITION_NULL;
}

ObjectsIRevisionable::~ObjectsIRevisionable()
{

}

const EntityID & ObjectsIRevisionable::getParent() const
{
	return parent;
}

bool ObjectsIRevisionable::requireParent() const
{
	// Default: l'oggetto non ha bisogno di un padre
	return false;
}

/*
const ObjectID & IRevisionable::getPrimary() const
{
	if(isRevision())
		return revision;

	return EntryBase::getPrimary();
}
*/


/*
bool IRevisionable::isObjectPrimary() const
{
	return revision->empty();
}
*/

/*
bool IRevisionable::isObjectVisible() const
{
	return visible;
}
*/

void ObjectsIRevisionable::changeToRevision(shared_ptr<ObjectsIRevisionable> src)
{
	// If "src" is not specified, assume is the object itself.

	id = ObjectID::EMPTY;
		
	if(src != nullptr)
	{
#ifdef OS_TODOCIP
		entity = src->entity;
		entity_author = src->entity_author;
		entity_date = src->entity_date;
		entity_parent = src->entity_parent;
#else
		revision = src->getEntityID();
#endif
	}
}

bool ObjectsIRevisionable::signEntity(const Buffer &private_key, const PortalID &portal)
{
	OS_EXCEPT_IF(portal.empty(), "invalid portal id");

	shared_ptr<ObjectsSigner> signer(OS_NEW ObjectsSigner(portal));

	// Genera il blocco ad firmare
	signer->add(entity);
	signer->add(entity_author);
	signer->add(entity_date);
	signer->add(entity_parent);

	Buffer buff;
	// Genera la firma del blocco
	if(signer->sign(private_key, buff) == false)
		return false;

	entity_signature = buff;
	return true;
}

/*
void ObjectsIRevisionable::exportBase(shared_ptr<XMLPortalExporter> exporter)
{
	// OS_TODOCIP
	// Qui in teoria con il CIP, si semplifica tutto demandando alla exportXML e forse togliendo queste exportBase.

	shared_ptr<XMLNode> node = exporter->getRoot();

	node->setAttributeString(_S("entity"), getEntityID().toUTF16());
	node->setAttributeDateTime(_S("entity_date"), entity_date);
	node->setAttributeString(_S("entity_author"), entity_author->toUTF16());
	node->setAttributeString(_S("entity_parent"), entity_parent->toUTF16());

	if(exporter->getMode() == XMLPortalExporter::emData)
	{
		EntryBase::exportBase(exporter);
	}
	else
	{
#ifdef OS_TODOCIP
		// CLODOVERYURGENT: le righe sotto le ho lasciate per non toccare gli attuali giri,
		// però è una vaccata sovrascrivere l'id e data. In giro si dovrebbe usare @entity o @id a seconda del caso.
		//node->setAttributeString(DBTABLES::ID, getEntityID().toUTF16()); 
		//node->setAttributeDateTime(DBTABLES::SUBMIT_DATE, entity_date);

		if(entity_date != submit_date)
		{
			node->setAttributeBool(_S("revisioned"), true);
			//node->setAttributeDateTime(_S("revision_date"), submit_date);
		}

		// CLODOVERYURGENT: Copiata dal giro sotto... perchè solo se entity?
		//if(isEntity())
			EntryBase::exportBase(exporter);
#else
		if(isEntity() == false)
		{
			// N.B.: l'id e la submit_date devono essere sempre quelli della primaria, i dati specifici
			// di una revisione vengono esportati in "revision" e "revision_date"
			node->setAttributeString(DBTABLES::ID, getEntityID().toUTF16());

			shared_ptr<ObjectsIRevisionable> primary = objects_revisionable_cast(exporter->getPage()->getObject(getEntityID().toObjectID()));
			if(primary != nullptr)
			{
				node->setAttributeDateTime(DBTABLES::SUBMIT_DATE, primary->submit_date);
				node->setAttributeDateTime(_S("revision_date"), submit_date);
			}
		}
		else
		{
			EntryBase::exportBase(exporter);
		}
#endif
	}
}
*/

void ObjectsIRevisionable::getDependencies(const shared_ptr<IPortalDatabase> &db, list<ObjectID>::type &dependencies)
{
	EntryBase::getDependencies(db, dependencies);

	// In CIP, non posso usar questa dipendenza: la primaria potrebbe non esistere più.
#ifndef OS_TODOCIP
	// Controlla se l'oggetto ha un padre
	if(hasParent())
		// Salva il padre nelle dipendenze
		dependencies.push_back(parent->toObjectID());
#endif

#ifdef OS_TODOCIP
	if(author != entity_author)
		dependencies.push_back(entity_author);
#endif

#ifndef OS_TODOCIP
	// Controlla se l'oggetto  una revisione
	if(isEntity() == false)
		// Salva la revisione base nelle dipendenze
		dependencies.push_back(revision->toObjectID());
#endif
}

bool ObjectsIRevisionable::validate(shared_ptr<IPortalDatabase> database) const
{
	if(EntryBase::validate(database) == false)
		return false;

#ifdef OS_TODOCIP
	/*
	if(entity->validateObject(false) == false)
		return false;
	*/
	
	// TOFIX_VALIDATING
	/*
	if(entity_parent->validateRevisionable(requireParent() == false, true) == false)
		return false;
	*/

	if(entity_parent->empty() == false)
	{
		if(getEntityID() == entity_parent)
			return false;		// L'oggetto non pu essere padre di se stesso

		if(ObjectsSystem::instance()->isVirtual(entity_parent) && entity_parent != ObjectsSystem::instance()->getRootID())
			return false;		// L'oggetto non pu essere figlio di una sezione virtuale, a meno che non sia la root.

		// TOFIX_VALIDATING
		/*
		if(allowParent(entity_parent->getObjectType()) == false)
			return false;
		*/
	}
	
#endif

	// TOFIX_VALIDATING
	/*
	if(parent->validateRevisionable(requireParent() == false, true) == false)
		return false;
	*/

	if(hasParent())
	{
		if(getEntityID() == getParent())
			return false;		// L'oggetto non pu essere padre di se stesso

		if(ObjectsSystem::instance()->isVirtual(getParent()) && getParent() != ObjectsSystem::instance()->getRootID())
			return false;		// L'oggetto non pu essere figlio di una sezione virtuale, a meno che non sia la root.

		// TOFIX_VALIDATING
		/*
		if(allowParent(parent->getObjectType()) == false)
			return false;
		*/
	}

#ifndef OS_TODOCIP
	if(isEntity() && visible == false)
		return false;	// Le primarie non possono essere invisibili

	if(revision->validateRevisionable(true, true) == false)
		return false;
#endif

	// Se la primaria  valorizzata la tipologia di oggetti deve corrispondere
	//if(revision->empty() == false && revision->getObjectType() != getObjectType())
	//	return false;
	// TOCLEAN_OBJECTID_OBJECTTYPE . Controllo abolibile?
	// TOFIX_VALIDATING: 
	/*
	if( (isEntity() == false) && (getEntityID().getObjectType() != getObjectType()) )
		return false;
	*/

	return true;
}

LanguageResult ObjectsIRevisionable::acceptable(shared_ptr<IPortalDatabase> database) const
{
	LanguageResult result = EntryBase::acceptable(database);
	if(result.empty() == false)
		return result;

	if(entity_author->empty())
		return LanguageResult("entity_author_required");
	if(entity_date->isValid() == false)
		return LanguageResult("entity_date_invalid");

	// Soglia editore
	{
		bool needThresholdEditor = false;

		// The same user can edit it's objects.
		// OS_TODOCIP: isEntity Da sistemare? 
#ifdef OS_TODOCIP
		//if( (isEntity() == false) && (getAuthor() != getEntityAuthor()) )
		if(getAuthor() != getEntityAuthor())
			needThresholdEditor = true;
#else
		if( (isEntity() == false) && (getAuthor() != objects_revisionable_cast(database->getObject(getEntityID().toObjectID()))->getAuthor()) )
			needThresholdEditor = true;
#endif

		if(getObjectType() == portalObjectTypeSection)
			needThresholdEditor = true;

		if(getObjectType() == portalObjectTypeInstance)
			needThresholdEditor = true;

		
		
		if(needThresholdEditor) 
		{
		
			ObjectsReputationThreshold threshold = database->getPortal()->getOptionsShared()->getEditorsReputationThreshold();
		
			ReputationsScore scoreDetail = database->getPortal()->getSnapshotManager()->computeReputation(database, getAuthor().toUTF16());
			bool valid = database->getPortal()->getSnapshotManager()->compareReputation(threshold, scoreDetail.getSafeValue());
			if(valid == false)
				return LanguageResult("under_editor_threshold");		
		}
	}

	return LanguageResult();
}

void ObjectsIRevisionable::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	EntryBase::exportXML(exporter);

	shared_ptr<XMLNode> node = exporter->getRoot();

	node->setAttributeString(_S("entity"), getEntityID().toUTF16());
	node->setAttributeDateTime(_S("entity_date"), entity_date);
	node->setAttributeString(_S("entity_author"), entity_author->toUTF16());
	node->setAttributeString(_S("entity_parent"), entity_parent->toUTF16());

	node->setAttributeString(_S("parent"), parent->toUTF16());

	if(entity_date != submit_date)
		node->setAttributeBool(_S("revisioned"), true); // Utility only			

	if(exporter->getMode() != XMLPortalExporter::emData)
	{
#ifdef OS_TODOCIP
		/*
		if(isEntity() == false)
			node->setAttributeString(_S("revision"), id->toUTF16());
		*/
		
		/* TOCLEAN: "moved" never used?
		if(entity_parent != parent)
			node->setAttributeBool(_S("moved"), true);
		*/
#else
		if(isEntity() == false)
		{
			node->setAttributeString(_S("revision"), id->toUTF16());

			shared_ptr<ObjectsIRevisionable> primary = objects_revisionable_cast(exporter->getPage()->getObject(getEntityID().toObjectID()));
			if(primary != nullptr && primary->parent != parent)
			{
				node->setAttributeBool(_S("moved"), true);
			}
		}
#endif

		node->setAttributeString(_S("edit_href"), exporter->getPortal()->getEditObjectLink(getEntityID()));
		node->setAttributeString(_S("view_href"), exporter->getPortal()->getViewObjectLink(getEntityID()));
	}

	if(visible != true)
		node->setAttributeBool(DBTABLES::VISIBLE, visible);
	node->setAttributeDouble(DBTABLES::POSITION, position);
	if(comment->empty() == false)
		node->setAttributeString(DBTABLES::COMMENT, comment);	
}

void ObjectsIRevisionable::exportAuthor(shared_ptr<XMLPortalExporter> exporter)
{
#ifdef OS_TODOCIP
	shared_ptr<ObjectsUser> authorCurrent = objects_user_cast(exporter->getPage()->getObject(author));
	if(authorCurrent == nullptr)
		return;

	if(author != entity_author)
	{
		shared_ptr<ObjectsUser> authorEntity = objects_user_cast(exporter->getPage()->getObject(entity_author));
		if(authorEntity == nullptr)
			return;

		shared_ptr<XMLPortalExporter> editorExporter(OS_NEW XMLPortalExporter(exporter->getNode(_S("editor")), exporter->getPage(), exporter->getMode()));
		authorCurrent->exportXML(editorExporter);

		shared_ptr<XMLPortalExporter> authorExporter(OS_NEW XMLPortalExporter(exporter->getNode(DBTABLES::AUTHOR), exporter->getPage(), exporter->getMode()));
		authorEntity->exportXML(authorExporter);	
	}
	else
	{
		shared_ptr<XMLPortalExporter> authorExporter(OS_NEW XMLPortalExporter(exporter->getNode(DBTABLES::AUTHOR), exporter->getPage(), exporter->getMode()));
		authorCurrent->exportXML(authorExporter);	
	}
	
#else
	shared_ptr<ObjectsUser> authorObj = objects_user_cast(exporter->getPage()->getObject(author));
	if(authorObj == nullptr)
		return;

	// Controlla se l'oggetto  una revisione
	if(authorObj != nullptr && isEntity() == false)
	{		
		// Carica la primaria della revisione
		shared_ptr<ObjectsIRevisionable> primary = objects_revisionable_cast(exporter->getPage()->getObject(getEntityID().toObjectID()));
		// Controlla se l'autore della revisione non coincide con quello della primaria
		if(primary != nullptr && primary->author != authorObj->id)
		{
			// Esporta l'autore della revisione sotto il nodo "editor"
			shared_ptr<XMLPortalExporter> editorExporter(OS_NEW XMLPortalExporter(exporter->getNode(_S("editor")), exporter->getPage(), exporter->getMode()));
			authorObj->exportXML(editorExporter);

			// Salva l'autore della primaria
			authorObj = objects_user_cast(exporter->getPage()->getObject(primary->author));
		}
	}

	// Esporta l'autore della primaria
	if(authorObj == nullptr)
		return;
	shared_ptr<XMLPortalExporter> authorExporter(OS_NEW XMLPortalExporter(exporter->getNode(DBTABLES::AUTHOR), exporter->getPage(), exporter->getMode()));
	authorObj->exportXML(authorExporter);	
#endif
}

void ObjectsIRevisionable::onObjectCreated(const shared_ptr<IPortalDatabase> &db, shared_ptr<Portal> portal)
{
	EntryBase::onObjectCreated(db, portal);

	// Se l'utente ha creato un oggetto, lo stabilizzo al volo, in modo che sia subito visibile.
	// Di solito ad esempio se  una sezione avviene una "view" che comporta la stabilizzazione, ma ci
	// sono casi in cui serve qui, ad esempio se creo un post (la "view" punta all'oggetto text).

	portal->getSnapshotManager()->ensure(db, getEntityID());
}

bool ObjectsIRevisionable::onCreate(const shared_ptr<IPortalDatabase> &db, const Buffer &privateKey)
{
	if(EntryBase::onCreate(db, privateKey) == false)
		return false;

	shared_ptr<Portal> portal = db->getPortal();

#ifndef OS_TODOCIP
	// Controlla se l'oggetto rappresenta una revisione
	if(isEntity() == false)
	{
		// Carica la primaria della revisione
		shared_ptr<ObjectsIRevisionable> primary_object = objects_revisionable_cast(portal->getObject(db, revision->toUTF16()));
		if(primary_object == nullptr)
			return false;

		// La primaria deve essere valida
		OS_ASSERT(db->validateRecord(primary_object));

		// Controlla se la primaria specificata in realt corrisponde ad una revisione
		if(primary_object->isEntity() == false)
			return false;

		// Verifica che la tipologia di oggetti corrisponda
		if(primary_object->getObjectType() != getObjectType())
			return false;

		// Se  stata specificata una data valida la nuova revisione non pu essere antecedente alla base
		if(submit_date->isValid() && (submit_date < primary_object->submit_date))
			return false;
	}
#endif

	// CLODOURGENT, tolto per ora.
	// Qui devo decidere se non fare in controllo (e poi la stabilizzazione li piazza sotto Orphans)
	// o se fare l'ensure qui (nell'ottica di abolire la stabilizzazione, che non sarebbe male).
	// In ogni caso la allowParent che faceva qui ci andrà da qualche parte...
	/*
	if(hasParent())
	{
		sbagliato...

		// Carica il padre corrente
		shared_ptr<ObjectsIObject> parent_object = portal->getObject(db, parent->toUTF16());
		if(parent_object == nullptr)
			return false;

		// Il parent deve essere valido
		OS_ASSERT(db->validateRecord(parent_object));

		// Verifica che il padre specificato sia ammesso per l'oggetto
		if(allowParent(parent_object->getObjectType()) == false)
			return false;

	}
	*/

	if(position == OS_PORTAL_OBJECT_POSITION_NULL)
	{
		// Se non  stata specificata una posizione per l'oggetto ne genera una di default
		position = static_cast<double>(submit_date->toTime_t());
	}

#ifdef OS_TODOCIP
	if(entity_author->empty())		
	{
		// Valorizzo i campi dell'entita'
#ifdef OS_NOOBJECTID
		entity = EntityID(EntityID::generate());
#else
		entity = id->toEntityID();
#endif
		entity_author = author;
		entity_date = submit_date;
		entity_parent = parent;
		signEntity(privateKey, db->getPortal()->getPortalID());
	}
#endif

	return true;
}

bool ObjectsIRevisionable::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(EntryBase::onSign(signer) == false)
		return false;

	signer->add(parent);
	
#ifdef OS_TODOCIP
	signer->add(entity);
	signer->add(entity_author);
	signer->add(entity_date);
	signer->add(entity_parent);
	signer->add(entity_signature);
#else
	signer->add(revision);
#endif
	signer->add(visible);
	signer->add(position);
	signer->add(comment);

	return true;
}

bool ObjectsIRevisionable::onRead(const ObjectID &id, const DataTree &data)
{
	if(EntryBase::onRead(id, data) == false)
		return false;
		
#ifdef OS_TODOCIP
	entity.read(data, DBTABLES::ENTITY);
	entity_author.read(data, DBTABLES::ENTITY_AUTHOR);
	entity_date.read(data, DBTABLES::ENTITY_DATE);
	entity_parent.read(data, DBTABLES::ENTITY_PARENT);
	entity_signature.read(data, DBTABLES::ENTITY_SIGNATURE);
#else
	revision.read(data, DBTABLES::REVISION);
#endif
	parent.read(data, DBTABLES::PARENT);
	visible.read(data, DBTABLES::VISIBLE);
	position.read(data, DBTABLES::POSITION);
	comment.read(data, DBTABLES::COMMENT);

	return true;
}

bool ObjectsIRevisionable::onWrite(DataTree &data) const
{
	if(EntryBase::onWrite(data) == false)
		return false;

	parent.write(data, DBTABLES::PARENT);
#ifdef OS_TODOCIP
	entity.write(data, DBTABLES::ENTITY);
	entity_author.write(data, DBTABLES::ENTITY_AUTHOR);
	entity_date.write(data, DBTABLES::ENTITY_DATE);
	entity_parent.write(data, DBTABLES::ENTITY_PARENT);
	entity_signature.write(data, DBTABLES::ENTITY_SIGNATURE);
#else
	revision.write(data, DBTABLES::REVISION);
#endif
	visible.write(data, DBTABLES::VISIBLE);
	position.write(data, DBTABLES::POSITION);
	comment.write(data, DBTABLES::COMMENT);

	return true;
}

bool ObjectsIRevisionable::onRead(const DataTableRow &row)
{
	if(EntryBase::onRead(row) == false)
		return false;

	parent.read(row, DBTABLES::PARENT);
#ifdef OS_TODOCIP	
	entity.read(row, DBTABLES::ENTITY);
	entity_author.read(row, DBTABLES::ENTITY_AUTHOR);
	entity_date.read(row, DBTABLES::ENTITY_DATE);
	entity_parent.read(row, DBTABLES::ENTITY_PARENT);
	entity_signature.read(row, DBTABLES::ENTITY_SIGNATURE);
#else
	revision.read(row, DBTABLES::REVISION);
#endif
	visible.read(row, DBTABLES::VISIBLE);
	position.read(row, DBTABLES::POSITION);
	comment.read(row, DBTABLES::COMMENT);

	return true;
}

bool ObjectsIRevisionable::onWrite(DbSqlValues &values) const
{
	if(EntryBase::onWrite(values) == false)
		return false;

	parent.write(values, DBTABLES::PARENT);
#ifdef OS_TODOCIP	
	entity.write(values, DBTABLES::ENTITY);
	entity_author.write(values, DBTABLES::ENTITY_AUTHOR);
	entity_date.write(values, DBTABLES::ENTITY_DATE);
	entity_parent.write(values, DBTABLES::ENTITY_PARENT);
	entity_signature.write(values, DBTABLES::ENTITY_SIGNATURE);
#else
	revision.write(values, DBTABLES::REVISION);
#endif
	visible.write(values, DBTABLES::VISIBLE);
	position.write(values, DBTABLES::POSITION);
	comment.write(values, DBTABLES::COMMENT);

	return true;
}



//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
