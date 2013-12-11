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
#include "idemainresources.h"

#include "conversions.h"
#include "entitiesentity.h"
#include "file.h"
#include "filesystem.h"
#include "objectsavatar.h"
#include "objectsfile.h"
#include "iportaldatabase.h"
#include "httpsession.h"
#include "httprequest.h"
#include "mimemanager.h"
#include "options.h"
#include "portalsportal.h"
#include "temporaryfile.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace main {

//////////////////////////////////////////////////////////////////////

const std::wstring Resources::PARAM_AVATAR(L"avatar");
const std::wstring Resources::PARAM_FILE(L"file");

//////////////////////////////////////////////////////////////////////

Resources::Resources(shared_ptr<HttpSession> session) : PageBase(session)
{

}

Resources::~Resources()
{

}

bool Resources::transmitAvatar(const shared_ptr<IPortalDatabase> &database, const String &hash)
{
	shared_ptr<ObjectsAvatar> avatar = database->getAvatar(hash.to_ascii());

	closeDatabase();

	if(avatar == null)
		return false;

	const Buffer &buffer = avatar->file_data;
	getSession()->transmitBuffer(buffer, avatar->file_name, avatar->file_type, avatar->submit_date->toPosixTime());
	return true;
}

bool Resources::transmitFile(const shared_ptr<IPortalDatabase> &database, const String &hash)
{
	shared_ptr<EntitiesEntity> entity = database->getPortal()->getEntity(database, hash.to_ascii());
	if(entity == null)
		return "";
	if(entity->getCurrent() == null)
		return "";

	shared_ptr<ObjectsFile> file = database->getFile(entity->getCurrent()->id->toAscii());

	closeDatabase();

	if(file == null)
		return false;

	const Buffer &buffer = file->file_data;
	getSession()->transmitBuffer(buffer, file->file_name, file->getMimeType(), file->submit_date->toPosixTime());
	return true;
}

String Resources::getPageName() const
{
	return _S("main.pages.resources");
}

bool Resources::isMcpModeRequired() const
{
	return false;
}

bool Resources::isIsisAllowed() const
{
	return true;
}

bool Resources::transmit()
{
	shared_ptr<IPortalDatabase> database = getDatabase();
	if(database == null)
		return false;	// not found

	String avatar = getRequest()->getUrlParam(PARAM_AVATAR);
	if(avatar.empty() == false)
		return transmitAvatar(database, avatar);

	String file = getRequest()->getUrlParam(PARAM_FILE);
	if(file.empty() == false)
		return transmitFile(database, file);

	closeDatabase();

	return false;	// not found
}

//////////////////////////////////////////////////////////////////////

} // main
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
