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
#include "ideaccountsmanager.h"

#include "buffer.h"
#include "convert.h"
#include "dataaccount.h"
#include "idbconnection.h"
#include "cryptmanager.h"
#include "dbsqlselect.h"
#include "dbtables.h"
#include "dataitem.h"
#include "engine.h"
#include "ideaccount.h"
#include "lock.h"
#include "options.h"
#include "xmldocument.h"
#include "xmlschema.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////

IdeAccountsManager::IdeAccountsManager()
{
	shared_ptr<IDbConnection> connection = Engine::instance()->createSystemConnection();
	
	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::ACCOUNTS_TABLE));
	select->fields.add(DbSqlField(DBTABLES::ACCOUNTS::ID));

	DataTable result;
	connection->query(select, result);

	for(uint32 r = 0; r < result.rows(); r++)
	{
		_load(connection, result.get(r, DBTABLES::ACCOUNTS::ID));
	}	
}

IdeAccountsManager::~IdeAccountsManager()
{

}

IdeAccountsManager::AccountsList IdeAccountsManager::getAccounts() const
{
	OS_LOCK(m_cs);
	return m_accounts;
}

bool IdeAccountsManager::hasAccounts() const
{
	OS_LOCK(m_cs);
	return m_accounts.empty() == false;
}

bool IdeAccountsManager::hasAccount(const String &id)
{
	return getByID(id) != nullptr;
}

shared_ptr<IdeAccount> IdeAccountsManager::getByID(const String &id)
{
	OS_LOCK(m_cs);

	for(AccountsList::const_iterator i = m_accounts.begin(); i != m_accounts.end(); ++i)
	{
		if((*i)->getID() == id)
			return *i;
	}

	return nullptr;
}

shared_ptr<IdeAccount> IdeAccountsManager::getByName(const String &username)
{
	OS_LOCK(m_cs);

	for(AccountsList::const_iterator i = m_accounts.begin(); i != m_accounts.end(); ++i)
	{
		if((*i)->getAccount()->username == username)
			return *i;
	}

	return nullptr;
}

bool IdeAccountsManager::exists(const String &id) const
{
	OS_LOCK(m_cs);

	for(AccountsList::const_iterator i = m_accounts.begin(); i != m_accounts.end(); ++i)
	{
		if((*i)->getID().compare_no_case(id))
			return true;
	}

	return false;
}

bool IdeAccountsManager::add(shared_ptr<IDbConnection> database, const String &id)
{	
	OS_LOCK(m_cs);

	return _load(database, id);
}

bool IdeAccountsManager::add(shared_ptr<IdeAccount> account)
{
	if(exists(account->getID()))
		return false;

	m_accounts.push_back(account);
	return true;
}

//bool IdeAccountsManager::createAccount(shared_ptr<IPortalDatabase> database, const String &username, const String &password, const String &secret_question, const String &secret_response, bool autoLogin, const Buffer *accountPublicKey, const Buffer *accountPrivateKey)

shared_ptr<IdeAccount> IdeAccountsManager::createAccount(shared_ptr<DataAccount> dataAccount)
{
	// Salva l'account
	shared_ptr<IDbConnection> connection = Engine::instance()->createSystemConnection();
	if(dataAccount->insert(connection) == false)
	{
		OS_ASSERTFALSE();
		return nullptr;
	}

	shared_ptr<IdeAccount> account(OS_NEW IdeAccount(dataAccount));

	add(account);

	return account;
}

shared_ptr<IdeAccount> IdeAccountsManager::createAccount(const String &username, const String &password, bool autoLogin)
{
	OS_LOCK(m_cs);

	/*
	if(validateUsername(username) == false || password.empty())
		return nullptr;
	*/
	if(username.empty())
		return nullptr;
	if(password.empty())
		return nullptr;

	Buffer public_key;
	Buffer private_key;

	if(CryptManager::instance()->rsaGenerateKeys(rsaType2048, private_key, public_key) == false)
	{
		OS_ASSERTFALSE();
		return nullptr;
	}

	// Genera l'id dell'utente associato all'account sulla base della chiave pubblica
	ObjectID user_id = DataAccount::getReferenceUser(public_key);

	shared_ptr<DataAccount> account(OS_NEW DataAccount());
	// Salva il nome dell'utente
	account->username = username;
	// Genera l'id dell'account
	account->id = static_cast<String>(CryptManager::instance()->randomSHA().toHex());
	// Salva la password dell'utente
	account->password = account->encodePassword(password);
	// Inizializza la password reale
	account->real_password = autoLogin ? password : String::EMPTY;
	// Codifica le chiavi dell'utente con la password
	if(account->encodeKeys(password, public_key, private_key) == false)
	{
		OS_ASSERTFALSE();
		return nullptr;
	}

	/*
	// Verifica che sia stata specificata una domanda segreta
	if(secret_question.empty() == false)
	{
		// Valorizza la domanda segreta
		account->secret_question = secret_question;
	}

	// Controlla se  stata specificata una risposta alla domanda segreta
	if(secret_response.empty() == false)
	{
		// Codica la risposta segreta
		if(account->encodePasswordInSecretResponse(password, secret_response) == false)
		{
			OS_ASSERTFALSE();
			return String::EMPTY;
		}
	}
	*/

	account->setViewMode(dataViewModeNormal);
		
	return createAccount(account);
}

bool IdeAccountsManager::remove(const String &id)
{
	shared_ptr<IdeAccount> account = getByID(id);
	if(account == nullptr)
		return false;

	account->setDeleted(true);
	save(account);
		
	m_accounts.remove(account);
	return true;
}

bool IdeAccountsManager::removePhysical(const String &id)
{
	OS_LOCK(m_cs);

	shared_ptr<IDbConnection> connection = Engine::instance()->createSystemConnection();
	connection->execute(connection->sql_remove(DBTABLES::ACCOUNTS_TABLE, DBTABLES::ACCOUNTS::ID, id));
		
	return true;
}

void IdeAccountsManager::save(shared_ptr<IdeAccount> account)
{
	shared_ptr<IDbConnection> connection = Engine::instance()->createSystemConnection();
	account->getAccount()->update(connection);
}

bool IdeAccountsManager::import(const Buffer &buffer)
{
	shared_ptr<XMLSchema> schema(OS_NEW XMLSchema());
	schema->parseFile(utils::makeFilePath(utils::makeFolderPath(Options::instance()->getSharePath(),OS_SCHEMAS_PATH), OS_SCHEMA_ACCOUNT));

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument(schema));
	if(document->parseBuffer(buffer) == false)
		return false;
	
	shared_ptr<DataAccount> dataAccount(OS_NEW DataAccount());
	if(dataAccount->importXML(document) == false)
		return false;
	
	shared_ptr<IdeAccount> account = IdeAccountsManager::instance()->createAccount(dataAccount);
	if(account == nullptr)
		return false;

	return true;	
}

bool IdeAccountsManager::_load(shared_ptr<IDbConnection> database, const String &id)
{
	OS_LOCK(m_cs);

	OS_ASSERT(hasAccount(id) == false);

	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::ACCOUNTS_TABLE));
	select->where.add(DBTABLES::ID, Convert::toSQL(id));
	select->limit.setCount(1);
		
	DataTable result;
	if(database->query(select, result) == false)
		return false;

	if(result.rows() != 1)
		return false;

	shared_ptr<DataAccount> dataAccount(OS_NEW DataAccount());

	dataAccount->read(result[0]);

	shared_ptr<IdeAccount> account(OS_NEW IdeAccount(dataAccount));
	if(account->getDeleted())
	{
		removePhysical(id);
	}
	else
	{
		m_accounts.push_back(account);
	}

	return true;
		
	/*

	// Carica l'account specificato
	shared_ptr<DataAccount> data_account = database->getAccount(id);
	if(data_account == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	shared_ptr<IdeAccount> account(OS_NEW IdeAccount(data_account));
	if(account->getDeleted())
		database->getPortal()->deleteAccount(database, account->getID());
	else
		m_accounts[id] = account;

	return true;
	*/
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
