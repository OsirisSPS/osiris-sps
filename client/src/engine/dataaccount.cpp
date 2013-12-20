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
#include "dataaccount.h"

#include "cryptmanager.h"
#include "cryptkey.h"
#include "dbbinder.h"
#include "dbtables.h"
#include "iportaldatabase.h"
#include "osiriscommon.h"
#include "version.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////
 
OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
static void exportNodeValue(shared_ptr<XMLNode> node, const String &name, const DbValue<T> &value)
{
	OS_ASSERT(node != nullptr);

	shared_ptr<XMLNode> child = node->addChild(name);
	child->setData(value.toXML());
}

template <typename T>
static void importNodeValue(shared_ptr<XMLNode> node, const String &name, DbValue<T> &value)
{
	OS_ASSERT(node != nullptr);

	shared_ptr<XMLNode> child = node->getNode(name);
	if(child != nullptr)	// Il nodo potrebbe essere opzionale
		value.fromXML(child->getData());
}

//////////////////////////////////////////////////////////////////////

const String DataAccount::options::view_mode = _S("ide.view_mode");
const String DataAccount::options::skin_id = _S("ide.skin.id");
const String DataAccount::options::language = _S("ide.language");
const String DataAccount::options::time_offset = _S("ide.time_offset");
const String DataAccount::options::time_dst = _S("ide.time_dst");

//////////////////////////////////////////////////////////////////////

DataAccount::DataAccount() : RecordBase(dataRecordTypeAccount)
{
	m_options.ensureOption(options::view_mode, static_cast<uint32>(dataViewModeNormal), false);
	m_options.ensureOption(options::skin_id, String::EMPTY, false);
	m_options.ensureOption(options::language, String::EMPTY, false);
	m_options.ensureOption(options::time_offset, OS_TIME_DETECTION_SYSTEM, false);
	m_options.ensureOption(options::time_dst, OS_TIME_DETECTION_SYSTEM, false);
}

DataAccount::~DataAccount()
{

}

DataItem DataAccount::getOption(const String &name) const
{
	return m_options.getOption(name);
}

void DataAccount::setOption(const String &name, const DataItem &value)
{
	m_options.setOption(name, value);
}

bool DataAccount::getUserID(ObjectID &id) const
{
	// Verifica che l'account abbia l'accesso automatico attivo
	if(hasAutologin() == false)
		return false;

	// Decodifica la chiave pubblica
	Buffer public_key;
	if(decodePublicKey(real_password, public_key) == false)
		return false;

	// Calcola l'id dell'utente di riferimento
	id = getReferenceUser(public_key);
	return true;
}

bool DataAccount::encodeKeys(const String &password, const Buffer &public_key, const Buffer &private_key)
{
    CryptKey passkey(password);

	Buffer encodedPublicKey;
	// Salva la chiave pubblica dell'utente (criptandola con la password specificata)
	if(CryptManager::instance()->aesEncrypt(passkey, public_key.getData(), public_key.getSize(), encodedPublicKey) == false)
		return false;
	this->public_key = encodedPublicKey;

	Buffer encodedPrivateKey;
	// Salva la chiave privata dell'utente (criptandola con la password specificata)
	if(CryptManager::instance()->aesEncrypt(passkey, private_key.getData(), private_key.getSize(), encodedPrivateKey) == false)
		return false;
	this->private_key = encodedPrivateKey;

	return true;
}

bool DataAccount::decodePublicKey(const String &password, Buffer &public_key) const
{
    CryptKey passkey(password);
	return CryptManager::instance()->aesDecrypt(passkey, this->public_key->getData(), this->public_key->getSize(), public_key);
}

bool DataAccount::decodePrivateKey(const String &password, Buffer &private_key) const
{
    CryptKey passkey(password);
	return CryptManager::instance()->aesDecrypt(passkey, this->private_key->getData(), this->private_key->getSize(), private_key);
}

bool DataAccount::encodePasswordInSecretResponse(const String &password, const String &secret_response)
{
    CryptKey responsekey(secret_response);

	Buffer encodedSecretResponse;
	if(CryptManager::instance()->aesEncrypt(responsekey, password.buffer(), password.buffer_size(), encodedSecretResponse) == false)
		return false;

	this->secret_response = encodedSecretResponse;
	return true;
}

bool DataAccount::decodePasswordFromSecretResponse(const String &secret_response, String &password)
{
    CryptKey responsekey(secret_response);

	Buffer decrypted;
	if(CryptManager::instance()->aesDecrypt(responsekey, this->secret_response->getData(), this->secret_response->getSize(), decrypted) == false)
		return false;

	//password = reinterpret_cast<uchar *>(decrypted.getData());

	// URGENT: da verificare
	if(decrypted.empty())
		password.clear();
	else
		password.assign(reinterpret_cast<uchar *>(decrypted.getData()), decrypted.getSize() / sizeof(uchar));

	return true;
}

String DataAccount::encodePassword(const String &password) const
{
	OS_EXCEPT_IF(id->empty(), "Invalid account id");

	CryptKey key;
	return key.generateKey(password, id).toHex();		// Deriva una chiave con l'id dell'account per evitare l'attacco del compleanno (http://en.wikipedia.org/wiki/Birthday_attack)
}

bool DataAccount::validatePassword(const String &password) const
{
	// La password  valida se la versione codificata coincide con quella salvata
	return encodePassword(password) == this->password;
}

String DataAccount::getTableName() const
{
	return DBTABLES::ACCOUNTS_TABLE;
}

String DataAccount::getRecordKeyName() const
{
	return DBTABLES::ACCOUNTS::ID;
}

String DataAccount::getRecordKeyValue() const
{
	return id;
}

shared_ptr<XMLDocument> DataAccount::exportXML() const
{
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	shared_ptr<XMLNode> root = document->create(_S("account"));
	root->setAttributeString(_S("version"), Version(OS_ACCOUNT_VERSION()).toString());

	exportNodeValue(root, DBTABLES::ACCOUNTS::ID, id);
	exportNodeValue(root, DBTABLES::ACCOUNTS::USERNAME, username);
	exportNodeValue(root, DBTABLES::ACCOUNTS::PASSWORD, password);
	exportNodeValue(root, DBTABLES::ACCOUNTS::REAL_PASSWORD, real_password);
	exportNodeValue(root, DBTABLES::ACCOUNTS::PUBLIC_KEY, public_key);
	exportNodeValue(root, DBTABLES::ACCOUNTS::PRIVATE_KEY, private_key);
	exportNodeValue(root, DBTABLES::ACCOUNTS::SECRET_QUESTION, secret_question);
	exportNodeValue(root, DBTABLES::ACCOUNTS::SECRET_RESPONSE, secret_response);

	/*
	// Decodifica la chiave pubblica
	Buffer public_key_clean;
	if(decodePublicKey(real_password, public_key_clean) == false)
		return nullptr;
	exportNodeValue(root, DBTABLES::ACCOUNTS::PUBLIC_KEY + _S("_clean"), DbValue<Buffer>(public_key_clean));
	public_key_clean.save(_S("d:\\temp\\osiris\\os_pub.ber"));

	// Decodifica la chiave privata
	Buffer private_key_clean;
	if(decodePrivateKey(real_password, private_key_clean) == false)
		return nullptr;
	exportNodeValue(root, DBTABLES::ACCOUNTS::PRIVATE_KEY + _S("_clean"), DbValue<Buffer>(private_key_clean));
	private_key_clean.save(_S("d:\\temp\\osiris\\os_priv.ber"));
	*/

	// TODO: bisognerebbe esportare l'id del portale di riferimento in modo tale da importare
	// queste date solo se si importa l'account nello stesso portale. Per il momento non vengono
	// esportate/importate per evitare il problema
	/*
	exportNodeValue(root, DBTABLES::ACCOUNTS::LAST_ACCESS_DATE, last_access_date);
	exportNodeValue(root, DBTABLES::ACCOUNTS::REFERENCE_DATE, reference_date);
	*/

	return document;
}

bool DataAccount::importXML(shared_ptr<XMLDocument> document)
{
	OS_ASSERT(document != nullptr);
	shared_ptr<XMLNode> root = document->getRoot();

	importNodeValue(root, DBTABLES::ACCOUNTS::ID, id);
	importNodeValue(root, DBTABLES::ACCOUNTS::USERNAME, username);

	if(root->getNode(DBTABLES::ACCOUNTS::PASSWORD) == nullptr)
	{
		// First format: don't have the password, and public/private key are not encrypted.
		// Used by external generator.		

		importNodeValue(root, DBTABLES::ACCOUNTS::PUBLIC_KEY, public_key);
		importNodeValue(root, DBTABLES::ACCOUNTS::PRIVATE_KEY, private_key);
		
		encodeKeys(String::EMPTY, public_key, private_key);
	}
	else
	{
		importNodeValue(root, DBTABLES::ACCOUNTS::PASSWORD, password);
		importNodeValue(root, DBTABLES::ACCOUNTS::REAL_PASSWORD, real_password);

		importNodeValue(root, DBTABLES::ACCOUNTS::PUBLIC_KEY, public_key);
		importNodeValue(root, DBTABLES::ACCOUNTS::PRIVATE_KEY, private_key);
	}
	
	
	importNodeValue(root, DBTABLES::ACCOUNTS::SECRET_QUESTION, secret_question);
	importNodeValue(root, DBTABLES::ACCOUNTS::SECRET_RESPONSE, secret_response);

	

	// TODO: vedi note Account::exportXML
	/*
	importNodeValue(root, DBTABLES::ACCOUNTS::LAST_ACCESS_DATE, last_access_date);
	importNodeValue(root, DBTABLES::ACCOUNTS::REFERENCE_DATE, reference_date);
	*/

	return true;
}

ObjectID DataAccount::getReferenceUser(const Buffer &public_key)
{
	// N.B.: l'id di un utente  dato dall'hash della sua chiave pubblica (in chiaro)
	// per permettere a chiunque di poter verificare l'autenticit dell'hash dell'id di un utente, cio
	// evitare che qualcuno possa cancellare un utente gi esistente e ricrearne un altro specificando
	// a mano lo stesso hash (con chiave diverse)

#ifdef OS_NOOBJECTID
	return CryptManager::instance()->SHA(public_key.getData(), public_key.getSize()).toHex();
#else
	return IPortalDatabase::generateID(portalObjectTypeUser, true, CryptManager::instance()->SHA(public_key.getData(), public_key.getSize()).toHex());
#endif
}

bool DataAccount::validate(shared_ptr<IPortalDatabase> database) const
{
	if(RecordBase::validate(database) == false)
		return false;

	if(real_password->empty() == false)
	{
		// Se l'account ha attivato l'accesso automatico la password deve corrispondere
		if(validatePassword(real_password) == false)
			return false;
	}

	switch(getViewMode())
	{
	case dataViewModeNormal:
	case dataViewModeEditor:
						break;

	default:			return false;
	}

	return true;
}

bool DataAccount::onRead(const DataTableRow &row)
{
	id.read(row, DBTABLES::ACCOUNTS::ID);
	username.read(row, DBTABLES::ACCOUNTS::USERNAME);
	password.read(row, DBTABLES::ACCOUNTS::PASSWORD);
	real_password.read(row, DBTABLES::ACCOUNTS::REAL_PASSWORD);
	description.read(row, DBTABLES::ACCOUNTS::DESCRIPTION);
	public_key.read(row, DBTABLES::ACCOUNTS::PUBLIC_KEY);
	private_key.read(row, DBTABLES::ACCOUNTS::PRIVATE_KEY);
	secret_question.read(row, DBTABLES::ACCOUNTS::SECRET_QUESTION);
	secret_response.read(row, DBTABLES::ACCOUNTS::SECRET_RESPONSE);
	last_access_date.read(row, DBTABLES::ACCOUNTS::LAST_ACCESS_DATE);
	reference_date.read(row, DBTABLES::ACCOUNTS::REFERENCE_DATE);
	//identity.read(row, DBTABLES::ACCOUNTS::IDENTITY);
	m_options.read(row, DBTABLES::ACCOUNTS::OPTIONS);

	return true;
}

bool DataAccount::onWrite(DbSqlValues &values) const
{
	id.write(values, DBTABLES::ACCOUNTS::ID);
	username.write(values, DBTABLES::ACCOUNTS::USERNAME);
	password.write(values, DBTABLES::ACCOUNTS::PASSWORD);
	real_password.write(values, DBTABLES::ACCOUNTS::REAL_PASSWORD);
	description.write(values, DBTABLES::ACCOUNTS::DESCRIPTION);
	public_key.write(values, DBTABLES::ACCOUNTS::PUBLIC_KEY);
	private_key.write(values, DBTABLES::ACCOUNTS::PRIVATE_KEY);
	secret_question.write(values, DBTABLES::ACCOUNTS::SECRET_QUESTION);
	secret_response.write(values, DBTABLES::ACCOUNTS::SECRET_RESPONSE);
	last_access_date.write(values, DBTABLES::ACCOUNTS::LAST_ACCESS_DATE);
	reference_date.write(values, DBTABLES::ACCOUNTS::REFERENCE_DATE);
	//identity.write(values, DBTABLES::ACCOUNTS::IDENTITY);
	m_options.write(values, DBTABLES::ACCOUNTS::OPTIONS);

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
