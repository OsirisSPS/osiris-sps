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
#include "idepickerdatetime.h"

#include "conversions.h"
#include "ideskin.h"
#include "htmlattributes.h"
#include "htmlcontrols.h"
#include "htmlimage.h"
#include "htmltext.h"
#include "htmltextbox.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdePickerDateTime::IdePickerDateTime() : m_input(OS_NEW HtmlTextBox())
{
	m_input->setID(_S("date"));	
}

IdePickerDateTime::~IdePickerDateTime()
{

}

void IdePickerDateTime::onLoad()
{
	m_input->setSize(19);
	m_input->setMaxLength(19);	
	getControls()->add(m_input);

	/*
	shared_ptr<HtmlImage> img(OS_NEW HtmlImage(getPage()->getSkin()->getImageUrl(_S("small/warning.gif"))));
	img->getAttributes()->set(_S("data-os-tooltip"),_S("Syntax:\r\nyyyy-mm-ddThh24:mi:ss"));
	getControls()->add(img);
	*/

	m_input->getAttributes()->set(_S("data-os-otype"), _S("datetime"));
	m_input->getAttributes()->set(_S("data-os-showtime"), conversions::to_utf16(m_showTime));
}

DateTime IdePickerDateTime::getValue() const
{	
	DateTime dt;
	dt.fromXML(m_input->getValue());
	dt.applyTimeOffset(-getPage()->getTimeOffset(), -getPage()->getTimeDST());
	return dt;
}

void IdePickerDateTime::setValue(const DateTime &date)
{
	DateTime dt = date;
	dt.applyTimeOffset(getPage()->getTimeOffset(), getPage()->getTimeDST());
	m_input->setValue(dt.toXML());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
