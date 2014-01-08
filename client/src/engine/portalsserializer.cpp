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
#include "portalsserializer.h"

#include "boost/bind.hpp"
#include "convert.h"
#include "conversions.h"
#include "cryptmanager.h"
#include "entitiesentity.h"
#include "entitiessnapshotmanager.h"
#include "iportaldatabase.h"
#include "idbconnection.h"
#include "idbresult.h"
#include "dbtables.h"
#include "dbsqlselect.h"
#include "ixmlhandler.h"
#include "log.h"
#include "objectsiobject.h"
#include "objectsavatar.h"
#include "objectsreputation.h"
#include "objectssection.h"
#include "objectssystem.h"
#include "objectstext.h"
#include "objectspost.h"
#include "objectsuser.h"
#include "omlmanager.h"
#include "osirislink.h"
#include "p2pexchangecontext.h"
#include "p2pexchangesession.h"
#include "portalsportal.h"
#include "portalstransaction.h"
#include "streamfilter.h"
#include "streamlayergzip.h"
#include "streamlayerzlib.h"
#include "temporaryfile.h"
#include "xmlnode.h"
#include "xmlwriter.h"

//////////////////////////////////////////////////////////////////////

#define OS_PORTALSERIALIZER_XML_ROOT				"osiris"
#define OS_PORTALSERIALIZER_XML_PORTAL				"portal"
#define OS_PORTALSERIALIZER_XML_PORTAL_LINK			"portal_link"
#define OS_PORTALSERIALIZER_XML_OBJECTS				"objects"
#define OS_PORTALSERIALIZER_XML_OBJECT				"object"
#define OS_PORTALSERIALIZER_XML_OBJECT_ID			"id"
#define OS_PORTALSERIALIZER_XML_OBJECT_DATA			"bin"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<PortalsSerializer>
{
// Internal
private:
	class Result : public PortalsSerializer::IResult
	{
	// Construction
	public:
		Result();
		virtual ~Result();

	// Attributes
	public:
		void setPortalLink(shared_ptr<OsirisLink> portalLink);

	// PortalsSerializer::IResult interface
	public:
		virtual shared_ptr<OsirisLink> getPortalLink() const;
		virtual uint32 getTotalObjects() const;
		virtual uint32 getImportedObjects() const;
		virtual uint32 getSkippedObjects() const;
		virtual uint32 getCorruptedObjects() const;

	// Operations
	public:
		void increaseTotalObjects();
		void increaseImportedObjects();
		void increaseSkippedObjects();
		void increaseCorruptedObjects();

	private:
		shared_ptr<OsirisLink> m_portalLink;
		uint32 m_totalObjects;
		uint32 m_importedObjects;
		uint32 m_skippedObjects;
		uint32 m_corruptedObjects;
	};

	class Parser : public IXMLHandler
	{
		typedef IXMLHandler HandlerBase;

		typedef pimpl<PortalsSerializer>::Result Result;

	// Internal
	private:
		enum ObjectImportResult
		{
			objectImportDone		= 0,
			objectImportSkipped		= 1,
			objectImportCorrupted	= 2,
		};

	// Construction
	public:
		Parser(shared_ptr<Portal> portal = null, shared_ptr<XMLSchema> schema = null);
		virtual ~Parser();

	// Attributes
	public:
		inline shared_ptr<Result> getResult() const;

		inline void setProgressCallback(const boost::function<bool ()> &progressCallback);

	private:
		void parseNodeRoot(const StringCollection<String> &attributes);
		void parseNodeObject(const StringCollection<String> &attributes);

		ObjectImportResult importObject(const std::string &id, const std::string &data);
		ObjectImportResult importObject(shared_ptr<ObjectsIObject> object);		
		void initializeImport();
		void finalizeImport();

		void fireProgress();

	// IXMLHandler interface
	protected:
		virtual void onStartDocument();
		virtual void onEndDocument();

		virtual void onStartElement(const String &name, const StringCollection<String> &attributes);
		virtual void onEndElement(const String &name);
		virtual void onCharacters(const String &str);

	private:
		boost::function<bool ()> m_progressCallback;
		shared_ptr<Portal> m_portal;
		shared_ptr<Result> m_result;
		shared_ptr<PortalsTransaction> m_importWork;
		p2p::exchange_context_ptr m_exchangeContext;
		p2p::exchange_session_ptr m_exchangeSession;
	};

// Construction
public:
	pimpl();
	virtual ~pimpl();

// Attributes
public:
	Locked<PortalsSerializer::ProgressEvent>::unique getProgressEvent();

	double getProgressPercentage() const;
	void setProgressPercentage(double progressPercentage);

	bool getStopped() const;
	void setStopped(bool stopped);

// Operations
public:
	bool exportFile(shared_ptr<Portal> portal, const String &filename, const String &format, const boost::function<void (double)> &progressCallback);
	bool exportStream(shared_ptr<Portal> portal, shared_ptr<IStream> stream, const String &format, const boost::function<void (double)> &progressCallback);

	shared_ptr<Result> importFile(shared_ptr<Portal> portal, const String &filename, const String &format, const boost::function<void (double)> &progressCallback);
	shared_ptr<Result> importStream(shared_ptr<Portal> portal, shared_ptr<IStream> stream, const String &format, const boost::function<void (double)> &progressCallback);

	shared_ptr<Result> parsePortalLink(const String &filename, const boost::function<void (double)> &progressCallback);
	shared_ptr<Result> parsePortalLink(shared_ptr<IStream> stream, const boost::function<void (double)> &progressCallback);

	void fireProgressEvent(double percentage);

	bool exportOsirisFormat(shared_ptr<IPortalDatabase> db, shared_ptr<Portal> portal, shared_ptr<IStream> stream, const boost::function<void (double)> &progressCallback);
	bool exportGourceFormat(shared_ptr<IPortalDatabase> db, shared_ptr<Portal> portal, shared_ptr<IStream> stream, const boost::function<void (double)> &progressCallback);

private:
	shared_ptr<IStream> initStream(shared_ptr<IStream> stream, bool read, const String &format);
	bool importStream(shared_ptr<Parser> parser, shared_ptr<IStream> stream, const boost::function<void (double)> &progressCallback);

	void resetData();
	bool handleParserProgress(shared_ptr<IStream> stream, uint64 totalSize, const boost::function<void (double)> &progressCallback);

	static shared_ptr<XMLNode> exportObjectXML(shared_ptr<ObjectsIObject> object);

private:
	boost::mutex m_taskMutex;
	mutable boost::mutex m_dataMutex;
	double m_progressPercentage;
	bool m_stopped;
	boost::recursive_mutex m_eventsMutex;
	PortalsSerializer::ProgressEvent m_progressEvent;
};

//////////////////////////////////////////////////////////////////////

pimpl<PortalsSerializer>::Result::Result() : m_totalObjects(0),
											m_importedObjects(0),
											m_skippedObjects(0),
											m_corruptedObjects(0)
{

}

pimpl<PortalsSerializer>::Result::~Result()
{

}

void pimpl<PortalsSerializer>::Result::setPortalLink(shared_ptr<OsirisLink> portalLink)
{
	m_portalLink = portalLink;
}

shared_ptr<OsirisLink> pimpl<PortalsSerializer>::Result::getPortalLink() const
{
	return m_portalLink;
}

uint32 pimpl<PortalsSerializer>::Result::getTotalObjects() const
{
	return m_totalObjects;
}

uint32 pimpl<PortalsSerializer>::Result::getImportedObjects() const
{
	return m_importedObjects;
}

uint32 pimpl<PortalsSerializer>::Result::getSkippedObjects() const
{
	return m_skippedObjects;
}

uint32 pimpl<PortalsSerializer>::Result::getCorruptedObjects() const
{
	return m_corruptedObjects;
}

void pimpl<PortalsSerializer>::Result::increaseTotalObjects()
{
	m_totalObjects++;
}

void pimpl<PortalsSerializer>::Result::increaseImportedObjects()
{
	m_importedObjects++;
}

void pimpl<PortalsSerializer>::Result::increaseSkippedObjects()
{
	m_skippedObjects++;
}

void pimpl<PortalsSerializer>::Result::increaseCorruptedObjects()
{
	m_corruptedObjects++;
}

//////////////////////////////////////////////////////////////////////

pimpl<PortalsSerializer>::Parser::Parser(shared_ptr<Portal> portal, shared_ptr<XMLSchema> schema) : HandlerBase(schema),
																									m_portal(portal),
																									m_result(OS_NEW Result())
{

}

pimpl<PortalsSerializer>::Parser::~Parser()
{

}

inline shared_ptr<pimpl<PortalsSerializer>::Parser::Result> pimpl<PortalsSerializer>::Parser::getResult() const
{
	return m_result;
}

inline void pimpl<PortalsSerializer>::Parser::setProgressCallback(const boost::function<bool ()> &progressCallback)
{
	m_progressCallback = progressCallback;
}

void pimpl<PortalsSerializer>::Parser::parseNodeRoot(const StringCollection<String> &attributes)
{
	OS_ASSERT(m_result->getPortalLink() == null);

	const String &linkAttribute = attributes.get(OS_PORTALSERIALIZER_XML_PORTAL_LINK);
	if(linkAttribute.empty() == false)
	{		
		shared_ptr<OsirisLink> link(OS_NEW OsirisLink());
		if(link->parse(linkAttribute.to_ascii()) == false)
			OS_EXCEPTION("Invalid portal link");

		if(link->getType() != OsirisLink::linkPortal)
			OS_EXCEPTION("Invalid link type");
		
		m_result->setPortalLink(link);

		if(m_portal == null)
		{
			setStopParser(true);
			return;
		}

		if(link->getPortal() != m_portal->getPortalID())
		{
			OS_ASSERTFALSE();
			OS_EXCEPTION("Internal error: invalid portal id");
		}

		if(link->getPov() != m_portal->getPovID())
		{
			OS_ASSERTFALSE();
			OS_EXCEPTION("Internal error: invalid pov id");
		}
	}
}

void pimpl<PortalsSerializer>::Parser::parseNodeObject(const StringCollection<String> &attributes)
{
	m_result->increaseTotalObjects();
		
	ObjectImportResult result;

	// If we have "bin", it's the binary format.
	if(attributes.exists(OS_PORTALSERIALIZER_XML_OBJECT_DATA))
	{
		result = importObject(attributes.get(OS_PORTALSERIALIZER_XML_OBJECT_ID).to_ascii(), attributes.get(OS_PORTALSERIALIZER_XML_OBJECT_DATA).to_ascii());

	}
	else
	{
		// Build the object.
		// A better approach will be some "importXML" virtual in objects itself.
		shared_ptr<IPortalDatabase> database = m_importWork->getDatabase();
		OS_ASSERT(database != null);

		shared_ptr<ObjectsIObject> object;

		String type = attributes.get(_S("type"));
		//String id = attributes.get(_S("id"));
		String entityAuthor = attributes.get(_S("entity_author"));
		String entityPrivateKeyHex = attributes.get(_S("entity_private_key"));
		String privateKeyHex = attributes.get(_S("private_key"));

		String note = attributes.get(_S("note")); // Only for logging, to understand problems.

		if(note == _S("Type: section, Original ID: 84"))
		{
			// Isis
			note += _S("x");
		}

		if(note == _S("Type: section, Original ID: 5"))
		{
			// English
			note += _S("x");
			// CLODOURGENT: breakpoint qui: Isis sopra (68) a sto punto è orfano, ma inserire questo DEVE invalidarlo.
		}

		if( (type == _S("avatar")) || (type == _S("file")) )
		{
			// file_path - read the path and fill the buffer
			// file_url - read the url and fill the buffer
		}
		
		if(type == _S("user"))
		{
			// TOCLEAN: niente public_key tra gli attributi, togliere anche lato PHP.
			Buffer publicKeyPhp;
			publicKeyPhp.fromHex(attributes.get(_S("public_key")).to_ascii());
			
			Buffer privateKey;
			privateKey.fromHex(privateKeyHex.to_ascii());

			Buffer publicKey;
			if(CryptManager::instance()->rsaGetPublicKey(privateKey, publicKey))
			{
				bool diff = (publicKeyPhp != publicKey);
				DateTime join_date(attributes.get(_S("join_date")));

				shared_ptr<ObjectsUser> user(OS_NEW ObjectsUser());			
				user->name = attributes.get(_S("name"));
				user->description = attributes.get(_S("description"));
				user->public_key = publicKey;
				user->join_date = join_date;
				user->mark = OMLManager::instance()->reverseParse(attributes.get(_S("mark")), attributes.get(_S("mark_type")).to_ascii());
				object = user;
			}
		}
		else if(type == _S("avatar"))
		{
			shared_ptr<ObjectsAvatar> avatar(OS_NEW ObjectsAvatar());			
			avatar->file_name = attributes.get(_S("file_name"));
			avatar->file_type = attributes.get(_S("file_type"));
			//avatar->file_size = conversions::from_utf16<uint32>(attributes.get(_S("file_size")));

			if(attributes.exists(_S("file_data")))
			{
				Buffer data;
				data.fromBase64(attributes.get(_S("file_data")).to_ascii());
				avatar->file_data = data;
				avatar->file_size = data.getSize();
			}
			
			object = avatar;
		}
		else if(type == _S("section"))
		{
			shared_ptr<ObjectsSection> section(OS_NEW ObjectsSection());			
						
			section->title = attributes.get(_S("title"));
			section->description = attributes.get(_S("description"));			
			section->component = ExtensionsComponentID(attributes.get(_S("component")).to_ascii());
			object = section;			
		}
		else if(type == _S("text"))
		{
			shared_ptr<ObjectsText> text(OS_NEW ObjectsText());			
						
			text->title = attributes.get(_S("title"));
			text->description = attributes.get(_S("description"));			
			text->content = OMLManager::instance()->reverseParse(attributes.get(_S("content")), attributes.get(_S("content_type")).to_ascii());
			text->pinned = (attributes.get(_S("pinned")) == _S("true"));
			object = text;			
		}
		else if(type == _S("post"))
		{
			shared_ptr<ObjectsPost> post(OS_NEW ObjectsPost());			
						
			post->title = attributes.get(_S("title"));
			post->content = OMLManager::instance()->reverseParse(attributes.get(_S("content")), attributes.get(_S("content_type")).to_ascii());
			object = post;	
		}
		else if(type == _S("reputation"))
		{
			shared_ptr<ObjectsReputation> reputation(OS_NEW ObjectsReputation());			
						
#ifdef OS_NOOBJECTID
			reputation->reference = ObjectID(attributes.get(_S("reference")).to_ascii());
#else
			reputation->reference = ObjectID::generate(portalObjectTypeUser, attributes.get(_S("reference")).to_ascii());
#endif
			reputation->score = conversions::from_utf16<double>(attributes.get(_S("score")));			
			reputation->description = attributes.get(_S("description"));			
			reputation->follow = (attributes.get(_S("follow")) == _S("true"));
			object = reputation;
		}

		if(object != null)
		{
			if(attributes.exists(_S("id")))
#ifdef OS_NOOBJECTID
				object->id = ObjectID(attributes.get(_S("id")).to_ascii());
#else
				object->id = ObjectID::generate(object->getObjectType(), attributes.get(_S("id")).to_ascii());
#endif

			if(attributes.exists(_S("submit_date")))
				object->submit_date = DateTime(attributes.get(_S("submit_date")));

			shared_ptr<ObjectsIEntry> entry = objects_entry_cast(object);

			if(entry)
			{
#ifdef OS_NOOBJECTID
				entry->author = ObjectID(attributes.get(_S("author")).to_ascii());			
#else
				entry->author = ObjectID::generate(portalObjectTypeUser, attributes.get(_S("author")).to_ascii());			
#endif
			}

			object->ensureID(database); // Calculate the ID for some types of objects, for example votes.

			shared_ptr<ObjectsIObject> existsObject = null; // Identify the latest object, to check if is already the same object.

			shared_ptr<ObjectsIRevisionable> revisionable = objects_revisionable_cast(object);			
			if(revisionable)
			{
				shared_ptr<ObjectsIRevisionable> revisionable = objects_revisionable_cast(object);

				EntityID entityID = attributes.get(_S("entity")).to_ascii();
								
				if(note == _S("Type: text, Original ID: 1417"))
				{
					revisionable = objects_revisionable_cast(object);
				}							

#ifdef OS_TODOCIP
				

#else
				shared_ptr<ObjectsIObject> primary = m_portal->getObject(database, entityID.toObjectID());
				if(primary != null)
				{
					revisionable->revision = entityID.toEntityID();					
				}
				else
				{
					revisionable->id = entityID.toObjectID();			
					revisionable->author = ObjectID(attributes.get(_S("entity_author")).to_ascii());
					revisionable->submit_date = DateTime(attributes.get(_S("entity_date")));

					privateKeyHex = attributes.get(_S("entity_private_key"));
				}				
#endif
				revisionable->parent = EntityID(attributes.get(_S("parent")).to_ascii());
				if(attributes.exists(_S("position")))
					revisionable->position = conversions::from_utf16<uint32>(attributes.get(_S("position")));
				
				revisionable->comment = String::EMPTY;

				revisionable->visible = true;
				if(attributes.exists(_S("visible")))
					revisionable->visible = (attributes.get(_S("visible")) == _S("true"));
				
				m_portal->getSnapshotManager()->ensure(database, entityID);
				shared_ptr<EntitiesEntity> entity = m_portal->getEntity(database, entityID);
				if(entity != null)
				{
					existsObject = entity->getCurrent();

					if(existsObject)
					{
						// We have entity data, use that.
						// TODO: Maybe here a check if is the same of xml data.
						revisionable->changeToRevision(objects_revisionable_cast(existsObject));
					}
					else
					{
						
					}
				}

				// If the object doesn't exists, not create the delete revision.
				if( (object) && (existsObject == null) && (revisionable->visible == false) ) 
					object = null;
				// If the object exists, and it's deleted, not create the delete revision.
				if( (object) && (existsObject != null) && (objects_revisionable_cast(existsObject)->visible == false) )
					object = null;

#ifdef OS_TODOCIP
				
				// Construct the entity data, with data available.						
				if( (object) && (existsObject == null) )
				{
					revisionable->entity = entityID;
					if(attributes.exists(_S("entity_author")))
#ifdef OS_NOOBJECTID
						revisionable->entity_author = ObjectID(attributes.get(_S("entity_author")).to_ascii());
#else
						revisionable->entity_author = ObjectID::generate(portalObjectTypeUser, attributes.get(_S("entity_author")).to_ascii());
#endif
					else
#ifdef OS_NOOBJECTID
						revisionable->entity_author = ObjectID(attributes.get(_S("author")).to_ascii());
#else
						revisionable->entity_author = ObjectID::generate(portalObjectTypeUser, attributes.get(_S("author")).to_ascii());
#endif
					if(attributes.exists(_S("entity_date")))
						revisionable->entity_date = DateTime(attributes.get(_S("entity_date")));				
					else
						revisionable->entity_date = DateTime(attributes.get(_S("submitdate")));				
					if(attributes.exists(_S("entity_parent")))
						revisionable->entity_parent = EntityID(attributes.get(_S("entity_parent")).to_ascii());				
					else
						revisionable->entity_parent = EntityID(attributes.get(_S("parent")).to_ascii());				

					Buffer entityPrivateKey;
					entityPrivateKey.fromHex(entityPrivateKeyHex.to_ascii());
					revisionable->signEntity(entityPrivateKey, m_portal->getPortalID());
				}
#endif
			}
			else
			{
				existsObject = m_portal->getObject(database, object->id);
			}

			// If don't have the submit_date, maybe compare content.
			if( (object != null) && (existsObject != null) )
			{
				if(object->submit_date->isNull())
				{
					// TODO: Compare by content. Actually skip.
					object = null;
				}
				else if(object->submit_date <= existsObject->submit_date)
				{
					object = null;
				}				
			}
						
			if(object != null)
			{
				Buffer privateKey;
				privateKey.fromHex(privateKeyHex.to_ascii());

				LanguageResult createResult = object->create(database, privateKey);
				if(createResult.empty())
				{
					result = objectImportDone;
					OS_LOG_NOTICE(_S("Object created. Note from source:") + note);
				}
				else
				{
					result = objectImportCorrupted;			
					OS_LOG_NOTICE(_S("Invalid object. Reason:") + String(createResult.getId()) + _S(". Note from source:") + note);
				}
			}
			else
				result = objectImportSkipped;			

		}	
		else
			result = objectImportCorrupted;			
	}

	switch(result)
	{
	case objectImportDone:			m_result->increaseImportedObjects();
									break;

	case objectImportSkipped:		m_result->increaseSkippedObjects();
									break;

	case objectImportCorrupted:		m_result->increaseCorruptedObjects();
									break;

	default:						OS_ASSERTFALSE();
									break;
	}
}

pimpl<PortalsSerializer>::Parser::ObjectImportResult pimpl<PortalsSerializer>::Parser::importObject(const std::string &id, const std::string &data)
{
	if(m_portal == null)
	{
		OS_ASSERTFALSE();
		OS_EXCEPTION("Internal error: invalid portal");
	}

	ObjectID objectID(id);
	if(objectID.validate(false) == false)
		return objectImportCorrupted;

	Buffer objectBuffer;
	if(objectBuffer.fromBase64(data) == false)
		return objectImportCorrupted;

	DataTree objectData;
	if(objectData.read(objectBuffer) == false)
		return objectImportCorrupted;

	PortalObjectType objectType = Convert::toObjectType(static_cast<uint32>(objectData.getV(DBTABLES::ENTRIES::TYPE)));

	shared_ptr<ObjectsIObject> object = ObjectsSystem::instance()->createObject(objectType);
	if(object == null)
		return objectImportSkipped;		// Oggetto sconosciuto

    if(object->read(id, objectData) == false)
		return objectImportCorrupted;

	return importObject(object);
}

pimpl<PortalsSerializer>::Parser::ObjectImportResult pimpl<PortalsSerializer>::Parser::importObject(shared_ptr<ObjectsIObject> object)
{
	shared_ptr<IPortalDatabase> database = m_importWork->getDatabase();
	OS_ASSERT(database != null);

	if(object->isUser() == false)
	{
		if(m_exchangeSession->requireObject(database, object->getAuthor()))
		{
			// In teoria non dovrebbe capitare: in fase di serializzazione vengono salvati prima gli utenti e poi gli oggetti, quindi l'autore
			// dell'oggetto corrente dovrebbe essere stato serializzato in precedenza
			OS_ASSERTFALSE();
			return objectImportSkipped;
		}
	}

	if(m_exchangeSession->needObject(database, object->id, object->submit_date) == false)
		return objectImportSkipped;

	if(m_exchangeSession->storeObject(database, object) == false)
	{
		return objectImportSkipped;
	}

	return objectImportDone;
}

void pimpl<PortalsSerializer>::Parser::initializeImport()
{
	if(m_portal == null) // If the parse need to read only the portal-link.
		return;

	if(m_portal == null)
	{
		OS_ASSERTFALSE();
		OS_EXCEPTION("Internal error: invalid portal");
	}

	if(m_exchangeContext == null)
	{
		m_exchangeContext = m_portal->getExchangeContext();
		if(m_exchangeContext == null)
		{
			OS_ASSERTFALSE();
			OS_EXCEPTION("Internal error: invalid exchange context");
		}
	}

	if(m_exchangeSession == null)
	{
		m_exchangeSession = m_exchangeContext->createSession();
		if(m_exchangeSession == null)
		{
			OS_ASSERTFALSE();
			OS_EXCEPTION("Internal error: invalid exchange session");
		}
	}

	if(m_importWork == null)
	{
		m_importWork = m_portal->startTransaction(true);
		if(m_importWork == null)
		{
			OS_ASSERTFALSE();
			OS_EXCEPTION("Internal error: invalid database work");
		}

		m_importWork->getDatabase()->beginTransaction();
	}
}

void pimpl<PortalsSerializer>::Parser::finalizeImport()
{
	if(m_exchangeContext != null)
	{
		if(m_exchangeSession != null)
		{
			OS_ASSERT(m_portal != null);
			m_exchangeSession->finalize(m_portal);

			m_exchangeContext->removeSession(m_exchangeSession);
			m_exchangeSession.reset();
		}

		m_exchangeContext.reset();
	}

	if(m_importWork != null)
	{
		m_importWork->getDatabase()->commit();
		m_importWork.reset();
	}
}

void pimpl<PortalsSerializer>::Parser::fireProgress()
{
	if(m_progressCallback.empty())
		return;

	// Se il callback restituisce false ferma il parser
	if(m_progressCallback() == false)
		setStopParser(true);
}

void pimpl<PortalsSerializer>::Parser::onStartDocument()
{
	initializeImport();

	fireProgress();
}

void pimpl<PortalsSerializer>::Parser::onEndDocument()
{
	finalizeImport();

	fireProgress();
}

void pimpl<PortalsSerializer>::Parser::onStartElement(const String &name, const StringCollection<String> &attributes)
{
	if(name == OS_PORTALSERIALIZER_XML_ROOT)
		parseNodeRoot(attributes);
	else if(name == OS_PORTALSERIALIZER_XML_OBJECT)
		parseNodeObject(attributes);

	fireProgress();
}

void pimpl<PortalsSerializer>::Parser::onEndElement(const String &name)
{
	fireProgress();
}

void pimpl<PortalsSerializer>::Parser::onCharacters(const String &str)
{
	fireProgress();
}

//////////////////////////////////////////////////////////////////////

pimpl<PortalsSerializer>::pimpl() : m_progressPercentage(0),
								   m_stopped(false)
{

}

pimpl<PortalsSerializer>::~pimpl()
{

}

Locked<PortalsSerializer::ProgressEvent>::unique pimpl<PortalsSerializer>::getProgressEvent()
{
	return Locked<PortalsSerializer::ProgressEvent>::unique(m_progressEvent, m_eventsMutex);
}

double pimpl<PortalsSerializer>::getProgressPercentage() const
{
	boost::mutex::scoped_lock lock(m_dataMutex);
	return m_progressPercentage;
}

void pimpl<PortalsSerializer>::setProgressPercentage(double progressPercentage)
{
	boost::mutex::scoped_lock lock(m_dataMutex);
	OS_ASSERT(progressPercentage >= 0 && progressPercentage <= 1);
	m_progressPercentage = progressPercentage;
}

bool pimpl<PortalsSerializer>::getStopped() const
{
	boost::mutex::scoped_lock lock(m_dataMutex);
	return m_stopped;
}

void pimpl<PortalsSerializer>::setStopped(bool stopped)
{
	boost::mutex::scoped_lock lock(m_dataMutex);
	m_stopped = stopped;
}

bool pimpl<PortalsSerializer>::exportFile(shared_ptr<Portal> portal, const String &filename, const String &format, const boost::function<void (double)> &progressCallback)
{
	shared_ptr<TemporaryFile> file(OS_NEW TemporaryFile(false));
	if(file->open(filename, File::ofWrite) == false)
		return false;

	if(exportStream(portal, file, format, progressCallback) == false)
		return false;

	file->setAutoDelete(false);

	return true;
}

bool pimpl<PortalsSerializer>::exportStream(shared_ptr<Portal> portal, shared_ptr<IStream> stream, const String &format, const boost::function<void (double)> &progressCallback)
{
	if(portal == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	shared_ptr<PortalsTransaction> exportWork = portal->startTransaction(false);
	if(exportWork == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	shared_ptr<IPortalDatabase> db = exportWork->getDatabase();
	if(db == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	boost::mutex::scoped_lock lock(m_taskMutex);

	resetData();

	stream = initStream(stream, false, format);
	if(stream == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	OS_ASSERT(portal != null);
	OS_ASSERT(db != null);

	if(format == _S("osiris"))
		return exportOsirisFormat(db, portal, stream, progressCallback);
	else if(format == _S("gource"))
		return exportGourceFormat(db, portal, stream, progressCallback);
	else
	{
		OS_ASSERTFALSE();
		return false;
	}

	
}

shared_ptr<pimpl<PortalsSerializer>::Result> pimpl<PortalsSerializer>::importFile(shared_ptr<Portal> portal, const String &filename, const String &format, const boost::function<void (double)> &progressCallback)
{
	shared_ptr<File> file(OS_NEW File());
	if(file->open(filename, File::ofRead) == false)
		return null;

	return importStream(portal, file, format, progressCallback);
}

shared_ptr<pimpl<PortalsSerializer>::Result> pimpl<PortalsSerializer>::importStream(shared_ptr<Portal> portal, shared_ptr<IStream> stream, const String &format, const boost::function<void (double)> &progressCallback)
{
	if(portal == null)
	{
		OS_ASSERTFALSE();
		return null;
	}

	shared_ptr<Parser> parser(OS_NEW Parser(portal));
	if(importStream(parser, stream, progressCallback) == false)
		return null;

	return parser->getResult();
}

shared_ptr<pimpl<PortalsSerializer>::Result> pimpl<PortalsSerializer>::parsePortalLink(const String &filename, const boost::function<void (double)> &progressCallback)
{
	shared_ptr<File> file(OS_NEW File());
	if(file->open(filename, File::ofRead) == false)
		return null;

	return parsePortalLink(file, progressCallback);
}

shared_ptr<pimpl<PortalsSerializer>::Result> pimpl<PortalsSerializer>::parsePortalLink(shared_ptr<IStream> stream, const boost::function<void (double)> &progressCallback)
{
	shared_ptr<Parser> parser(OS_NEW Parser());
	// N.B.: in fase di estrazione del link il parser viene volutamente fermato causando un valore di ritorno "false"
	if(importStream(parser, stream, progressCallback) == false)
		return null;

	return parser->getResult();
}

void pimpl<PortalsSerializer>::fireProgressEvent(double percentage)
{
	OS_LOCK(m_eventsMutex);
	m_progressEvent(percentage);
}

shared_ptr<IStream> pimpl<PortalsSerializer>::initStream(shared_ptr<IStream> stream, bool read, const String &format)
{
	if(stream == null || stream->is_open() == false)
	{
		OS_ASSERTFALSE();
		return null;
	}

	if(stream->seekToBegin() == false)
	{
		OS_ASSERTFALSE();
		return null;
	}

	shared_ptr<StreamFilter> filter(OS_NEW StreamFilter(stream));

	if(read)
	{
		try
		{
			switch(CryptManager::instance()->detectCompressor(stream))
			{
			case compressorTypeUnknown:
											break;

			case compressorTypeZlib:		filter->addLayer(shared_ptr<StreamLayerZlib>(OS_NEW StreamLayerZlib()));
											break;

			case compressorTypeGzip:		filter->addLayer(shared_ptr<StreamLayerGzip>(OS_NEW StreamLayerGzip()));
											break;

			default:						OS_ASSERTFALSE();
											return null;
			}
		}
		catch(std::exception &e)
		{
			OS_LOG_ERROR(e.what());
			return null;
		}
	}
	else
	{
		if(format == _S("osiris"))
			filter->addLayer(shared_ptr<StreamLayerGzip>(OS_NEW StreamLayerGzip(compressionLevelHigh)));
	}

	OS_ASSERT(filter->position() == 0);
	return filter;
}

bool pimpl<PortalsSerializer>::importStream(shared_ptr<Parser> parser, shared_ptr<IStream> stream, const boost::function<void (double)> &progressCallback)
{
	OS_ASSERT(parser != null);

	boost::mutex::scoped_lock lock(m_taskMutex);

	resetData();

	stream = initStream(stream, true, _S("osiris"));
	if(stream == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	parser->setProgressCallback(boost::bind(&pimpl<PortalsSerializer>::handleParserProgress, this, stream, stream->size(), progressCallback));

	if(parser->parseStream(stream) == false)
		return false;

//	if(parser->getStopParser())
//		return false;

	return true;
}

void pimpl<PortalsSerializer>::resetData()
{
	boost::mutex::scoped_lock lock(m_dataMutex);
	m_progressPercentage = 0;
	m_stopped = false;
}

bool pimpl<PortalsSerializer>::handleParserProgress(shared_ptr<IStream> stream, uint64 totalSize, const boost::function<void (double)> &progressCallback)
{
	OS_ASSERT(stream != null);
	OS_ASSERT(progressCallback.empty() == false);

	uint64 position = stream->position();
	OS_ASSERT(position <= totalSize);

	double progressPercentage = totalSize > 0 ? (position / static_cast<double>(totalSize)) : 1;
	setProgressPercentage(progressPercentage);
	progressCallback(progressPercentage);

	return getStopped() == false;
}

shared_ptr<XMLNode> pimpl<PortalsSerializer>::exportObjectXML(shared_ptr<ObjectsIObject> object)
{
	if(object == null)
	{
		OS_ASSERTFALSE();
		return null;
	}

	DataTree objectData;
	if(object->write(objectData) == false)
	{
		OS_ASSERTFALSE();
		return null;
	}

	Buffer objectBuffer;
	if(objectData.write(objectBuffer, false) == false)
	{
		OS_ASSERTFALSE();
		return null;
	}

	shared_ptr<XMLNode> node(OS_NEW XMLNode(OS_PORTALSERIALIZER_XML_OBJECT));
	node->setAttributeString(OS_PORTALSERIALIZER_XML_OBJECT_ID, object->id->toUTF16());
	node->setAttributeBuffer(OS_PORTALSERIALIZER_XML_OBJECT_DATA, objectBuffer);

	return node;
}

bool pimpl<PortalsSerializer>::exportOsirisFormat(shared_ptr<IPortalDatabase> db, shared_ptr<Portal> portal, shared_ptr<IStream> stream, const boost::function<void (double)> &progressCallback)
{
	shared_ptr<XMLWriter> xmlWriter(OS_NEW XMLWriter());
	if(xmlWriter->openStream(stream) == false)
		return false;

	if(xmlWriter->writeDeclaration() == false)
		return false;

	shared_ptr<XMLNode> nodeRoot(OS_NEW XMLNode(OS_PORTALSERIALIZER_XML_ROOT));
	nodeRoot->setAttributeString(OS_PORTALSERIALIZER_XML_PORTAL_LINK, portal->generateInviteLink(false));
	if(xmlWriter->writeOpen(nodeRoot) == false)
		return false;

	xmlWriter->increaseIndent();

	shared_ptr<XMLNode> nodeObjects(OS_NEW XMLNode(OS_PORTALSERIALIZER_XML_OBJECTS));
	if(xmlWriter->writeOpen(nodeObjects) == false)
		return false;

	xmlWriter->increaseIndent();

	shared_ptr<DbSqlSelect> query(OS_NEW DbSqlSelect(DBTABLES::ENTRIES_TABLE));

	query->count = true;

	DataTable result;
	if(db->execute(query, result) == false)
		return false;

	uint32 totalObjects = static_cast<uint32>(*result[0][0]);

	query->count = false;

	// Compone la query per estrarre prima l'utente POV, poi gli altri utenti (nessun ordine particolare), poi le reputazioni, e poi gli altri oggetti.

	query->fields.add(DbSqlField(DBTABLES::ID));
	query->where.add(DBTABLES::ENTRIES::ID, Convert::toSQL(portal->getPovUserID().toUTF16()), DbSqlCondition::cfEqual | DbSqlCondition::cfAnd);
	
	shared_ptr<DbSqlSelect> objectsSelect2(OS_NEW DbSqlSelect(DBTABLES::ENTRIES_TABLE));
	objectsSelect2->fields.add(DbSqlField(DBTABLES::ID));
	objectsSelect2->where.add(DBTABLES::ENTRIES::TYPE, Convert::toSQL(static_cast<uint32>(portalObjectTypeUser)), DbSqlCondition::cfEqual | DbSqlCondition::cfAnd);
	objectsSelect2->where.add(DBTABLES::ENTRIES::ID, Convert::toSQL(portal->getPovUserID().toUTF16()), DbSqlCondition::cfDifferent | DbSqlCondition::cfAnd);

	shared_ptr<DbSqlSelect> objectsSelect3(OS_NEW DbSqlSelect(DBTABLES::ENTRIES_TABLE));
	objectsSelect3->fields.add(DbSqlField(DBTABLES::ID));
	objectsSelect3->where.add(DBTABLES::ENTRIES::TYPE, Convert::toSQL(static_cast<uint32>(portalObjectTypeReputation)), DbSqlCondition::cfEqual | DbSqlCondition::cfAnd);
	
	shared_ptr<DbSqlSelect> objectsSelect4(OS_NEW DbSqlSelect(DBTABLES::ENTRIES_TABLE));
	objectsSelect4->fields.add(DbSqlField(DBTABLES::ID));
	objectsSelect4->where.add(DBTABLES::ENTRIES::TYPE, Convert::toSQL(static_cast<uint32>(portalObjectTypeUser)), DbSqlCondition::cfDifferent | DbSqlCondition::cfAnd);
	objectsSelect4->where.add(DBTABLES::ENTRIES::TYPE, Convert::toSQL(static_cast<uint32>(portalObjectTypeReputation)), DbSqlCondition::cfDifferent | DbSqlCondition::cfAnd);
	
	objectsSelect3->createConnection(objectsSelect4, DbSqlSelect::ctUnionAll);
	objectsSelect2->createConnection(objectsSelect3, DbSqlSelect::ctUnionAll);
	query->createConnection(objectsSelect2, DbSqlSelect::ctUnionAll);		

	try
	{
		

		/*
		String sql;
		db->getConnection()->parse(query, sql);
		if(sql.empty())
		{
			OS_ASSERTFALSE();
			return false;
		}
		*/

		shared_ptr<IDbResult> result = db->getConnection()->query(query);
		if(result == null)
		{
			OS_ASSERTFALSE();
			return false;
		}

		DataTable table;
		result->init(table);
		DataTableRow row = table.addRow();

		uint32 parsedObject = 0;
		while(result->end() == false)
		{
			if(getStopped())
				return false;

			result->bind(row);

			ObjectID objectID = static_cast<String>(*row[0]).to_ascii();
			shared_ptr<ObjectsIObject> object = portal->getObject(db, objectID);
			if(object == null)
			{
				OS_ASSERTFALSE();
				return false;
			}

			shared_ptr<XMLNode> objectXML = exportObjectXML(object);
			if(objectXML == null)
			{
				OS_ASSERTFALSE();
				return false;
			}

			if(xmlWriter->writeNode(objectXML) == false)
				return false;

			result->moveNext();

			parsedObject++;

			// N.B.: il portale non è loccato in esclusivo in fase di esportazione pertanto è possibile che vengano inseriti nuovi oggetti...
			// Se si dovesse cambiare la modalità di loccaggio del portale, si può omettere questo controllo
			totalObjects = std::max<uint32>(parsedObject, totalObjects);

			double progressPercentage = parsedObject / static_cast<double>(totalObjects);
			setProgressPercentage(progressPercentage);
			progressCallback(progressPercentage);
		}

		OS_ASSERT(parsedObject == totalObjects);
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
		return false;
	}

	xmlWriter->decreaseIndent();

	if(xmlWriter->writeClose(nodeObjects) == false)
		return false;

	xmlWriter->decreaseIndent();
	
	if(xmlWriter->writeClose(nodeRoot) == false)
		return false;

	return true;
}

bool pimpl<PortalsSerializer>::exportGourceFormat(shared_ptr<IPortalDatabase> db, shared_ptr<Portal> portal, shared_ptr<IStream> stream, const boost::function<void (double)> &progressCallback)
{
	return true;
}

//////////////////////////////////////////////////////////////////////

PortalsSerializer::IResult::IResult()
{

}

PortalsSerializer::IResult::~IResult()
{

}

//////////////////////////////////////////////////////////////////////

PortalsSerializer::PortalsSerializer()
{

}

PortalsSerializer::~PortalsSerializer()
{

}

Locked<PortalsSerializer::ProgressEvent>::unique PortalsSerializer::getProgressEvent()
{
	return m_impl->getProgressEvent();
}

double PortalsSerializer::getProgressPercentage() const
{
	return m_impl->getProgressPercentage();
}

bool PortalsSerializer::getStopped() const
{
	return m_impl->getStopped();
}

void PortalsSerializer::setStopped(bool stopped)
{
	m_impl->setStopped(stopped);
}

bool PortalsSerializer::exportFile(shared_ptr<Portal> portal, const String &filename, const String &format)
{
	return m_impl->exportFile(portal, filename, format, boost::bind(&PortalsSerializer::onProgress, this, _1));
}

bool PortalsSerializer::exportStream(shared_ptr<Portal> portal, shared_ptr<IStream> stream, const String &format)
{
	return m_impl->exportStream(portal, stream, format, boost::bind(&PortalsSerializer::onProgress, this, _1));
}

shared_ptr<PortalsSerializer::IResult> PortalsSerializer::importFile(shared_ptr<Portal> portal, const String &filename, const String &format)
{
	return m_impl->importFile(portal, filename, format, boost::bind(&PortalsSerializer::onProgress, this, _1));
}

shared_ptr<PortalsSerializer::IResult> PortalsSerializer::importStream(shared_ptr<Portal> portal, shared_ptr<IStream> stream, const String &format)
{
	return m_impl->importStream(portal, stream, format, boost::bind(&PortalsSerializer::onProgress, this, _1));
}

shared_ptr<PortalsSerializer::IResult> PortalsSerializer::parsePortalLink(const String &filename)
{
	return m_impl->parsePortalLink(filename, boost::bind(&PortalsSerializer::onProgress, this, _1));
}

shared_ptr<PortalsSerializer::IResult> PortalsSerializer::parsePortalLink(shared_ptr<IStream> stream)
{
	return m_impl->parsePortalLink(stream, boost::bind(&PortalsSerializer::onProgress, this, _1));
}

void PortalsSerializer::onProgress(double percentage)
{
	m_impl->fireProgressEvent(percentage);
}

bool PortalsSerializer::exportOsirisFormat(shared_ptr<IPortalDatabase> db, shared_ptr<Portal> portal, shared_ptr<IStream> stream, const boost::function<void (double)> &progressCallback)
{
	return m_impl->exportOsirisFormat(db, portal, stream, progressCallback);
}

bool PortalsSerializer::exportGourceFormat(shared_ptr<IPortalDatabase> db, shared_ptr<Portal> portal, shared_ptr<IStream> stream, const boost::function<void (double)> &progressCallback)
{
	return m_impl->exportGourceFormat(db, portal, stream, progressCallback);
}



//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
