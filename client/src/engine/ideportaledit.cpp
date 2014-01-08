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
#include "ideportaledit.h"

#include "boost/bind.hpp"
#include "convert.h"
#include "conversions.h"
#include "iportaldatabase.h"
#include "entitiesentity.h"
#include "entitiesentitychilds.h"
#include "entitiessnapshotmanager.h"
#include "eventssource.h"
#include "ideeditiobjecteditor.h"
#include "idepathway.h"
#include "ideviewiobjectviewer.h"
#include "ideblock.h"
#include "idesession.h"
#include "idesessionportal.h"
#include "ideskin.h"
#include "ideutils.h"
#include "objectsdescriptor.h"
#include "objectsirevisionable.h"
#include "objectssystem.h"
#include "options.h"
#include "platformmanager.h"
#include "portalsportal.h"
#include "htmlevent.h"
#include "css.h"
#include "htmlwriter.h"
#include "httprequest.h"
#include "htmlcheckbox.h"
#include "htmllink.h"
#include "htmlliteral.h"
#include "htmlpopup.h"
#include "htmltable.h"
#include "htmltablecell.h"
#include "htmltablerow.h"
#include "htmltext.h"
#include "htmltextbox.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace portal {

//////////////////////////////////////////////////////////////////////

const std::wstring Edit::ID = _W("id");
const std::wstring Edit::ACTION = _W("act");
const std::wstring Edit::OBJECTTYPE = _W("ot");
const std::wstring Edit::MOVEID = _W("move_id");

//////////////////////////////////////////////////////////////////////

Edit::Edit(shared_ptr<HttpSession> session) : PageBase(session),
												m_showSystem(OS_NEW HtmlCheckBox()),	
												m_showInvisible(OS_NEW HtmlCheckBox())
{
	m_showSystem->setID(_S("showSystem"));
	m_showInvisible->setID(_S("showInvisible"));

	m_showSystem->setAutoPostBack(true);
	m_showInvisible->setAutoPostBack(true);

	getEvents()->get("onSelect")->connect(boost::bind(&Edit::onSelect, this));
	getEvents()->get("onUnselect")->connect(boost::bind(&Edit::onUnselect, this));
	getEvents()->get("onDelete")->connect(boost::bind(&Edit::onDelete, this));
	getEvents()->get("onMove")->connect(boost::bind(&Edit::onMove, this, _1));
	getEvents()->get("onRestore")->connect(boost::bind(&Edit::onRestore, this, _1));
}

Edit::~Edit()
{

}

String Edit::getUrlID() const
{
	return getRequest()->getUrlParam(ID);
}

Edit::Action Edit::getUrlAction() const
{
	return static_cast<Edit::Action>(conversions::from_utf16<uint32>(getRequest()->getUrlParam(ACTION)));
}

PortalObjectType Edit::getUrlAddChildObjectType() const
{
	return Convert::toObjectType(getRequest()->getUrlParam(OBJECTTYPE));
}

ObjectID Edit::getUrlAddMoveObject() const
{
	return utils::wtoa(getRequest()->getUrlParam(MOVEID));
}

String Edit::getUrl(shared_ptr<Portal> portal, const EntityID &id, Action action, const ObjectID &revision)
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(ID, id.toWide());
	params.set(ACTION, conversions::to_wstring(static_cast<int32>(action)));
	if(revision.empty() == false)
		params.set(OS_URL_PARAM_REVISION, revision.toWide());

	return portal->getLink("edit", params);
}

String Edit::getAddChildUrl(shared_ptr<Portal> portal, const EntityID &id, PortalObjectType type)
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(ID, id.toWide());
	params.set(ACTION, conversions::to_wstring(static_cast<int32>(acAddChild)));
	params.set(OBJECTTYPE, Convert::toString(type).to_wide());

	return portal->getLink("edit", params);
}

/*
String Edit::getAddMoveUrl(shared_ptr<Portal> portal, const ObjectID &id, const ObjectID &moveId)
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(ID.to_ascii(), id.getString());
	params.set(ACTION.to_ascii(), conversions::to_string(static_cast<int32>(acAddMove)));
	params.set(MOVEID.to_ascii(), moveId.getString());

	return portal->getLink(portal::ppEdit, params);
}
*/

void Edit::exportHistory(shared_ptr<EntitiesEntity> entity, shared_ptr<XMLPortalExporter> exporter)
{
	OS_ASSERT(entity != nullptr);

	if(ObjectsSystem::instance()->isVirtual(entity->getEntityID()))
		return;

	/*
	// Devo evitare di mostrare la prima, o la primaria se results.size()==0.
	// N.B. La primaria potrebbe non essere valida (saltata dalla snapshotManager, ma qui dumpata comunque...
	// Da sistemare...

	StringList results;
	getLoggedUser()->getSnapshotProfile()->getObjectRevisions(getDatabase(), entity->getPrimary(), results);

	shared_ptr<XMLNode> nodeHistory = exporter->createNode(_S("history"));
	for(StringList::const_iterator i = results.begin(); i != results.end(); ++i)
	{
		shared_ptr<ObjectsIObject> revision = objects_revisionable_cast(getLoggedUser()->getObject(*i));
		if(revision != nullptr)
		{
			_exportRevision(nodeHistory, entity, revision);
		}
	}

	_exportRevision(nodeHistory, entity, entity->getPrimary());
	*/
	StringList results;
	getPortal()->getSnapshotManager()->getObjectRevisions(getDatabase(), entity->getEntityID(), entity->getObjectType(), results);

	shared_ptr<XMLNode> nodeHistory = exporter->createNode(_S("history"));
	for(StringList::const_iterator i = results.begin(); i != results.end(); ++i)
	{
		shared_ptr<ObjectsIObject> entry = objects_revisionable_cast(getObject((*i).to_ascii()));
		if(entry != nullptr)
		{
			_exportRevision(nodeHistory, entity, entry);
		}
	}
}

void Edit::exportParent(shared_ptr<EntitiesEntity> entity, shared_ptr<XMLPortalExporter> exporter)
{
	shared_ptr<EntitiesEntity> parentEntity = entity->getParent(getDatabase());
	if(parentEntity != nullptr)
	{
		shared_ptr<XMLPortalExporter> parentExporter(OS_NEW XMLPortalExporter(exporter->createNode(_S("parent")), get_this_ptr<IPortalPage>(), XMLPortalExporter::emLite));
		parentEntity->exportXML(parentExporter);
	}
}

void Edit::exportChilds(shared_ptr<EntitiesEntity> entity, shared_ptr<XMLPortalExporter> exporter)
{
	ObjectsTypes allowedChilds;
	entity->getCurrent()->getDescriptor()->getAllowedChilds(allowedChilds);
	if(allowedChilds.empty() == true)
		return;

	bool includeSystem = false;
	bool includeInvisible = m_showInvisible->getCheck();
	if(entity->getEntityID() == ObjectsSystem::instance()->getRootID())
		includeSystem = m_showSystem->getCheck();	

	shared_ptr<XMLNode> objects_node = exporter->createNode(_S("childs"));

	// Numero di figli da mostrare per pagina
	uint32 childs_to_show = getSkin()->getPagerItems();

	shared_ptr<DbSqlSelect> select;
	// Inizializza lo statement di base per il caricamento dei figli
	entity->getChildsStatement(getDatabase(), portalObjectTypeUnknown, RangeUint32::EMPTY, true, includeSystem, includeInvisible, select);

	// Calcola il numero totale di figli
	uint32 total_childs = entity->getChildsCount(getDatabase(), portalObjectTypeUnknown, select);
	if(total_childs > 0)
	{
		uint32 offset = conversions::from_utf16<uint32>(getRequest()->getUrlParam(OS_URL_PARAM_OFFSET));

		// Corregge l'offset se necessario
		adjustOffset(total_childs, childs_to_show, offset);

		RangeUint32 range(offset, childs_to_show);

		select.reset();
		entity->getChildsStatement(getDatabase(), portalObjectTypeUnknown, range, false, includeSystem, includeInvisible, select);

		// Aggiunge l'ordinamento per tipo
		DbSqlField type_field(DBTABLES::SNAPSHOT_OBJECTS::TYPE, DBTABLES::SNAPSHOT_OBJECTS_TABLE);
		type_field.setOrder(DbSqlField::foAsc);
		select->orderBy.fields.add(type_field);
		// Aggiunge all'estrazione l'ordinamento per data di creazione
		DbSqlField submitdate_field(DBTABLES::SNAPSHOT_OBJECTS::SUBMIT_DATE, DBTABLES::SNAPSHOT_OBJECTS_TABLE);
		submitdate_field.setOrder(DbSqlField::foDesc);
		select->orderBy.fields.add(submitdate_field);

		shared_ptr<EntitiesEntities> section_childs = entity->getChilds(getDatabase(), portalObjectTypeUnknown, range, select);
		
		for(EntitiesEntities::iterator i = section_childs->begin(); i != section_childs->end(); ++i)
		{
			shared_ptr<EntitiesEntity> child_entity = section_childs->get(getDatabase(), *i);
			if(child_entity != nullptr)
			{
				shared_ptr<XMLPortalExporter> childExporter(OS_NEW XMLPortalExporter(objects_node->addChild(OS_PORTAL_OBJECT_OBJECT_TYPENAME), get_this_ptr<IPortalPage>(), XMLPortalExporter::emFull));
				child_entity->exportXML(childExporter);
			}
		}

		createPager(getSkin(), exporter->getRoot(), total_childs, childs_to_show, offset);
	}
}

String Edit::getPageHref(uint32 offset) const
{
	String href;
	if(m_entity != nullptr)
	{
		ordered_map<std::wstring, std::wstring> params;
		if(offset != 0)
			params.set(OS_URL_PARAM_OFFSET, conversions::to_wstring(offset));

		href = m_entity->getEditLink(getPortal(), params);
	}

	return href;
}

void Edit::_exportRevision(shared_ptr<XMLNode> node, shared_ptr<EntitiesEntity> entity, shared_ptr<ObjectsIObject> revision)
{
	if(revision != nullptr)
	{
		shared_ptr<XMLPortalExporter> revisionExporter(OS_NEW XMLPortalExporter(node->addChild(OS_PORTAL_OBJECT_OBJECT_TYPENAME), get_this_ptr<IPortalPage>(), XMLPortalExporter::emLite));
		revision->exportXML(revisionExporter);

		if( (getSessionAccount()->isPortalGuest(getDatabase()) == false) && (entity->getCurrent()->id != revision->id) )
		{
			//String actionHref = getUrl(getPortal(), revision->getPrimary(), acRestoreRevision, revision->id);
			String actionHref = getEventCommand("onRestore", revision->id->toUTF16());
			revisionExporter->createAction(_S("restore"), actionHref);
		}
	}
}

String Edit::getPageName() const
{
	return _S("portal.pages.edit");
}

String Edit::getPagePathway() const
{
	return getPortal()->getLink("edit");
}

EntityID Edit::getTargetObject() const
{
	return static_cast<String>(getRequest()->getUrlParam(OS_URL_PARAM_ID)).to_ascii();
}

void Edit::onLoad()
{
	PageBase::onLoad();

	m_showSystem->setCaption(getText(_S("portal.pages.edit.show.system")));
	m_showInvisible->setCaption(getText(_S("portal.pages.edit.show.invisible")));

	EntityID id = getUrlID().to_ascii();

	m_entity = getPortal()->getEntity(getDatabase(), id, true);

	bool isRoot = (id == ObjectsSystem::instance()->getRootID());
	bool isVirtual = ObjectsSystem::instance()->isVirtual(id);	
	bool isReadOnly = (getSessionAccount()->isPortalGuest(getDatabase()));

	if(m_entity == nullptr)
	{
		showError(getText(_S("ide.messages.unknown_object")));
		return;
	}

	if (m_entity->getCurrent() == nullptr)
	{
		showError(getText(_S("ide.messages.invalid_object")));
		return;
	}
	
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	shared_ptr<XMLNode> node_root = document->create(_S("edit"));

	//shared_ptr<XMLNode> node_object = node_root->addChild(OS_PORTAL_OBJECT_OBJECT_TYPENAME);

	String path = getSkin()->getTemplatePath(_S("edit.xsl"));
	m_template.reset(OS_NEW HtmlXSLControl(loadStylesheet(path), document));

	if(m_entity->getEntityID() == ObjectsSystem::instance()->getRootID())
		m_template->addChildParam(m_showSystem, _S("show_system"));	
	m_template->addChildParam(m_showInvisible, _S("show_invisible"));	

	// Carica il template di riferimento
	getArea(pageAreaContent)->getControls()->add(m_template);



	// Titolo
	setPageTitle(parseOml(m_entity->getCurrent()->getTitle(), false, false, true) + _S(" ") + getText(_S("portal.pages.edit.title_suffix")));	
	setPageDescription(parseOml(m_entity->getCurrent()->getDescription(), false, false, true));

	// Carica il pathway dell'entit
	getPathway()->add(getDatabase(), m_entity, rmEdit);

	//shared_ptr<ObjectsIRevisionable> primary = m_entity->getPrimary();
	shared_ptr<ObjectsIRevisionable> current = m_entity->getCurrent();

	StringMap &selectedObjects = getSessionAccount()->getSessionPortal(getPortal())->getSelectedObjects();

	shared_ptr<XMLNode> nodeRoot = m_template->getDocument()->getRoot();

	shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(nodeRoot, get_this_ptr<IPortalPage>(), XMLPortalExporter::emFull));

	shared_ptr<IHtmlControl> activity_ctrl;

	Action action = getUrlAction();

	switch(action)
	{
		case acOverview:
		{
		} break;

		case acHistory:
		{
		} break;	
		
		case acMoveable:
		{
		} break;

		case acAddChild:
		{
			PortalObjectType ot = getUrlAddChildObjectType();
			activity_ctrl = ObjectsSystem::instance()->getDescriptor(ot)->createEditControl(nullptr, m_entity);
			activity_ctrl->setID(_S("activity"));
			m_template->addChildParam(activity_ctrl);
		} break;

		case acAddRevision:
		{
			PortalObjectType ot = m_entity->getObjectType();
			activity_ctrl = ObjectsSystem::instance()->getDescriptor(ot)->createEditControl(m_entity);
			activity_ctrl->setID(_S("activity"));
			m_template->addChildParam(activity_ctrl);
		} break;

		default:
		{
			OS_EXCEPTION("Unsupported params");
		} break;
	}

}

void Edit::onPreRender()
{
	PageBase::onPreRender();

	if(m_entity == nullptr)
		return;
		
	if (m_entity->getCurrent() == nullptr)
		return;

	EntityID id = m_entity->getEntityID();

	bool isRoot = (id == ObjectsSystem::instance()->getRootID());
	bool isVirtual = ObjectsSystem::instance()->isVirtual(id);
	bool isReadOnly = (getSessionAccount()->isPortalGuest(getDatabase()));

	//shared_ptr<ObjectsIRevisionable> primary = m_entity->getPrimary();
	shared_ptr<ObjectsIRevisionable> current = m_entity->getCurrent();

	StringMap &selectedObjects = getSessionAccount()->getSessionPortal(getPortal())->getSelectedObjects();

	shared_ptr<XMLNode> nodeRoot = m_template->getDocument()->getRoot();	

	shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(nodeRoot, get_this_ptr<IPortalPage>(), XMLPortalExporter::emFull));

	m_entity->exportXML(exporter);

	shared_ptr<XMLNode> nodeActions2 = nodeRoot->addChild(_S("actions2"));

	StringMap::const_iterator i = selectedObjects.find(m_entity->getEntityID().toUTF16());
	bool isSelected = (i != selectedObjects.end());



	Action action = getUrlAction();

	switch(action)
	{
	case acOverview:
		{
			m_template->getDocument()->getRoot()->setAttributeString("action","overview");
//			activity_ctrl = entity->getPrimary()->getDescriptor()->createViewControl(entity);
//			m_template->addChildParam(activity_ctrl, _S("activity"));
			
			exportParent(m_entity, exporter);
			exportChilds(m_entity, exporter);

		}
		break;
	case acHistory:
		{
			m_template->getDocument()->getRoot()->setAttributeString("action","history");
			exportHistory(m_entity, exporter);
		} break;	
		
	case acMoveable:
		{
			m_template->getDocument()->getRoot()->setAttributeString("action","moveable");
			shared_ptr<XMLNode> nodeMoveable = exporter->createNode(_S("moveable"));

			// Verifica tra gli oggetti selezionati quali potrebbero essere figli dell'oggetto corrente.
			bool found = false;
			for(StringMap::const_iterator i = selectedObjects.begin(); i != selectedObjects.end(); ++i)
			{
				String objectID = i->first;

				shared_ptr<EntitiesEntity> objectEntity = getPortal()->getEntity(getDatabase(), objectID.to_ascii());

				// Se l'oggetto pu essere figlio
				if( (objectEntity != nullptr) && (m_entity->allowChild(objectEntity->getObjectType())) && (objectEntity->getParentID()!=id) )
				{
					PortalObjectType objectOt = objectEntity->getObjectType();

					//String objectId = objectEntity->geEntityID();

					//String actionName = _S("move");
					//String actionHref = getAddMoveUrl(getPortal(), id, objectId.to_ascii());
					String actionHref = getEventCommand("onMove", objectEntity->getEntityID().toUTF16());

					shared_ptr<XMLPortalExporter> selectedObjectExporter(OS_NEW XMLPortalExporter(nodeMoveable->addChild("object"), get_this_ptr<IPortalPage>(), XMLPortalExporter::emLite));
					objectEntity->exportXML(selectedObjectExporter);

					selectedObjectExporter->setAttributeString("move_href",actionHref);

					//Utils::addAction(nodeActions, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_OBJECT_TYPENAME, actionName, actionHref, selectedObjectExporter->getRoot(), true);
				}
			}
		} break;
	
	}

	

	

	// Idem come sopra, viene ri-eseguito in modo da computare le modifiche svolte in base all'action sopra.
	/*
	m_entity = getLoggedUser()->getEntity(getDatabase(), id);
	primary = m_entity->getPrimary();
	current = m_entity->getCurrent();
	*/

	/*
	// L'history lo esporto solo se action = acHistory
	if(entity != nullptr)
	{
		shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(node_object, get_this_ptr<IPortalPage>(), XMLPortalExporter::emFull));
		entity->exportXML(exporter);

		if(getUrlAction() == acHistory)
			// Esporta l'history dell'entit
			exportHistory(entity, exporter);
	}
	*/

	/* --- Actions -------------------------------------------------------- */

	shared_ptr<XMLNode> nodeActions = nodeRoot->addChild(_S("actions"));
	
	Utils::addAction(nodeActions, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_OBJECT_TYPENAME, _S("overview"), getUrl(getPortal(), id, acOverview), (action == acOverview), false);

	if( (isReadOnly == false) && (isVirtual == false) )
	{
		// Add revision
		String actionName = _S("revise");
		String actionHref = getUrl(getPortal(), id, acAddRevision);

		Utils::addAction(nodeActions, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_OBJECT_TYPENAME, actionName, actionHref, (action == acAddRevision), false);
	}

	if(isVirtual == false)
		Utils::addAction(nodeActions, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_OBJECT_TYPENAME, _S("history"), getUrl(getPortal(), id, acHistory), (action == acHistory), false);
	
	// Add Childs
	if( (isReadOnly == false) && (!isVirtual || isRoot) )
	{
		ObjectsTypes allowedChilds;
		current->getDescriptor()->getAllowedChilds(allowedChilds);
		if(allowedChilds.empty() == false)
		{
			PortalObjectType otCurrent = getUrlAddChildObjectType();

			for(ObjectsTypes::const_iterator i = allowedChilds.begin(); i != allowedChilds.end(); ++i)
			{
				PortalObjectType ot = *i;

				if(OS_NAMESPACE_NAME::Options::instance()->getOption<bool>(_S("developer.enabled")) == false)
				{
					if(ot == portalObjectTypeCalendarEvent) // 0.13 RC4
						continue;
					if(ot == portalObjectTypeAttachment) // 0.13 RC4
						continue;
					if(ot == portalObjectTypeTag)
						continue;
					if(ot == portalObjectTypeAttribute)
						continue;					
				}

				String actionName = _S("add_") + ObjectsSystem::instance()->getDescriptor(ot)->getTypeName();
				String actionHref = getAddChildUrl(getPortal(), id, ot);

				Utils::addAction(nodeActions, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_OBJECT_TYPENAME, actionName, actionHref, ( (action == acAddChild) && (ot == otCurrent) ), false);
			}
		}
	}

	
	

	if( (isReadOnly == false) && (!isVirtual || isRoot) )
	{
		/*
		// Add revision move
		{
			// Verifica tra gli oggetti selezionati quali potrebbero essere figli dell'oggetto corrente.
			bool found = false;
			for(StringMap::const_iterator i = selectedObjects.begin(); i != selectedObjects.end(); ++i)
			{
				String objectID = i->first;

				shared_ptr<EntitiesEntity> objectEntity = getLoggedUser()->getEntity(getDatabase(), objectID.to_ascii());

				// Se l'oggetto pu essere figlio
				if( (objectEntity != nullptr) && (m_entity->allowChild(objectEntity->getObjectType())) && (objectEntity->getParentID()!=id) )
				{
					PortalObjectType objectOt = objectEntity->getObjectType();

					String objectId = objectEntity->getPrimary()->id->toUTF16();

					String actionName = _S("move");
					String actionHref = getAddMoveUrl(getPortal(), id, objectId.to_ascii());

					shared_ptr<XMLPortalExporter> selectedObjectExporter(OS_NEW XMLPortalExporter(shared_ptr<XMLNode>(OS_NEW XMLNode(OS_PORTAL_OBJECT_OBJECT_TYPENAME, m_template->getDocument())), get_this_ptr<IPortalPage>(), XMLPortalExporter::emLite));
					objectEntity->exportXML(selectedObjectExporter);

					Utils::addAction(nodeActions, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_OBJECT_TYPENAME, actionName, actionHref, selectedObjectExporter->getRoot(), true);
				}
			}
		}
		*/

		


		// Moveable enabled?
		{
			bool found = false;
			for(StringMap::const_iterator i = selectedObjects.begin(); i != selectedObjects.end(); ++i)
			{
				String objectID = i->first;

				shared_ptr<EntitiesEntity> objectEntity = getPortal()->getEntity(getDatabase(), objectID.to_ascii());

				// Se l'oggetto pu essere figlio
				if( (objectEntity != nullptr) && (m_entity->allowChild(objectEntity->getObjectType())) && (objectEntity->getParentID() != id) && (objectID != id.getString()) )
				{
					found = true;
					break;
				}
			}

			if(found)
				Utils::addAction(nodeActions, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_OBJECT_TYPENAME, _S("move"), getUrl(getPortal(), id, acMoveable), (action == acMoveable), false);
		}

		
	}	



	// View
	Utils::addAction(nodeActions2, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_OBJECT_TYPENAME, _S("view"), getPortal()->getViewObjectLink(id), false, false);

	if(isSelected)
	{
		//Utils::addAction(nodeActions2, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_OBJECT_TYPENAME, _S("unselect"), getUrl(getPortal(), id, acUnselect), (action == acUnselect), false);
		Utils::addAction(nodeActions2, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_OBJECT_TYPENAME, _S("unselect"), getEventCommand("onUnselect"), false, false);
	}
	else
	{
		//Utils::addAction(nodeActions2, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_OBJECT_TYPENAME, _S("select"), getUrl(getPortal(), id, acSelect), (action == acSelect), false);
		Utils::addAction(nodeActions2, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_OBJECT_TYPENAME, _S("select"), getEventCommand("onSelect"), false, false);
	}

	if( (isReadOnly == false) && (isVirtual == false) )
	{
		// Add revision delete
		if( (current != nullptr) && (current->visible) )
		{
			//String actionName = _S("delete");
			//String actionHref = getUrl(getPortal(), id, acDelete);

			//Utils::addAction(nodeActions2, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_OBJECT_TYPENAME, actionName, actionHref, (action == acDelete), true);
			Utils::addAction(nodeActions2, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_OBJECT_TYPENAME, _S("delete"), getEventCommand("onDelete"), false, true);
		}
	}
}

void Edit::onSelect()
{
	if(m_entity == nullptr)
		return;

	EntityID id = m_entity->getEntityID();
	StringMap &selectedObjects = getSessionAccount()->getSessionPortal(getPortal())->getSelectedObjects();
	selectedObjects[id.toUTF16()] = true;		
}

void Edit::onUnselect()
{
	if(m_entity == nullptr)
		return;

	EntityID id = m_entity->getEntityID();
	StringMap &selectedObjects = getSessionAccount()->getSessionPortal(getPortal())->getSelectedObjects();
	selectedObjects.erase(id.toUTF16());
}

void Edit::onDelete()
{
	if(m_entity == nullptr)
		return;
		
	if (m_entity->getCurrent() == nullptr)
		return;

	//ObjectID id = m_entity->getPrimary()->id;
	try
	{
		shared_ptr<ObjectsIRevisionable> current = m_entity->getCurrent();

		current->changeToRevision();
		current->visible = false;

		//ObjectID newID;
		//getDatabase()->__createRevisionDelete(current,newID,getLoggedUser()->getUserID().toUTF16(),getLoggedUser()->getPrivateKey());

		

		LanguageResult result = current->create(getDatabase(), getSessionAccount());
		if(result.empty())
		{
			showMessage(_S("Oggetto rimosso."));
		}
		else
		{
			showError(getText(result));
		}
	}
	catch(std::exception &e)
	{
		showError(e.what());
	}
}

void Edit::onMove(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == nullptr)
		return;

	if(m_entity == nullptr)
		return;

	if (m_entity->getCurrent() == nullptr)
		return;

	//ObjectID id = m_entity->getPrimary()->id;

	ObjectID movedId = htmlEvent->get(0).to_ascii();

	StringMap &selectedObjects = getSessionAccount()->getSessionPortal(getPortal())->getSelectedObjects();

	try
	{		
		shared_ptr<EntitiesEntity> movedEntity = getPortal()->getEntity(getDatabase(), movedId);
		if(movedEntity == nullptr)
			OS_EXCEPTION("Invalid entity");

				
		shared_ptr<ObjectsIRevisionable> obj = objects_revisionable_cast(movedEntity->getCurrent());
		obj->changeToRevision();
		obj->parent = m_entity->getCurrent()->getEntityID();

		LanguageResult result = obj->create(getDatabase(), getSessionAccount());
		if(result.empty())
		{
			// Deseleziono l'oggetto
			selectedObjects.erase(movedId.toUTF16());

			showMessage(_S("Oggetto mosso."));
		}
		else
		{
			showError(getText(result));
		}
	}
	catch(std::exception &e)
	{
		showError(e.what());
	}
}

void Edit::onRestore(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == nullptr)
		return;

	if(m_entity == nullptr)
		return;

	if (m_entity->getCurrent() == nullptr)
		return;

	//ObjectID id = m_entity->getPrimary()->id;

	ObjectID restoreId = htmlEvent->get(0).to_ascii();


	ObjectID newID;
	shared_ptr<ObjectsIRevisionable> revision_to_restore = objects_revisionable_cast(getPortal()->getObject(getDatabase(),restoreId));

	revision_to_restore->changeToRevision();
	// Razor 
	//if(getDatabase()->__createRevisionRestore(revision_to_restore, newID, getLoggedUser()->getUserID().toUTF16(), getLoggedUser()->getPrivateKey()))
	LanguageResult result = revision_to_restore->create(getDatabase(), getSessionAccount());
	if(result.empty())
	{
		showMessage(_S("Revisione ripristinata"));
	}
	else
	{
		showError(getText(result));
	}
}

//////////////////////////////////////////////////////////////////////

} // portal
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
