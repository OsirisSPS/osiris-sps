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
#include "idepickernumber.h"

#include "conversions.h"
#include "htmlattributes.h"
#include "htmlcontrols.h"
#include "htmlimage.h"
#include "htmltext.h"
#include "htmltextbox.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdePickerNumber::IdePickerNumber() : m_input(OS_NEW HtmlTextBox())
{
	m_valueMin = 0;
	m_valueMax = 100;

	m_input->setID(_S("number"));
}

IdePickerNumber::~IdePickerNumber()
{

}

void IdePickerNumber::onLoad()
{
	m_input->setSize(19);
	m_input->setMaxLength(19);	
	m_input->getAttributes()->set(_S("data-os-otype"), _S("number"));
	m_input->getAttributes()->set(_S("data-os-min"), conversions::to_utf16(m_valueMin));
	m_input->getAttributes()->set(_S("data-os-max"), conversions::to_utf16(m_valueMax));
	getControls()->add(m_input);	
}

int IdePickerNumber::getValue() const
{	
	return conversions::from_string<int>(m_input->getValue().to_ascii());	
}

void IdePickerNumber::setValue(const int &value)
{
	m_input->setValue(conversions::to_string(value));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
