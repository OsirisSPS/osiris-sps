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
#include "forumforummanager.h"

#include "timerperformance.h"
#include "entitiesentity.h"
#include "entitiesentitychilds.h"
#include "forumforumconfig.h"
#include "iideportalpage.h"
#include "portalsportal.h"
#include "realtimestats.h"
#include "utils.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

extern OS_NAMESPACE_NAME::String g_componentsPath;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {

//////////////////////////////////////////////////////////////////////

namespace forum {

//////////////////////////////////////////////////////////////////////

ForumManager::ForumManager()
{

}

ForumManager::~ForumManager()
{

}

String ForumManager::getPath() const
{
	return utils::makeFolderPath(g_componentsPath, _S("forum"));
}

void ForumManager::renderForum(shared_ptr<EntitiesEntity> forum, shared_ptr<XMLPortalExporter> exporter)
{
	_render(forum, exporter, false, true, 0);
}

void ForumManager::renderSection(shared_ptr<EntitiesEntity> section, shared_ptr<XMLPortalExporter> exporter)
{
	RealtimeStatsScopeTimer RSS(_S("Debug"), _S("ForumManager::renderSection"));
	_render(section, exporter, true, true, 0);
}

void ForumManager::_render(shared_ptr<EntitiesEntity> entity, shared_ptr<XMLPortalExporter> exporter, bool renderSections, bool renderForums, uint32 level)
{
	if(entity == nullptr)
		return;

	RealtimeStatsScopeTimer RSS1(_S("Debug"), _S("ForumManager::renderSection::exportXML"));
	entity->exportXML(exporter);
	RSS1.stop();

	if(renderSections && level <= OS_COMFORUM_FORUM_EXPORT_LEVEL)
	{
		RealtimeStatsScopeTimer RSS(_S("Debug"), _S("ForumManager::renderSection::renderSections::getChilds"));
		shared_ptr<EntitiesEntities> subsections = entity->getChilds(exporter->getDatabase(), ExtensionsComponentID(OS_COMFORUM_SECTION_ID.to_ascii()));		
		RSS.stop();
		if(subsections != nullptr && subsections->empty() == false)
		{
			// Crea il nodo delle sottosezioni
			shared_ptr<XMLNode> nodeSections = exporter->getRoot()->addChild(OS_COMFORUM_SECTIONS_NODE);
			for(EntitiesEntities::const_iterator i = subsections->begin(); i != subsections->end(); ++i)
			{
				_render(exporter->getPage()->getPortal()->getEntity(exporter->getDatabase(),*i), exporter->createChild<XMLPortalExporter>(nodeSections->addChild(OS_COMFORUM_SECTION_NODE)), true, true, level + 1);
			}	
		}
	}

	if(renderForums && level <= OS_COMFORUM_FORUM_EXPORT_LEVEL)
	{
		RealtimeStatsScopeTimer RSS(_S("Debug"), _S("ForumManager::renderSection::renderForums::getChilds"));
		shared_ptr<EntitiesEntities> subforums = entity->getChilds(exporter->getDatabase(), ExtensionsComponentID(OS_COMFORUM_FORUM_ID.to_ascii()));
		RSS.stop();
		if(subforums != nullptr && subforums->empty() == false)
		{		
			// Crea il nodo dei sottoforums
			shared_ptr<XMLNode> nodeForums = exporter->getRoot()->addChild(OS_COMFORUM_FORUMS_NODE);
			for(EntitiesEntities::const_iterator i = subforums->begin(); i != subforums->end(); ++i)
			{
				// Esporta il nodo corrente (i topics dei sottoforums non vengono esportati)
				_render(exporter->getPage()->getPortal()->getEntity(exporter->getDatabase(),*i), exporter->createChild<XMLPortalExporter>(nodeForums->addChild(OS_COMFORUM_FORUM_NODE)), false, true, level + 1);
			}		
		}
	}
}

//////////////////////////////////////////////////////////////////////

} // forum
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
