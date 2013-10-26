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
#include "htmlevent.h"

#include "utils.h"

//////////////////////////////////////////////////////////////////////

#define OS_HTMLEVENT_DELIMITER		","

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const HtmlEvent HtmlEvent::EMPTY;

//////////////////////////////////////////////////////////////////////

HtmlEvent::HtmlEvent()
{

}

HtmlEvent::HtmlEvent(const String &args)
{
	decode(args);
}

HtmlEvent::~HtmlEvent()
{

}

const String & HtmlEvent::get(size_t index) const
{
	OS_EXCEPT_IF(index >= size(), "Invalid arg index");
	return m_args[index];
}

String HtmlEvent::encode() const
{
	return utils::desplit(m_args, OS_HTMLEVENT_DELIMITER);
}

void HtmlEvent::decode(const String &args)
{
	m_args.clear();
	utils::split(args, OS_HTMLEVENT_DELIMITER, m_args, false);
}

void HtmlEvent::add(const String &arg)
{
	OS_EXCEPT_IF(arg.find(OS_HTMLEVENT_DELIMITER) != String::npos, "Invalid arg value"); 
	m_args.push_back(arg);	
}

const String & HtmlEvent::operator[](size_t index) const
{
	return get(index);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
