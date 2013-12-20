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
#include "objectsicommentable.h"

#include "datadiscussionstats.h"
#include "iportaldatabase.h"
#include "dbbinder.h"
#include "dbtables.h"
#include "entitiesentity.h"
#include "iideportalpage.h"
#include "datatree.h"
#include "objectsmodel.h"
#include "objectssigner.h"
#include "portalsportal.h"
#include "idesession.h"
#include "portalsoptionsshared.h"
#include "realtimestats.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ObjectsICommentable::ObjectsICommentable(PortalObjectType object_type, uint32 object_version) : RevisionableBase(object_type, object_version)
{
	pinned = false;
}

ObjectsICommentable::~ObjectsICommentable()
{

}

bool ObjectsICommentable::_applyModel(String &str, shared_ptr<IPortalDatabase> database, shared_ptr<Portal> portal)
{
	OS_ASSERT(str.empty());
	OS_ASSERT(portal != nullptr);
	OS_ASSERT(hasModel());

	// Carica il modello di riferimento
	shared_ptr<EntitiesEntity> model_entity = portal->getEntity(database, model);
	if(model_entity != nullptr)
	{
		shared_ptr<ObjectsModel> text_model = objects_model_cast(model_entity->getCurrent());
		if(text_model != nullptr)
		{
			// Applica l'xml del testo al modello
			return text_model->apply(content, str);
		}
	}

	return false;
}

void ObjectsICommentable::getDependencies(const shared_ptr<IPortalDatabase> &db, list<ObjectID>::type &dependencies)
{
	RevisionableBase::getDependencies(db, dependencies);

#ifndef OS_TODOCIP
	if(hasModel())
		dependencies.push_back(model->toObjectID());
#endif
}

bool ObjectsICommentable::validate(shared_ptr<IPortalDatabase> database) const
{
	if(RevisionableBase::validate(database) == false)
		return false;
	
	// TOFIX_VALIDATING
	/*
	if(model->empty() == false)
	{
		if(model->getObjectType() != portalObjectTypeModel || model->validateRevisionable(true, true) == false)
			return false;
	}

	if(hasModel())
	{
		shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
		if(document->parseString(content) == false)
			return false;
	}
	*/

	return true;
}

LanguageResult ObjectsICommentable::acceptable(shared_ptr<IPortalDatabase> database) const
{
	LanguageResult result = RevisionableBase::acceptable(database);
	if(result.empty() == false)
		return result;

	if(title->empty())
		return LanguageResult("title_required");

	result = acceptableForBadWords(database, title);
	if(result.empty() == false)
		return result;
	
	result = acceptableForBadWords(database, description);
	if(result.empty() == false)
		return result;

	result = acceptableForBadWords(database, content);
	if(result.empty() == false)
		return result;

	return LanguageResult();
}


const String & ObjectsICommentable::getTitle() const
{
	return title;
}

const String & ObjectsICommentable::getDescription() const
{
	return description;
}

const String & ObjectsICommentable::getContent() const
{
	return content;
}

void ObjectsICommentable::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	RevisionableBase::exportXML(exporter);

	shared_ptr<XMLNode> node = exporter->getRoot();

	if(exporter->getMode() == XMLPortalExporter::emData)
	{
		node->setAttributeString(DBTABLES::TITLE, title);

		if(description->empty() == false)
			node->setAttributeString(DBTABLES::DESCRIPTION, description);

		node->setAttributeString(DBTABLES::CONTENT, content);

		node->setAttributeString(DBTABLES::MODEL, model->toUTF16());

		node->setAttributeBool(DBTABLES::PINNED, pinned);

		if(closed_date.isNull() == false && closed_date->isValid())
			node->setAttributeDateTime(DBTABLES::CLOSED_DATE, closed_date);
	}
	else if(exporter->getMode() == XMLPortalExporter::emFull)
	{
		node->setAttributeString(DBTABLES::TITLE, title);
		if(description->empty() == false)
			node->setAttributeString(DBTABLES::DESCRIPTION, description);

		bool exportContent = true;

		if(hasModel())
		{
			// L'esportazione del modello potrebbe servire per testare se un oggetto testo  associato ad un modello
			node->setAttributeString(DBTABLES::MODEL, model->toUTF16());

			String str;
			if(_applyModel(str, exporter->getDatabase(), exporter->getPage()->getPortal()))
			{
				exportContent = false;
				// Esporta l'output del modello
				node->setAttributeString(DBTABLES::CONTENT, str);
			}
		}

		if(exportContent)
			node->setAttributeString(DBTABLES::CONTENT, content);

		node->setAttributeBool(DBTABLES::PINNED, pinned);

		if(closed_date.isNull() == false && closed_date->isValid())
			node->setAttributeDateTime(DBTABLES::CLOSED_DATE, closed_date);
	}

	if(exporter->getWithStats())
	{
		shared_ptr<DataIStatistics> stats = exporter->getDatabase()->loadStatistics<DataDiscussionStats>(DBTABLES::DISCUSSIONS_STATS_TABLE, getEntityID().getString());
		if(stats != nullptr)
			stats->exportXML(exporter);
	}
}

void ObjectsICommentable::onExportContent(String &content, shared_ptr<XMLPortalExporter> exporter)
{
	RevisionableBase::onExportContent(content, exporter);

	// Controlla se il documento  associato ad un modello
	if(hasModel())
	{
		String str;
		// Applica il modello di trasformazione
		if(_applyModel(str, exporter->getDatabase(), exporter->getPage()->getPortal()))
		{
			// Esporta il contenuto trasformato
			content = str;
		}
	}
}

bool ObjectsICommentable::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(RevisionableBase::onSign(signer) == false)
		return false;

	signer->add(title);
	signer->add(description);
	signer->add(content);
	signer->add(model);
	signer->add(pinned);
	signer->add(closed_date);

	return true;
}

bool ObjectsICommentable::onRead(const ObjectID &id, const DataTree &data)
{
	if(RevisionableBase::onRead(id, data) == false)
		return false;

	title.read(data, DBTABLES::TITLE);
	description.read(data, DBTABLES::DESCRIPTION);
	content.read(data, DBTABLES::CONTENT);
	model.read(data, DBTABLES::MODEL);
	pinned.read(data, DBTABLES::PINNED);
	closed_date.read(data, DBTABLES::CLOSED_DATE);

	return true;
}

bool ObjectsICommentable::onWrite(DataTree &data) const
{
	if(RevisionableBase::onWrite(data) == false)
		return false;

	title.write(data, DBTABLES::TITLE);
	description.write(data, DBTABLES::DESCRIPTION);
	content.write(data, DBTABLES::CONTENT);
	model.write(data, DBTABLES::MODEL);
	pinned.write(data, DBTABLES::PINNED);
	closed_date.write(data, DBTABLES::CLOSED_DATE);

	return true;
}

bool ObjectsICommentable::onRead(const DataTableRow &row)
{
	if(RevisionableBase::onRead(row) == false)
		return false;

	title.read(row, DBTABLES::TITLE);
	description.read(row, DBTABLES::DESCRIPTION);
	content.read(row, DBTABLES::CONTENT);
	model.read(row, DBTABLES::MODEL);
	pinned.read(row, DBTABLES::PINNED);
	closed_date.read(row, DBTABLES::CLOSED_DATE);

	return true;
}

bool ObjectsICommentable::onWrite(DbSqlValues &values) const
{
	if(RevisionableBase::onWrite(values) == false)
		return false;

	title.write(values, DBTABLES::TITLE);
	description.write(values, DBTABLES::DESCRIPTION);
	content.write(values, DBTABLES::CONTENT);
	model.write(values, DBTABLES::MODEL);
	pinned.write(values, DBTABLES::PINNED);
	closed_date.write(values, DBTABLES::CLOSED_DATE);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsICommentableDescriptor::ObjectsICommentableDescriptor()
{

}

ObjectsICommentableDescriptor::~ObjectsICommentableDescriptor()
{

}

void ObjectsICommentableDescriptor::getAllowedChilds(ObjectsTypes &allowed_childs) const
{
	DescriptorBase::getAllowedChilds(allowed_childs);

	allowed_childs.push_back(portalObjectTypePost);
	allowed_childs.push_back(portalObjectTypeTag);				// Tutti gli oggetti commentabili sono anche taggabili
	allowed_childs.push_back(portalObjectTypeAttachment);		// Tutti gli oggetti commentabili ammettono degli allegati
}

bool ObjectsICommentableDescriptor::hasStatistics()
{
	return true;
}

void ObjectsICommentableDescriptor::createStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	DescriptorBase::createStatistics(db, object);

	shared_ptr<ObjectsICommentable> commentableObject = objects_commentable_cast(object);
	OS_ASSERT(commentableObject != nullptr);

	shared_ptr<DataDiscussionStats> statistics(OS_NEW DataDiscussionStats());

	// E' inoltre importante effettuare l'inizializzazione perch in fase di estrazione su alcuni oggetti viene
	// fatto un ordinamento per data quindi  importante avere tale statistica gi inizializzata

	statistics->last_entry = commentableObject->getEntityID();
	statistics->last_entry_date = commentableObject->submit_date;
	statistics->pinned = commentableObject->pinned;
	statistics->reference = commentableObject->getEntityID().toUTF16();

	statistics->create_ZZZ(db); // URGENT: Dentro qui fa una select inutile?
}

void ObjectsICommentableDescriptor::computeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	RealtimeStatsScopeTimer RSSX(_S("Debug"), _S("SnapshotManager::updateObjectStatistics::ICommentableDescriptor::computeStatistics"));

	DescriptorBase::computeStatistics(db, object);

	shared_ptr<ObjectsICommentable> commentableObject = objects_commentable_cast(object);
	OS_ASSERT(commentableObject != nullptr);

	shared_ptr<DataDiscussionStats> stats(OS_NEW DataDiscussionStats());
	stats->reference = commentableObject->getEntityID().toUTF16();
	stats->parent = commentableObject->getParent();
	RealtimeStatsScopeTimer RSS3(_S("Debug"), _S("SnapshotManager::updateObjectStatistics::ICommentableDescriptor::getStableObjectsCount"));
	stats->posts = db->getStableObjectsCount(commentableObject->getEntityID(), portalObjectTypePost);
	RSS3.stop();

	RealtimeStatsScopeTimer RSS1(_S("Debug"), _S("SnapshotManager::updateObjectStatistics::ICommentableDescriptor::computeVotesStats"));
	db->computeVotesStats(commentableObject, stats->votes, stats->votes_average);
	RSS1.stop();
	RealtimeStatsScopeTimer RSS2(_S("Debug"), _S("SnapshotManager::updateObjectStatistics::ICommentableDescriptor::computeLastObjectStats"));
	db->computeLastObjectStats(commentableObject, stats->last_entry, stats->last_entry_date);
	RSS2.stop();

	if(stats->last_entry->empty())
	{
		stats->last_entry = commentableObject->getEntityID();

		// Nota:
		// La computeLastObjectStats valorizza last_entry_date alla submit_date della primaria.
		// Qui devo quindi far la stessa cosa.
		// 0.11 - stats->last_entry_date = commentableObject->submit_date;
		// 0.12

		//shared_ptr<ObjectsIObject> primaryObject = db->getObject(commentableObject->getPrimary());
#ifdef OS_TODOCIP
		stats->last_entry_date = commentableObject->entity_date;
#else
		shared_ptr<ObjectsIObject> primaryObject = db->getPortal()->getObject(db, commentableObject->getEntityID().toUTF16());

		if(primaryObject != nullptr)
			stats->last_entry_date = primaryObject->submit_date;
#endif
	}
	stats->pinned = commentableObject->pinned;

	db->updateRecord(stats);
}

void ObjectsICommentableDescriptor::removeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	DescriptorBase::removeStatistics(db, object);

	ObjectOrEntityID id;
	if(object)
	{
		shared_ptr<ObjectsICommentable> commentableObject = objects_commentable_cast(object);
		id = commentableObject->getEntityID().toUTF16();
	}

	db->removeProfileStatistics(DBTABLES::DISCUSSIONS_STATS_TABLE, id);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
