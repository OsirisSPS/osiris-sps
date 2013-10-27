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
#include "idepickerbool.h"

#include "htmlattributes.h"
#include "htmlcontrols.h"
#include "htmlliteral.h"
#include "htmlcheckbox.h"
#include "htmlwriter.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdePickerBool::IdePickerBool() : m_checkbox(OS_NEW HtmlCheckBox())
{
	m_autoPostBack = false;

	getAttributes()->set(_S("data-os-otype"), _S("bool"));
}

IdePickerBool::~IdePickerBool()
{

}

bool IdePickerBool::getCheck() const
{
	const String &value = getValue();
	return value == _S("1") || value == _S("on");
}

void IdePickerBool::setCheck(bool check)
{
	setValue(check ? _S("1") : String::EMPTY);
}

void IdePickerBool::onPreRender()
{
	ControlBase::onPreRender();

	//getControls()->add(m_checkbox);
	//m_checkbox->setCheck(getCheck());
	//m_checkbox->getAttributes()->set(_S("onchange"), _S("javascript:osGetById('") + getTarget() + _S("').value = this.checked ? 1:0"));
}

void IdePickerBool::renderAttributes(HtmlWriter &writer)
{
	ControlBase::renderAttributes(writer);
		
	writer.writeAttribute(_S("type"), _S("hidden"));
	
	if(getAutoPostBack())
		writer.writeAttribute(_S("data-os_autopostback"), _S("1"));
		//writer.writeAttribute(_S("onchange"), _S("javascript:return osAutoPostBack(this);"));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
