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

#ifndef _COMPONENTS_FORUM_FORUMMANAGER_H
#define _COMPONENTS_FORUM_FORUMMANAGER_H

#include "entitiesentities.h"
#include "ideide.h"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLPortalExporter;

//////////////////////////////////////////////////////////////////////

namespace components {
namespace forum {

//////////////////////////////////////////////////////////////////////

class ForumManager : public StaticSingleton<ForumManager>
{
// Construction
protected:
	ForumManager();

public:
	virtual ~ForumManager();

public:
	String getPath() const;

// Operations
public:
	void renderForum(shared_ptr<EntitiesEntity> forum, shared_ptr<XMLPortalExporter> exporter);
	void renderSection(shared_ptr<EntitiesEntity> section, shared_ptr<XMLPortalExporter> exporter);

private:
	void _render(shared_ptr<EntitiesEntity> entity, shared_ptr<XMLPortalExporter> exporter, bool renderSections, bool renderForums, uint32 level);
};

//////////////////////////////////////////////////////////////////////

} // forum
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _COMPONENTS_FORUM_FORUMMANAGER_H
