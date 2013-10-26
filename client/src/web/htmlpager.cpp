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
#include "htmlpager.h"

#include "constants.h"
#include "css.h"
#include "htmlcontrols.h"
#include "htmllink.h"
#include "htmlspace.h"
#include "htmlspan.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlPager::Entry::Entry(const String &href)
{
	m_href = href;
}

HtmlPager::Entry::~Entry()
{
	
}

//////////////////////////////////////////////////////////////////////

HtmlPager::HtmlPager() : m_format(pfPagesNumbers),
						 m_options(poShowAll),
						 m_count(3),
						 m_current(constants::npos)
{

}

HtmlPager::~HtmlPager()
{

}

HtmlPager::entry_ptr HtmlPager::addPage(const String &href)
{
	entry_ptr entry(OS_NEW Entry(href));
	m_entries.push_back(entry);
	return entry;
}

String HtmlPager::formatName(uint32 index, const entry_ptr &entry) const
{
	switch(m_format)
	{
	case pfPagesNumbers:	return String::format(_S("%d").c_str(), index + 1);
							
	case pfPagesCaptions:	return entry->getCaption();
							
	default:				OS_ASSERTFALSE();
							break;
	}

	return String::EMPTY;
}

void HtmlPager::onLoad()
{
	ControlBase::onLoad();

	uint32 pages = static_cast<uint32>(m_entries.size());
	if(pages == 0)
		return;

	bool hasCurrent = m_current != constants::npos;
	OS_ASSERT(m_current < pages || hasCurrent == false);
	uint32 current = hasCurrent ? std::min <uint32>(m_current, pages - 1) : constants::npos;

	if((current != 0 && getOption(poShowFirstPage)) || hasCurrent == false)
	{
		_insertPage(_S("<<"), m_entries.front()->getHref(), OS_CSS_PAGER_FIRSTLAST);
	}

	if(current > 0 && getOption(poShowPrevPage) && hasCurrent)
	{
		_insertPage(_S("<"), m_entries.at(current - 1)->getHref(), OS_CSS_PAGER_PREVNEXT);
	}

	for(uint32 i = 0; i < pages; i++)
	{
		entry_ptr entry = m_entries.at(i);
		_insertPage(formatName(i, entry), entry->getHref(), i == current ? OS_CSS_PAGER_CURRENT : OS_CSS_PAGER_LINK);		
	}

	if(current < pages - 1 && getOption(poShowNextPage) && hasCurrent)
	{
		_insertPage(_S(">"), m_entries.at(current + 1)->getHref(), OS_CSS_PAGER_PREVNEXT);
	}

	if((current != pages - 1 && getOption(poShowLastPage)) || hasCurrent == false)
	{
		_insertPage(_S(">>"), m_entries.back()->getHref(), OS_CSS_PAGER_FIRSTLAST);
	}	
}

void HtmlPager::_insertPage(const String &caption, const String &href, const String &css)
{
	shared_ptr<HtmlSpan> span(OS_NEW HtmlSpan());
	span->setCss(css);
	getControls()->add(span);

	span->getControls()->add(shared_ptr<HtmlLink>(OS_NEW HtmlLink(caption, href)));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
