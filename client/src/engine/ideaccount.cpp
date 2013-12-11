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
#include "ideaccount.h"

#include "dataaccount.h"
#include "objectsuser.h"
#include "objectsirevisionable.h"
#include "iportaldatabase.h"
#include "entitiesentitiescache.h"
#include "entitiessnapshotmanager.h"
#include "entitiesstatisticscache.h"
#include "idesession.h"
#include "ideskin.h"
#include "dataitem.h"
#include "portalsportal.h"
#include "portalsprofilescache.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdeAccount::IdeAccount(shared_ptr<DataAccount> account) : m_account(account)
{
	m_deleted = false;

	//_sinkSkin();
}

IdeAccount::~IdeAccount()
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

const String & IdeAccount::getID() const
{
	return m_account->id;
}

const String & IdeAccount::getName() const
{
	return m_account->username;
}

void IdeAccount::setName(const String& name)
{
	m_account->username = name;
}

SkinID IdeAccount::getSkinID() const
{
	return m_account->getSkinID();
}

/*
bool Account::isLogged() const
{
	return getPortal()->isAccountLogged(getID());
}
*/

/* // TOCLEAN
shared_ptr<ObjectsUser> IdeAccount::loadUser(shared_ptr<IPortalDatabase> database)
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
*/

bool IdeAccount::getDeleted() const
{
	OS_LOCK(m_cs);
	return m_deleted;
}

void IdeAccount::setDeleted(bool deleted)
{
	OS_LOCK(m_cs);
	m_deleted = deleted;
}

DataViewMode IdeAccount::getViewMode() const
{
	return m_account->getViewMode();
}

void IdeAccount::setViewMode(DataViewMode mode)
{
	m_account->setViewMode(mode);
}

String IdeAccount::getLanguage() const
{
	return m_account->getLanguage();
}

bool IdeAccount::hasLanguage() const
{
	return m_account->hasLanguage();
}

void IdeAccount::setLanguage(const String &language)
{
	m_account->setLanguage(language);
}

/*
shared_ptr<IdeSkin> Account::getSkin() const
{
	return m_skin;
}
*/

void IdeAccount::setSkin(const SkinID &id)
{
	m_account->setSkinID(id);

	//return _sinkSkin();
}

const String & IdeAccount::getRealPassword() const
{
	return m_account->real_password;
}

bool IdeAccount::setRealPassword(const String &password)
{
	if(password.empty() == false && m_account->validatePassword(password) == false)
		return false;

	m_account->real_password = password;
	return true;
}

const DateTime & IdeAccount::getLastAccessDate() const
{
	return m_account->last_access_date;
}

void IdeAccount::setLastAccessDate(const DateTime &date)
{
	m_account->last_access_date = date;
}

const DateTime & IdeAccount::getReferenceDate() const
{
	return m_account->reference_date;
}

void IdeAccount::setReferenceDate(const DateTime &date)
{
	m_account->reference_date = date;
}

/* // TOCLEAN
bool IdeAccount::updateAccount(shared_ptr<IPortalDatabase> database)
{
	return database->updateRecord(m_account);
}
*/

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

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
