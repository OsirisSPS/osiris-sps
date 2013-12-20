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
#include "omlview.h"

#include "omlcontext.h"
#include "omlitem.h"
#include "iideportalpage.h"
#include "ideskin.h"

#include "objectsiobject.h"
#include "options.h"
#include "idesession.h"
#include "portalsportal.h"
#include "portalsportalssystem.h"
#include "css.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLView::OMLView(const String &tag) : IOMLXsl(tag)
{	
}

OMLView::~OMLView()
{

}

String OMLView::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{	
	if(context->checkPortalPageAvailable(i) == false) 
		return String::EMPTY;

	shared_ptr<IPortalPage> page = context->getPortalPage();
	if(page == nullptr) return String::EMPTY;

	mapDefaultParamTo(i, _S("mode"));

	String id = i->getHtmlChilds(context);

	String mode = i->getParam(_S("mode"));
	if(mode.empty())
	{
		mode = _S("row");
		i->setParam(_S("mode"), mode);
	}

	if( (mode != _S("row")) && (mode != _S("lite")) && (mode != _S("full")) )
	{
		context->addWarning(String::format(_S("%S doesn't support the mode '%S'.").c_str(), context->getFriendlyName(i).c_str(), mode.c_str()));
		return String::EMPTY;
	}

	shared_ptr<XMLDocument> doc = initDocument(i, context);
		
	XMLPortalExporter::ExportMode exportMode = (mode != _S("row")) ? XMLPortalExporter::emFull : XMLPortalExporter::emLite;
	bool withStats = (mode != _S("row"));

	shared_ptr<XMLNode> nodeUser = doc->getRoot()->addChild(_S("object"));
	shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(nodeUser, page, exportMode, withStats));
	shared_ptr<ObjectsIObject> object = page->getObject(id.to_ascii());
	if(object != nullptr)
	{
		object->exportXML(exporter);
	}

	return processDocument(doc, i, context);	
}


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
