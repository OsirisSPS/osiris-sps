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
#include "modulesmirror.h"

#include "entitiesentity.h"
#include "modulesmirroreditor.h"
#include "modulesmirrorviewer.h"
#include "modulesmodulesconfig.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace modules {

//////////////////////////////////////////////////////////////////////

Mirror::Mirror()
{

}

Mirror::~Mirror()
{

}

bool Mirror::isValidMirrorObject(shared_ptr<EntitiesEntity> mirrorEntity)
{
	if(mirrorEntity == nullptr)
		return false;

	if(mirrorEntity->getObjectType() == portalObjectTypeInstance)
		return false;

	return true;
}

uint32 Mirror::getRenderMode(shared_ptr<XMLNode> root)
{
	OS_ASSERT(root != nullptr);
	if(root != nullptr)
	{
		uint32 value = root->getAttributeUint32(OS_MODULES_MIRROR_RENDER_MODE);
		switch(value)
		{
		case OS_MODULES_MIRROR_RENDER_MODE_DEFAULT:
		case OS_MODULES_MIRROR_RENDER_MODE_CONTENT:
														return value;																					

		default:										OS_ASSERTFALSE();
														break;
		}
	}
	
	return OS_MODULES_MIRROR_RENDER_MODE_DEFAULT;
}

void Mirror::setRenderMode(shared_ptr<XMLNode> root, uint32 renderMode)
{
	if(root == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	switch(renderMode)
	{
	case OS_MODULES_MIRROR_RENDER_MODE_DEFAULT:
	case OS_MODULES_MIRROR_RENDER_MODE_CONTENT:
													break;

	default:										OS_ASSERTFALSE();
													renderMode = OS_MODULES_MIRROR_RENDER_MODE_DEFAULT;
													break;
	}
	
	root->setAttributeUint32(OS_MODULES_MIRROR_RENDER_MODE, renderMode);
}

ExtensionsModuleID Mirror::getID()
{
	return ExtensionsModuleID(OS_MODULES_MIRROR_ID.to_ascii());
}

String Mirror::getName()
{
	return _S("Mirror module");
}

shared_ptr<ExtensionsModuleEditor> Mirror::createEditor()
{
	return shared_ptr<ExtensionsModuleEditor>(OS_NEW MirrorEditor());
}

shared_ptr<ExtensionsModuleViewer> Mirror::createViewer()
{
	return shared_ptr<ExtensionsModuleViewer>(OS_NEW MirrorViewer());
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
