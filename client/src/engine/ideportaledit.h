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

#ifndef _IDE_PORTAL_EDIT_H
#define _IDE_PORTAL_EDIT_H

#include "iidepageable.h"
#include "iideportalpage.h"


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlCheckBox;
class HtmlXSLControl;
class XMLPortalExporter;

//////////////////////////////////////////////////////////////////////

namespace ide {
namespace portal {

//////////////////////////////////////////////////////////////////////

class EngineExport Edit :	public IPortalPage,
							public IIdePageable
{
	typedef IPortalPage PageBase;

// Enumerated types
public:
	enum Action
	{
		acOverview			= 0,
		acAddChild			= 1,
		acAddRevision		= 2,
		//acAddMove			= 3,
		//acDelete			= 4,
		//acRestoreRevision	= 5,
		//acSelect			= 6,
		//acUnselect		= 7,
		acMoveable			= 8,
		acHistory			= 9,
	};

// Construction
public:
	Edit(shared_ptr<HttpSession> session);
	virtual ~Edit();

// Operations:
public:
	// Gets for url-params.
	String getUrlID() const;
	Action getUrlAction() const;
	PortalObjectType getUrlAddChildObjectType() const;
	ObjectID getUrlAddMoveObject() const;

	// Urls builders
	static String getUrl(shared_ptr<Portal> portal, const EntityID &id, Action action, const ObjectID &revision = ObjectID::EMPTY);
	static String getAddChildUrl(shared_ptr<Portal> portal, const EntityID &id, PortalObjectType type);
	//static String getAddMoveUrl(shared_ptr<Portal> portal, const ObjectID &id, const ObjectID &moveId);

	void exportHistory(shared_ptr<EntitiesEntity> entity, shared_ptr<XMLPortalExporter> exporter);
	void exportParent(shared_ptr<EntitiesEntity> entity, shared_ptr<XMLPortalExporter> exporter);
	void exportChilds(shared_ptr<EntitiesEntity> entity, shared_ptr<XMLPortalExporter> exporter);

private:
	void _exportRevision(shared_ptr<XMLNode> node, shared_ptr<EntitiesEntity> entity, shared_ptr<ObjectsIObject> revision);

// IPage interface
public:
	virtual String getPageName() const;

// Pager interface
protected:
	virtual String getPageHref(uint32 offset) const;

// IPortalPage overrides
public:
	virtual String getPagePathway() const;
	virtual EntityID getTargetObject() const;

// Events
private:
	void onSelect();
	void onUnselect();
	void onDelete();
	void onMove(IEvent *e);
	void onRestore(IEvent *e);

protected:
	virtual void onLoad();
	virtual void onPreRender();

public:
	static const std::wstring ID;
	static const std::wstring ACTION;
	static const std::wstring OBJECTTYPE;
	static const std::wstring MOVEID;

	shared_ptr<HtmlXSLControl> m_template;
	shared_ptr<HtmlCheckBox> m_showSystem;
	shared_ptr<HtmlCheckBox> m_showInvisible;
	shared_ptr<EntitiesEntity> m_entity;
	//shared_ptr<HtmlXSLControl> m_pageTemplate;
};

//////////////////////////////////////////////////////////////////////

} // portal
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_PORTAL_EDIT_H
