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
#include "portalsaccount.h"

#include "dataaccount.h"
#include "objectsuser.h"
#include "objectsirevisionable.h"
#include "iportaldatabase.h"
#include "entitiesentitiescache.h"
#include "entitiessnapshotmanager.h"
#include "entitiesstatisticscache.h"
#include "ideskin.h"
#include "dataitem.h"
#include "idesession.h"
#include "portalsportal.h"
#include "portalsprofilescache.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace portals {

//////////////////////////////////////////////////////////////////////

//Account::Account(shared_ptr<Portal> portal, shared_ptr<DataAccount> account, uint32 priority) : m_portal(portal),
Account::Account(shared_ptr<DataAccount> account, uint32 priority) : 
																						  m_account(account)/*,
																						  m_skin(OS_NEW IdeSkin())*/
{
	m_deleted = false;

	//_sinkSkin();
}

Account::~Account()
{
	/*
	// URGENT

	if(getDeleted())
	{
		shared_ptr<Portal> portal = getPortal();
		OS_ASSERT(portal != null);
		if(portal != null)
		{
			portal->deleteAccount(getID());
		}
	}
	*/
}

const String & Account::getID() const
{
	return m_account->id;
}

const String & Account::getUserName() const
{
	return m_account->username;
}

void Account::setUserName(const String& userName)
{
	m_account->username = userName;
}

SkinID Account::getSkinID() const
{
	return m_account->getSkinID();
}

/*
bool Account::isLogged() const
{
	return getPortal()->isAccountLogged(getID());
}
*/

shared_ptr<ObjectsUser> Account::loadUser(shared_ptr<IPortalDatabase> database)
{
	OS_ASSERT(database != null);

	shared_ptr<ObjectsUser> user;

	ObjectID user_id;
	// Controlla se all'account  direttamente associabile un utente
	if(getAccount()->getUserID(user_id))
	{
		// Carica l'utente associato all'account
		user = database->getUser(user_id);
	}

	return user;
}

bool Account::getDeleted() const
{
	OS_LOCK(m_cs);
	return m_deleted;
}

void Account::setDeleted(bool deleted)
{
	OS_LOCK(m_cs);
	m_deleted = deleted;
}

DataViewMode Account::getViewMode() const
{
	return m_account->getViewMode();
}

void Account::setViewMode(DataViewMode mode)
{
	m_account->setViewMode(mode);
}

String Account::getLanguage() const
{
	return m_account->getLanguage();
}

bool Account::hasLanguage() const
{
	return m_account->hasLanguage();
}

void Account::setLanguage(const String &language)
{
	m_account->setLanguage(language);
}

/*
shared_ptr<IdeSkin> Account::getSkin() const
{
	return m_skin;
}
*/

void Account::setSkin(const SkinID &id)
{
	m_account->setSkinID(id);

	//return _sinkSkin();
}

const String & Account::getRealPassword() const
{
	return m_account->real_password;
}

bool Account::setRealPassword(const String &password)
{
	if(password.empty() == false && m_account->validatePassword(password) == false)
		return false;

	m_account->real_password = password;
	return true;
}

const DateTime & Account::getLastAccessDate() const
{
	return m_account->last_access_date;
}

void Account::setLastAccessDate(const DateTime &date)
{
	m_account->last_access_date = date;
}

const DateTime & Account::getReferenceDate() const
{
	return m_account->reference_date;
}

void Account::setReferenceDate(const DateTime &date)
{
	m_account->reference_date = date;
}

bool Account::updateAccount(shared_ptr<IPortalDatabase> database)
{
	return database->updateRecord(m_account);
}

/*
bool Account::_sinkSkin()
{
	if(m_account->hasSkinID() == false)
	{
		m_skin->unload();
		return true;
	}

	if(m_skin->load(m_account->getSkinID().to_ascii()) == false)
		return false;

	return m_account->hasSkinSchema() ? m_skin->setSchema(m_account->getSkinSchema()) : true;
}
*/

//////////////////////////////////////////////////////////////////////

} // portals
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
