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
#include "ideviewpostviewer.h"

#include "convert.h"
#include "entitiesentity.h"
#include "httprequest.h"
#include "httpsession.h"
#include "idesession.h"
#include "ideskin.h"
#include "objectsirevisionable.h"
#include "objectspost.h"
#include "portalsportal.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace view {

//////////////////////////////////////////////////////////////////////

PostViewer::PostViewer(shared_ptr<EntitiesEntity> entity) : ControlBase(entity)
{

}

PostViewer::~PostViewer()
{

}

void PostViewer::addPost(shared_ptr<EntitiesEntity> entity)
{
	if(entity != null)
		m_posts.push_back(entity);
}

String PostViewer::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("post_viewer.xsl"));
}

void PostViewer::onLoad()
{
	ControlBase::onLoad();

	shared_ptr<EntitiesEntity> post = getEntity();
	if(post != null)
	{
		ordered_map<std::wstring, std::wstring> params;
		params.set(OS_URL_PARAM_TARGET, post->getEntityID().toWide());
		String threadLink = getPortal()->getViewObjectLink(post->getParentID(), params);

		// 0.14
		// If "mode" is "thread", redirect.
		String mode = getRequest()->getUrlParam(OS_URL_PARAM_MODE);
		if(mode == _S("thread"))
		{
			getSession()->redirect(threadLink.to_ascii());
		}
		else
		{
			shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(getDocument()->create(OS_PORTAL_OBJECT_POST_TYPENAME), getPage(), XMLPortalExporter::emFull));
			post->exportXML(exporter);
		
			exporter->createAction(_S("back"), threadLink);
		}
	}
	else
	{
		if(m_posts.empty() == false)
		{
			shared_ptr<XMLNode> nodePosts = getDocument()->create(OS_PORTAL_OBJECT_POST_GROUPNAME);
			for(EntitiesEntity::EntitiesList::const_iterator i = m_posts.begin(); i != m_posts.end(); ++i)
			{
				shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(nodePosts->addChild(OS_PORTAL_OBJECT_POST_TYPENAME), getPage(), XMLPortalExporter::emFull));
				(*i)->exportXML(exporter);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

} // view
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
