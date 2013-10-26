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

#ifndef _IDE_PORTAL_USER_H
#define _IDE_PORTAL_USER_H

#include "iidexslrenderer.h"
#include "iideportalpage.h"
#include "reputationsreputations.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlCheckBox;
class HtmlComboBox;
class HtmlTextBox;

//////////////////////////////////////////////////////////////////////

namespace ide {
namespace portal {

//////////////////////////////////////////////////////////////////////

class EngineExport User : public IPortalPage
{
	typedef IPortalPage PageBase;

// Enumerated types
public:
	enum Action
	{
		acInfo			= 0,
		acInvite		= 1,
		acReputations	= 2,
		acSearch		= 3,
	};

// Construction
public:
	User(shared_ptr<HttpSession> session);
	virtual ~User();

// Operations
private:
	// Gets for url-params.
	String getUrlID() const;
	Action getUrlAction() const;

	// Urls builders
	String getUrl(const String &id, Action action) const;

	//void createReputationsFeedback(const shared_ptr<ReputationsFeedback> &feedback);
	void createReputationsFeedback(const shared_ptr<XMLNode> &node, const shared_ptr<ReputationsFeedback> &feedback);	

// Events
protected:
	void onChangeReputation();

// IPage interface
public:
	virtual String getPageName() const;

// IPortalPage overrides
protected:
	virtual void onPathway2() const;
	virtual void onInit();
	virtual void onLoad();
	virtual void onPreRender();

private:
	shared_ptr<ObjectsUser> m_user;

	shared_ptr<HtmlXSLControl> m_pageTemplate;
	shared_ptr<HtmlComboBox> m_reputationValue;
	shared_ptr<HtmlTextBox> m_reputationComment;
	shared_ptr<HtmlCheckBox> m_reputationFollow;

protected:
	static const std::wstring ID;
	static const std::wstring ACTION;
};

//////////////////////////////////////////////////////////////////////

} // portal
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_PORTAL_USER_H
