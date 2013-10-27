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

#ifndef _IDE_PORTAL_USERS_H
#define _IDE_PORTAL_USERS_H

#include "iidexslrenderer.h"
#include "iidepageable.h"
#include "iideportalpage.h"
#include "reputationsreputations.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IdeButton;
class HtmlComboBox;
class HtmlTextBox;

//////////////////////////////////////////////////////////////////////

namespace ide {
namespace portal {

//////////////////////////////////////////////////////////////////////

class EngineExport Users : public IPortalPage,
						   public IIdePageable
{
	typedef IPortalPage PageBase;

// Enumerated types
public:
	enum NickMode
	{
		nmStartsWith			= 0,
		nmExact					= 1,
		nmContains				= 2,
	};

	enum OrderMode
	{
		omJoinDate				= 0,
		omReputationAndName		= 1,
		omName					= 2,
		omObjects				= 3,
		omRevisions				= 4,
	};

	enum Template
	{
		utBox					= 0,		
		utTable					= 1,
	};

// Construction
public:
	Users(shared_ptr<HttpSession> session);
	virtual ~Users();

// Attributes
public:
	inline uint32 getOffset() const;
	inline void setOffset(uint32 offset);

// Events
protected:
	void onFilterAlphabetic(IEvent *e);
	void onChangeOffset(IEvent *e);
	void onSearch();	

// IPage interface
public:
	virtual String getPageName() const;

// IPageable interface
public:
	virtual String getPageHref(uint32 offset) const;

// IPortalPage overrides
protected:
	virtual void onLoad();
	virtual void onPreRender();

protected:
	shared_ptr<HtmlXSLControl> m_pageTemplate;
	uint32 m_offset;
	shared_ptr<HtmlComboBox> m_nickMode;
	shared_ptr<HtmlTextBox> m_nickName;
	shared_ptr<HtmlComboBox> m_orderMode;
	shared_ptr<HtmlComboBox> m_orderDirection;
	shared_ptr<HtmlComboBox> m_templateType;
	shared_ptr<HtmlComboBox> m_pagerSize;
	shared_ptr<IdeButton> m_submit;

private:
	static const String EVENT_ONCHANGEOFFSET;
	static const String EVENT_ONFILTERALPHABETIC;
};

//////////////////////////////////////////////////////////////////////

inline uint32 Users::getOffset() const { return m_offset; }
inline void Users::setOffset(uint32 offset) { m_offset = offset; }

//////////////////////////////////////////////////////////////////////

} // portal
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_PORTAL_USERS_H
