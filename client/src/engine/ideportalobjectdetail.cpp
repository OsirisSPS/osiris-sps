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
#include "ideportalobjectdetail.h"

#include "convert.h"
#include "entitiesentity.h"
#include "entitiessnapshotmanager.h"
#include "objectsdescriptor.h"
#include "objectssystem.h"
#include "objectsreputation.h"
#include "objectsuser.h"
#include "idebutton.h"
#include "idesession.h"
#include "ideskin.h"
#include "idetablequery.h"
#include "ideutils.h"
#include "isearchresults.h"
#include "buffer.h"
#include "portalsaccount.h"
#include "portalsportal.h"
#include "portalsprofile.h"
#include "reputationsmanager.h"
#include "reputationsfeedback.h"
#include "searchquery.h"
#include "htmlevent.h"
#include "css.h"
#include "httprequest.h"
#include "htmlbutton.h"
#include "htmlcheckbox.h"
#include "htmlcombobox.h"
#include "htmldiv.h"
#include "htmlfilebrowser.h"
#include "htmllink.h"
#include "htmlpanel.h"
#include "htmlspan.h"
#include "htmltable.h"
#include "htmltext.h"
#include "htmltextbox.h"
#include "htmlxslcontrol.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace portal {

//////////////////////////////////////////////////////////////////////

const std::wstring ObjectDetail::ID = _W("id");

//////////////////////////////////////////////////////////////////////

ObjectDetail::ObjectDetail(shared_ptr<HttpSession> session) : PageBase(session)
{

}

ObjectDetail::~ObjectDetail()
{

}

String ObjectDetail::getUrlID() const
{
	return getRequest()->getUrlParam(ID);
}

String ObjectDetail::getPageName() const
{
	return _S("portal.pages.objectDetail");
}

void ObjectDetail::onLoad()
{
	PageBase::onLoad();

	// Ottiene l'id dell'utente richiesto
	EntityID id = getUrlID().to_ascii();

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());	
	shared_ptr<XMLNode> nodeRoot = document->create(_S("object"));

	shared_ptr<EntitiesEntity> entity = getPortal()->getEntity(getDatabase(), id);	
	if(entity != null)
	{
		shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(nodeRoot, get_this_ptr<IPortalPage>(), XMLPortalExporter::emFull, true));
		entity->exportXML(exporter);		
	}
	
	String path = getSkin()->getTemplatePath(_S("object_detail.xsl"));
	shared_ptr<HtmlXSLControl> pageTemplate(OS_NEW HtmlXSLControl(loadStylesheet(path), document));	
	
	// Carica il template di riferimento
	getControls()->add(pageTemplate);	
}

void ObjectDetail::onPreRender()
{
	PageBase::onPreRender();

}

//////////////////////////////////////////////////////////////////////

} // portal
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
