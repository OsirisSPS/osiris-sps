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
#include "isearchresults.h"

#include "convert.h"
#include "entitiesentity.h"
#include "iideportalpage.h"
#include "idesystem.h"
#include "objectsdescriptor.h"
#include "objectsirevisionable.h"
#include "objectssystem.h"
#include "platformmanager.h"
#include "idesession.h"
#include "portalsportal.h"
#include "searchquery.h"
#include "searchresult.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ISearchResults::ISearchResults()
{
	m_partial = false;
	m_elapsed = 0;
}

ISearchResults::~ISearchResults()
{

}

void ISearchResults::exportXML(shared_ptr<XMLNode> nodeRoot, shared_ptr<SearchQuery> query, shared_ptr<IPortalPage> page, bool external)
{
	nodeRoot->setAttributeString(_S("templateView"), query->getTemplateView());
	nodeRoot->setAttributeString(_S("templatePager"), query->getTemplatePager());

	nodeRoot->setAttributeBool(_S("showFilterAlphabetic"), query->getShowFilterAlphabetic());

	if(query->getShowInfo())
	{
		nodeRoot->setAttributeBool(_S("showInfo"), true);
		
		nodeRoot->setAttributeString(_S("elapsed"), String::format(_S("%.2f").c_str(), getElapsed() / 1000));		// xsl:format-number() non  attualmente supportata almeno di non integrare l'ICU, pertanto la formattazione viene effettuata da C
		nodeRoot->setAttributeBool(_S("partial"), getPartial());
	}
	else
	{
		nodeRoot->setAttributeBool(_S("showInfo"), false);
	}

	if(query->getRssUrl().empty() == false)
		nodeRoot->setAttributeString(_S("rssUrl"), query->getRssUrl());

	uint32 results = getResults();
	if(query->getTreeMode() == false)
	{
		for(uint32 i = 0; i < results; i++)
		{
			shared_ptr<SearchResult> result = getResult(page->getDatabase(), i);
			if(result != nullptr)
			{
				_exportNode(nodeRoot, query, page, result->getID(), result->getScore(), nullptr, true, external);
			}
		}
	}
	else
	{
		/*
		// Creazione del nodo di portale
		shared_ptr<XMLNode> nodePortal = nodeRoot->addChild(_S("portal"));
		nodePortal->setAttributeString(_S("special"),_S("portal"));
		nodePortal->setAttributeString(_S("edit_href"),page->getPortal()->getEditObjectLink(ObjectID::getRootID()));
		*/

		XMLNodesCache nodeCache;
		for(uint32 i = 0; i < results; i++)
		{
			shared_ptr<SearchResult> result = getResult(page->getDatabase(), i);
			if(result != nullptr)
			{
				_exportNode(nodeRoot, query, page, result->getID(), result->getScore(), &nodeCache, true, external);
			}
		}
	}
}

void ISearchResults::search(shared_ptr<IPortalDatabase> database, shared_ptr<Portal> portal, shared_ptr<SearchQuery> query, const StringMap& objectsMap)
{
	double start = PlatformManager::instance()->tickCount();
	// Esegue la query
	executeQuery(database, portal, query, objectsMap);
	// Calcola il tempo di esecuzione
	m_elapsed = PlatformManager::instance()->tickCount() - start;
}

shared_ptr<XMLNode> ISearchResults::_exportNode(shared_ptr<XMLNode> nodeRoot, shared_ptr<SearchQuery> query, shared_ptr<IPortalPage> page, const EntityID id, const double score, XMLNodesCache *nodeCache, bool isResult, bool external)
{
	shared_ptr<XMLNode> node_result;
	shared_ptr<XMLNode> node_sub_results;

	if(query->getTreeMode())
	{
		// Verifico che non l'abbia già calcolato
		XMLNodesCache::const_iterator i = nodeCache->find(id.toUTF16());
		if(i != nodeCache->end())
		{
			node_result = i->second;
		}
	}

	if(node_result == nullptr)
	{
		shared_ptr<EntitiesEntity> entity = page->getPortal()->getEntity(page->getDatabase(), id);
		if( (entity != NULL) && (entity->getCurrent() != NULL) )
		{
			node_result.reset(OS_NEW XMLNode(OS_PORTAL_OBJECT_OBJECT_TYPENAME, nodeRoot->getDocument()));

			shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(node_result, page, XMLPortalExporter::emLite));
			entity->exportXML(exporter);

			if(external)
				node_result->setAttributeString("view_href",IdeSystem::instance()->getLocalUrl(node_result->getAttributeString("view_href")));

			if(!query->getTreeMode())
			{
				nodeRoot->addChild(node_result);
			}
			else
			{
				node_sub_results = node_result->addChild(OS_PORTAL_OBJECT_OBJECT_GROUPNAME);

				EntityID parentID = entity->getParentID();
				shared_ptr<XMLNode> node_parent;
				if(parentID == EntityID::EMPTY)
				{
					node_parent = nodeRoot;
				}
				else
				{
					XMLNodesCache::const_iterator i = nodeCache->find(parentID.toUTF16());
					if(i == nodeCache->end())
					{
						node_parent = _exportNode(nodeRoot, query, page, parentID, 0, nodeCache, false, external);
					}
					else
					{
						node_parent = i->second->getNode(OS_PORTAL_OBJECT_OBJECT_GROUPNAME);
					}
				}

				if(node_parent != nullptr)
				{
					node_parent->addChild(node_result);

					// Aggiunge alla mappa
					//(*nodeCache)[id.toString()] = node_sub_results;
					(*nodeCache)[id.toUTF16()] = node_result;
				}
			}
		}
	}

	// Segno che l'oggetto  effettivamente uno degli oggetti ricercati.
	// Gli oggetti senza questo attributo sono gli oggetti esportati per rappresentare le gerarchie, ma
	// non sono stati effettivamente estratti dalla ricerca.
	if(node_result)
	{
		node_result->setAttributeBool(_S("isResult"), isResult); // Forse non serve, basta interpretare lo 'score'. isResult = (score!=0);
	}

	// Score / Relevance
	if(score > 0)
	{
		uint32 relevance = static_cast<uint32>(score * 100);
		node_result->setAttributeUint32(_S("relevance"), relevance);
	}

	return node_sub_results;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
