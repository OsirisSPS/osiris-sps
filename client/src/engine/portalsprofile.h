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

#ifndef _PORTALS_PROFILE_H
#define _PORTALS_PROFILE_H

#include "base/object.h"
#include "objectsobjects.h"
#include "dbdb.h"
#include "entitiesentities.h"
#include "portalsportals.h"
#include "reputationsscore.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;
class Portal;

//////////////////////////////////////////////////////////////////////

namespace portals {

//////////////////////////////////////////////////////////////////////

class EngineExport Profile : public Object
{
// Construction
public:
	Profile(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> database, shared_ptr<ObjectsUser> user);
	virtual ~Profile();

// Attributes
public:
	inline shared_ptr<Portal> getPortal() const;
	inline shared_ptr<ObjectsUser> getUser() const;

	inline ReputationsScore getScore() const;

// Operations
public:
	
protected:
	weak_ptr<Portal> m_portal;
	shared_ptr<ObjectsUser> m_user;
	ReputationsScore m_score;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<Portal> Profile::getPortal() const { return m_portal.lock(); }
inline shared_ptr<ObjectsUser> Profile::getUser() const { return m_user; }
inline ReputationsScore Profile::getScore() const { return m_score; }

//////////////////////////////////////////////////////////////////////

} // portals
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _PORTALS_PROFILE_H
