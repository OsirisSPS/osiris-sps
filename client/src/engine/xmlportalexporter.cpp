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
#include "xmlportalexporter.h"

#include "iideportalpage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

XMLPortalExporter::XMLPortalExporter(shared_ptr<XMLNode> root, shared_ptr<IPortalPage> page, ExportMode mode, bool withStats) : ExporterBase(root),
																																					m_page(page),
																																					m_mode(mode),
																																					m_withStats(withStats)
{
	OS_ASSERT(page != null);
}

XMLPortalExporter::XMLPortalExporter(shared_ptr<XMLNode> root, const XMLPortalExporter &second) : ExporterBase(root, second),
																								  m_page(second.getPage()),
																								  m_mode(second.getMode()),
																								  m_withStats(second.getWithStats())
{

}

XMLPortalExporter::~XMLPortalExporter()
{

}

shared_ptr<Portal> XMLPortalExporter::getPortal() const
{
	shared_ptr<const IPortalPage> page = getPage();
	if(page != null)
		return page->getPortal();

	return null;
}

shared_ptr<IPortalDatabase> XMLPortalExporter::getDatabase() const
{
	shared_ptr<const IPortalPage> page = getPage();
	if(page != null)
		return page->getDatabase();

	return null;
}

shared_ptr<IdeSession> XMLPortalExporter::getSessionAccount()
{
	shared_ptr<IPortalPage> page = getPage();
	if(page != null)
		return page->getSessionAccount();

	return null;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
