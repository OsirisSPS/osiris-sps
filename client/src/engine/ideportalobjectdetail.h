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

#ifndef _IDE_PORTAL_OBJECTDETAIL_H
#define _IDE_PORTAL_OBJECTDETAIL_H

#include "iidexslrenderer.h"
#include "iideportalpage.h"
#include "htmlvirtualpage.h"

#include "reputationsreputations.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace portal {

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectDetail : public HtmlVirtualPage<IPortalPage>
{
	typedef HtmlVirtualPage<IPortalPage> PageBase;

// Enumerated types
public:
	enum Action
	{		

	};

// Construction
public:
	ObjectDetail(shared_ptr<HttpSession> session);
	virtual ~ObjectDetail();

// Operations
private:
	// Gets for url-params.
	String getUrlID() const;

	// Urls builders
	String getUrl(String id, Action action) const;
	
// Events
protected:

// IPage interface
public:
	virtual String getPageName() const;

// IVirtualPage overrides
protected:
	virtual void onLoad();
	virtual void onPreRender();

protected:
	static const std::wstring ID;
};

//////////////////////////////////////////////////////////////////////

} // portal
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_PORTAL_OBJECTDETAIL_H
