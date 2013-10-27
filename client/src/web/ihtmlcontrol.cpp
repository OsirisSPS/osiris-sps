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
#include "ihtmlcontrol.h"

#include "datatree.h"
#include "htmlattribute.h"
#include "htmlattributes.h"
#include "htmlcontrols.h"
#include "htmlevent.h"
#include "htmlpage.h"
#include "htmlunit.h"
#include "htmlwriter.h"
#include "httprequest.h"
#include "httpsession.h"
#include "realtimestats.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String IHtmlControl::EVENT_ONINIT = _S("onInit");
const String IHtmlControl::EVENT_ONLOAD = _S("onLoad");
const String IHtmlControl::EVENT_ONPRERENDER = _S("onPreRender");

const String IHtmlControl::TARGET_DELIMITER = _S("-");

//////////////////////////////////////////////////////////////////////

IHtmlControl::IHtmlControl() : m_attributes(OS_NEW HtmlAttributes(*this)),
							   m_controls(OS_NEW HtmlControls(*this)),
							   m_events(OS_NEW HtmlEvents()),
							   m_status(csNone),
							   m_visible(true)
{
	
}

IHtmlControl::~IHtmlControl()
{

}

shared_ptr<EventSource> IHtmlControl::getEventInit() const 
{ 
	return getEvents()->get(EVENT_ONINIT); 
}

shared_ptr<EventSource> IHtmlControl::getEventLoad() const 
{ 
	return getEvents()->get(EVENT_ONLOAD); 
}

shared_ptr<EventSource> IHtmlControl::getEventPreRender() const
{
	return getEvents()->get(EVENT_ONPRERENDER); 
}

bool IHtmlControl::hasChilds() const 
{ 
	return m_controls->empty() == false; 
}

const DataTree & IHtmlControl::getViewState() const
{
	OS_ASSERT(getTarget().empty() == false);

	shared_ptr<HtmlPage> page = getPage();
	if(page != null && hasID())
	{	
		DataTree *view_state = page->getViewStates().getTP(getTarget());
		if(view_state != null)
            return *view_state;
	}

	return DataTree::EMPTY;
}

bool IHtmlControl::getPostBack() const
{
	return getRequest()->getMethod() == httpMethodPost;
}

shared_ptr<HttpSession> IHtmlControl::getSession() const
{
	return getPage()->getSession();
}

shared_ptr<HttpRequest> IHtmlControl::getRequest() const
{
	return getSession()->getRequest();
}

shared_ptr<HttpResponse> IHtmlControl::getResponse() const
{
	return getSession()->getResponse();
}

String IHtmlControl::getTarget() const
{
	if(m_uid.empty() == false)
		return m_uid;

	OS_ASSERT(m_id.empty() == false); // Il diretto (e solo il diretto) deve essere identificabile.

	String target;
	shared_ptr<const IHtmlControl> current = get_this_ptr();
	while(current != null)
	{
		const String &id = current->getID();		
		if(id.empty() == false)
		{
			OS_ASSERT(id.find(TARGET_DELIMITER) == String::npos); // l'ID non deve contenere il delimitatore.
			
			if(target.empty()) 
				target = id;
			else 
				target = id + TARGET_DELIMITER + target;
		}

		current = current->getParent();
	}

	return target;
}

String IHtmlControl::getEventCommand(const String &eventName, const HtmlEvent *e) const
{
	OS_EXCEPT_IF(m_id.empty(), "Invalid control id");

	String encodeEventStr = encodeEvent(eventName, e);
	return String::format(_S("javascript:void(osEventPostCtl('%S', '%S'));").c_str(), getTarget().c_str(), encodeEventStr.c_str());	
}

String IHtmlControl::getEventCommand(const String &eventName, const String &e) const
{
	HtmlEvent eventValue(e);
	return getEventCommand(eventName, &eventValue);
}

HtmlUnit IHtmlControl::getWidth() const
{
	return HtmlUnit::fromString(getAttributes()->value_of(_S("width")));
}

void IHtmlControl::setWidth(const HtmlUnit &width)
{
	getAttributes()->set(_S("width"), width);
}

HtmlUnit IHtmlControl::getHeight() const
{
	return HtmlUnit::fromString(getAttributes()->value_of(_S("height")));
}

void IHtmlControl::setHeight(const HtmlUnit &height)
{
	getAttributes()->set(_S("height"), height);
}

bool IHtmlControl::getVisible() const
{
	if(m_visible == false)
		return false;

	shared_ptr<IHtmlControl> parent = getParent();
	if(parent != null)
		return parent->getVisible();

	return true;
}

void IHtmlControl::setVisible(bool visible)
{
	m_visible = visible;
}

String IHtmlControl::encodeEvent(const String &eventName, const HtmlEvent *e) const
{
	OS_EXCEPT_IF(eventName.find(_S(":")) != String::npos, "Invalid event name");
	
	if((e == null) || e->empty())
		return eventName;
    
	return eventName + _S(":") + e->encode();
}

bool IHtmlControl::decodeEvent(const String &command, String &eventName, HtmlEvent &e) const
{
	size_t pos = command.find(_S(":"));
	if(pos == String::npos)
	{
		eventName = command;
	}
	else
	{
		eventName = command.left(pos);
		e.decode(command.mid(pos + 1));
	}	

	return eventName.empty() == false;
}

shared_ptr<IHtmlControl> IHtmlControl::findTarget(const String &target)
{
	if(target.empty()) 
		return null;

	if(m_id == target) 
		return get_this_ptr();

	String targetNew = target;
	
	String::size_type pos = target.find(TARGET_DELIMITER);
	if(pos != String::npos)
	{
		String targetIn = target.mid(0, pos);
		if(m_id.empty() == false)
		{
			if(targetIn != m_id) 
				return null;
		   
			targetNew = target.mid(pos + 1);
		}
	}

	for(HtmlControls::const_iterator i = m_controls->begin(); i != m_controls->end(); ++i)
	{
		shared_ptr<IHtmlControl> child = (*i)->findTarget(targetNew);
		if(child != null) 
			return child;
	}

	return null;
}

void IHtmlControl::render(shared_ptr<HtmlPage> page, HtmlWriter &writer)
{
	if(ensureStatus(page, csPreRender))
	{
		if(getVisible())
			onRender(writer);
	}
}

void IHtmlControl::renderAttributes(HtmlWriter &writer)
{
	if(hasID())
		writer.writeAttribute(_S("id"), getTarget());

	if(m_css.empty() == false)
		writer.writeAttribute(_S("class"), m_css);

	if(m_style.empty() == false)
		writer.writeAttribute(_S("style"), m_style);

	for(HtmlAttributes::const_iterator i = m_attributes->begin(); i != m_attributes->end(); ++i)
	{
		writer.writeAttribute((*i)->getName(), (*i)->getValue());
	}
}

void IHtmlControl::renderChilds(HtmlWriter &writer)
{
	writer.indent();

	OS_ASSERT(getPage() != null);
	for(HtmlControls::iterator i = m_controls->begin(); i != m_controls->end(); (*i)->render(getPage(), writer), ++i);

	writer.unindent();
}

void IHtmlControl::raiseEvent(const String &command)
{
	String eventName;
	HtmlEvent e;
	// Decodifica il nome dell'evento e i parametri
	if(decodeEvent(command, eventName, e))
		// Richiama l'evento
		onEvent(eventName, &e);
}

void IHtmlControl::saveViewState(DataTree &states)
{
	if(hasID())
	{
		DataTree state;
		onSaveViewState(state);

		if(state.empty() == false)
		{
			OS_ASSERT(getTarget().empty() == false);
			OS_ASSERT(states.exists(getTarget()) == false);

			states.setT(getTarget(), state);
		}		
	}

	for(HtmlControls::const_iterator i = m_controls->begin(); i != m_controls->end(); ++i)
        (*i)->saveViewState(states);
}

bool IHtmlControl::ensureStatus(shared_ptr<HtmlPage> page, const Status &status)
{	
	RealtimeStatsScopeTimer RSS1(_S("Debug"), String::format(_S("IHtmlControl::ensureStatus %d").c_str(),status));

	OS_ASSERT(page->getSession()->getProcessed() == false);
	if(page->getSession()->getProcessed())
			return false;

	if(getPage() == null) 
		m_page = page;

	if(status == csNone)
	{
		OS_ASSERTFALSE();
		return false;
	}

	Status prevStatus = static_cast<Status>(status - 1);
	if(prevStatus != csNone)
	{
		if(ensureStatus(page, prevStatus) == false)
			return false;
	}

	if(m_status < status)
	{
		switch(status)
		{
		case csInit:		{
								onInit();

								m_status = csInit;
								HtmlEvent e;
								onEvent(EVENT_ONINIT, &e);
							}
					
							break;

		case csLoad:		{
								onLoad();

								m_status = csLoad;
								HtmlEvent e;
								onEvent(EVENT_ONLOAD, &e);
							}
					
							break;

		case csPreRender:	{
								onPreRender();

								m_status = csPreRender;
								HtmlEvent e;
								onEvent(EVENT_ONPRERENDER, &e);
							}
							
							break;

		case csRender:		
							break;

		default:			OS_ASSERTFALSE();
							break;

		}		

		// 5/11/09: se la sessione viene processata in seguito ad un evento, interrompe il ciclo di rendering
		if(page->getSession()->getProcessed())
			return false;

		for(HtmlControls::iterator i = m_controls->begin(); i != m_controls->end(); ++i)
		{
			if((*i)->ensureStatus(page, status) == false)
				return false;
		}
	}	

	return true;
}

void IHtmlControl::onInit()
{
	if(hasID())
	{
		OS_ASSERT(getTarget().empty() == false);

		DataTree *state = getPage()->getViewStates().getTP(getTarget());
		if(state != null)
            onLoadViewState(*state);
	}	
}

void IHtmlControl::onLoad()
{
	
}

void IHtmlControl::onPreRender()
{

}

void IHtmlControl::onRender(HtmlWriter &writer)
{
	OS_ASSERT(getSession()->getProcessed() == false);

	renderChilds(writer);
}

void IHtmlControl::onLoadViewState(const DataTree &state)
{
	
}

void IHtmlControl::onSaveViewState(DataTree &state)
{

}

void IHtmlControl::onEvent(const String &name, IEvent *e)
{	
	getEvents()->fire(name, e);	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
