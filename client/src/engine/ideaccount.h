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

#ifndef _ENGINE_IDEACCOUNT_H
#define _ENGINE_IDEACCOUNT_H

#include "base/object.h"
#include "boost/thread/recursive_mutex.hpp"
#include "datadata.h"
#include "dbdb.h"
#include "entitiesentities.h"
#include "ideide.h"
#include "ids.h"
#include "objectsobjects.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DateTime;
class IdeSkin;
class IPortalDatabase;
class Portal;

//////////////////////////////////////////////////////////////////////

class EngineExport IdeAccount : public Object
{
// Construction
public:
	IdeAccount(shared_ptr<DataAccount> account);
	virtual ~IdeAccount();

// Attributes
public:
	//inline shared_ptr<Portal> getPortal() const;
	inline shared_ptr<DataAccount> getAccount() const;
		
	const String & getID() const;
	const String & getName() const;
	void setName(const String& name);

	SkinID getSkinID() const;
	
	String getLanguage() const;
	void setLanguage(const String &language);
	bool hasLanguage() const;

	// Indica se nel portale c' almeno un utente loggato con questo account
	//bool isLogged() const;

// Operations
public:
	// Se associabile (nel caso di accesso automatico) carica l'utente di riferimento
	//shared_ptr<ObjectsUser> loadUser(shared_ptr<IPortalDatabase> database); // TOCLEANPY

	bool getDeleted() const;
	void setDeleted(bool deleted);

	DataViewMode getViewMode() const;
	void setViewMode(DataViewMode mode);

	const String & getRealPassword() const;
	bool setRealPassword(const String &password);

	//shared_ptr<IdeSkin> getSkin() const;
	void setSkin(const SkinID &id);

	const DateTime & getLastAccessDate() const;
	void setLastAccessDate(const DateTime &date);

	const DateTime & getReferenceDate() const;
	void setReferenceDate(const DateTime &date);

	//bool updateAccount(shared_ptr<IPortalDatabase> database); // TOCLEANPY

private:
	//bool _sinkSkin();

protected:
	mutable boost::recursive_mutex m_cs;
	//weak_ptr<Portal> m_portal;
	shared_ptr<DataAccount> m_account;
	//shared_ptr<IdeSkin> m_skin;	
	bool m_deleted;
};

//////////////////////////////////////////////////////////////////////

//inline shared_ptr<Portal> Account::getPortal() const { return m_portal.lock(); }
inline shared_ptr<DataAccount> IdeAccount::getAccount() const { return m_account; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_IDEACCOUNT_H
