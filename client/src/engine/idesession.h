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

#ifndef _ENGINE_IDESESSION_H
#define _ENGINE_IDESESSION_H

#include "boost/thread/recursive_mutex.hpp"
#include "datadata.h"
#include "datetime.h"
#include "enable_this_ptr.h"
#include "languageresult.h"
#include "objectsobjects.h"
#include "dbdb.h"
#include "ids.h"
#include "entitiesentities.h"
#include "buffer.h"
#include "ideide.h"
#include "base/object.h"
#include "portalsportals.h"
#include "range.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;
class IPortalPage;
class ObjectID;
class Portal;
class IdeAccount;
class IdeSessionPortal;
class IdeSkin;

//////////////////////////////////////////////////////////////////////

class EngineExport IdeSession : public Object,
								public enable_this_ptr<IdeSession>
{
// Typedef
public:
	typedef map<String, shared_ptr<IdeSessionPortal> >::type IdeSessionPortalMap;

// Construction
public:
	IdeSession(const String &session);
	virtual ~IdeSession();

// Attributes
public:	
	// Restituisce l'id della sessione associata
	inline const String & getSessionID() const;

	shared_ptr<IdeSessionPortal> getSessionPortal(shared_ptr<Portal> portal);

	/*
	// Restituisce se  l'account guest
	bool getGuestMode() const;
	// Imposta l'anonimicit dell'accesso
	void setGuestMode(bool guestMode);
	// Restituisce se è in sola lettura
	bool getReadOnlyMode() const;
	*/

	// Restituisce l'account di riferimento
	inline shared_ptr<IdeAccount> getAccount() const;

	/*
	// Restituisce l'utente di riferimento
	inline shared_ptr<ObjectsUser> getUser() const;
	*/
	
	// Restituisce se il LoggedUser  valido, cio gi agganciato ad un account o ad un anonimo.
	inline bool isLogged() const;

	// Restituisce la chiave pubblic (in chiaro) dell'utente di riferimento
	const Buffer & getPublicKey() const;
	// Restituisce la chiave privata (in chiaro) dell'utente di riferimento
	const Buffer & getPrivateKey() const;

	// Restituisce la data di attivit
	const DateTime & getReferenceDate() const;

	/*
	// Restituisce l'hash dell'utente di riferimento
	const ObjectID & getUserID() const;
	*/
		
	// Restituisce la skin corrente
	shared_ptr<IdeSkin> getCurrentSkin() const;

	// Restituisce la lingua
	String getLanguage() const;
		
// Operations
public:

	ObjectID getUserID() const;

	bool isPortalGuest(shared_ptr<IPortalDatabase> database);	
	shared_ptr<ObjectsUser> getUser(shared_ptr<IPortalDatabase> database) const;
	bool hasUserX(shared_ptr<IPortalDatabase> database) const;
	//LanguageResult createUser(shared_ptr<IPortalDatabase> database) const;
	LanguageResult portalLogin(shared_ptr<IPortalDatabase> database);
	void portalLogout(shared_ptr<Portal> portal);

	//bool loginGuest(shared_ptr<const IPortalPage> page, shared_ptr<IPortalDatabase> database);
	bool loginAutomatic(const String &id);
	bool login(const String &accountID, const String &password, bool save_password = false);
	void logout(bool reset_password = false);	

	void updateReferenceDate();

private:
	//Razor bool ensureInviteReputation(shared_ptr<IPortalDatabase> database, const ObjectID &userID);

	//shared_ptr<ObjectsUser> _loadUser(shared_ptr<IPortalDatabase> database, shared_ptr<IdeAccount> account, const Buffer &public_key, const Buffer &private_key);

protected:
	//weak_ptr<Portal> m_portal;
	String m_session;
	IdeSessionPortalMap m_sessionsPortal;

	//bool m_guestMode;
	//bool m_readMode;
	Buffer m_publicKey;
	Buffer m_privateKey;
	shared_ptr<IdeAccount> m_account;
	//entities::snapshot_profile_ptr m_snapshotProfile;
	//shared_ptr<ObjectsUser> m_user;
	//mutable shared_ptr<IPortalDatabase> m_database;
	mutable boost::recursive_mutex m_cs;	
};

//////////////////////////////////////////////////////////////////////

inline const String & IdeSession::getSessionID() const { return m_session; }

inline shared_ptr<IdeAccount> IdeSession::getAccount() const { return m_account; }
//inline shared_ptr<ObjectsUser> IdeSession::getUser() const { return m_user; }

inline bool IdeSession::isLogged() const { return (m_account != null); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_IDESESSION_H
