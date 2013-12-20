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
#include "modulesmirrorviewer.h"

#include "boost/format.hpp"
#include "entitiesentity.h"
#include "htmlliteral.h"
#include "ideviewiobjectviewer.h"
#include "log.h"
#include "modulesmirror.h"
#include "modulesmodulesconfig.h"
#include "objectsdescriptor.h"
#include "objectsicommentable.h"
#include "objectssystem.h"
#include "portalsportal.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace modules {

//////////////////////////////////////////////////////////////////////

MirrorViewer::MirrorViewer()
{

}

MirrorViewer::~MirrorViewer()
{

}

shared_ptr<IHtmlControl> MirrorViewer::renderContent(shared_ptr<EntitiesEntity> entity)
{
	shared_ptr<ObjectsICommentable> commentable = objects_commentable_cast(entity->getCurrent());
	if(commentable == nullptr)
		return nullptr;

	return shared_ptr<HtmlLiteral>(OS_NEW HtmlLiteral(getPage()->parseOml(commentable->getContent(), true, false, false, omlRenderModeOsiris, entity->getEntityID().getString())));
}

shared_ptr<IHtmlControl> MirrorViewer::renderViewer(shared_ptr<EntitiesEntity> entity)
{
	shared_ptr<ObjectsIDescriptor> descriptor = ObjectsSystem::instance()->getDescriptor(entity->getObjectType());
	if(descriptor == nullptr)
		return nullptr;
		
	return descriptor->createViewControl(entity);
}

void MirrorViewer::onLoad()
{
	ViewerBase::onLoad();

	shared_ptr<XMLNode> root = getModuleDocument()->getRoot();
	if(root != nullptr)
	{
		EntityID entityID = root->getAttributeString(OS_MODULES_MIRROR_OBJECT_ID).to_ascii();

		shared_ptr<IHtmlControl> mirrorControl;

		shared_ptr<EntitiesEntity> mirrorEntity = getPortal()->getEntity(getDatabase(), entityID);
		if(mirrorEntity != nullptr)
		{
			if(Mirror::isValidMirrorObject(mirrorEntity))
			{
			

				uint32 renderMode = Mirror::getRenderMode(root);
				switch(renderMode)
				{
				case OS_MODULES_MIRROR_RENDER_MODE_DEFAULT:			mirrorControl = renderViewer(mirrorEntity);
																	break;

				case OS_MODULES_MIRROR_RENDER_MODE_CONTENT:			mirrorControl = renderContent(mirrorEntity);
																	break;

				default:											OS_ASSERTFALSE();
																	OS_LOG_ERROR(boost::format("Unknown module mirror render mode '%d'") % renderMode);
																	break;
				}
			}
		}

		// TODO: se mirrorControl == nullptr andrebbe creato un controllo che visualizza un messaggio di errore
		if(mirrorControl != nullptr)
		{
			mirrorControl->setID(_S("mirror"));
			getControls()->add(mirrorControl);
		}		
	}
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
