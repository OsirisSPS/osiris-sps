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
#include "portalsportaloptions.h"

#include "conversions.h"
#include "datetime.h"
#include "dbdatabasessystem.h"
#include "idbdriver.h"
#include "idboptions.h"
#include "isisendpoint.h"
#include "lock.h"
#include "log.h"

#include "option.h"
#include "options.h"
#include "osiriscommon.h"
#include "portalsportal.h"
#include "portalsportalssystem.h"
#include "utils.h"
#include "xmlattribute.h"
#include "xmlattributes.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"
#include "xmlschema.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String PortalOptions::ID = _S("id");
const String PortalOptions::POV = _S("pov");
const String PortalOptions::NAME = _S("name");
//const String PortalOptions::DESCRIPTION = _S("description");
const String PortalOptions::PORTAL = _S("portal");
const String PortalOptions::DELETED = _S("deleted");
const String PortalOptions::DATABASE = _S("database");
const String PortalOptions::VERSION = _S("version");
const String PortalOptions::DRIVER = _S("driver");
const String PortalOptions::PASSWORD = _S("");
const String PortalOptions::VALUE = _S("value");
const String PortalOptions::PARAMS = _S("params");
const String PortalOptions::PARAM = _S("param");
const String PortalOptions::SERVER = _S("server");
const String PortalOptions::ISIS = _S("isis");
const String PortalOptions::ISIS_SERVER = _S("server");
const String PortalOptions::ISIS_NAME = _S("name");
const String PortalOptions::ISIS_URL = _S("url");
const String PortalOptions::ISIS_ENABLED = _S("enabled");
const String PortalOptions::ISIS_PASSWORD = _S("password");
const String PortalOptions::ISIS_USER = _S("user");
//const String PortalOptions::ISIS_LAST_EVENT = _S("lastEvent");

//////////////////////////////////////////////////////////////////////

const String PortalOptions::options::name = _S("name");
//const String PortalOptions::options::description = _S("description");
const String PortalOptions::options::deleted = _S("deleted");
const String PortalOptions::options::exchange_enabled = _S("enable_exchange");
const String PortalOptions::options::public_enabled = _S("public_enabled");
const String PortalOptions::options::login_enabled = _S("login_enabled");
const String PortalOptions::options::register_enabled = _S("register_enabled");
const String PortalOptions::options::password = _S("password");
const String PortalOptions::options::sync = _S("sync");
const String PortalOptions::options::optimize_threshold = _S("optimize.threshold");
const String PortalOptions::options::last_sync_date = _S("last_sync_date");
const String PortalOptions::options::last_checking_date = _S("last_checking_date");
const String PortalOptions::options::last_stabilization_date = _S("last_stabilization_date");
const String PortalOptions::options::align_hash = _S("align_hash");
const String PortalOptions::options::last_exchanged_object = _S("last_exchanged_object");
const String PortalOptions::options::last_optimization_date = _S("last_optimization_date");
const String PortalOptions::options::last_object_date = _S("last_object_date");
const String PortalOptions::options::last_exchange_date = _S("last_exchange_date");
const String PortalOptions::options::last_downloaded_object_date = _S("last_downloaded_object_date");
const String PortalOptions::options::last_uploaded_object_date = _S("last_uploaded_object_date");
const String PortalOptions::options::database_name = _S("database.name");
const String PortalOptions::options::database_driver = _S("database.driver");
const String PortalOptions::options::database_version = _S("database.version");
const String PortalOptions::options::database_params = _S("database.params");

//////////////////////////////////////////////////////////////////////

//PortalOptions::PortalOptions(const PortalID &id) : m_id(id),
PortalOptions::PortalOptions() : m_isisID(0)
{
	m_portalOptions.ensureOption(options::name, String::EMPTY, true);
	//m_portalOptions.ensureOption(options::description, String::EMPTY);
	m_portalOptions.ensureOption(options::deleted, false, false);
	m_portalOptions.ensureOption(options::exchange_enabled, true, true);
	m_portalOptions.ensureOption(options::public_enabled, false, true);
	m_portalOptions.ensureOption(options::login_enabled, true, true);
	m_portalOptions.ensureOption(options::register_enabled, true, true);
	m_portalOptions.ensureOption(options::password, String::EMPTY, true);
	m_portalOptions.ensureOption(options::sync, String::EMPTY, true);
	m_portalOptions.ensureOption(options::optimize_threshold, -100, true);
	m_portalOptions.ensureOption(options::last_sync_date, String::EMPTY, false);
	m_portalOptions.ensureOption(options::last_checking_date, String::EMPTY, false);
	m_portalOptions.ensureOption(options::last_stabilization_date, String::EMPTY, false);
	m_portalOptions.ensureOption(options::align_hash, String::EMPTY, false);
	m_portalOptions.ensureOption(options::last_exchanged_object, ObjectID::EMPTY.toUTF16(), false);
	m_portalOptions.ensureOption(options::last_object_date, String::EMPTY, false);
	m_portalOptions.ensureOption(options::last_exchange_date, String::EMPTY, false);
	m_portalOptions.ensureOption(options::last_downloaded_object_date, String::EMPTY, false);
	m_portalOptions.ensureOption(options::last_uploaded_object_date, String::EMPTY, false);
	m_portalOptions.ensureOption(options::last_optimization_date, String::EMPTY, false);
	m_portalOptions.ensureOption(options::database_name, String::EMPTY, true);
	m_portalOptions.ensureOption(options::database_driver, String::EMPTY, true);
	m_portalOptions.ensureOption(options::database_version, conversions::to_utf16(Version(OS_PORTAL_DB_VERSION)), conversions::to_utf16(Version::EMPTY), false);
}

PortalOptions::~PortalOptions()
{

}

String PortalOptions::getName() const
{
	return m_portalOptions.getOption(options::name);
}

void PortalOptions::setName(const String &name)
{
	m_portalOptions.setOption(options::name, name);
}

/*
String PortalOptions::getDescription() const
{
	return m_portalOptions.getOption(options::description);
}

void PortalOptions::setDescription(const String &description)
{
	m_portalOptions.setOption(options::description, description);
}
*/

bool PortalOptions::getPublicEnabled() const
{
	bool enabled = m_portalOptions.getOption(options::public_enabled);

	// RAZORVERYURGENT: rinominare getWebDefaultPortal in getWebDefaultPov
	return (enabled || (Portal::generatePovID(m_portalID, m_povID) == Options::instance()->getWebDefaultPortal()) );
}

void PortalOptions::setPublicEnabled(bool publicEnabled)
{
	m_portalOptions.setOption(options::public_enabled, publicEnabled);
}

bool PortalOptions::getLoginEnabled() const
{
	return m_portalOptions.getOption(options::login_enabled);
}

void PortalOptions::setLoginEnabled(bool loginEnabled)
{
	m_portalOptions.setOption(options::login_enabled, loginEnabled);
}

bool PortalOptions::getRegisterEnabled() const
{
	return m_portalOptions.getOption(options::register_enabled);
}

void PortalOptions::setRegisterEnabled(bool registerEnabled)
{
	m_portalOptions.setOption(options::register_enabled, registerEnabled);
}

bool PortalOptions::getDeleted() const
{
	return m_portalOptions.getOption(options::deleted);
}

void PortalOptions::setDeleted(bool deleted)
{
	m_portalOptions.setOption(options::deleted, deleted);
}

DateTime PortalOptions::getLastSyncDate() const
{
	return static_cast<String>(m_portalOptions.getOption(options::last_sync_date));
}

void PortalOptions::setLastSyncDate(const DateTime &lastSyncDate)
{
	m_portalOptions.setOption(options::last_sync_date, lastSyncDate.toString());
}

DateTime PortalOptions::getLastCheckingDate() const
{
	return static_cast<String>(m_portalOptions.getOption(options::last_checking_date));
}

void PortalOptions::setLastCheckingDate(const DateTime &lastCheckingDate)
{
	m_portalOptions.setOption(options::last_checking_date, lastCheckingDate.toString());
}

DateTime PortalOptions::getLastStabilizationDate() const
{
	return static_cast<String>(m_portalOptions.getOption(options::last_stabilization_date));
}

void PortalOptions::setLastStabilizationDate(const DateTime &lastStabilizationDate)
{
	m_portalOptions.setOption(options::last_stabilization_date, lastStabilizationDate.toString());
}

UniqueID PortalOptions::getAlignHash() const
{
	return String(m_portalOptions.getOption(options::align_hash)).to_ascii();
}

void PortalOptions::setAlignHash(const UniqueID &alignHash)
{
	m_portalOptions.setOption(options::align_hash, alignHash.toUTF16());
}

void PortalOptions::resetAlignmentHash()
{
	m_portalOptions.setOption(options::align_hash, String::EMPTY);
}

void PortalOptions::updateAlignmentHash(const ObjectID &id, bool add)
{
#ifdef OS_NOOBJECTID
	setAlignHash(utils::generateHashMerge(getAlignHash().getString(), id.toAscii(), add)); 
#else
	setAlignHash(utils::generateHashMerge(getAlignHash().getString(), id.getHash(), add)); 
#endif
}

ObjectID PortalOptions::getLastExchangedObject() const
{
	return static_cast<String>(m_portalOptions.getOption(options::last_exchanged_object)).to_ascii();
}

void PortalOptions::setLastExchangedObject(const ObjectID &lastExchangedObject)
{
	m_portalOptions.setOption(options::last_exchanged_object, lastExchangedObject.toUTF16());
}

/*
ObjectID PortalOptions::getUsersReference() const
{
    return static_cast<String>(m_portalOptions.getOption(options::users_reference)).to_ascii();
}

void PortalOptions::setUsersReference(const ObjectID &reference)
{
    m_portalOptions.setOption(options::users_reference, reference.toUTF16());
}
*/

DateTime PortalOptions::getLastOptimizationDate() const
{
	return static_cast<String>(m_portalOptions.getOption(options::last_optimization_date));
}

void PortalOptions::setLastOptimizationDate(const DateTime &lastOptimizationDate)
{
	m_portalOptions.setOption(options::last_optimization_date, lastOptimizationDate.toString());
}

DateTime PortalOptions::getLastObjectDate() const
{
	return static_cast<String>(m_portalOptions.getOption(options::last_object_date));
}

DateTime PortalOptions::getLastExchangeDate() const
{
	return static_cast<String>(m_portalOptions.getOption(options::last_exchange_date));
}

DateTime PortalOptions::getLastDownloadedObjectDate() const
{
	return static_cast<String>(m_portalOptions.getOption(options::last_downloaded_object_date));
}

DateTime PortalOptions::getLastUploadedObjectDate() const
{
	return static_cast<String>(m_portalOptions.getOption(options::last_uploaded_object_date));
}

String PortalOptions::getDatabaseDriver() const
{
	return m_portalOptions.getOption(options::database_driver);
}

void PortalOptions::setDatabaseDriver(const String &databaseDriver)
{
	m_portalOptions.setOption(options::database_driver, databaseDriver);
}

String PortalOptions::getDatabaseName() const
{
	return m_portalOptions.getOption(options::database_name);
}

void PortalOptions::setDatabaseName(const String &databaseName)
{
	m_portalOptions.setOption(options::database_name, databaseName);
}

Version PortalOptions::getDatabaseVersion() const
{
	return conversions::from_utf16<Version>(m_portalOptions.getOption(options::database_version));
}

void PortalOptions::setDatabaseVersion(const Version &version)
{
	m_portalOptions.setOption(options::database_version, static_cast<String>(version.toString()));
}

shared_ptr<IsisEndpoint> PortalOptions::getIsisEndpoint(uint32 id) const
{
	OS_LOCK(m_isisCS);

	IsisEndpoints::const_iterator i = m_isisEndpoints.find(id);
	if(i != m_isisEndpoints.end())
		return i->second;

	return null;
}

Locked<const PortalOptions::IsisEndpoints>::unique PortalOptions::getIsisEndpoints() const
{
	return Locked<const PortalOptions::IsisEndpoints>::unique(m_isisEndpoints, m_isisCS);
}

void PortalOptions::clear()
{
	m_portalID.clear();
	m_povID.clear();
	m_portalOptions.toDefault();
	m_databaseOptions.reset();
}

bool PortalOptions::readFile(const String &filename)
{
	shared_ptr<XMLSchema> schema(OS_NEW XMLSchema());
	
	schema->parseFile(utils::makeFilePath(utils::makeFolderPath(Options::instance()->getSharePath(),OS_SCHEMAS_PATH), OS_SCHEMA_PORTAL));

	// FORCOMPATIBILITY - To allow reading of old 0.X portal xml. The alternative (avoided) is to set "pov" as optional in XSD of 1.X series.
	{
		String::size_type pos = filename.find(_S("000001"));
		if(pos != String::npos)
			schema = null;
	}
	
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument(schema));
	if(document->parseFile(filename) == false)
		return false;

	return _readXML(document);
}

bool PortalOptions::readFromPath(const String &path)
{
	OS_ASSERT(path.empty() == false);
	return readFile(utils::makeFilePath(path, OS_PORTAL_XML));
}

bool PortalOptions::writeFile(const String &filename) const
{
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	if(_writeXML(document) == false)
		return false;

	return document->writeFile(filename);
}

bool PortalOptions::writeToPath(const String &path) const
{
	OS_ASSERT(path.empty() == false);
	return writeFile(utils::makeFilePath(path, OS_PORTAL_XML));
}

bool PortalOptions::validate() const
{
	if(m_portalID.validate(false) == false)
		return false;

	if(getDatabaseVersion() <= OS_MAKEVERSION(8, 0)) // FORCOMPATIBILITY
	{
		// Compatibility with Osiris < Razor
	}
	else
	{
		if(m_povID.validate(false) == false)
		return false;
	}
		
	if(getName().empty())
		return false;

	if(getDatabaseVersion().empty())
		return false;

	if(DatabasesSystem::instance()->getDriver(getDatabaseDriver().to_ascii()) == null)
		return false;

	return true;
}

bool PortalOptions::createIsisEndpoint(const String &name, const std::string &url, bool enabled, const String &password)
{
	OS_LOCK(m_isisCS);

	HttpUrl isisUrl;
	if(isisUrl.parse(url) == false)
	{
		OS_ASSERTFALSE();
		return false;
	}

	for(IsisEndpoints::const_iterator i = m_isisEndpoints.begin(); i != m_isisEndpoints.end(); ++i)
	{
		if(i->second->matchUrl(isisUrl))
			return false;
	}

	shared_ptr<IsisEndpoint> endpoint(OS_NEW IsisEndpoint(getPortalID(),getPovID()));
	endpoint->setName(name);
	endpoint->setUrl(url);
	endpoint->setEnabled(enabled);
	endpoint->setPassword(password);

	m_isisEndpoints[peekIsisID()] = endpoint;
	return true;
}

void PortalOptions::updateIsisEndpoints(shared_ptr<ConnectionsManager> connectionsManager)
{
	OS_LOCK(m_isisCS);

	for(IsisEndpoints::const_iterator i = m_isisEndpoints.begin(); i != m_isisEndpoints.end(); ++i)
	{
		i->second->update(connectionsManager);
	}
}

shared_ptr<IsisEndpoint> PortalOptions::findIsisEndpoint(const std::string &url)
{
	OS_LOCK(m_isisCS);

	for(IsisEndpoints::const_iterator i = m_isisEndpoints.begin(); i != m_isisEndpoints.end(); ++i)
	{
		if(i->second->getUrl().toString() == url)
			return i->second;
	}

	return null;
}

bool PortalOptions::removeIsisEndpoint(uint32 id)
{
	OS_LOCK(m_isisCS);
	return m_isisEndpoints.erase(id) > 0;
}

void PortalOptions::updateLastObjectDate()
{
	m_portalOptions.setOption(options::last_object_date, DateTime::now().toString());
}

void PortalOptions::updateLastExchangeDate()
{
	m_portalOptions.setOption(options::last_exchange_date, DateTime::now().toString());
}

void PortalOptions::updateLastDownloadedObjectDate()
{
	m_portalOptions.setOption(options::last_downloaded_object_date, DateTime::now().toString());
}

void PortalOptions::updateLastUploadedObjectDate()
{
	m_portalOptions.setOption(options::last_uploaded_object_date, DateTime::now().toString());
}

void PortalOptions::unload()
{
	OS_LOCK(m_isisCS);

	for(IsisEndpoints::const_iterator i = m_isisEndpoints.begin(); i != m_isisEndpoints.end(); ++i)
	{
		i->second->close();
	}
}

bool PortalOptions::_readXML(shared_ptr<XMLDocument> document)
{
	clear();

	shared_ptr<XMLNode> root = document->getRoot();
	if(root == null)
		return false;

	m_portalID = root->getAttributeString(ID).to_ascii();
	m_povID = root->getAttributeString(POV).to_ascii();
	
	_readCompatibilityPortalOptions(root);

	shared_ptr<XMLNode> databaseNnode = root->getNode(DATABASE);
	if(databaseNnode != null)
		_readCompatibilityDatabaseOptions(databaseNnode);

	shared_ptr<XMLNode> optionsNode = root->getNode(OS_XMLOPTIONS_NODE_ROOT);
	if(optionsNode != null)
		m_portalOptions.parseXMLNode(optionsNode);

	readIsisOptions();

	if(initDatabaseOptions() == false)
		return false;

	return validate();
}

bool PortalOptions::_writeXML(shared_ptr<XMLDocument> document) const
{
	if(validate() == false)
		return false;

	shared_ptr<XMLNode> root = document->create(PORTAL);
	if(root == null)
		return false;

	root->setAttributeString(ID, m_portalID.toUTF16());
	root->setAttributeString(POV, m_povID.toUTF16());

	writeDatabaseOptions();
	writeIsisOptions();

	shared_ptr<XMLNode> optionsNode = m_portalOptions.toXMLNode();
	if(optionsNode != null)
		root->addChild(optionsNode);

	return true;
}

void PortalOptions::_readCompatibilityPortalOptions(shared_ptr<XMLNode> node)
{
	OS_ASSERT(node != null);

	shared_ptr<XMLAttribute> name = node->getAttribute(NAME);
	if(name != null)
		setName(name->getValue());

	/*
	shared_ptr<XMLAttribute> description = node->getAttribute(DESCRIPTION);
	if(description != null)
		setDescription(description->getValue());
	*/

	shared_ptr<XMLAttribute> attributeDeleted = node->getAttribute(DELETED);
	if(attributeDeleted != null)
		setDeleted(conversions::from_utf16<bool>(attributeDeleted->getValue()));
}

void PortalOptions::_readCompatibilityDatabaseOptions(shared_ptr<XMLNode> node)
{
	OS_ASSERT(node != null);

	shared_ptr<XMLAttributes> attributes = node->getAttributes();
	for(XMLAttributes::const_iterator i = attributes->begin(); i != attributes->end(); ++i)
	{
		const String &name = (*i)->getName();
		if(name == NAME)
		{
			setDatabaseName((*i)->getValue());
		}
		else if(name == VERSION)
		{
			setDatabaseVersion(conversions::from_utf16<Version>((*i)->getValue()));
		}
		else if(name == DRIVER)
		{
			setDatabaseDriver((*i)->getValue());
		}
	}
}

void PortalOptions::readIsisOptions()
{
	OS_LOCK(m_isisCS);

	m_isisEndpoints.clear();

	map<String, shared_ptr<IsisEndpoint> >::type isisServers;

	Locked<const XMLOptions::Options, boost::shared_mutex>::shared options = m_portalOptions.getOptions();
	for(XMLOptions::Options::const_iterator i = options->begin(); i != options->end(); ++i)
	{
		String optionName = i->first;
		optionName.to_lower();

		if(optionName.starts_with(ISIS) == false)
			continue;

		StringVector tokens;
		if(utils::split(optionName, _S("."), tokens, true) > 2)
		{
			String serverID = tokens[1];

			shared_ptr<IsisEndpoint> isisEndpoint;

			map<String, shared_ptr<IsisEndpoint> >::type::const_iterator s = isisServers.find(serverID);
			if(s == isisServers.end())
			{
				isisEndpoint.reset(OS_NEW IsisEndpoint(getPortalID(),getPovID()));
				isisServers[serverID] = isisEndpoint;
			}
			else
				isisEndpoint = s->second;

			OS_ASSERT(isisEndpoint != null);

			String isisOption = tokens[2];

			if(isisOption.compare_no_case(ISIS_NAME))
				isisEndpoint->setName(i->second->getValue());
			else if(isisOption.compare_no_case(ISIS_URL))
				isisEndpoint->setUrl(static_cast<String>(i->second->getValue()).to_ascii());
			else if(isisOption.compare_no_case(ISIS_ENABLED))
				isisEndpoint->setEnabled(static_cast<bool>(i->second->getValue()));
			else if(isisOption.compare_no_case(ISIS_PASSWORD))
				isisEndpoint->setPassword(static_cast<String>(i->second->getValue()));
			//else if(isisOption.compare_no_case(ISIS_USER))
			//	isisEndpoint->setUser(static_cast<String>(i->second->getValue()).to_ascii());
			/*else if(isisOption.compare_no_case(ISIS_LAST_EVENT))
				isisEndpoint->setLastEvent(static_cast<String>(i->second->getValue()));*/
		}
	}

	for(map<String, shared_ptr<IsisEndpoint> >::type::const_iterator s = isisServers.begin(); s != isisServers.end(); ++s)
	{
		if(s->second->isValid())
			m_isisEndpoints[peekIsisID()] = s->second;
	}
}

void PortalOptions::writeIsisOptions() const
{
	OS_LOCK(m_isisCS);

	m_portalOptions.removePattern(ISIS);

	uint32 index = 1;
	for(IsisEndpoints::const_iterator i = m_isisEndpoints.begin(); i != m_isisEndpoints.end(); ++i, ++index)
	{
		shared_ptr<IsisEndpoint> isisEndpoint = i->second;

		String isisServer = ISIS + _S(".") + ISIS_SERVER + conversions::to_utf16(index) + _S(".");

		// N.B.: specifica anche il valore di default in modo forzare il salvataggio dell'opzione (se non esiste il valore verrebbe considerato come predefinito e sarebbe saltato in fase di salvataggio)

		m_portalOptions.ensureOption(isisServer + ISIS_NAME, isisEndpoint->getName(), String::EMPTY, false);
		m_portalOptions.ensureOption(isisServer + ISIS_URL, String(isisEndpoint->getUrl().toString()), String::EMPTY, false);
		m_portalOptions.ensureOption(isisServer + ISIS_ENABLED, isisEndpoint->getEnabled(), true, false, null);
		m_portalOptions.ensureOption(isisServer + ISIS_PASSWORD, isisEndpoint->getPassword(), String::EMPTY, false);
		//m_portalOptions.ensureOption(isisServer + ISIS_USER, isisEndpoint->getUser().toUTF16(), ObjectID::EMPTY.toUTF16(), false);
		//m_portalOptions.ensureOption(isisServer + ISIS_LAST_EVENT, isisEndpoint->getLastEvent(), String::EMPTY, false);
	}
}

bool PortalOptions::initDatabaseOptions()
{
	String databaseDriver = getDatabaseDriver();

	shared_ptr<IDbDriver> driver = DatabasesSystem::instance()->getDriver(databaseDriver.to_ascii());
	if(driver == null)
	{
		OS_LOG_ERROR(String::format(_S("Invalid database driver '%S'").c_str(), databaseDriver.c_str()));
		return false;
	}

	m_databaseOptions = driver->createOptions();
	if(m_databaseOptions != null)
	{
		// TODO: caricare e settare i parametri del database

		String databaseParams = DATABASE + _S(".") + PARAMS + _S(".");

		Locked<const XMLOptions::Options, boost::shared_mutex>::shared options = m_portalOptions.getOptions();
		for(XMLOptions::Options::const_iterator i = options->begin(); i != options->end(); ++i)
		{
			String optionName = i->first;
			optionName.to_lower();

			if(optionName.starts_with(databaseParams) == false)
				continue;

			StringVector tokens;
			if(utils::split(optionName, _S("."), tokens, true) > 2)
			{
				String paramName = tokens[2];
				String paramValue = i->second->getValue();

				m_databaseOptions->setParam(paramName, paramValue);
			}
		}
	}

	return true;
}

void PortalOptions::writeDatabaseOptions() const
{
	String databaseParams = DATABASE + _S(".") + PARAMS + _S(".");

	m_portalOptions.removePattern(databaseParams);

	if(m_databaseOptions == null)
		return;

	const ordered_map<String, String> &params = m_databaseOptions->getParams();
	for(ordered_map<String, String>::const_iterator i = params.begin(); i != params.end(); ++i)
	{
		String paramName = databaseParams + i->first;
		String paramValue = i->second;

		// N.B.: specifica anche il valore di default in modo forzare il salvataggio dell'opzione (se non esiste il valore verrebbe considerato come predefinito e sarebbe saltato in fase di salvataggio)

		m_portalOptions.ensureOption(paramName, paramValue, String::EMPTY, false);
	}
}

uint32 PortalOptions::peekIsisID()
{
	OS_LOCK(m_isisCS);
	return m_isisID++;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
