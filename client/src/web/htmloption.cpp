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
#include "htmloption.h"

#include "htmlparser.h"
#include "htmlwriter.h"
#include "htmlcontrols.h"
#include "htmlliteral.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlOption::HtmlOption(const String &label, const String &value) : m_label(OS_NEW HtmlLiteral(label)),
																   m_value(value),
																   m_selected(false)
{

}

HtmlOption::~HtmlOption()
{

}

const String & HtmlOption::getLabel() const
{
	return m_label->getHtml();
}

void HtmlOption::setLabel(const String &label)
{
	m_label->setHtml(label);
}

void HtmlOption::renderAttributes(HtmlWriter &writer)
{
	ControlBase::renderAttributes(writer);

	// N.B.: m_value == String::EMPTY è un valore valido quindi va renderizzato
	writer.writeAttribute(_S("value"), m_value);

	if(getSelected())
		writer.writeAttribute(_S("selected"), _S("selected"));
}

void HtmlOption::onInit()
{
	ControlBase::onInit();

	getControls()->add(m_label);
}

void HtmlOption::onRender(HtmlWriter &writer)
{
	writer.openTag(_S("option"));
	renderAttributes(writer);
	writer.closeTag();
	writer.writeLine();
	renderChilds(writer);
	writer.closeTag(_S("option"));
	writer.writeLine();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
