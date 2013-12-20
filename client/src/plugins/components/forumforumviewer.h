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

#ifndef _COMPONENTS_FORUM_FORUMVIEWER_H
#define _COMPONENTS_FORUM_FORUMVIEWER_H

#include "iidepageable.h"
#include "iidexslrenderer.h"
#include "extensionscomponentviewer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLPortalExporter;

//////////////////////////////////////////////////////////////////////

namespace components {
namespace forum {

//////////////////////////////////////////////////////////////////////

class ForumViewer : public IXSLRenderer<ExtensionsComponentViewer>,
					public IIdePageable
{
	typedef IXSLRenderer<ExtensionsComponentViewer> ViewerBase;

public:
	// Enumerated types
	enum Action
	{
		faNone			= 0,
		faCreateTopic	= 1,
		faCreatePoll	= 2,
	};

// Construction
public:
	ForumViewer(shared_ptr<EntitiesEntity> topic = nullptr);
	virtual ~ForumViewer();

// Attributes
public:
	Action getAction() const;
	String getCreateTopicLink(const EntityID &forum);
	String getCreatePollLink(const EntityID &forum);

// IXSLRenderer interface
public:
	virtual String getTemplatePath();

// IPageable interface
protected:
	virtual String getPageHref(uint32 offset) const;

// IControls overrides
protected:
	virtual void onLoad();

// Events
private:
	void onCreateTopic();
	void onCreatePoll();
	void onViewForum(uint32 offset);

// Operations
private:
	void renderForumActions(shared_ptr<EntitiesEntity> entity, shared_ptr<XMLPortalExporter> exporter);
	void renderForumContent(shared_ptr<EntitiesEntity> entity, shared_ptr<XMLPortalExporter> exporter, uint32 offset);

protected:
	shared_ptr<EntitiesEntity> m_topic;
};

//////////////////////////////////////////////////////////////////////

} // forum
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _COMPONENTS_FORUM_FORUMVIEWER_H
