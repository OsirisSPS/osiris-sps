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
#include "idesession.h"

#include "compatibilitymanager.h"
#include "dataaccount.h"
#include "objectsuser.h"
#include "iportaldatabase.h"
#include "engine.h"
#include "entitiesentitiescache.h"
#include "entitiessnapshotmanager.h"
#include "entitiesstatisticscache.h"
#include "ideaccount.h"
#include "ideaccountsmanager.h"
#include "idesessionportal.h"
#include "ideskin.h"
#include "idesystem.h"
#include "iidepage.h"
#include "iideportalpage.h"
#include "isisendpoint.h"
#include "isissystem.h"
#include "languagemanager.h"
#include "lock.h"
#include "log.h"
#include "options.h"
#include "osiriscommon.h"
#include "platformmanager.h"
#include "portalsaccount.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalsportalssystem.h"
#include "portalsprofilescache.h"
#include "reputationsmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdeSession::IdeSession(const String &session)
{
	OS_ASSERT(session.empty() == false);
	m_session = session;

	//m_guestMode = false;
}

IdeSession::~IdeSession()
{

}

shared_ptr<IdeSessionPortal> IdeSession::getSessionPortal(shared_ptr<Portal> portal)
{
	String k = portal->getFullPovID();
	IdeSessionPortalMap::const_iterator i = m_sessionsPortal.find(k);
	if(i == m_sessionsPortal.end())
	{
		shared_ptr<IdeSessionPortal> sessionPortal(OS_NEW IdeSessionPortal());
		m_sessionsPortal[k] = sessionPortal;
		return sessionPortal;
	}
	else
		return i->second;
}

/*
bool IdeSession::getGuestMode() const
{
	return m_guestMode;
}

void IdeSession::setGuestMode(bool guestMode)
{
	m_guestMode = guestMode;
}

bool IdeSession::getReadOnlyMode() const
{
	return ( (m_guestMode) || (m_privateKey.empty()) );
}
*/

const Buffer & IdeSession::getPublicKey() const
{
	return m_publicKey;
	/*
	OS_LOCK(m_cs);

	if(m_user != null)
		return m_user->public_key;

	return Buffer::EMPTY;
	*/
}

const Buffer & IdeSession::getPrivateKey() const
{
	return m_privateKey;
}

const DateTime & IdeSession::getReferenceDate() const
{
	OS_LOCK(m_cs);

	if(m_account != null)
		return m_account->getReferenceDate();

	return DateTime::EMPTY;
}


shared_ptr<IdeSkin> IdeSession::getCurrentSkin() const
{
	shared_ptr<IdeSkin> skin;

	if(m_account != null)
	{
		SkinID skinID = m_account->getSkinID();
		if(skinID.empty() == false)
			skin = IdeSystem::instance()->getSkin(skinID, true);
	}


	// Se l'account non ha una skin specifica usa quella del portale
	if(skin == null || skin->isValid() == false)
		skin = IdeSystem::instance()->getDefaultSkin();

	return skin;
}

String IdeSession::getLanguage() const
{
	shared_ptr<IdeAccount> account = getAccount();
	if(account != null && account->hasLanguage())
		return account->getLanguage();

	return Options::instance()->getDefaultLanguage();
}

/*
bool LoggedUser::loginGuest(shared_ptr<const IPortalPage> page, shared_ptr<IPortalDatabase> database)
{
	logout(database);

	OS_LOCK(m_cs);

	bool result = true;
	
	setGuestMode(result);

	return result;
}
*/


ObjectID IdeSession::getUserID() const
{
	OS_LOCK(m_cs);

	if(m_account == null)
		return ObjectID::EMPTY;
	else
		return DataAccount::getReferenceUser(m_publicKey); // TOOPTIMIZE: We can save that, to avoid SHA every time in the page?	
}

bool IdeSession::isPortalGuest(shared_ptr<IPortalDatabase> database)
{
	if(getUser(database) == null)
		return true;

	return getSessionPortal(database->getPortal())->getGuest();
}

shared_ptr<ObjectsUser> IdeSession::getUser(shared_ptr<IPortalDatabase> database) const
{
	if(m_account == null)
		return null;

	ObjectID userID = getUserID();
	shared_ptr<ObjectsUser> user = objects_user_cast(database->getPortal()->getObject(database, userID));
	
	return user;
}

bool IdeSession::hasUserX(shared_ptr<IPortalDatabase> database) const
{
	return (getUser(database) != null);
}

LanguageResult IdeSession::portalLogin(shared_ptr<IPortalDatabase> database)
{
	if(getUser(database) == null)
	{
		DateTime now = IsisSystem::instance()->getInternetDateTime();
		if(now.isNull())
			return LanguageResult("no_clock_sync");

		shared_ptr<ObjectsUser> user(OS_NEW ObjectsUser());
		user->id = getUserID();
		user->public_key = m_publicKey;
		user->name = m_account->getAccount()->username;
		user->description = String::EMPTY;	
		user->join_date = now;
		user->birth_date = DateTime::EMPTY;
		user->misc = String::EMPTY;
		user->mark = String::EMPTY;
		//user->show_mark = true;
		//user->show_avatar = true;

		LanguageResult result = user->create(database, m_privateKey);

		if(result.empty() == false)
			return result;
	}

	getSessionPortal(database->getPortal())->setGuest(false);

	CompatibilityManager::instance()->resigner(database, get_this_ptr());

	return LanguageResult();
}

void IdeSession::portalLogout(shared_ptr<Portal> portal)
{
	getSessionPortal(portal)->setGuest(true);
}


bool IdeSession::loginAutomatic(const String &id)
{
	if(id.empty())
		return false;

	shared_ptr<IdeAccount> account = IdeAccountsManager::instance()->getByID(id);
	// Verifica che l'account sia valido e abbia l'autologin
	if(account != null && account->getAccount()->hasAutologin())
	{
		return login(account->getID(), account->getRealPassword());
	}

	return false;
}

bool IdeSession::login(const String &accountID, const String &password, bool save_password)
{
	logout();

	OS_LOCK(m_cs);

	shared_ptr<IdeAccount> account = IdeAccountsManager::instance()->getByID(accountID);
	if(account == null)
		return false;
		
	// Verifica che la password specificata coincida
	if(account->getAccount()->validatePassword(password) == false)
		return false;

	// Decodifica la chiave privata dell'utente
	if(account->getAccount()->decodePrivateKey(password, m_privateKey) == false)
		return false;

	// Decodifica la chiave pubblica dell'utente
	if(account->getAccount()->decodePublicKey(password, m_publicKey) == false)
		return false;

	/*
	// Carica l'utente di riferimento
	m_user = _loadUser(database, account, m_publicKey, m_privateKey);
	if(m_user == null)
		return false;
	*/

	m_account = account;

	// Aggiorna la data di ultimo accesso
	m_account->setLastAccessDate(DateTime::now());

	// Controlla se bisogna salvare la password
	if(save_password)
		m_account->setRealPassword(password);

	// Aggiorna l'account
	//m_account->updateAccount(database);
	IdeAccountsManager::instance()->save(m_account);

	return true;
}

void IdeSession::logout(bool reset_password)
{
	OS_LOCK(m_cs);

	if(m_account != null)
	{
		if(reset_password && m_account->getAccount()->hasAutologin())
		{
			// Resetta l'accesso automatico
			m_account->setRealPassword(String::EMPTY);
			// Aggiorna l'account
			//m_account->updateAccount(database);
			IdeAccountsManager::instance()->save(m_account);
		}
	}

	//m_user.reset();
	m_publicKey.clear();
	m_privateKey.clear();
	m_account.reset();
	//m_guestMode = false;
}

void IdeSession::updateReferenceDate()
{
	OS_LOCK(m_cs);

	if(m_account != null)
	{
		m_account->setReferenceDate(DateTime::now());
		//m_account->updateAccount(database);
		IdeAccountsManager::instance()->save(m_account);
	}
}

/*
shared_ptr<ObjectsUser> IdeSession::_loadUser(shared_ptr<IPortalDatabase> database, shared_ptr<IdeAccount> account, const Buffer &public_key, const Buffer &private_key)
{
	OS_ASSERT(account != null);

	ObjectID userID = data::Account::getReferenceUser(public_key);
	shared_ptr<ObjectsUser> userExists = objects_user_cast(getPortal()->getObject(database, userID.toUTF16()));
	if(userExists != null)
		return userExists;

	// User creation
	//if(database->__createUser(account->getAccount()->username, public_key, private_key, userID) == false)
	//	return null;

	DateTime now;
	if(Engine::instance()->getSynchronizedDateTime(now) == false)
		return null;

	shared_ptr<ObjectsUser> user(OS_NEW ObjectsUser());
	user->id = userID;
	user->public_key = public_key;
	user->name = account->getAccount()->username;
	user->description = String::EMPTY;	
	user->join_date = now;
	user->birth_date = DateTime::EMPTY;
	user->misc = String::EMPTY;
	user->mark = String::EMPTY;
	user->show_mark = true;
	user->show_avatar = true;

	LanguageResult result = user->create(database, private_key);
	if(result.empty() == false)
		return null;

	OS_ASSERT(objects_user_cast(getPortal()->getObject(database, userID.toUTF16())) != null);
	return objects_user_cast(getPortal()->getObject(database, userID.toUTF16()));
}
*/

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
