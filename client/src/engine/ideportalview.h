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

#ifndef _IDE_PORTAL_VIEW_H
#define _IDE_PORTAL_VIEW_H

#include "iideportalpage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace portal {

//////////////////////////////////////////////////////////////////////

class EngineExport View : public IPortalPage
{
	typedef IPortalPage PageBase;

// Construction
public:
	View(shared_ptr<HttpSession> session);
	virtual ~View();

// Operations
private:
	shared_ptr<IHtmlControl> createView(shared_ptr<EntitiesEntity> targetEntity) const;

// IPage interface
public:
	virtual String getPageName() const;

// IPortalPage overrides
public:
	// Pathway
	void onPathway2() const;
	virtual String getPagePathway() const;
	virtual EntityID getTargetObject() const;

protected:
	virtual bool getShowInstance(shared_ptr<ObjectsInstance> instance) const;
	virtual uint32 loadInstances(const EntityID &ID);

protected:
	virtual void onLoad();
};

//////////////////////////////////////////////////////////////////////

} // portal
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_PORTAL_VIEW_H
