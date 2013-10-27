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
#include "htmltabctrl.h"

#include "conversions.h"
#include "css.h"
#include "eventssource.h"
#include "httpparser.h"
#include "htmlcontrols.h"
#include "htmllink.h"
#include "htmlimage.h"
#include "htmltabpage.h"
#include "httprequest.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlTabCtrl::HtmlTabCtrl() : m_pages(OS_NEW HtmlDiv()),
							 m_view(OS_NEW HtmlDiv())
{
	
}

HtmlTabCtrl::~HtmlTabCtrl()
{

}

int32 HtmlTabCtrl::getCurSel() const
{
	return hasID() ? conversions::from_wstring<int32>(getRequest()->getUrlParam(getTarget().to_wide())) : -1;
}

int32 HtmlTabCtrl::getPagesCount() const
{
	return static_cast<uint32>(m_pages->getControls()->size());
}

String HtmlTabCtrl::getPageLink(uint32 index)
{
	OS_EXCEPT_IF(hasID() == false, "Invalid tabctrl id");
	OS_EXCEPT_IF(getRequest() == null, "Invalid request object");

	ordered_map<std::wstring, std::wstring> params(getRequest()->getUrlParams());
	params.set(getTarget().to_wide(), conversions::to_wstring(index));

	return HttpParser::generateLink(getRequest()->getRawUrl(), params);
}

shared_ptr<HtmlTabPage> HtmlTabCtrl::addPage(const String &caption, shared_ptr<HtmlImage> icon)
{
	int32 page_index = getPagesCount();
	shared_ptr<HtmlTabPage> page = createPage(page_index);
	page->setCss(page_index == getCurSel() ? OS_CSS_TABCTRL_ACTIVEPAGE : OS_CSS_TABCTRL_PAGE);

	m_pages->getControls()->add(page);

	if(icon != null)
		page->getControls()->add(icon);

	if(caption.empty() == false)
	{
		shared_ptr<HtmlLink> link(OS_NEW HtmlLink(caption, getPageLink(page_index)));
		page->getControls()->add(link);
	}

	return page;
}

shared_ptr<HtmlTabPage> HtmlTabCtrl::addPage(const String &caption, const String &image_path)
{
	shared_ptr<HtmlImage> image;
	if(image_path.empty() == false)
		image.reset(OS_NEW HtmlImage(image_path));

	return addPage(caption, image);
}

shared_ptr<HtmlTabPage> HtmlTabCtrl::createPage(int32 index) const
{
	return shared_ptr<HtmlTabPage>(OS_NEW HtmlTabPage(index));
}

void HtmlTabCtrl::onInit()
{
	ControlBase::onInit();

	getControls()->add(m_pages);
	getControls()->add(m_view);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
