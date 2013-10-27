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
#include "idepickerdatetimespecial.h"

#include "conversions.h"
#include "ideskin.h"
#include "htmlattributes.h"
#include "htmlcontrols.h"
#include "htmlimage.h"
#include "htmltext.h"
#include "htmlcombobox.h"
#include "htmltextbox.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdePickerDateTimeSpecial::IdePickerDateTimeSpecial() :	m_type(OS_NEW HtmlComboBox()),
													m_specific(OS_NEW IdePickerDateTime())
{
	m_type->setID(_S("type"));	
	m_specific->setID(_S("specific"));	
}

IdePickerDateTimeSpecial::~IdePickerDateTimeSpecial()
{

}

void IdePickerDateTimeSpecial::onLoad()
{
	ControlBase::onLoad();

	m_type->setAutoPostBack(true);

	m_type->addOption(getPage()->getText(_S("search.datemode.not_specified")), conversions::to_utf16<uint32>(searchDateTimeNotSpecified));
	m_type->addOption(getPage()->getText(_S("search.datemode.specific_date")), conversions::to_utf16<uint32>(searchDateTimeSpecifiedDate));
	m_type->addOption(getPage()->getText(_S("search.datemode.now")), conversions::to_utf16<uint32>(searchDateTimeNow));
	m_type->addOption(getPage()->getText(_S("search.datemode.reference")), conversions::to_utf16<uint32>(searchDateTimeReference));
	m_type->addOption(getPage()->getText(_S("search.datemode.last_24_hour")), conversions::to_utf16<uint32>(searchDateTimeLast24Hour));
	m_type->addOption(getPage()->getText(_S("search.datemode.last_week")), conversions::to_utf16<uint32>(searchDateTimeLastWeek));
	m_type->addOption(getPage()->getText(_S("search.datemode.last_two_week")), conversions::to_utf16<uint32>(searchDateTimeLastTwoWeek));
	m_type->addOption(getPage()->getText(_S("search.datemode.last_month")), conversions::to_utf16<uint32>(searchDateTimeLastMonth));
	m_type->addOption(getPage()->getText(_S("search.datemode.last_three_month")), conversions::to_utf16<uint32>(searchDateTimeLastThreeMonth));
	m_type->addOption(getPage()->getText(_S("search.datemode.last_year")), conversions::to_utf16<uint32>(searchDateTimeLastYear));

	getControls()->add(m_type);

	if(getValueType() == searchDateTimeSpecifiedDate)
		getControls()->add(m_specific);	
}

void IdePickerDateTimeSpecial::onPreRender()
{
	ControlBase::onPreRender();

	if(getValueType() == searchDateTimeSpecifiedDate)
		getControls()->add(m_specific);	
}

void IdePickerDateTimeSpecial::setValue(const SearchDateTime &type, const DateTime &date)
{
	setValueType(type);
	if(getValueType() == searchDateTimeSpecifiedDate)
		setValueSpecific(date);	
}

SearchDateTime IdePickerDateTimeSpecial::getValueType() const
{	
	return (SearchDateTime) conversions::from_utf16<uint32>(m_type->getValue());
}

void IdePickerDateTimeSpecial::setValueType(const SearchDateTime &type)
{
	m_type->setValue(conversions::to_utf16<uint32>(type));
}

DateTime IdePickerDateTimeSpecial::getValueSpecific() const
{	
	if(getValueType() == searchDateTimeSpecifiedDate)
		return m_specific->getValue();
	else
		return DateTime::EMPTY;
}

void IdePickerDateTimeSpecial::setValueSpecific(const DateTime &date)
{
	setValueType(searchDateTimeSpecifiedDate);
	m_specific->setValue(date);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
