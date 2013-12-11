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
#include "portalsprofile.h"

#include "convert.h"
#include "objectsuser.h"
#include "iportaldatabase.h"
#include "entitiessnapshotmanager.h"
#include "dataitem.h"
#include "portalsaccount.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalsportalssystem.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace portals {

//////////////////////////////////////////////////////////////////////

Profile::Profile(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> database, shared_ptr<ObjectsUser> user) : m_portal(portal),
																				 						   m_user(user)
{
	// Calcolo Score
	DataTable result;
	String sql = String::format(_S("select score, description, follow from os_snapshot_users where id='%S'").c_str(), m_user->id->toUTF16().c_str());
	database->execute(sql, result);
	if(result.hasRow(0))
	{
		// Estrae il punteggio
		m_score.setValue(result.get(0, _S("score")));
		// Estrae la descrizione
		m_score.setDescription(result.get(0, _S("description")));
		// Estrae il follow
		m_score.setFollow(result.get(0, _S("follow")));
	}
}

Profile::~Profile()
{

}

/*
String Profile::getSubscribeLink(bool http) const
{
	shared_ptr<Portal> portal = getPortal();
	if(portal == null)
	{
		OS_ASSERTFALSE();
		return String::EMPTY;
	}

	return portal->generateInviteLink(getUser()->id, http);
}

String Profile::getExportLink() const
{
	shared_ptr<Portal> portal = getPortal();

	return portal->generateExportLink(getUser()->id);
}
*/

//////////////////////////////////////////////////////////////////////

} // portals
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
