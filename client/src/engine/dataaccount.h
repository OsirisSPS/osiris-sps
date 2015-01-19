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

#ifndef _ENGINE_DATACCOUNT_H
#define _ENGINE_DATACCOUNT_H

#include "dbvalue.h"
#include "buffer.h"
#include "datetime.h"
#include "datairecord.h"
#include "ids.h"
#include "xmloptions.h"

//////////////////////////////////////////////////////////////////////

#define OS_ACCOUNT_VERSION()				OS_MAKEVERSION(0, 1)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLDocument;

//////////////////////////////////////////////////////////////////////

class EngineExport DataAccount : public DataIRecord
{
	typedef DataIRecord RecordBase;

public:
	struct EngineExport options
	{
		static const String view_mode;				// Modalit di navigazione dell'account (vista/editazione)
		static const String skin_id;				// Nome della skin utilizzata dall'account (se vuota usa quella del portale)
		static const String language;				// Lingua preferenziale dell'account (se vuota usa quella predefinita del portale)
		static const String time_offset;
		static const String time_dst;
	};

// Construction
public:
	DataAccount();
	virtual ~DataAccount();

// Attributes
public:
	inline bool hasAutologin() const;

	inline SkinID getSkinID() const;
	inline bool hasSkinID() const;
	inline void setSkinID(const SkinID &skinID);
	
	inline String getLanguage() const;
	inline bool hasLanguage() const;
	inline void setLanguage(const String &language);

	inline int32 getTimeOffset() const;
	inline void setTimeOffset(int32 value);

	inline int32 getTimeDST() const;
	inline void setTimeDST(int32 value);

	inline DataViewMode getViewMode() const;
	inline void setViewMode(DataViewMode mode);

	// Se l'account ha abilitato l'accesso automatico, restituisce l'id dell'utente di riferimento
	bool getUserID(ObjectID &id) const;

	// Restituisce l'id dell'utente di riferimento di un account sulla base della sua chiave privata (in chiaro)
	static ObjectID getReferenceUser(const Buffer &public_key);

	// Restituisce un'opzione a livello di account
	DataItem getOption(const String &name) const;
	// Setta un'opzione a livello di account
	void setOption(const String &name, const DataItem &value);

// Operations
public:
	// Cripta le chiavi dell'utente con la password specificata
	bool encodeKeys(const String &password, const Buffer &public_key, const Buffer &private_key);
	// Decripta la chiave pubblica dell'utente
	bool decodePublicKey(const String &password, Buffer &public_key) const;
	// Decripta la chiave privata dell'utente
	bool decodePrivateKey(const String &password, Buffer &private_key) const;

	// Cripta con la risposta segreta la password dell'utente
	bool encodePasswordInSecretResponse(const String &password, const String &secret_response);
	// Decripta la password dell'utente utilizzando la risposta segreta
	bool decodePasswordFromSecretResponse(const String &secret_response, String &password);

	String encodePassword(const String &password) const;
	String encodePasswordOld(const String &password) const;
	// Verifica che la password specificata (in chiaro) corrisponda a quella dell'account
	bool validatePassword(const String &password) const;

	// Esporta l'account in xml
	shared_ptr<XMLDocument> exportXML() const;
	// Importa un account da xml
	bool importXML(shared_ptr<XMLDocument> document);

// IRecord overrides
public:
	virtual bool validate(shared_ptr<IPortalDatabase> database) const;
	virtual String getTableName() const;
	virtual String getRecordKeyName() const;
	virtual String getRecordKeyValue() const; // String only expected...

// IRecord interface
protected:
	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:
	DbValue<String> id;						// ID dell'account (viene anche riutilizzato come codice di sicurezza per il calcolo dell'hash della password)
	DbValue<String> username;					// Nome dell'account
	DbValue<String> description;					// Description
	DbValue<String> password;					// SHA della password dell'account
	DbValue<String> real_password;			// Password (in chiaro) se l'account ha abilitato l'accesso automatico
	DbValue<Buffer> public_key;				// Chiave pubblica dell'utente (criptata con la password utente)
	DbValue<Buffer> private_key;				// Chiave privata dell'utente (criptata con la password utente)
	DbValue<String> secret_question;			// Domanda segreta nel caso di smarrimento della password (in chiaro)
	DbValue<Buffer> secret_response;			// Password dell'account criptata con la risposta alla domanda segreta
	DbValue<DateTime> last_access_date;		// Data di ultimo accesso dell'account
	DbValue<DateTime> reference_date;			// Data di riferimento dell'account
	//DbValue<Buffer> identity;					// ID dell'utente di riferimento (criptato con la password master)

private:
	XMLOptions m_options;						// Opzioni dell'account
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<DataAccount> account_cast(shared_ptr<DataIRecord> record) { return boost::dynamic_pointer_cast<DataAccount>(record); }

//////////////////////////////////////////////////////////////////////

inline bool DataAccount::hasAutologin() const { return real_password->empty() == false; }

inline SkinID DataAccount::getSkinID() const { return getOption(options::skin_id).getStringAscii(); }
inline bool DataAccount::hasSkinID() const { return getSkinID().empty() == false; }
inline void DataAccount::setSkinID(const SkinID &skinID) { setOption(options::skin_id, skinID.getString()); }

inline String DataAccount::getLanguage() const { return getOption(options::language); }
inline bool DataAccount::hasLanguage() const { return getLanguage().empty() == false; }
inline void DataAccount::setLanguage(const String &language) { setOption(options::language, language); }

inline int32 DataAccount::getTimeOffset() const { return getOption(options::time_offset); }
inline void DataAccount::setTimeOffset(int32 value) { setOption(options::time_offset, value); }

inline int32 DataAccount::getTimeDST() const { return getOption(options::time_dst); }
inline void DataAccount::setTimeDST(int32 value) { setOption(options::time_dst, value); }

inline DataViewMode DataAccount::getViewMode() const { return static_cast<DataViewMode>(static_cast<uint32>(getOption(options::view_mode))); }
inline void DataAccount::setViewMode(DataViewMode mode) { setOption(options::view_mode, static_cast<uint32>(mode)); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_DATACCOUNT_H
