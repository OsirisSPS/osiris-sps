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

#ifndef _PORTALS_PORTALOPTIONS_H
#define _PORTALS_PORTALOPTIONS_H

#include "base/object.h"
#include "boost/thread/recursive_mutex.hpp"
#include "collections.h"
#include "dbdb.h"
#include "locked.h"
#include "portalsportals.h"
#include "version.h"
#include "xmloptions.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ConnectionsManager;
class DateTime;
class IDbOptions;
class IsisEndpoint;
class XMLDocument;
class XMLNode;

//////////////////////////////////////////////////////////////////////

class EngineExport PortalOptions : public Object
{
public:
	typedef unordered_map<uint32, shared_ptr<IsisEndpoint> >::type IsisEndpoints;

// Construction
public:
	//PortalOptions(const PortalID &id = PortalID::EMPTY);
	PortalOptions();
	virtual ~PortalOptions();

// Attributes
public:

	template <typename T>
	T getOption(const String &name) const;
	DataItem getOptionValue(const String &name) const;
	void setOptionValue(const String &name, const DataItem &value);

	inline const PortalID & getPortalID() const;
	inline void setPortalID(const PortalID &portalID);

	inline const PovID & getPovID() const;
	inline void setPovID(const PovID &povID);

	inline String getPassword() const;
	inline void setPassword(const String &password);

	inline int getOptimizeThreshold() const;
	inline void setOptimizeThreshold(const int &threshold);

	inline shared_ptr<IDbOptions> getDatabaseOptions() const;
	inline void setDatabaseOptions(shared_ptr<IDbOptions> databaseOptions);

	inline bool getExchangeEnabled() const;
	inline void setExchangeEnabled(bool exchangeEnabled);

	bool getPublicEnabled() const;
	void setPublicEnabled(bool publicEnabled);

	bool getLoginEnabled() const;
	void setLoginEnabled(bool loginEnabled);

	bool getRegisterEnabled() const;
	void setRegisterEnabled(bool registerEnabled);
	
	//inline String getGuestAccount() const;
	//inline void setGuestAccount(const String &id);

	String getName() const;
	void setName(const String &name);

	//String getDescription() const;
	//void setDescription(const String &description);

	bool getDeleted() const;
	void setDeleted(bool deleted);

	DateTime getLastSyncDate() const;
	void setLastSyncDate(const DateTime &lastSyncDate);

	//DateTime getLastCheckingDate() const;
	//void setLastCheckingDate(const DateTime &lastCheckingDate);

	DateTime getLastStabilizationDate() const;
	void setLastStabilizationDate(const DateTime &lastStabilizationDate);

	UniqueID getAlignHash() const;
	void setAlignHash(const UniqueID &alignHash);

	void resetAlignmentHash();
	void updateAlignmentHash(const ObjectID &id, const DateTime &submitDate, bool add);

	ObjectID getLastExchangedObject() const;
    void setLastExchangedObject(const ObjectID &lastExchangedObject);

    //ObjectID getUsersReference() const;
    //void setUsersReference(const ObjectID &reference);

	DateTime getLastOptimizationDate() const;
	void setLastOptimizationDate(const DateTime &lastOptimizationDate);

	DateTime getLastObjectDate() const;
	DateTime getLastExchangeDate() const;
	DateTime getLastDownloadedObjectDate() const;
	DateTime getLastUploadedObjectDate() const;

	String getDatabaseDriver() const;
	void setDatabaseDriver(const String &databaseDriver);

	String getDatabaseName() const;
	void setDatabaseName(const String &databaseName);

	Version getDatabaseVersion() const;
	void setDatabaseVersion(const Version &version);

	shared_ptr<IsisEndpoint> getIsisEndpoint(uint32 id) const;
	Locked<const PortalOptions::IsisEndpoints>::unique getIsisEndpoints() const;

// Operations
public:
	void clear();

	bool readFile(const String &filename);
	bool readFromPath(const String &path);

	bool writeFile(const String &filename) const;
	bool writeToPath(const String &path) const;

	bool validate() const;

	bool createIsisEndpoint(const String &name, const std::string &url, bool enabled, const String &password);
	void updateIsisEndpoints(shared_ptr<ConnectionsManager> connectionsManager);
	shared_ptr<IsisEndpoint> findIsisEndpoint(const std::string &url);
	bool removeIsisEndpoint(uint32 id);

	void updateLastObjectDate();
	void updateLastExchangeDate();
	void updateLastDownloadedObjectDate();
	void updateLastUploadedObjectDate();

	void unload();

private:
	bool _readXML(shared_ptr<XMLDocument> document);
	bool _writeXML(shared_ptr<XMLDocument> document) const;

	void _readCompatibilityPortalOptions(shared_ptr<XMLNode> root_node);
	void _readCompatibilityDatabaseOptions(shared_ptr<XMLNode> database_node);

	void readIsisOptions();
	void writeIsisOptions() const;

	bool initDatabaseOptions();
	void writeDatabaseOptions() const;

	uint32 peekIsisID();

public:
	static const String ID;
	static const String POV;
	static const String NAME;
	//static const String DESCRIPTION;
	static const String PORTAL;	
	static const String DELETED;
	static const String DATABASE;
	static const String VERSION;
	static const String DRIVER;
	static const String PASSWORD;
	static const String VALUE;
	static const String PARAMS;
	static const String PARAM;
	static const String SERVER;
	static const String ISIS;
	static const String ISIS_SERVER;
	static const String ISIS_NAME;
	static const String ISIS_URL;
	static const String ISIS_ENABLED;
	static const String ISIS_PASSWORD;
	static const String ISIS_USER;
	//static const String ISIS_LAST_EVENT;

	struct EngineExport options
	{
		static const String name;
		//static const String description;
		static const String deleted;
		static const String exchange_enabled;
		static const String public_enabled;
		static const String login_enabled;
		static const String register_enabled;
		static const String password;
		static const String sync;
		static const String optimize_threshold;
		//static const String guest_account;
		static const String last_sync_date;
		static const String last_checking_date;
		static const String last_stabilization_date;
		static const String align_hash;		
		static const String last_exchanged_object;
		static const String last_optimization_date;
		static const String last_object_date;					// Data ultimo oggetto inserito/aggiornato
		static const String last_exchange_date;					// Data ultimo allineamento completato con successo
		static const String last_downloaded_object_date;		// Data ultimo oggetto ricevuto via allineamento
		static const String last_uploaded_object_date;			// Data ultimo oggetto inviato via allineamento
		static const String database_name;
		static const String database_driver;
		static const String database_version;
		static const String database_params;
		//static const String users_reference;
	};

private:
	PortalID m_portalID;
	PovID m_povID;
	shared_ptr<IDbOptions> m_databaseOptions;
	mutable XMLOptions m_portalOptions;
	mutable boost::recursive_mutex m_isisCS;
	IsisEndpoints m_isisEndpoints;
	uint32 m_isisID;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
T PortalOptions::getOption(const String &name) const
{
    // Mac FIX: la riga sottostante non si compila SOLO sotto Mac.
    // return getOptionValue(name).get<T>();
	return static_cast<DataItem>(getOptionValue(name)).get<T>();
}

inline DataItem PortalOptions::getOptionValue(const String &name) const { return m_portalOptions.getOption(name); }
inline void PortalOptions::setOptionValue(const String &name, const DataItem &value) { m_portalOptions.setOption(name, value); }

inline const PortalID & PortalOptions::getPortalID() const { return m_portalID; }
inline void PortalOptions::setPortalID(const PortalID &portalID) { m_portalID = portalID; }

inline const PovID & PortalOptions::getPovID() const { return m_povID; }
inline void PortalOptions::setPovID(const PovID &povID) { m_povID = povID; }

inline String PortalOptions::getPassword() const { return m_portalOptions.getOption(options::password); }
inline void PortalOptions::setPassword(const String &password) { m_portalOptions.setOption(options::password, password); }

inline int PortalOptions::getOptimizeThreshold() const { return m_portalOptions.getOption(options::optimize_threshold); }
inline void PortalOptions::setOptimizeThreshold(const int &threshold) { m_portalOptions.setOption(options::optimize_threshold, threshold); }

inline shared_ptr<IDbOptions> PortalOptions::getDatabaseOptions() const { return m_databaseOptions; }
inline void PortalOptions::setDatabaseOptions(shared_ptr<IDbOptions> databaseOptions) { m_databaseOptions = databaseOptions; }

inline bool PortalOptions::getExchangeEnabled() const { return m_portalOptions.getOption(options::exchange_enabled); }
inline void PortalOptions::setExchangeEnabled(bool exchangeEnabled) { m_portalOptions.setOption(options::exchange_enabled, exchangeEnabled); }

//inline String PortalOptions::getGuestAccount() const { return m_portalOptions.getOption(options::guest_account); }
//inline void PortalOptions::setGuestAccount(const String &id) { m_portalOptions.setOption(options::guest_account, id); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _PORTALS_PORTALOPTIONS_H
