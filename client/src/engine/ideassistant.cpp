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
#include "ideassistant.h"

#include "boost/bind.hpp"
#include "css.h"
#include "dataaccount.h"
#include "eventssource.h"
#include "objectsicommentable.h"
#include "objectsobjectscache.h"
#include "objectsuser.h"
#include "iportaldatabase.h"
#include "entitiesentity.h"
#include "entitiesentitiescache.h"
#include "entitiessnapshotmanager.h"
#include "idesystem.h"
#include "idesessionportal.h"
#include "ideskin.h"
#include "ideutils.h"
#include "objectssystem.h"
#include "htmlcontrols.h"
#include "htmlevent.h"
#include "htmlliteral.h"
#include "htmlscript.h"
#include "htmltext.h"
#include "htmltextbox.h"
#include "htmltextarea.h"
#include "httpparser.h"
#include "httprequest.h"
#include "httpurl.h"
#include "idepages.h"
#include "idesession.h"
#include "ideutils.h"
#include "osirislink.h"
#include "portalsaccount.h"
#include "portalsportal.h"
#include "portalsportalssystem.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String IdeAssistant::EVENT_ONVIEWMODE = _S("onViewMode");
const String IdeAssistant::EVENT_ONOBJECTADD = _S("onObjectAdd");
const String IdeAssistant::EVENT_ONOBJECTREMOVE = _S("onObjectRemove");
const String IdeAssistant::EVENT_ONOBJECTSCLEAR = _S("onObjectsClear");
const String IdeAssistant::EVENT_ONOBJECTVIEW = _S("onObjectView");
const String IdeAssistant::EVENT_ONOBJECTEDIT = _S("onObjectEdit");

//////////////////////////////////////////////////////////////////////

IdeAssistant::IdeAssistant() : m_notes(OS_NEW HtmlTextArea())
{
	setID(_S("myassistant"));
	getEvents()->get(EVENT_ONVIEWMODE)->connect(boost::bind(&IdeAssistant::onViewMode, this, _1));
	getEvents()->get(EVENT_ONOBJECTADD)->connect(boost::bind(&IdeAssistant::onObjectAdd, this, _1));
	getEvents()->get(EVENT_ONOBJECTREMOVE)->connect(boost::bind(&IdeAssistant::onObjectRemove, this, _1));
	getEvents()->get(EVENT_ONOBJECTSCLEAR)->connect(boost::bind(&IdeAssistant::onObjectsClear, this, _1));
	getEvents()->get(EVENT_ONOBJECTVIEW)->connect(boost::bind(&IdeAssistant::onObjectView, this, _1));
	getEvents()->get(EVENT_ONOBJECTEDIT)->connect(boost::bind(&IdeAssistant::onObjectEdit, this, _1));

	m_notes->setID(_S("notes"));
	m_notes->setCss(_S("os_assistant_notes"));
}

IdeAssistant::~IdeAssistant()
{

}

void IdeAssistant::_exportTarget()
{
	if( (getPage()->getPageName() == _S("portal.pages.view")) || (getPage()->getPageName() == _S("portal.pages.edit")) )
	{
		EntityID id = getPage()->getTargetObject();
		shared_ptr<EntitiesEntity> targetObject = getPortal()->getEntity(getDatabase(), getPage()->getTargetObject());
		if( (targetObject != nullptr) && (targetObject->getCurrent() != nullptr) )
		{
			shared_ptr<XMLPortalExporter> exporterObject = m_exporter->createChild<XMLPortalExporter>(OS_PORTAL_OBJECT_OBJECT_TYPENAME);
			targetObject->exportXML(exporterObject);
		}			
	}
	else
	{
		// Other pages
		shared_ptr<XMLPortalExporter> exporterPage = m_exporter->createChild<XMLPortalExporter>(_S("page"));
		
		//String portal = getPage()->getPortal()->getPortalID().getString();
		String title = getPage()->getPageTitle();
		String description = getPage()->getPageDescription();

		HttpUrl url = getRequest()->getRawUrl();
		String urlPath = url.getPath();

		/* <0.15
		urlPath.trim_left(_S("/"));
		String urlParams = url.getParams();
		
		String omlCodeView2 = String::format(_S("[iurl=\"osiris://|url|%S|%S\"]%S[/iurl]").c_str(), urlPath.c_str(), urlParams.c_str(), title.c_str());
		exporterPage->setAttributeString(_S("oml_code2"), omlCodeView2);
		*/


		OsirisLink link;
		link.setParam("type", _W("url"));
		
		bool params = 0;
		std::string urlFull = url.getPath();
		const ordered_map<std::wstring, std::wstring> & params2 = getRequest()->getUrlParams();
		for(ordered_map<std::wstring, std::wstring>::const_iterator i = params2.begin(); i != params2.end(); ++i)
		{
			if(i->first == _W("portal"))
				link.setParam("portal",i->second);
			else if(i->first == _W("pov"))
				link.setParam("pov",i->second);
			else
			{
				if(params == 0)
					urlFull += "?";
				else
					urlFull += "&";

				urlFull += HttpParser::urlEncodeW(i->first) + "=" + HttpParser::urlEncodeW(i->second);
			}
		}
				
		link.setParam("url", urlFull);
		String omlCodeView = _S("[iurl=\"") + link.generate() + _S("\"]") + title + _S("[/iurl]");

		exporterPage->setAttributeString(_S("title"), title);
		exporterPage->setAttributeString(_S("description"), description);
		exporterPage->setAttributeString(_S("oml_code"), omlCodeView);
	}
}

void IdeAssistant::_exportSelectedObjects()
{
	StringMap &selectedObjects = getSessionAccount()->getSessionPortal(getPortal())->getSelectedObjects();
	for(StringMap::const_iterator i = selectedObjects.begin(); i != selectedObjects.end(); ++i)
	{
		shared_ptr<EntitiesEntity> objectEntity = getPortal()->getEntity(getDatabase(), i->first.to_ascii());
		if( (objectEntity != nullptr) && (objectEntity->getCurrent() != nullptr) )
		{
			shared_ptr<XMLPortalExporter> exporter = m_exporter->createChild<XMLPortalExporter>(m_exporter->getNode(OS_PORTAL_OBJECT_OBJECT_GROUPNAME)->addChild(OS_PORTAL_OBJECT_OBJECT_TYPENAME));
			objectEntity->exportXML(exporter);
		}
	}
}

void IdeAssistant::onLoad()
{
	ControlBase::onLoad();

	getPage()->addCss(getSessionAccount()->getCurrentSkin()->getResourceUrl(_S("css/assistant.less")));
	getPage()->addJavascript(getSessionAccount()->getCurrentSkin()->getResourceUrl(_S("js/assistant.js")));

	getControls()->add(shared_ptr<HtmlScript>(OS_NEW HtmlScript(_S("Osiris.Assistant.init('") + getPortal()->getPortalID().toUTF16() + _S("');"))));

	shared_ptr<XMLDocument> document = getDocument();
	shared_ptr<XMLNode> root = document->create(_S("assistant"));

	ordered_map<std::wstring, std::wstring> paramsDialog;
	paramsDialog.set(_W("mode"), _W("dialog"));

	//root->setAttributeString(_S("home_href"), getPortal()->getLink(_S("view")));	
	//root->setAttributeString(_S("edit_href"), getPortal()->getLink(_S("edit")));
	root->setAttributeString(_S("more_href"), getPortal()->getLink("more",paramsDialog));
	root->setAttributeString(_S("search_href"), getPortal()->getLink("search"));

	m_exporter.reset(OS_NEW XMLPortalExporter(root, getPage(), XMLPortalExporter::emLite));
	root->setAttributeString(_S("viewmode_href"), getEventCommand(EVENT_ONVIEWMODE));
	root->setAttributeString(_S("objectsClear_href"), getEventCommand(EVENT_ONOBJECTSCLEAR));
	root->setAttributeBool(_S("guest"), (getSessionAccount()->isPortalGuest(getDatabase())));	
}

void IdeAssistant::onPreRender()
{
	ControlBase::onPreRender();

	_exportTarget();
	_exportSelectedObjects();
}

String IdeAssistant::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("assistant.xsl"));
}

void IdeAssistant::onViewMode(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == nullptr)
		return;

	// TODO
}

void IdeAssistant::onObjectAdd(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == nullptr)
		return;

	StringMap &selectedObjects = getSessionAccount()->getSessionPortal(getPortal())->getSelectedObjects();
	selectedObjects[htmlEvent->get(0)] = true;
}

void IdeAssistant::onObjectRemove(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == nullptr)
		return;

	StringMap &selectedObjects = getSessionAccount()->getSessionPortal(getPortal())->getSelectedObjects();
	selectedObjects.erase(htmlEvent->get(0));
}

void IdeAssistant::onObjectsClear(IEvent *e)
{
	StringMap &selectedObjects = getSessionAccount()->getSessionPortal(getPortal())->getSelectedObjects();
	selectedObjects.clear();
}

void IdeAssistant::onObjectView(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == nullptr)
		return;

	getPage()->redirect(getPortal()->getViewObjectLink(htmlEvent->get(0).to_ascii()));
}

void IdeAssistant::onObjectEdit(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == nullptr)
		return;

	getPage()->redirect(getPortal()->getEditObjectLink(htmlEvent->get(0).to_ascii()));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
