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
#include "forumsectionviewer.h"

#include "timerperformance.h"
#include "entitiesentity.h"
#include "forumforumconfig.h"
#include "forumforummanager.h"
#include "realtimestats.h"
#include "xmlportalexporter.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace forum {

//////////////////////////////////////////////////////////////////////

SectionViewer::SectionViewer()
{

}

SectionViewer::~SectionViewer()
{

}

String SectionViewer::getTemplatePath()
{
	return utils::makeFilePath(ForumManager::instance()->getPath(), _S("section_viewer.xsl"));
}

void SectionViewer::onLoad()
{
	ViewerBase::onLoad();

	shared_ptr<EntitiesEntity> section = getEntity(getDatabase());
	if(section != nullptr)
	{
		shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(getDocument()->create(OS_COMFORUM_SECTION_NODE), getPage(), XMLPortalExporter::emFull));
		ForumManager::instance()->renderSection(section, exporter);
	}
}

//////////////////////////////////////////////////////////////////////

} // forum
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
