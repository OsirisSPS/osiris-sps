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
#include "idepickercolor.h"

#include "conversions.h"
#include "htmlattributes.h"
#include "htmlcontrols.h"
#include "htmlimage.h"
#include "htmltext.h"
#include "htmltextbox.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdePickerColor::IdePickerColor() : m_input(OS_NEW HtmlTextBox())
{	
	m_input->setID(_S("color"));
}

IdePickerColor::~IdePickerColor()
{

}

void IdePickerColor::onLoad()
{
	m_input->getAttributes()->set(_S("data-os-otype"), _S("color"));
	getControls()->add(m_input);	
}

std::string IdePickerColor::getValue() const
{	
	return m_input->getValue().to_ascii();	
}

void IdePickerColor::setValue(const std::string &value)
{
	m_input->setValue(conversions::to_string(value));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
