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
#include "conversions.h"
#include "documentationdocumentationviewer.h"
#include "entitiesentity.h"
#include "entitiesentitychilds.h"
#include "portalsportal.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

extern OS_NAMESPACE_NAME::String g_componentsPath;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {

//////////////////////////////////////////////////////////////////////

namespace documentation {

//////////////////////////////////////////////////////////////////////

DocumentationViewer::DocumentationViewer(shared_ptr<EntitiesEntity> entity) : m_entity(entity)
{

}

DocumentationViewer::~DocumentationViewer()
{

}

String DocumentationViewer::getTemplatePath()
{
	return utils::makeFilePath(utils::makeFolderPath(g_componentsPath, _S("documentation")), _S("documentation_viewer.xsl"));
}

String DocumentationViewer::getPageHref(uint32 offset) const
{
	String href;

	shared_ptr<EntitiesEntity> entity = m_entity;
	if(entity != nullptr)
	{
		ordered_map<std::wstring, std::wstring> params;
		if(offset != 0)
			params.set(OS_URL_PARAM_OFFSET, conversions::to_wstring(offset));

		href = entity->getViewLink(getPortal(), params);
	}

	return href;
}

void DocumentationViewer::onLoad()
{
	ViewerBase::onLoad();

	shared_ptr<XMLNode> root = getComponentDocument()->getRoot();
	OS_ASSERT(root != nullptr);

	shared_ptr<EntitiesEntity> entity = m_entity;
	if(entity == nullptr)
		entity = getEntity(getDatabase());

	/* Defaults
	String tocLayout = _S("right");
	int32 tocMinTexts = 3;
	int32 contentLevel = 1;
	int32 subindexLevel = -1;
	bool subindexShowTexts = true;
	*/
	String tocLayout = root->getAttributeString("toc_layout");
	int32 tocMinTexts = root->getAttributeInt32("toc_mintexts");
	int32 contentLevel = root->getAttributeInt32("content_level");
	int32 subindexLevel = root->getAttributeInt32("subindex_level");
	bool subindexShowTexts = root->getAttributeBool("subindex_showtexts");

	shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(getDocument()->create(_S("documentation")), getPage(), XMLPortalExporter::emFull));

	exporter->setAttributeString("toc_layout", tocLayout);
	exporter->setAttributeInt32("toc_mintexts", tocMinTexts);
	exporter->setAttributeInt32("content_level", contentLevel);
	exporter->setAttributeInt32("subindex_level", subindexLevel);
	exporter->setAttributeBool("subindex_showtexts", subindexShowTexts);

	shared_ptr<XMLPortalExporter> contentsExporter = exporter->createChild<XMLPortalExporter>(_S("contents"));

	if(entity != nullptr)
		_render(entity, contentsExporter, 0, contentLevel, true, true);

	shared_ptr<XMLPortalExporter> subindexExporter = exporter->createChild<XMLPortalExporter>(_S("subindex"));

	if(entity != nullptr)
		_render(entity, subindexExporter, 0, subindexLevel, subindexShowTexts, false);
}

void DocumentationViewer::_render(shared_ptr<EntitiesEntity> entity, shared_ptr<XMLPortalExporter> exporter, uint32 level, uint32 levelMax, bool withTexts, bool forContent)
{
	if(entity == nullptr)
		return;

	if(levelMax == 0) // Disabled.
		return;

	if(forContent)
		exporter->setMode(XMLPortalExporter::emFull);
	else
		exporter->setMode(XMLPortalExporter::emLite);
	
	entity->exportXML(exporter);

	bool exportTexts = withTexts;
	// I testi già comparsi sopra non compaiono nel sottoindice.
	if( (exportTexts) && (forContent == false) )
		exportTexts = (level > 0);

	// Esporta gli oggetti testo
	if(exportTexts)
	{
		shared_ptr<EntitiesEntities> texts = entity->getChilds(getDatabase(), portalObjectTypeText, RangeUint32(0, 0), EntitiesEntity::coPositionAsc);
		if(texts != nullptr && texts->empty() == false)
		{
			shared_ptr<XMLNode> nodeTexts = exporter->getRoot()->addChild(_S("texts"));
			for(EntitiesEntities::const_iterator i = texts->begin(); i != texts->end(); ++i)
			{
				exporter->getPage()->getPortal()->getEntity(exporter->getDatabase(), *i)->exportXML(exporter->createChild<XMLPortalExporter>(nodeTexts->addChild(_S("text"))));
			}
		}
	}

	bool exportSections = true;
	if(levelMax != static_cast<uint32>(-1))
	{
		if(forContent)
			exportSections = (level<(levelMax-1));
		else
			exportSections = (level<=(levelMax-1));
	}

	// Crea un nodo delle sotto-sezioni
	if(exportSections)
	{
		shared_ptr<EntitiesEntities> subSections = entity->getChilds(getDatabase(), portalObjectTypeSection, RangeUint32(0, 0), EntitiesEntity::coPositionAsc);
		if(subSections != nullptr && subSections->empty() == false)
		{
			shared_ptr<XMLNode> nodeSubSections = exporter->getRoot()->addChild(_S("sections"));
			for(EntitiesEntities::const_iterator i = subSections->begin(); i != subSections->end(); ++i)
			{
				_render(exporter->getPage()->getPortal()->getEntity(getDatabase(), *i), exporter->createChild<XMLPortalExporter>(nodeSubSections->addChild(_S("section"))), level +1, levelMax, withTexts, forContent);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

} // documentation
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
