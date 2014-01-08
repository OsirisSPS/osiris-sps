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

#ifndef _ENGINE_IDEACCOUNTSMANAGER_H
#define _ENGINE_IDEACCOUNTSMANAGER_H

#include "base/object.h"
#include "boost/thread/recursive_mutex.hpp"
#include "datadata.h"
#include "dbdb.h"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;
class IDbConnection;
class IdeAccount;

//////////////////////////////////////////////////////////////////////

class EngineExport IdeAccountsManager : public StaticSingleton<IdeAccountsManager>
{
public:
	typedef list<shared_ptr<IdeAccount> >::type AccountsList;
// Construction
public:
	IdeAccountsManager();
	virtual ~IdeAccountsManager();

// Attributes
public:
	//inline shared_ptr<Portal> getPortal() const;

	AccountsList getAccounts() const;
	bool hasAccounts() const;
	bool hasAccount(const String &id);

	shared_ptr<IdeAccount> getByID(const String &id);
	shared_ptr<IdeAccount> getByName(const String &username);
	
	bool exists(const String &id) const;
		
// Operations
public:
	bool add(shared_ptr<IDbConnection> database, const String &id);
	bool add(shared_ptr<IdeAccount> account);
	shared_ptr<IdeAccount> createAccount(shared_ptr<DataAccount> dataAccount);
	shared_ptr<IdeAccount> createAccount(const String &username, const String &password, bool autoLogin);
	bool remove(const String &id);
	bool removePhysical(const String &id);
	void save(shared_ptr<IdeAccount> account);
	bool importXml(const Buffer &buffer);

private:
	bool _load(shared_ptr<IDbConnection> database, const String &id);

protected:
	mutable boost::recursive_mutex m_cs;	
	AccountsList m_accounts;
};

//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_IDEACCOUNTSMANAGER_H
