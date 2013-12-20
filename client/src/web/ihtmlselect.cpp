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
#include "ihtmlselect.h"

#include "eventssource.h"
#include "htmlcontrols.h"
#include "htmlevent.h"
#include "htmloption.h"
#include "htmloptiongroup.h"
#include "htmlwriter.h"
#include "httppostparam.h"
#include "httprequest.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String IHtmlSelect::EVENT_ONCHANGE = _S("onChange");

//////////////////////////////////////////////////////////////////////

IHtmlSelect::IHtmlSelect() : m_autoPostBack(false)
{

}

IHtmlSelect::~IHtmlSelect()
{

}

const String & IHtmlSelect::getValue() const
{
	shared_ptr<HtmlOption> option = getSelected();
	if(option != nullptr)
		return option->getValue();

	return String::EMPTY;
}

void IHtmlSelect::setValue(const String &value)
{
	// Se viene chiamata questa setValue, il Select è a selezione singola.
	// E' necessario eseguirlo su tutti i figli options, in modo da togliere il 'selected' a tutti gli altri e lasciarlo solo all'option con getValue()==value
	for(HtmlControls::iterator i = m_controls->begin(); i != m_controls->end(); ++i)
	{
		for(HtmlControls::iterator i = m_controls->begin(); i != m_controls->end(); ++i)
		{
			shared_ptr<HtmlOption> option = boost::dynamic_pointer_cast<HtmlOption>(*i);
			if(option != nullptr)
			{
				option->setSelected(option->getValue() == value);
			}
			else
			{
				shared_ptr<HtmlOptionGroup> optionGroup = boost::dynamic_pointer_cast<HtmlOptionGroup>(*i);
				if(optionGroup != nullptr)
				{
					optionGroup->setSelectedByValue(value);
				}
			}
		}
	}
}

shared_ptr<HtmlOption> IHtmlSelect::getSelected() const
{
	for(HtmlControls::iterator i = m_controls->begin(); i != m_controls->end(); ++i)
	{
		shared_ptr<HtmlOption> option = boost:: dynamic_pointer_cast<HtmlOption>(*i);
		if(option != nullptr)
		{
			if(option->getSelected())
				return option;
		}
		else
		{
			shared_ptr<HtmlOptionGroup> optionGroup = boost:: dynamic_pointer_cast<HtmlOptionGroup>(*i);
			if(optionGroup != nullptr)
			{
				shared_ptr<HtmlOption> optionSelected = optionGroup->getSelected();
				if(optionSelected != nullptr)
					return optionSelected;
			}
		}
	}

	return nullptr;
}

shared_ptr<EventSource> IHtmlSelect::getEventChange() const
{
	return getEvents()->get(EVENT_ONCHANGE);
}

shared_ptr<HtmlOption> IHtmlSelect::addOption(const String &label, const String &value)
{
	shared_ptr<HtmlOption> option(OS_NEW HtmlOption(label, value));
	getControls()->add(option);
	return option;
}

shared_ptr<HtmlOptionGroup> IHtmlSelect::addOptionGroup(const String &label)
{
	shared_ptr<HtmlOptionGroup> option_group(OS_NEW HtmlOptionGroup(label));
	getControls()->add(option_group);
	return option_group;
}

void IHtmlSelect::renderAttributes(HtmlWriter &writer)
{
	ControlBase::renderAttributes(writer);

	if(hasID())
	{
		writer.writeAttribute(_S("id"), getTarget());
		writer.writeAttribute(_S("name"), getTarget());
	}

	if(getEventChange()->empty() == false)
		writer.writeAttribute(_S("onchange"), getEventCommand(EVENT_ONCHANGE));
	else if(getAutoPostBack())
		writer.writeAttribute(_S("onchange"), _S("javascript:return osAutoPostBack(this);"));	
}

void IHtmlSelect::onInit()
{
	ControlBase::onInit();

	// 01/12/2009 0.13-RC2
	/*
	if(getPostBack() && hasID())
		// Inizializza il valore del controllo
		setValue(getRequest()->getPostParamString(getTarget().to_ascii()));
	*/
	if(getPostBack() && hasID())
	{
		// Ottiene il parametro associato al controllo
		shared_ptr<HttpPostParam> param = getRequest()->getPostParam(getTarget().to_wide());
		// Verifica che sia stato postato un parametro relativo al controllo
		if(param != nullptr)
			// Inizializza il valore del controllo
			setValue(param->getString());
	}
}

void IHtmlSelect::onRender(HtmlWriter &writer)
{
	writer.openTag(_S("select"));
	renderAttributes(writer);
	writer.closeTag();
	writer.writeLine();
	renderChilds(writer);
	writer.closeTag(_S("select"));
	writer.writeLine();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
