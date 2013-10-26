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

#ifndef _IDE_VIEW_COMMENTABLEOBJECTVIEWER_H
#define _IDE_VIEW_COMMENTABLEOBJECTVIEWER_H

#include "ideviewiobjectviewer.h"


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace view {

//////////////////////////////////////////////////////////////////////

class EngineExport CommentableObjectViewer : public IObjectViewer
{
	typedef IObjectViewer ControlBase;

// Enumerated types
public:
	enum Action
	{
		actNone			= 0,
		actCreatePost	= 1,
	};

// Construction
public:
	CommentableObjectViewer(shared_ptr<EntitiesEntity> entity = null);
	virtual ~CommentableObjectViewer();

// Attributes
public:
	Action getAction() const;
	String getCreatePostLink(const EntityID &quoted_object = EntityID::EMPTY);
	String getEditObjectLink(shared_ptr<ObjectsIRevisionable> object) const;
	
// Operations
private:
	String localizePost(const EntityID &id);

// IControl overrides
public:
	virtual void onInit();
	virtual void onLoad();

// Operations
public:
	void onViewObject();
	void onCreatePost();

private:
	void _createLastPosts();

// Overridables
public:
	virtual void renderObject(shared_ptr<XMLPortalExporter> exporter, shared_ptr<ObjectsIRevisionable> object);
	virtual void renderPosts(shared_ptr<XMLPortalExporter> exporter);
	virtual void renderPost(shared_ptr<XMLPortalExporter> exporter, shared_ptr<ObjectsPost> post);
};

//////////////////////////////////////////////////////////////////////

} // view
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_VIEW_COMMENTABLEOBJECTVIEWER_H
