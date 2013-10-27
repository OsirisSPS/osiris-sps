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

#ifndef _PORTALS_PORTALS_H
#define _PORTALS_PORTALS_H

#include "ids.h"

//////////////////////////////////////////////////////////////////////

#define OS_PORTAL_DB_VERSION				OS_MAKEVERSION(0, 1)
#define OS_PORTAL_DB_TYPE					_S("portal")

#define OS_PORTAL_XML						_S("portal.xml")
#define OS_PORTAL_SEARCH_PATH				_S("search")

#define OS_SCHEMA_PORTAL					_S("portal_razor.xsd")

#define OS_URL_PARAM_ACTION					_W("action")
#define OS_URL_PARAM_ID						_W("id")
#define OS_URL_PARAM_OFFSET					_W("offset")
#define OS_URL_PARAM_PAGE					_W("page")
#define OS_URL_PARAM_PORTAL					_W("portal")
#define OS_URL_PARAM_TARGET					_W("target")
#define OS_URL_PARAM_TYPE					_W("type")
#define OS_URL_PARAM_DT						_W("dt")
#define OS_URL_PARAM_QUERY					_W("query")
#define OS_URL_PARAM_QUOTE					_W("quote")
#define OS_URL_PARAM_REVISION				_W("revision")
#define OS_URL_PARAM_MESSAGE				_W("message")
#define OS_URL_PARAM_MODE					_W("mode")
#define OS_URL_PARAM_URL					_W("url")
#define OS_URL_PARAM_CONFIRM				_W("confirm")
#define OS_URL_PARAM_USER					_W("user")
#define OS_URL_PARAM_POV					_W("portal")

#define OS_DEFAULT_ADMIN_USERNAME			_S("admin")
#define OS_DEFAULT_ADMIN_PASSWORD			_S("admin")

/*
// N.B.: non modificare mai questi valori
#define OS_PORTAL_TYPE_UNKNOWN			   -1
#define OS_PORTAL_TYPE_ANARCHIC				0
#define OS_PORTAL_TYPE_MONARCHIC			1

#define OS_PORTAL_ACCESS_UNKNOWN		   -1
#define OS_PORTAL_ACCESS_PUBLIC				0
#define OS_PORTAL_ACCESS_PRIVATE			1
*/

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

/*
// Enumerated types
enum PortalType
{
	portalTypeUnknown		= OS_PORTAL_TYPE_UNKNOWN,
	portalTypeAnarchic		= OS_PORTAL_TYPE_ANARCHIC,
	portalTypeMonarchic		= OS_PORTAL_TYPE_MONARCHIC,
};


enum PortalAccess
{
	portalAccessUnknown		= OS_PORTAL_ACCESS_UNKNOWN,
	portalAccessPublic		= OS_PORTAL_ACCESS_PUBLIC,
	portalAccessPrivate		= OS_PORTAL_ACCESS_PRIVATE,
};
*/

//////////////////////////////////////////////////////////////////////

namespace portals {

//////////////////////////////////////////////////////////////////////

// Classes
class Account;
class AccountsManager;
class CleanThread;
class MainDirectory;
class Messenger;
class PortalsDirectory;
class PortalProcessor;
class PortalSessionState;
class Profile;
class ProfilesCache;
class StabilityThread;

// Types
typedef shared_ptr<Account> account_ptr;
typedef weak_ptr<Account> account_weak_ptr;
typedef map<String, account_ptr>::type accounts_map;
typedef shared_ptr<PortalSessionState> portal_session_state_ptr;
typedef shared_ptr<Profile> profile_ptr;

//////////////////////////////////////////////////////////////////////

} // portals
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _PORTALS_PORTALS_H
