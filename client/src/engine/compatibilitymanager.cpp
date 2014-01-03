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
#include "compatibilityrazor.h"
#include "compatibilitymanager.h"
#include "cryptmanager.h"
#include "dataaccount.h"
#include "datatable.h"
#include "dbdatabasessystem.h"
#include "file.h"
#include "htmlparser.h"
#include "id.h"
#include "idbconnection.h"
#include "idbdriver.h"
#include "idbresult.h"
#include "idesession.h"
#include "iportaldatabase.h"
#include "dbschemaschema.h"
#include "engine.h"
#include "filesystem.h"
#include "log.h"
#include "notificationsmanager.h"
#include "objectsirevisionable.h"
#include "objectsuser.h"
#include "options.h"
#include "osirislink.h"
#include "platformmanager.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalsportalsdirectory.h"
#include "regexmanager.h"
#include "xmlnode.h"
#include "xmlnodes.h"

//////////////////////////////////////////////////////////////////////

/*
IMPORTANT NOTE:

This class manage the upgrading from 0.X to 1.X versions.
Maybe drop in future.
*/

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

CompatibilityManager::CompatibilityManager()
{		
	/*
	ObjectID oID;
	EntityID eID;
		
	ObjectID o2 = eID;
	EntityID e2 = oID;
	*/
}

CompatibilityManager::~CompatibilityManager()
{

}

bool CompatibilityManager::razorUpgrade(const String &folder)
{
	try
	{
		if(FileSystem::instance()->directoryExists(folder))
		{
			StringList portals;
			FileSystem::instance()->getDirectories(folder, portals, false);
			for(StringList::const_iterator i = portals.begin(); i != portals.end(); ++i)
			{
				// 000000018F94C9554C8B227CF000C6E30EB281AEAE1A77E7
				String name = *i;
				if(name.length() == 48)
				{
					// Portal < Razor
					std::string oldID = name.to_ascii();

					// Options
					String oldPath = utils::standardisePath(folder + oldID);
					shared_ptr<PortalOptions> options(OS_NEW PortalOptions());
					// Carica le impostazioni del portale
					if(options->readFromPath(oldPath) == false)
						return false;
					
					shared_ptr<IDbDriver> driver = DatabasesSystem::instance()->getDriver("sqlite");

					// Query Database
					shared_ptr<IDbConnection> connection = driver->createConnection(oldID, driver->createOptions());

					connection->open();
					
					String portalName = options->getName();
					//String portalDescription = static_cast<String>(options->getOptionValue("description"));


					NotificationsManager::instance()->notify(_S("Migration of '") + portalName + _S("' to 1.0 series"));
					PlatformManager::instance()->sleep(500);

					//ObjectID userID = static_cast<String>(m_portalOptions.getOption(options::users_reference)).to_ascii();
					ObjectID userID = static_cast<String>(options->getOptionValue("users.reference")).to_ascii();

					//bool monarchic = (options->getPortalID().getPortalType() == portalTypeMonarchic);
					ID oldPortalID(oldID);
					uint8 portalType = OS_UINT8_LO(oldPortalID.getHeader().getFirst());
					bool monarchic = (portalType == 1);

					// Calcolo UserID.
					// Se non lo posso dedurre, uso il primo che trovo loggato.					
					if(userID.empty())
					{
						if(monarchic)
						{
							//userID = options->getPortalID().getAdministratorID();
#ifdef OS_NOOBJECTID
							userID = oldPortalID.getHash();
#else
							userID = ObjectID::generate(portalObjectTypeUser, oldPortalID.getHash());
#endif
						}
						else
						{
							String sql = String::EMPTY;
							sql += _S(" select tp.profile, tu.reference ");
							sql += _S(" from ");
							sql += _S(" os_snapshot_profiles tp, ");
							sql += _S(" os_snapshot_users tu ");
							sql += _S(" where  ");
							sql += _S(" tp.profile=tu.profile and ");
							sql += _S(" tu.score=1 and ");
							sql += _S(" tu.follow=1 and ");
							sql += _S(" tu.description='Auto valutation'");

							shared_ptr<IDbResult> result = connection->query(sql);
							DataTable table;
							result->init(table);
							DataTableRow row = table.addRow();

							while(result->end() == false)
							{
								result->bind(row);
								String objectID = static_cast<String>(*row[1]);

								userID = objectID.to_ascii();

								result->moveNext();

								break;
							}
						}
					}
					
#ifdef OS_NOOBJECTID
					PovID povID = userID;
#else
					PovID povID = userID.getHash();
#endif
					
					if(userID.empty() == false)
					{
						// Clean old snapshot
						connection->execute(_S("delete from os_snapshot_objects"));
						connection->execute(_S("delete from os_snapshot_users"));
						connection->execute(_S("delete from os_snapshot_profiles"));
						connection->execute(_S("delete from os_discussions_stats"));
						//connection->execute(_S("delete from os_forums_forum_stats"));
						//connection->execute(_S("delete from os_forums_section_stats"));
						connection->execute(_S("delete from os_polls_stats"));
						connection->execute(_S("delete from os_polls_options_stats"));
						connection->execute(_S("delete from os_users_stats"));

						// Old
						connection->execute(_S("drop table if exists os_forums_forum_stats"));
						connection->execute(_S("drop table if exists os_forums_section_stats"));

						// Reset acceptable
						connection->execute(_S("update os_entries set rank=-2"));

						// Clean signatures
#ifdef OS_NEWSIGNATURE
						connection->execute(_S("update os_attributes set signature=x'00'"));
						connection->execute(_S("update os_avatars set signature=x'00'"));
						connection->execute(_S("update os_calendar_events set signature=x'00'"));
						connection->execute(_S("update os_files set signature=x'00'"));
						connection->execute(_S("update os_instances set signature=x'00'"));
						connection->execute(_S("update os_models set signature=x'00'"));
						connection->execute(_S("update os_polls set signature=x'00'"));
						connection->execute(_S("update os_polls_options set signature=x'00'"));
						connection->execute(_S("update os_polls_votes set signature=x'00'"));
						connection->execute(_S("update os_posts set signature=x'00'"));
						connection->execute(_S("update os_private_messages set signature=x'00'"));
						connection->execute(_S("update os_reputations set signature=x'00'"));
						connection->execute(_S("update os_sections set signature=x'00'"));
						connection->execute(_S("update os_tags set signature=x'00'"));
						connection->execute(_S("update os_texts set signature=x'00'"));
						connection->execute(_S("update os_users set signature=x'00'"));
						connection->execute(_S("update os_votes set signature=x'00'"));
#endif
						

						// identificarli:
						// select * from os_users where signature=x'00'		

						
					}
					

					connection->close();

					if(userID.empty() == false)
					{					
						String newPortalIDKey = oldID.substr(8);
						PortalID newPortalID = CryptManager:: instance()->SHA(newPortalIDKey.buffer(), newPortalIDKey.buffer_size()).toHex();

						String newPovID = Portal::generatePovID(newPortalID, povID);
						String newPath = utils::standardisePath(folder + newPovID);

						options->setPortalID(newPortalID);
						options->setPovID(povID);
						options->setDatabaseName(newPovID);

						FileSystem::instance()->ensureDirectory(newPath);
					
						options->writeToPath(newPath);

						String sqlitePluginID = _S("532E9E0A68EB22E08240965CFA9366DFA6A26A62");
						String databaseRootPath = utils::makeFolderPath(Options::instance()->getDataPath(), utils::makeFolderPath(OS_STORE_PATH, sqlitePluginID));
						String oldDatabasePath = utils::makeFilePath(databaseRootPath,oldID) + _S(".db");
						String newDatabasePath = utils::makeFilePath(databaseRootPath,newPovID) + _S(".db");
		
						FileSystem::instance()->copyFile(oldDatabasePath, newDatabasePath);						

						FileSystem::instance()->remove(oldDatabasePath);
						FileSystem::instance()->removeDirectory(oldPath);
					}
					else
					{
						OS_LOG_ERROR("Unable to migrate portal '" + portalName + "', ID:" + oldID + ". Contact our forum, thanks.");
					}

				}			
			}
		}
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
		return false;
	}
	catch(...)
	{
		OS_LOG_ERROR(_S("Unknown error during Razor upgrade.'"));
		return false;
	}

	return true;
}

bool CompatibilityManager::razorPortalDatabaseUpgrade(const shared_ptr<IPortalDatabase> &database)
{
	PovID povID = database->getPortal()->getPovID();
#ifdef OS_NOOBJECTID
	ObjectID userID = povID;
#else
	ObjectID userID = ObjectID::generate(portalObjectTypeUser, povID.getString());
#endif

	migrateAccounts(database);

	
	String portalName = database->getPortal()->getOptions()->getName();
	String portalDescription = static_cast<String>(database->getPortal()->getOptions()->getOptionValue("description"));

	// Clean old snapshot
	database->execute(_S("delete from os_snapshot_objects"));
	database->execute(_S("delete from os_snapshot_users"));
	//database->execute(_S("delete from os_snapshot_profiles"));
	database->execute(_S("delete from os_discussions_stats"));
	//database->execute(_S("delete from os_forums_forum_stats"));
	//database->execute(_S("delete from os_forums_section_stats"));
	database->execute(_S("delete from os_polls_stats"));
	database->execute(_S("delete from os_polls_options_stats"));
	database->execute(_S("delete from os_users_stats"));

	// Old
	database->execute(_S("drop table if exists os_forums_forum_stats"));
	database->execute(_S("drop table if exists os_forums_section_stats"));

	// Reset acceptable
	database->execute(_S("update os_entries set rank=-2"));

	// Clean signatures
	database->execute(_S("update os_attributes set signature=x'00'"));
	database->execute(_S("update os_avatars set signature=x'00'"));
	database->execute(_S("update os_calendar_events set signature=x'00'"));
	database->execute(_S("update os_files set signature=x'00'"));
	database->execute(_S("update os_instances set signature=x'00'"));
	database->execute(_S("update os_models set signature=x'00'"));
	database->execute(_S("update os_polls set signature=x'00'"));
	database->execute(_S("update os_polls_options set signature=x'00'"));
	database->execute(_S("update os_polls_votes set signature=x'00'"));
	database->execute(_S("update os_posts set signature=x'00'"));
	database->execute(_S("update os_private_messages set signature=x'00'"));
	database->execute(_S("update os_reputations set signature=x'00'"));
	database->execute(_S("update os_sections set signature=x'00'"));
	database->execute(_S("update os_tags set signature=x'00'"));
	database->execute(_S("update os_texts set signature=x'00'"));
	database->execute(_S("update os_users set signature=x'00'"));
	database->execute(_S("update os_votes set signature=x'00'"));

	// Other conversions
	database->execute(_S("update os_entries set entity=null where entity='';\r\n"));
	database->execute(_S("update os_calendar_events set entity=null where entity='';\r\n"));
	database->execute(_S("update os_files set entity=null where entity='';\r\n"));
	database->execute(_S("update os_instances set entity=null where entity='';\r\n"));
	database->execute(_S("update os_models set entity=null where entity='';\r\n"));
	database->execute(_S("update os_polls set entity=null where entity='';\r\n"));
	database->execute(_S("update os_polls_options set entity=null where entity='';\r\n"));
	database->execute(_S("update os_posts set entity=null where entity='';\r\n"));
	database->execute(_S("update os_sections set entity=null where entity='';\r\n"));
	database->execute(_S("update os_tags set entity=null where entity='';\r\n"));
	database->execute(_S("update os_texts set entity=null where entity='';\r\n"));
		
	database->execute(_S("update os_attributes set pov='") + povID.toUTF16() + _S("';\r\n"));
	database->execute(_S("update os_avatars set pov='") + povID.toUTF16() + _S("';\r\n"));
	database->execute(_S("update os_calendar_events set pov='") + povID.toUTF16() + _S("';\r\n"));
	database->execute(_S("update os_files set pov='") + povID.toUTF16() + _S("';\r\n"));
	database->execute(_S("update os_instances set pov='") + povID.toUTF16() + _S("';\r\n"));
	database->execute(_S("update os_models set pov='") + povID.toUTF16() + _S("';\r\n"));
	database->execute(_S("update os_polls set pov='") + povID.toUTF16() + _S("';\r\n"));
	database->execute(_S("update os_polls_options set pov='") + povID.toUTF16() + _S("';\r\n"));
	database->execute(_S("update os_polls_votes set pov='") + povID.toUTF16() + _S("';\r\n"));
	database->execute(_S("update os_posts set pov='") + povID.toUTF16() + _S("';\r\n"));
	database->execute(_S("update os_private_messages set pov='") + povID.toUTF16() + _S("';\r\n"));
	database->execute(_S("update os_reputations set pov='") + povID.toUTF16() + _S("';\r\n"));
	database->execute(_S("update os_sections set pov='") + povID.toUTF16() + _S("';\r\n"));
	database->execute(_S("update os_tags set pov='") + povID.toUTF16() + _S("';\r\n"));
	database->execute(_S("update os_texts set pov='") + povID.toUTF16() + _S("';\r\n"));
	database->execute(_S("update os_users set pov='") + povID.toUTF16() + _S("';\r\n"));
	database->execute(_S("update os_votes set pov='") + povID.toUTF16() + _S("';\r\n"));

	database->execute(_S("update os_entries set entity=ifnull(entity,id);\r\n"));

	database->execute(_S("update os_calendar_events set entity=ifnull(entity,id);\r\n"));
	database->execute(_S("update os_files set entity=ifnull(entity,id);\r\n"));
	database->execute(_S("update os_instances set entity=ifnull(entity,id);\r\n"));
	database->execute(_S("update os_models set entity=ifnull(entity,id);\r\n"));
	database->execute(_S("update os_polls set entity=ifnull(entity,id);\r\n"));
	database->execute(_S("update os_polls_options set entity=ifnull(entity,id);\r\n"));
	database->execute(_S("update os_posts set entity=ifnull(entity,id);\r\n"));
	database->execute(_S("update os_sections set entity=ifnull(entity,id);\r\n"));
	database->execute(_S("update os_tags set entity=ifnull(entity,id);\r\n"));
	database->execute(_S("update os_texts set entity=ifnull(entity,id);\r\n"));
				
	database->execute(_S("update os_calendar_events set entity_author=(select author from os_entries where os_entries.id = os_calendar_events.entity);\r\n"));
	database->execute(_S("update os_files set entity_author=(select author from os_entries where os_entries.id = os_files.entity);\r\n"));
	database->execute(_S("update os_instances set entity_author=(select author from os_entries where os_entries.id = os_instances.entity);\r\n"));
	database->execute(_S("update os_models set entity_author=(select author from os_entries where os_entries.id = os_models.entity);\r\n"));
	database->execute(_S("update os_polls set entity_author=(select author from os_entries where os_entries.id = os_polls.entity);\r\n"));
	database->execute(_S("update os_polls_options set entity_author=(select author from os_entries where os_entries.id = os_polls_options.entity);\r\n"));
	database->execute(_S("update os_posts set entity_author=(select author from os_entries where os_entries.id = os_posts.entity);\r\n"));
	database->execute(_S("update os_sections set entity_author=(select author from os_entries where os_entries.id = os_sections.entity);\r\n"));
	database->execute(_S("update os_tags set entity_author=(select author from os_entries where os_entries.id = os_tags.entity);\r\n"));
	database->execute(_S("update os_texts set entity_author=(select author from os_entries where os_entries.id = os_texts.entity);\r\n"));

	database->execute(_S("update os_calendar_events set entity_date=(select submit_date from os_entries where os_entries.id = os_calendar_events.entity);\r\n"));
	database->execute(_S("update os_files set entity_date=(select submit_date from os_entries where os_entries.id = os_files.entity);\r\n"));
	database->execute(_S("update os_instances set entity_date=(select submit_date from os_entries where os_entries.id = os_instances.entity);\r\n"));
	database->execute(_S("update os_models set entity_date=(select submit_date from os_entries where os_entries.id = os_models.entity);\r\n"));
	database->execute(_S("update os_polls set entity_date=(select submit_date from os_entries where os_entries.id = os_polls.entity);\r\n"));
	database->execute(_S("update os_polls_options set entity_date=(select submit_date from os_entries where os_entries.id = os_polls_options.entity);\r\n"));
	database->execute(_S("update os_posts set entity_date=(select submit_date from os_entries where os_entries.id = os_posts.entity);\r\n"));
	database->execute(_S("update os_sections set entity_date=(select submit_date from os_entries where os_entries.id = os_sections.entity);\r\n"));
	database->execute(_S("update os_tags set entity_date=(select submit_date from os_entries where os_entries.id = os_tags.entity);\r\n"));
	database->execute(_S("update os_texts set entity_date=(select submit_date from os_entries where os_entries.id = os_texts.entity);\r\n"));

	database->execute(_S("update os_calendar_events set entity_parent=(select parent from os_entries where os_entries.id = os_calendar_events.entity);\r\n"));
	database->execute(_S("update os_files set entity_parent=(select parent from os_entries where os_entries.id = os_files.entity);\r\n"));
	database->execute(_S("update os_instances set entity_parent=(select parent from os_entries where os_entries.id = os_instances.entity);\r\n"));
	database->execute(_S("update os_models set entity_parent=(select parent from os_entries where os_entries.id = os_models.entity);\r\n"));
	database->execute(_S("update os_polls set entity_parent=(select parent from os_entries where os_entries.id = os_polls.entity);\r\n"));
	database->execute(_S("update os_polls_options set entity_parent=(select parent from os_entries where os_entries.id = os_polls_options.entity);\r\n"));
	database->execute(_S("update os_posts set entity_parent=(select parent from os_entries where os_entries.id = os_posts.entity);\r\n"));
	database->execute(_S("update os_sections set entity_parent=(select parent from os_entries where os_entries.id = os_sections.entity);\r\n"));
	database->execute(_S("update os_tags set entity_parent=(select parent from os_entries where os_entries.id = os_tags.entity);\r\n"));
	database->execute(_S("update os_texts set entity_parent=(select parent from os_entries where os_entries.id = os_texts.entity);\r\n"));

	database->execute(_S("CREATE INDEX os_files_index_entity_temp on os_files (entity asc);\r\n"));
	database->execute(_S("CREATE INDEX os_texts_index_entity_temp on os_texts (entity asc);\r\n"));
	database->execute(_S("CREATE INDEX os_posts_index_entity_temp on os_posts (entity asc);\r\n"));

	database->execute(_S("update os_entries set entity_author=(select entity_author from os_calendar_events where os_entries.entity = os_calendar_events.entity) where type=16;\r\n"));
	database->execute(_S("update os_entries set entity_author=(select entity_author from os_files where os_entries.entity = os_files.entity) where type=8;\r\n"));
	database->execute(_S("update os_entries set entity_author=(select entity_author from os_instances where os_entries.entity = os_instances.entity) where type=4;\r\n"));
	database->execute(_S("update os_entries set entity_author=(select entity_author from os_models where os_entries.entity = os_models.entity) where type=15;\r\n"));
	database->execute(_S("update os_entries set entity_author=(select entity_author from os_polls where os_entries.entity = os_polls.entity) where type=12;\r\n"));
	database->execute(_S("update os_entries set entity_author=(select entity_author from os_polls_options where os_entries.entity = os_polls_options.entity) where type=13;\r\n"));
	database->execute(_S("update os_entries set entity_author=(select entity_author from os_posts where os_entries.entity = os_posts.entity) where type=6;\r\n"));
	database->execute(_S("update os_entries set entity_author=(select entity_author from os_sections where os_entries.entity = os_sections.entity) where type=3;\r\n"));
	database->execute(_S("update os_entries set entity_author=(select entity_author from os_tags where os_entries.entity = os_tags.entity) where type=9;\r\n"));
	database->execute(_S("update os_entries set entity_author=(select entity_author from os_texts where os_entries.entity = os_texts.entity) where type=5;\r\n"));

	database->execute(_S("update os_entries set entity_date=(select entity_date from os_calendar_events where os_entries.entity = os_calendar_events.entity) where type=16;\r\n"));
	database->execute(_S("update os_entries set entity_date=(select entity_date from os_files where os_entries.entity = os_files.entity) where type=8;\r\n"));
	database->execute(_S("update os_entries set entity_date=(select entity_date from os_instances where os_entries.entity = os_instances.entity) where type=4;\r\n"));
	database->execute(_S("update os_entries set entity_date=(select entity_date from os_models where os_entries.entity = os_models.entity) where type=15;\r\n"));
	database->execute(_S("update os_entries set entity_date=(select entity_date from os_polls where os_entries.entity = os_polls.entity) where type=12;\r\n"));
	database->execute(_S("update os_entries set entity_date=(select entity_date from os_polls_options where os_entries.entity = os_polls_options.entity) where type=13;\r\n"));
	database->execute(_S("update os_entries set entity_date=(select entity_date from os_posts where os_entries.entity = os_posts.entity) where type=6;\r\n"));
	database->execute(_S("update os_entries set entity_date=(select entity_date from os_sections where os_entries.entity = os_sections.entity) where type=3;\r\n"));
	database->execute(_S("update os_entries set entity_date=(select entity_date from os_tags where os_entries.entity = os_tags.entity) where type=9;\r\n"));
	database->execute(_S("update os_entries set entity_date=(select entity_date from os_texts where os_entries.entity = os_texts.entity) where type=5;\r\n"));

	database->execute(_S("update os_entries set entity=null,entity_author=null,entity_date=null where type not in (16,8,4,15,12,13,6,3,9,5);\r\n"));

	database->execute(_S("drop index os_files_index_entity_temp;\r\n"));
	database->execute(_S("drop index os_texts_index_entity_temp;\r\n"));
	database->execute(_S("drop index os_posts_index_entity_temp;\r\n"));

	// Object header cleaning - Entity
	database->execute(_S("update os_entries set entity=substr(entity,9);\r\n"));
	database->execute(_S("update os_calendar_events set entity=substr(entity,9);\r\n"));
	database->execute(_S("update os_files set entity=substr(entity,9);\r\n"));
	database->execute(_S("update os_instances set entity=substr(entity,9);\r\n"));
	database->execute(_S("update os_models set entity=substr(entity,9);\r\n"));
	database->execute(_S("update os_polls set entity=substr(entity,9);\r\n"));
	database->execute(_S("update os_polls_options set entity=substr(entity,9);\r\n"));
	database->execute(_S("update os_posts set entity=substr(entity,9);\r\n"));
	database->execute(_S("update os_sections set entity=substr(entity,9);\r\n"));
	database->execute(_S("update os_tags set entity=substr(entity,9);\r\n"));
	database->execute(_S("update os_texts set entity=substr(entity,9);\r\n"));

	database->execute(_S("update os_entries set parent=substr(parent,9);\r\n"));
	database->execute(_S("update os_calendar_events set parent=substr(parent,9);\r\n"));
	database->execute(_S("update os_files set parent=substr(parent,9);\r\n"));
	database->execute(_S("update os_instances set parent=substr(parent,9);\r\n"));
	database->execute(_S("update os_models set parent=substr(parent,9);\r\n"));
	database->execute(_S("update os_polls set parent=substr(parent,9);\r\n"));
	database->execute(_S("update os_polls_options set parent=substr(parent,9);\r\n"));
	database->execute(_S("update os_posts set parent=substr(parent,9);\r\n"));
	database->execute(_S("update os_sections set parent=substr(parent,9);\r\n"));
	database->execute(_S("update os_tags set parent=substr(parent,9);\r\n"));
	database->execute(_S("update os_texts set parent=substr(parent,9);\r\n"));

	database->execute(_S("update os_calendar_events set entity_parent=substr(entity_parent,9);\r\n"));
	database->execute(_S("update os_files set entity_parent=substr(entity_parent,9);\r\n"));
	database->execute(_S("update os_instances set entity_parent=substr(entity_parent,9);\r\n"));
	database->execute(_S("update os_models set entity_parent=substr(entity_parent,9);\r\n"));
	database->execute(_S("update os_polls set entity_parent=substr(entity_parent,9);\r\n"));
	database->execute(_S("update os_polls_options set entity_parent=substr(entity_parent,9);\r\n"));
	database->execute(_S("update os_posts set entity_parent=substr(entity_parent,9);\r\n"));
	database->execute(_S("update os_sections set entity_parent=substr(entity_parent,9);\r\n"));
	database->execute(_S("update os_tags set entity_parent=substr(entity_parent,9);\r\n"));
	database->execute(_S("update os_texts set entity_parent=substr(entity_parent,9);\r\n"));

	// Object header cleaning - ObjectID
	database->execute(_S("update os_entries set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_entries set author=substr(author,9) where length(author)=48;\r\n"));
	database->execute(_S("update os_entries set entity_author=substr(entity_author,9) where length(entity_author)=48;\r\n"));

	database->execute(_S("update os_attributes set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_attributes set author=substr(author,9) where length(author)=48;\r\n"));

	database->execute(_S("update os_avatars set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_avatars set author=substr(author,9) where length(author)=48;\r\n"));

	database->execute(_S("update os_local_messages set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_local_messages set author=substr(author,9) where length(author)=48;\r\n"));

	database->execute(_S("update os_polls_votes set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_polls_votes set author=substr(author,9) where length(author)=48;\r\n"));
	database->execute(_S("update os_polls_votes set reference=substr(reference,9) where length(reference)=48;\r\n"));

	database->execute(_S("update os_votes set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_votes set author=substr(author,9) where length(author)=48;\r\n"));
	database->execute(_S("update os_votes set reference=substr(reference,9) where length(reference)=48;\r\n"));

	database->execute(_S("update os_private_messages set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_private_messages set author=substr(author,9) where length(author)=48;\r\n"));
	database->execute(_S("update os_private_messages set adressee=substr(adressee,9) where length(adressee)=48;\r\n"));

	database->execute(_S("update os_reputations set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_reputations set author=substr(author,9) where length(author)=48;\r\n"));
	database->execute(_S("update os_reputations set reference=substr(reference,9) where length(reference)=48;\r\n"));

	database->execute(_S("update os_users set id=substr(id,9) where length(id)=48;\r\n"));

	database->execute(_S("update os_calendar_events set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_calendar_events set author=substr(author,9) where length(author)=48;\r\n"));
	database->execute(_S("update os_calendar_events set entity_author=substr(entity_author,9) where length(entity_author)=48;\r\n"));
	database->execute(_S("update os_files set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_files set author=substr(author,9) where length(author)=48;\r\n"));
	database->execute(_S("update os_files set entity_author=substr(entity_author,9) where length(entity_author)=48;\r\n"));
	database->execute(_S("update os_instances set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_instances set author=substr(author,9) where length(author)=48;\r\n"));
	database->execute(_S("update os_instances set entity_author=substr(entity_author,9) where length(entity_author)=48;\r\n"));
	database->execute(_S("update os_models set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_models set author=substr(author,9) where length(author)=48;\r\n"));
	database->execute(_S("update os_models set entity_author=substr(entity_author,9) where length(entity_author)=48;\r\n"));
	database->execute(_S("update os_polls set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_polls set author=substr(author,9) where length(author)=48;\r\n"));
	database->execute(_S("update os_polls set entity_author=substr(entity_author,9) where length(entity_author)=48;\r\n"));
	database->execute(_S("update os_polls_options set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_polls_options set author=substr(author,9) where length(author)=48;\r\n"));
	database->execute(_S("update os_polls_options set entity_author=substr(entity_author,9) where length(entity_author)=48;\r\n"));
	database->execute(_S("update os_posts set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_posts set author=substr(author,9) where length(author)=48;\r\n"));
	database->execute(_S("update os_posts set entity_author=substr(entity_author,9) where length(entity_author)=48;\r\n"));
	database->execute(_S("update os_sections set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_sections set author=substr(author,9) where length(author)=48;\r\n"));
	database->execute(_S("update os_sections set entity_author=substr(entity_author,9) where length(entity_author)=48;\r\n"));
	database->execute(_S("update os_tags set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_tags set author=substr(author,9) where length(author)=48;\r\n"));
	database->execute(_S("update os_tags set entity_author=substr(entity_author,9) where length(entity_author)=48;\r\n"));
	database->execute(_S("update os_texts set id=substr(id,9) where length(id)=48;\r\n"));
	database->execute(_S("update os_texts set author=substr(author,9) where length(author)=48;\r\n"));
	database->execute(_S("update os_texts set entity_author=substr(entity_author,9) where length(entity_author)=48;\r\n"));
	
	// Special for Osiris Official
	database->execute(_S("update os_entries set author='3CCB3260950B80505CBB062C0B9B65E7028BD53F' where author='EFA5BEDD264F19D35BA92F05E503AD477D432E9F' and type=3;\r\n"));
	database->execute(_S("update os_entries set author='3CCB3260950B80505CBB062C0B9B65E7028BD53F' where author='EFA5BEDD264F19D35BA92F05E503AD477D432E9F' and type=8;\r\n"));
	database->execute(_S("update os_files set author='3CCB3260950B80505CBB062C0B9B65E7028BD53F',entity_author='3CCB3260950B80505CBB062C0B9B65E7028BD53F' where author='EFA5BEDD264F19D35BA92F05E503AD477D432E9F';\r\n"));
	database->execute(_S("update os_sections set author='3CCB3260950B80505CBB062C0B9B65E7028BD53F',entity_author='3CCB3260950B80505CBB062C0B9B65E7028BD53F' where author='EFA5BEDD264F19D35BA92F05E503AD477D432E9F';\r\n"));

	// Cleaning... issue found in some old portals.
	database->execute(_S("delete from os_models where id not in (select id from os_entries);\r\n"));
	database->execute(_S("drop table if exists os_commentables_stats;\r\n"));

	//if(database->execute(patch) == false)
	//	return false;

	// Default POV Options
	{
		String povOptions;
		povOptions += _S("<options>");
		povOptions += _S("<option name=\"acp.objects.allow_unsigned\" value=\"true\"/>");
		povOptions += _S("<option name=\"acp.portal.name\" value=\"") + HtmlParser::instance()->encode(portalName) + _S("\"/>");
		povOptions += _S("<option name=\"acp.portal.description\" value=\"") + HtmlParser::instance()->encode(portalDescription) + _S("\"/>");						
		povOptions += _S("</options>");

		String sql = _S("update os_users set options=") + Convert::toSQL(povOptions) + _S(" where id=") + Convert::toSQL(userID.toUTF16()) + _S(";");
		database->execute(sql);
	}

	// Rename changed ID
	renameIDS(database);

	// Update contents	
	{
		if(upgradeContentOML(database, _S("os_calendar_events"), _S("content"), false) == false)
			return false;
		if(upgradeContentOML(database, _S("os_files"), _S("content"), false) == false)
			return false;
		if(upgradeContentOML(database, _S("os_models"), _S("content"), false) == false)
			return false;
		if(upgradeContentOML(database, _S("os_polls"), _S("content"), false) == false)
			return false;
		if(upgradeContentOML(database, _S("os_posts"), _S("content"), false) == false)
			return false;
		if(upgradeContentOML(database, _S("os_texts"), _S("content"), false) == false)
			return false;
		if(upgradeContentOML(database, _S("os_users"), _S("mark"), false) == false)
			return false;
		if(upgradeContentOML(database, _S("os_instances"), _S("data"), true) == false)
			return false;
		if(upgradeContentOML(database, _S("os_sections"), _S("data"), true) == false)
			return false;
		if(upgradeContentOML(database, _S("os_sections"), _S("component"), false) == false)
			return false;
		if(upgradeContentOML(database, _S("os_instances"), _S("module"), false) == false)
			return false;
	}	

	String filename = utils::makeFilePath(Options::instance()->getLogPath(), _S("migration_") + database->getPortal()->getPortalID().toWide() + _S(".log"));

	shared_ptr<File> file(OS_NEW File());
	if(file->open(filename, File::ofWrite) == false)
		return false;

	std::string debugAscii = m_DebugLog.to_ascii();
	file->write(debugAscii.data(), static_cast<uint32>(debugAscii.size()));

	file->close();

	return true;
}

bool CompatibilityManager::upgradeContentOML(const shared_ptr<IPortalDatabase> &database, const String &tableName, const String& fieldName, bool xmlEncoded)
{
	NotificationsManager::instance()->notify(_S("Migration (OML texts) of '") + database->getPortal()->getName() + _S("' to 1.0 series"));

	DataTable result;
	String sql = _S("select id,") + fieldName + _S(" as oml from ") + tableName;
	//database->execute(sql,result);

	shared_ptr<IDbResult> result2 = database->getConnection()->query(sql);
	result2->init(result);
	DataTableRow row = result.addRow();

	//String patch;

	//for(uint32 r=0;r<result.rows();r++)
	while(result2->end() == false)
	{
		result2->bind(row);

		//String id = result.get(r,_S("id"));
		//String oml = result.get(r,_S("oml"));
		String id = *row[_S("id")];
		String omlOrig = *row[_S("oml")];

		String oldOml = omlOrig;

		// portal=0D2C4E505C1D98B221E222CABE4E167BECB2D669
		
#ifdef OS_NOOBJECTID
		omlOrig = RegexManager::instance()->replace(omlOrig, _S("[0-1][0-9]0[0-1]000[0-4]([0-9A-F]{40})"), _S("$1"), false);
#else
		omlOrig = RegexManager::instance()->replace(omlOrig, _S("[0-1][0-9]0[0-1]000[2-4]([0-9A-F]{40})"), _S("$1"), false);
#endif

		String oml = omlOrig;

		/*
		bool xmlEncoded = false;

		if(oml.starts_with(_S("<?xml version=\"1.0\" encoding=\"UTF-16\" standalone=\"no\" ?>")))
		{
			oml = HtmlParser::instance()->decode(omlOrig);
			xmlEncoded = true;
		}*/
		if(xmlEncoded)
			oml = HtmlParser::instance()->decode(omlOrig);

		int nReplace = 0;
		String::size_type lastPosLink = 0;
		for(;;)
		{
			nReplace++;
			String::size_type posLink = oml.find(_S("osiris://"),lastPosLink);
			if(posLink == String::npos)
				break;

			String::size_type posEnd = String::npos;

			{
				String::size_type posEndC = oml.find(_S("\""), posLink+1);
				if( (posEndC != String::npos) && ( (posEndC<posEnd) || (posEnd == String::npos) ) ) posEnd = posEndC;
			}
			{
				String::size_type posEndC = oml.find(_S("'"), posLink+1);
				if( (posEndC != String::npos) && ( (posEndC<posEnd) || (posEnd == String::npos) ) ) posEnd = posEndC;
			}
			{
				String::size_type posEndC = oml.find(_S("["), posLink+1);
				if( (posEndC != String::npos) && ( (posEndC<posEnd) || (posEnd == String::npos) ) ) posEnd = posEndC;
			}
			{
				String::size_type posEndC = oml.find(_S("]"), posLink+1);
				if( (posEndC != String::npos) && ( (posEndC<posEnd) || (posEnd == String::npos) ) ) posEnd = posEndC;
			}

			String linkEx = oml.substr(posLink);

			bool valid = true;
			bool warning = true;

			if(posEnd == String::npos)
				valid = false;
			
			if(valid)
			{			
				String link = oml.substr(posLink, posEnd-posLink);

				if( (link == _S("osiris://")) )
				{
					valid = false;
					warning = false;
				}
				else if( (link == _S("osiris://.")) )
				{
					valid = false;
					warning = false;
				}
				else
				{
					OsirisLink osirisLink(link.to_ascii());
					if(osirisLink.isValid() == false)
						valid = false;
					else
					{
						String linkNew = osirisLink.generate();

						oml = oml.replace_all(link, linkNew);

						if(id == _S("FE10596C2B406B19020DE70118C5A36208CE7B06"))
						{
							valid = true;
						}

						if(xmlEncoded == false)
							omlOrig = omlOrig.replace_all(link, linkNew);
						else
							omlOrig = omlOrig.replace_all(HtmlParser::instance()->encodeEx(link), HtmlParser::instance()->encodeEx(linkNew));


						//m_DebugLog += id + _S(" --> ") + link + _S(" --> ") + linkNew + _S("\r\n");
					}
				}
			}

			if(valid == false)
			{
				linkEx = linkEx.replace_all(_S("\r"),_S(" {\\r} "));
				linkEx = linkEx.replace_all(_S("\n"),_S(" {\\n} "));
				m_DebugLog += id + _S(" --> ") + linkEx + _S(" --> invalid!\r\n");
				lastPosLink = posLink+1;
			}
			
			if(nReplace == 10000) // Something wrong.
				return false;

		}

		// Qui rilevo eventuale parametro 'portal' della 0.x. Lo converto nel nuovo ID.

		if(omlOrig != oldOml)
		{
			String sql = _S("update ") + tableName + _S(" set ") + fieldName + _S("=") + Convert::toSQL(omlOrig) + _S(" where id='") + id + _S("'");
			if(database->execute(sql) == false)
				return false;
			//patch.append();			
		}

		result2->moveNext();
	}

	/*
	if(patch.empty() == false)
		if(database->execute(patch) == false)
			return false;
	*/

	return true;
}

void CompatibilityManager::renameIDS(const shared_ptr<IPortalDatabase> &database)
{
	/*
		if(id == 2) // Reputation
			newID = database->getUserReputationID(
		else if(id == 7) // Avatar
		else if(id == 14) // Vote
		else if(id == 17) // PollVote
	}
	*/

	// Reputations
	{
		DataTable result;
		String sql = _S("select id, author, reference from os_reputations");
		database->execute(sql,result);

		for(uint32 r=0;r<result.rows();r++)
		{
			String id = result.get(r,_S("id"));
			String author = result.get(r,_S("author"));
			String reference = result.get(r,_S("reference"));

			ObjectID newID = database->getUserReputationID(author.to_ascii(), reference.to_ascii());

			String sql = _S("update os_reputations set id=") + Convert::toSQL(newID) + _S(" where id=") + Convert::toSQL(id);
			database->execute(sql);
			sql = _S("update os_entries set id=") + Convert::toSQL(newID) + _S(" where id=") + Convert::toSQL(id);
			database->execute(sql);
		}
	}

	// Avatars
	{
		DataTable result;
		String sql = _S("select id, author from os_avatars");
		database->execute(sql,result);

		for(uint32 r=0;r<result.rows();r++)
		{
			String id = result.get(r,_S("id"));
			String author = result.get(r,_S("author"));
			
			ObjectID newID = database->getUserAvatarID(author.to_ascii());

			String sql = _S("update os_avatars set id=") + Convert::toSQL(newID) + _S(" where id=") + Convert::toSQL(id);
			database->execute(sql);
			sql = _S("update os_entries set id=") + Convert::toSQL(newID) + _S(" where id=") + Convert::toSQL(id);
			database->execute(sql);
		}
	}

	// Vote
	{
		DataTable result;
		String sql = _S("select id, author, reference from os_votes");
		database->execute(sql,result);

		for(uint32 r=0;r<result.rows();r++)
		{
			String id = result.get(r,_S("id"));
			String author = result.get(r,_S("author"));
			String reference = result.get(r,_S("reference"));
			
			ObjectID newID = database->getUserVoteID(author.to_ascii(), reference.to_ascii());

			String sql = _S("update os_votes set id=") + Convert::toSQL(newID) + _S(" where id=") + Convert::toSQL(id);
			database->execute(sql);
			sql = _S("update os_entries set id=") + Convert::toSQL(newID) + _S(" where id=") + Convert::toSQL(id);
			database->execute(sql);
		}
	}

	// PollVote
	{
		DataTable result;
		String sql = _S("select id, author, reference from os_polls_votes");
		database->execute(sql,result);

		for(uint32 r=0;r<result.rows();r++)
		{
			String id = result.get(r,_S("id"));
			String author = result.get(r,_S("author"));
			String reference = result.get(r,_S("reference"));
			
			ObjectID newID = database->getUserPollVoteID(author.to_ascii(), reference.to_ascii());

			String sql = _S("update os_polls_votes set id=") + Convert::toSQL(newID) + _S(" where id=") + Convert::toSQL(id);
			database->execute(sql);
			sql = _S("update os_entries set id=") + Convert::toSQL(newID) + _S(" where id=") + Convert::toSQL(id);
			database->execute(sql);
		}
	}
}

void CompatibilityManager::migrateAccounts(const shared_ptr<IPortalDatabase> &database)
{
	shared_ptr<IDbConnection> globalConnection = Engine::instance()->createSystemConnection();

	DataTable result;
	String sql = _S("select * from os_accounts");
	
	database->execute(sql,result);

	for(uint32 r=0;r<result.rows();r++)
	{
		String id = result.get(r,_S("id"));

		shared_ptr<DataAccount> dataAccount(OS_NEW DataAccount());

		dataAccount->read(result[r]);

		dataAccount->description = _S("Imported from portal '") + database->getPortal()->getName();
		
		String sql = String::format(_S("select count(*) from os_accounts where id='%S'").c_str(), id.c_str());
		int nRecordExists = globalConnection->value_of(sql);
		bool alreadyExists = (nRecordExists>0); 

		DbSqlValues values;
		dataAccount->write(values);
		if(alreadyExists)
			globalConnection->executeStatement(globalConnection->prepare_update(DBTABLES::ACCOUNTS_TABLE, values, DBTABLES::ACCOUNTS::ID, Convert::toSQL(dataAccount->id)), values);
		else
		
			globalConnection->executeStatement(globalConnection->prepare_insert(DBTABLES::ACCOUNTS_TABLE, values), values);
	}
}

void CompatibilityManager::resigner(const shared_ptr<IPortalDatabase> &database, shared_ptr<IdeSession> session)
{
	shared_ptr<ObjectsUser> user = session->getUser(database);
	if(user != nullptr)
	{
		Buffer signature = user->signature;
		if( (signature.getSize() == 1) && (signature.getAt(0) == 0) )
		{
			// Need re-sign.
			DataTable result;
			String sql = String::format(_S("select id,entity_author,author from os_entries where author='%S' union select id,entity_author,author from os_entries where entity_author='%S'").c_str(),user->id->toUTF16().c_str(),user->id->toUTF16().c_str());
			database->execute(sql,result);

			for(uint32 r=0;r<result.rows();r++)
			{
				String id = result.get(r,_S("id"));
				String entityAuthor = result.get(r,_S("entity_author"));
				String author = result.get(r,_S("author"));

				shared_ptr<ObjectsIObject> object = database->getPortal()->getObject(database, id.to_ascii());
				if(object != nullptr)
				{
					LanguageResult acceptable = object->acceptable(database);
					if(acceptable.empty())
					{
						if( (entityAuthor == user->id->toUTF16()) && (author == user->id->toUTF16()) )
						{
							shared_ptr<ObjectsIRevisionable> revisionable = objects_revisionable_cast(object);			
							if(revisionable != nullptr)
							{
								bool done = revisionable->signEntity(session->getPrivateKey(), database->getPortal()->getPortalID());
								OS_ASSERT(done);
							}
						}

						if(author == user->id->toUTF16())
						{
							bool done = object->sign(session->getPrivateKey(), database->getPortal()->getPortalID());							
							OS_ASSERT(done);
						}

						LanguageResult result = object->store(database);
						OS_ASSERT(result.empty());						
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
