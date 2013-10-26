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
#include "ideviewinstanceviewer.h"

#include "datainstancecache.h"
#include "iportaldatabase.h"
#include "entitiesentity.h"
#include "iextensionsmodule.h"
#include "extensionssystem.h"
#include "extensionsmoduleviewer.h"
#include "omlmanager.h"
#include "ideblock.h"
#include "idesession.h"
#include "ideskin.h"
#include "objectsinstance.h"
#include "portalsaccount.h"
#include "portalsportal.h"
#include "lock.h"
#include "htmlattributes.h"
#include "htmlcontrols.h"
#include "htmlliteral.h"
#include "htmlwriter.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace view {

//////////////////////////////////////////////////////////////////////

InstanceViewer::InstanceViewer(shared_ptr<EntitiesEntity> entity) : ControlBase(entity)
{

}

InstanceViewer::~InstanceViewer()
{

}

String InstanceViewer::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("instance_viewer.xsl"));
}

void InstanceViewer::onLoad()
{
	ControlBase::onLoad();

	shared_ptr<EntitiesEntity> entity = getEntity();
	if(entity == null)
		return;

	// URGENT: assolutamente da sistemare !
	// A causa di problemi di lock adesso si usa la lock globale del portale,
	// il problema  che la render del controllo non deve essere sotto lock.
	// Da valutare inoltre se precreare tutti i recordi di cache delle istanze

	OS_LOCK(getPortal()->getPortalLock());

	shared_ptr<ObjectsInstance> instance = objects_instance_cast(entity->getCurrent());
	if(instance == null)
		return;

	shared_ptr<XMLPortalExporter> instanceExporter(OS_NEW XMLPortalExporter(getDocument()->create(OS_PORTAL_OBJECT_INSTANCE_TYPENAME), getPage(), XMLPortalExporter::emFull));
	entity->exportXML(instanceExporter);

	// chiamata alla 'encodeCss' per evitare javascript-injection
	instanceExporter->getRoot()->setAttributeString(_S("style"), OMLManager::instance()->parseCss(instance->style, getPage(), false, true));

	shared_ptr<IHtmlControl> instance_viewer;

	bool createCache = false;
	bool updateCache = false;

	/*
	extensions::module_ptr module = ExtensionsSystem::instance()->getModule(instance->module);
	if(module != null && module->getUseCache())
	{
		data::instance_cache_ptr cache = getDatabase()->getInstanceCache(getLoggedUser()->getAccount()->getID(), instance->getPrimary());
		if(cache == null)
		{
			createCache = true;

			cache.reset(OS_NEW data::InstanceCache());
			cache->profile = getLoggedUser()->getAccount()->getID();
			cache->reference = instance->getPrimary();
		}
		else if(cache->elapsed(objects::Instance::getCacheDuration()))
		{
			updateCache = true;
		}

		if(createCache || updateCache)
		{
			shared_ptr<IHtmlControl> control = createInstanceViewer(instance);
			HtmlWriter writer;
			control->render(getPage(), writer);

			cache->update(writer.getHtml());

			if(createCache)
				getDatabase()->insertRecord(cache);
			else
				getDatabase()->updateRecord(cache);
		}

		instance_viewer.reset(OS_NEW HtmlLiteral(cache->cache));
	}
	else
	{
	*/
		instance_viewer = createInstanceViewer(instance);
	//}

	OS_ASSERT(instance_viewer != null);
	getTemplate()->addChildParam(instance_viewer, _S("viewer"));
}

shared_ptr<IHtmlControl> InstanceViewer::createInstanceViewer(shared_ptr<ObjectsInstance> instance)
{
	shared_ptr<ExtensionsModuleViewer> viewer = ExtensionsSystem::instance()->createModuleViewer(instance->module, true);
	OS_EXCEPT_IF(viewer == null, "Invalid instance module");
	viewer->init(ExtensionsSystem::instance()->getModule(instance->module), instance->title, getEntityID(), instance->data);

	if(viewer->getShowBlock())
	{
		shared_ptr<IdeBlock> block(OS_NEW IdeBlock(instance->title));
		block->setType(instance->getBlockType());
		block->getBody()->getControls()->add(viewer);
		return block;
	}
	else
	{
		return viewer;
	}	
}

//////////////////////////////////////////////////////////////////////

} // view
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
