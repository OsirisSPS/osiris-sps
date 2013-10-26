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
#include "htmloptiongroup.h"

#include "htmlwriter.h"
#include "htmlcontrols.h"
#include "htmloption.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlOptionGroup::HtmlOptionGroup(const String &label) : m_label(label)
{

}

HtmlOptionGroup::~HtmlOptionGroup()
{

}

void HtmlOptionGroup::setSelectedByValue(const String &value)
{
	// Se viene chiamata questa setValue, il Select è a selezione singola.
	// E' necessario eseguirlo su tutti i figli options, in modo da togliere il 'selected' a tutti gli altri e lasciarlo solo all'option con getValue()==value
	for(HtmlControls::iterator i = m_controls->begin(); i != m_controls->end(); ++i)
	{
		shared_ptr<HtmlOption> option = boost:: dynamic_pointer_cast<HtmlOption>(*i);
		if(option != null)
		{
			option->setSelected(option->getValue() == value);
		}
		else
		{
			shared_ptr<HtmlOptionGroup> optionGroup = boost:: dynamic_pointer_cast<HtmlOptionGroup>(*i);
			if(optionGroup != null)
			{
				optionGroup->setSelectedByValue(value);
			}
		}
	}
}

shared_ptr<HtmlOption> HtmlOptionGroup::getSelected() const
{
	for(HtmlControls::iterator i = m_controls->begin(); i != m_controls->end(); ++i)
	{
		shared_ptr<HtmlOption> option = boost:: dynamic_pointer_cast<HtmlOption>(*i);
		if(option != null)
		{
			if(option->getSelected())
				return option;
		}
		else
		{
			shared_ptr<HtmlOptionGroup> optionGroup = boost:: dynamic_pointer_cast<HtmlOptionGroup>(*i);
			if(optionGroup != null)
			{
				shared_ptr<HtmlOption> optionSelected = optionGroup->getSelected();
				if(optionSelected != null)
					return optionSelected;
			}
		}
	}

	return null;
}

shared_ptr<HtmlOption> HtmlOptionGroup::addOption(const String &label, const String &value)
{
	shared_ptr<HtmlOption> option(OS_NEW HtmlOption(label, value));
	getControls()->add(option);
	return option;
}

void HtmlOptionGroup::renderAttributes(HtmlWriter &writer)
{
	ControlBase::renderAttributes(writer);

	if(m_label.empty() == false)
		writer.writeAttribute(_S("label"), m_label);
}

void HtmlOptionGroup::onRender(HtmlWriter &writer)
{
	writer.openTag(_S("optgroup"));
	renderAttributes(writer);
	writer.closeTag();
	writer.writeLine();
	renderChilds(writer);
	writer.closeTag(_S("optgroup"));
	writer.writeLine();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
