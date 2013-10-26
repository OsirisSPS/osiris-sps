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
#include "dbtables.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String DBTABLES::AUTHOR = _S("author");
const String DBTABLES::COMMENT = _S("comment");
const String DBTABLES::CONTENT = _S("content");
const String DBTABLES::DESCRIPTION = _S("description");
#ifdef OS_TODOCIP
const String DBTABLES::ENTITY = _S("entity");
const String DBTABLES::ENTITY_AUTHOR = _S("entity_author");
const String DBTABLES::ENTITY_DATE = _S("entity_date");
const String DBTABLES::ENTITY_PARENT = _S("entity_parent");
const String DBTABLES::ENTITY_SIGNATURE = _S("entity_signature");
#endif
const String DBTABLES::ID = _S("id");
const String DBTABLES::POV = _S("pov");
const String DBTABLES::MODEL = _S("model");
const String DBTABLES::PARENT = _S("parent");
const String DBTABLES::PINNED = _S("pinned");
const String DBTABLES::CLOSED_DATE = _S("closed_date");
const String DBTABLES::POSITION = _S("position");
const String DBTABLES::REVISION = _S("revision");
const String DBTABLES::REFERENCE = _S("reference");
const String DBTABLES::SIGNATURE = _S("signature");
const String DBTABLES::SUBMIT_DATE = _S("submit_date");
const String DBTABLES::TITLE = _S("title");
const String DBTABLES::VERSION = _S("version");
const String DBTABLES::VISIBLE = _S("visible");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::ACCOUNTS_TABLE = _S("os_accounts");
const String DBTABLES::ACCOUNTS::USERNAME = _S("username");
const String DBTABLES::ACCOUNTS::DESCRIPTION = _S("description");
const String DBTABLES::ACCOUNTS::PASSWORD = _S("password");
const String DBTABLES::ACCOUNTS::ID = _S("id");
const String DBTABLES::ACCOUNTS::REAL_PASSWORD = _S("real_password");
const String DBTABLES::ACCOUNTS::PUBLIC_KEY = _S("public_key");
const String DBTABLES::ACCOUNTS::PRIVATE_KEY = _S("private_key");
const String DBTABLES::ACCOUNTS::SECRET_QUESTION = _S("secret_question");
const String DBTABLES::ACCOUNTS::SECRET_RESPONSE = _S("secret_response");
const String DBTABLES::ACCOUNTS::LAST_ACCESS_DATE = _S("last_access_date");
const String DBTABLES::ACCOUNTS::REFERENCE_DATE = _S("reference_date");
const String DBTABLES::ACCOUNTS::OPTIONS = _S("options");
const String DBTABLES::ACCOUNTS::IDENTITY = _S("identity");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::ATTACHMENTS_TABLE = _S("os_attachments");
const String DBTABLES::ATTACHMENTS::ID = DBTABLES::ID;
const String DBTABLES::ATTACHMENTS::PARENT = DBTABLES::PARENT;
const String DBTABLES::ATTACHMENTS::REVISION = DBTABLES::REVISION;
const String DBTABLES::ATTACHMENTS::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::ATTACHMENTS::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::ATTACHMENTS::VISIBLE = DBTABLES::VISIBLE;
const String DBTABLES::ATTACHMENTS::POSITION = DBTABLES::POSITION;
const String DBTABLES::ATTACHMENTS::COMMENT = DBTABLES::COMMENT;
const String DBTABLES::ATTACHMENTS::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::ATTACHMENTS::FILE_NAME = _S("file_name");
const String DBTABLES::ATTACHMENTS::FILE_DATA = _S("file_data");
const String DBTABLES::ATTACHMENTS::FILE_TYPE = _S("file_type");
const String DBTABLES::ATTACHMENTS::FILE_SIZE = _S("file_size");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::AVATARS_TABLE = _S("os_avatars");
const String DBTABLES::AVATARS::ID = DBTABLES::ID;
const String DBTABLES::AVATARS::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::AVATARS::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::AVATARS::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::AVATARS::FILE_NAME = _S("file_name");
const String DBTABLES::AVATARS::FILE_DATA = _S("file_data");
const String DBTABLES::AVATARS::FILE_TYPE = _S("file_type");
const String DBTABLES::AVATARS::FILE_SIZE = _S("file_size");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::ATTRIBUTES_TABLE = _S("os_attributes");
const String DBTABLES::ATTRIBUTES::ID = DBTABLES::ID;
const String DBTABLES::ATTRIBUTES::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::ATTRIBUTES::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::ATTRIBUTES::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::ATTRIBUTES::REFERENCE = DBTABLES::REFERENCE;
const String DBTABLES::ATTRIBUTES::TYPE = _S("type");
const String DBTABLES::ATTRIBUTES::VALUE = _S("value");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::CALENDAR_EVENTS_TABLE = _S("os_calendar_events");
const String DBTABLES::CALENDAR_EVENTS::ID = DBTABLES::ID;
const String DBTABLES::CALENDAR_EVENTS::PARENT = DBTABLES::PARENT;
const String DBTABLES::CALENDAR_EVENTS::REVISION = DBTABLES::REVISION;
const String DBTABLES::CALENDAR_EVENTS::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::CALENDAR_EVENTS::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::CALENDAR_EVENTS::VISIBLE = DBTABLES::VISIBLE;
const String DBTABLES::CALENDAR_EVENTS::POSITION = DBTABLES::POSITION;
const String DBTABLES::CALENDAR_EVENTS::COMMENT = DBTABLES::COMMENT;
const String DBTABLES::CALENDAR_EVENTS::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::CALENDAR_EVENTS::TITLE = DBTABLES::TITLE;
const String DBTABLES::CALENDAR_EVENTS::DESCRIPTION = DBTABLES::DESCRIPTION;
const String DBTABLES::CALENDAR_EVENTS::CONTENT = DBTABLES::CONTENT;
const String DBTABLES::CALENDAR_EVENTS::MODEL = DBTABLES::MODEL;
const String DBTABLES::CALENDAR_EVENTS::PINNED = DBTABLES::PINNED;
const String DBTABLES::CALENDAR_EVENTS::CLOSED_DATE = DBTABLES::CLOSED_DATE;
const String DBTABLES::CALENDAR_EVENTS::START_DATE = _S("start_date");
const String DBTABLES::CALENDAR_EVENTS::END_DATE = _S("end_date");
const String DBTABLES::CALENDAR_EVENTS::REPEAT_MODE = _S("repeat_mode");
const String DBTABLES::CALENDAR_EVENTS::EXPIRATION_DATE = _S("expiration_date");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::DISCUSSIONS_STATS_TABLE = _S("os_discussions_stats");
const String DBTABLES::DISCUSSIONS_STATS::REFERENCE = DBTABLES::REFERENCE;
const String DBTABLES::DISCUSSIONS_STATS::PARENT = _S("parent");
const String DBTABLES::DISCUSSIONS_STATS::POSTS = _S("posts");
const String DBTABLES::DISCUSSIONS_STATS::VOTES = _S("votes");
const String DBTABLES::DISCUSSIONS_STATS::VOTES_AVERAGE = _S("votes_average");
const String DBTABLES::DISCUSSIONS_STATS::LAST_ENTRY = _S("last_entry");
const String DBTABLES::DISCUSSIONS_STATS::LAST_ENTRY_DATE = _S("last_entry_date");
const String DBTABLES::DISCUSSIONS_STATS::PINNED = DBTABLES::PINNED;

//////////////////////////////////////////////////////////////////////

const String DBTABLES::FILES_TABLE = _S("os_files");
const String DBTABLES::FILES::ID = DBTABLES::ID;
const String DBTABLES::FILES::PARENT = DBTABLES::PARENT;
const String DBTABLES::FILES::REVISION = DBTABLES::REVISION;
const String DBTABLES::FILES::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::FILES::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::FILES::VISIBLE = DBTABLES::VISIBLE;
const String DBTABLES::FILES::POSITION = DBTABLES::POSITION;
const String DBTABLES::FILES::COMMENT = DBTABLES::COMMENT;
const String DBTABLES::FILES::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::FILES::TITLE = DBTABLES::TITLE;
const String DBTABLES::FILES::DESCRIPTION = DBTABLES::DESCRIPTION;
const String DBTABLES::FILES::CONTENT = DBTABLES::CONTENT;
const String DBTABLES::FILES::MODEL = DBTABLES::MODEL;
const String DBTABLES::FILES::PINNED = DBTABLES::PINNED;
const String DBTABLES::FILES::CLOSED_DATE = DBTABLES::CLOSED_DATE;
const String DBTABLES::FILES::FILE_NAME = _S("file_name");
const String DBTABLES::FILES::FILE_DATA = _S("file_data");
const String DBTABLES::FILES::FILE_TYPE = _S("file_type");
const String DBTABLES::FILES::FILE_SIZE = _S("file_size");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::ENTRIES_TABLE = _S("os_entries");
const String DBTABLES::ENTRIES::ID = DBTABLES::ID;
const String DBTABLES::ENTRIES::PARENT = DBTABLES::PARENT;
#ifdef OS_TODOCIP
const String DBTABLES::ENTRIES::ENTITY = DBTABLES::ENTITY;
const String DBTABLES::ENTRIES::ENTITY_AUTHOR = DBTABLES::ENTITY_AUTHOR;
const String DBTABLES::ENTRIES::ENTITY_DATE = DBTABLES::ENTITY_DATE;
#else
const String DBTABLES::ENTRIES::REVISION = DBTABLES::REVISION;
#endif
const String DBTABLES::ENTRIES::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::ENTRIES::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::ENTRIES::INSERT_DATE = _S("insert_date");
const String DBTABLES::ENTRIES::TYPE = _S("type");
const String DBTABLES::ENTRIES::RANK = _S("rank");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::EXTENSIONS_TABLE = _S("os_extensions");
const String DBTABLES::EXTENSIONS::ID = _S("id");
const String DBTABLES::EXTENSIONS::VERSION = _S("version");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::INSTANCES_TABLE = _S("os_instances");
const String DBTABLES::INSTANCES::ID = DBTABLES::ID;
const String DBTABLES::INSTANCES::PARENT = DBTABLES::PARENT;
const String DBTABLES::INSTANCES::REVISION = DBTABLES::REVISION;
const String DBTABLES::INSTANCES::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::INSTANCES::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::INSTANCES::VISIBLE = DBTABLES::VISIBLE;
const String DBTABLES::INSTANCES::POSITION = DBTABLES::POSITION;
const String DBTABLES::INSTANCES::COMMENT = DBTABLES::COMMENT;
const String DBTABLES::INSTANCES::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::INSTANCES::TITLE = DBTABLES::TITLE;
const String DBTABLES::INSTANCES::AREA = _S("area");
const String DBTABLES::INSTANCES::INHERIT = _S("inherit");
const String DBTABLES::INSTANCES::MODULE = _S("module");
const String DBTABLES::INSTANCES::DATA = _S("data");
const String DBTABLES::INSTANCES::BLOCK = _S("block");
const String DBTABLES::INSTANCES::STYLE = _S("style");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::INSTANCES_CACHE_TABLE = _S("os_instances_cache");
const String DBTABLES::INSTANCES_CACHE::ENTITY = DBTABLES::ENTITY;
const String DBTABLES::INSTANCES_CACHE::CACHE = _S("cache");
const String DBTABLES::INSTANCES_CACHE::CACHE_DATE = _S("cache_date");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::LOCAL_MESSAGES_TABLE = _S("os_local_messages");
const String DBTABLES::LOCAL_MESSAGES::PROFILE = _S("profile");
const String DBTABLES::LOCAL_MESSAGES::ID = DBTABLES::ID;
const String DBTABLES::LOCAL_MESSAGES::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::LOCAL_MESSAGES::SESSION = _S("session");
const String DBTABLES::LOCAL_MESSAGES::CONTENT = DBTABLES::CONTENT;
const String DBTABLES::LOCAL_MESSAGES::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::LOCAL_MESSAGES::READ_DATE = _S("read_date");
const String DBTABLES::LOCAL_MESSAGES::STATUS = _S("status");
const String DBTABLES::LOCAL_MESSAGES::FOLDER = _S("folder");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::MODELS_TABLE = _S("os_models");
const String DBTABLES::MODELS::ID = DBTABLES::ID;
const String DBTABLES::MODELS::PARENT = DBTABLES::PARENT;
const String DBTABLES::MODELS::REVISION = DBTABLES::REVISION;
const String DBTABLES::MODELS::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::MODELS::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::MODELS::VISIBLE = DBTABLES::VISIBLE;
const String DBTABLES::MODELS::POSITION = DBTABLES::POSITION;
const String DBTABLES::MODELS::COMMENT = DBTABLES::COMMENT;
const String DBTABLES::MODELS::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::MODELS::TITLE = DBTABLES::TITLE;
const String DBTABLES::MODELS::DESCRIPTION = DBTABLES::DESCRIPTION;
const String DBTABLES::MODELS::CONTENT = DBTABLES::CONTENT;
const String DBTABLES::MODELS::MODEL = DBTABLES::MODEL;
const String DBTABLES::MODELS::PINNED = DBTABLES::PINNED;
const String DBTABLES::MODELS::CLOSED_DATE = DBTABLES::CLOSED_DATE;
const String DBTABLES::MODELS::SCHEMA = _S("schema");
const String DBTABLES::MODELS::STYLESHEET = _S("stylesheet");
const String DBTABLES::MODELS::SAMPLE = _S("sample");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::PEERS_TABLE = _S("os_peers");
const String DBTABLES::PEERS::IP = _S("ip");
const String DBTABLES::PEERS::PORT = _S("port");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::POLLS_TABLE = _S("os_polls");
const String DBTABLES::POLLS::ID = DBTABLES::ID;
const String DBTABLES::POLLS::PARENT = DBTABLES::PARENT;
const String DBTABLES::POLLS::REVISION = DBTABLES::REVISION;
const String DBTABLES::POLLS::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::POLLS::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::POLLS::VISIBLE = DBTABLES::VISIBLE;
const String DBTABLES::POLLS::POSITION = DBTABLES::POSITION;
const String DBTABLES::POLLS::COMMENT = DBTABLES::COMMENT;
const String DBTABLES::POLLS::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::POLLS::TITLE = DBTABLES::TITLE;
const String DBTABLES::POLLS::DESCRIPTION = DBTABLES::DESCRIPTION;
const String DBTABLES::POLLS::CONTENT = DBTABLES::CONTENT;
const String DBTABLES::POLLS::MODEL = DBTABLES::MODEL;
const String DBTABLES::POLLS::PINNED = DBTABLES::PINNED;
const String DBTABLES::POLLS::CLOSED_DATE = DBTABLES::CLOSED_DATE;
const String DBTABLES::POLLS::TYPE = _S("type");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::POLLS_STATS_TABLE = _S("os_polls_stats");
const String DBTABLES::POLLS_STATS::REFERENCE = DBTABLES::REFERENCE;
const String DBTABLES::POLLS_STATS::POLL_VOTES = _S("poll_votes");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::POLLS_OPTIONS_TABLE = _S("os_polls_options");
const String DBTABLES::POLLS_OPTIONS::ID = DBTABLES::ID;
const String DBTABLES::POLLS_OPTIONS::PARENT = DBTABLES::PARENT;
const String DBTABLES::POLLS_OPTIONS::REVISION = DBTABLES::REVISION;
const String DBTABLES::POLLS_OPTIONS::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::POLLS_OPTIONS::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::POLLS_OPTIONS::VISIBLE = DBTABLES::VISIBLE;
const String DBTABLES::POLLS_OPTIONS::POSITION = DBTABLES::POSITION;
const String DBTABLES::POLLS_OPTIONS::COMMENT = DBTABLES::COMMENT;
const String DBTABLES::POLLS_OPTIONS::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::POLLS_OPTIONS::TITLE = DBTABLES::TITLE;

//////////////////////////////////////////////////////////////////////

const String DBTABLES::POLLS_OPTIONS_STATS_TABLE = _S("os_polls_options_stats");
const String DBTABLES::POLLS_OPTIONS_STATS::REFERENCE = DBTABLES::REFERENCE;
const String DBTABLES::POLLS_OPTIONS_STATS::VOTES = _S("votes");
const String DBTABLES::POLLS_OPTIONS_STATS::VOTES_AVERAGE = _S("votes_average");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::POLLS_VOTES_TABLE = _S("os_polls_votes");
const String DBTABLES::POLLS_VOTES::ID = DBTABLES::ID;
const String DBTABLES::POLLS_VOTES::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::POLLS_VOTES::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::POLLS_VOTES::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::POLLS_VOTES::PARENT = DBTABLES::PARENT;
const String DBTABLES::POLLS_VOTES::REFERENCE = DBTABLES::REFERENCE;
const String DBTABLES::POLLS_VOTES::SCORE = _S("score");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::PRIVATE_MESSAGES_TABLE = _S("os_private_messages");
const String DBTABLES::PRIVATE_MESSAGES::ID = DBTABLES::ID;
const String DBTABLES::PRIVATE_MESSAGES::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::PRIVATE_MESSAGES::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::PRIVATE_MESSAGES::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::PRIVATE_MESSAGES::ADRESSEE = _S("adressee");
const String DBTABLES::PRIVATE_MESSAGES::SESSION = _S("session");
const String DBTABLES::PRIVATE_MESSAGES::CONTENT = DBTABLES::CONTENT;

//////////////////////////////////////////////////////////////////////

const String DBTABLES::POSTS_TABLE = _S("os_posts");
const String DBTABLES::POSTS::ID = DBTABLES::ID;
const String DBTABLES::POSTS::PARENT = DBTABLES::PARENT;
const String DBTABLES::POSTS::REVISION = DBTABLES::REVISION;
const String DBTABLES::POSTS::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::POSTS::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::POSTS::VISIBLE = DBTABLES::VISIBLE;
const String DBTABLES::POSTS::POSITION = DBTABLES::POSITION;
const String DBTABLES::POSTS::COMMENT = DBTABLES::COMMENT;
const String DBTABLES::POSTS::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::POSTS::TITLE = DBTABLES::TITLE;
const String DBTABLES::POSTS::CONTENT = DBTABLES::CONTENT;
const String DBTABLES::POSTS::REFERENCE = DBTABLES::REFERENCE;

//////////////////////////////////////////////////////////////////////

const String DBTABLES::REPUTATIONS_TABLE = _S("os_reputations");
const String DBTABLES::REPUTATIONS::ID = DBTABLES::ID;
const String DBTABLES::REPUTATIONS::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::REPUTATIONS::REFERENCE = DBTABLES::REFERENCE;
const String DBTABLES::REPUTATIONS::SCORE = _S("score");
const String DBTABLES::REPUTATIONS::DESCRIPTION = DBTABLES::DESCRIPTION;
const String DBTABLES::REPUTATIONS::FOLLOW = _S("follow");
const String DBTABLES::REPUTATIONS::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::REPUTATIONS::SIGNATURE = DBTABLES::SIGNATURE;

//////////////////////////////////////////////////////////////////////

const String DBTABLES::SECTIONS_TABLE = _S("os_sections");
const String DBTABLES::SECTIONS::ID = DBTABLES::ID;
const String DBTABLES::SECTIONS::PARENT = DBTABLES::PARENT;
const String DBTABLES::SECTIONS::REVISION = DBTABLES::REVISION;
const String DBTABLES::SECTIONS::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::SECTIONS::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::SECTIONS::VISIBLE = DBTABLES::VISIBLE;
const String DBTABLES::SECTIONS::POSITION = DBTABLES::POSITION;
const String DBTABLES::SECTIONS::COMMENT = DBTABLES::COMMENT;
const String DBTABLES::SECTIONS::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::SECTIONS::TITLE = DBTABLES::TITLE;
const String DBTABLES::SECTIONS::DESCRIPTION = DBTABLES::DESCRIPTION;
const String DBTABLES::SECTIONS::COMPONENT = _S("component");
const String DBTABLES::SECTIONS::DATA = _S("data");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::SNAPSHOT_OBJECTS_TABLE = _S("os_snapshot_objects");
const String DBTABLES::SNAPSHOT_OBJECTS::ENTITY = DBTABLES::ENTITY;
const String DBTABLES::SNAPSHOT_OBJECTS::TYPE = _S("type");
const String DBTABLES::SNAPSHOT_OBJECTS::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::SNAPSHOT_OBJECTS::PARENT = DBTABLES::PARENT;
const String DBTABLES::SNAPSHOT_OBJECTS::CURRENT = _S("current");
const String DBTABLES::SNAPSHOT_OBJECTS::VISIBLE = DBTABLES::VISIBLE;
const String DBTABLES::SNAPSHOT_OBJECTS::POSITION = DBTABLES::POSITION;
const String DBTABLES::SNAPSHOT_OBJECTS::TITLE = DBTABLES::TITLE;
const String DBTABLES::SNAPSHOT_OBJECTS::SCORE = _S("score");
const String DBTABLES::SNAPSHOT_OBJECTS::INSERT_DATE = _S("insert_date");
const String DBTABLES::SNAPSHOT_OBJECTS::STABILITY_DATE = _S("stability_date");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::SNAPSHOT_USERS_TABLE = _S("os_snapshot_users");
const String DBTABLES::SNAPSHOT_USERS::ID = DBTABLES::ID;
const String DBTABLES::SNAPSHOT_USERS::SCORE = _S("score");
const String DBTABLES::SNAPSHOT_USERS::DESCRIPTION = _S("description");
const String DBTABLES::SNAPSHOT_USERS::FOLLOW = _S("follow");
const String DBTABLES::SNAPSHOT_USERS::STABILITY_DATE = _S("stability_date");
const String DBTABLES::SNAPSHOT_USERS::STATS = _S("stats");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::TAGS_TABLE = _S("os_tags");
const String DBTABLES::TAGS::ID = DBTABLES::ID;
const String DBTABLES::TAGS::PARENT = DBTABLES::PARENT;
const String DBTABLES::TAGS::REVISION = DBTABLES::REVISION;
const String DBTABLES::TAGS::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::TAGS::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::TAGS::VISIBLE = DBTABLES::VISIBLE;
const String DBTABLES::TAGS::POSITION = DBTABLES::POSITION;
const String DBTABLES::TAGS::COMMENT = DBTABLES::COMMENT;
const String DBTABLES::TAGS::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::TAGS::NAME = _S("name");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::TEXTS_TABLE = _S("os_texts");
const String DBTABLES::TEXTS::ID = DBTABLES::ID;
const String DBTABLES::TEXTS::PARENT = DBTABLES::PARENT;
const String DBTABLES::TEXTS::REVISION = DBTABLES::REVISION;
const String DBTABLES::TEXTS::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::TEXTS::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::TEXTS::VISIBLE = DBTABLES::VISIBLE;
const String DBTABLES::TEXTS::POSITION = DBTABLES::POSITION;
const String DBTABLES::TEXTS::COMMENT = DBTABLES::COMMENT;
const String DBTABLES::TEXTS::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::TEXTS::TITLE = DBTABLES::TITLE;
const String DBTABLES::TEXTS::DESCRIPTION = DBTABLES::DESCRIPTION;
const String DBTABLES::TEXTS::CONTENT = DBTABLES::CONTENT;
const String DBTABLES::TEXTS::MODEL = DBTABLES::MODEL;
const String DBTABLES::TEXTS::PINNED = DBTABLES::PINNED;
const String DBTABLES::TEXTS::CLOSED_DATE = DBTABLES::CLOSED_DATE;

//////////////////////////////////////////////////////////////////////

const String DBTABLES::USERS_TABLE = _S("os_users");
const String DBTABLES::USERS::ID = DBTABLES::ID;
const String DBTABLES::USERS::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::USERS::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::USERS::PUBLIC_KEY = _S("public_key");
const String DBTABLES::USERS::NAME = _S("name");
const String DBTABLES::USERS::DESCRIPTION = DBTABLES::DESCRIPTION;
const String DBTABLES::USERS::AUTHORS_THRESHOLD = _S("authors_threshold");
const String DBTABLES::USERS::EDITORS_THRESHOLD = _S("editors_threshold");
const String DBTABLES::USERS::JOIN_DATE = _S("join_date");
const String DBTABLES::USERS::BIRTH_DATE = _S("birth_date");
const String DBTABLES::USERS::GENDER = _S("gender");
const String DBTABLES::USERS::LOCATION = _S("location");
const String DBTABLES::USERS::TIME_OFFSET = _S("time_offset");
const String DBTABLES::USERS::LANGUAGE = _S("language");
const String DBTABLES::USERS::MISC = _S("misc");
const String DBTABLES::USERS::MARK = _S("mark");
const String DBTABLES::USERS::SHOW_MARK = _S("show_mark");
const String DBTABLES::USERS::SHOW_AVATAR = _S("show_avatar");
const String DBTABLES::USERS::EMAIL = _S("email");
const String DBTABLES::USERS::WEBSITE = _S("website");
const String DBTABLES::USERS::CONTACTS = _S("contacts");
const String DBTABLES::USERS::OPTIONS = _S("options");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::USERS_STATS_TABLE = _S("os_users_stats");
const String DBTABLES::USERS_STATS::REFERENCE = DBTABLES::REFERENCE;
const String DBTABLES::USERS_STATS::OBJECTS = _S("objects");
const String DBTABLES::USERS_STATS::REVISIONS = _S("revisions");
const String DBTABLES::USERS_STATS::PROGRESSIVE_NUMBER = _S("progressive_number");

//////////////////////////////////////////////////////////////////////

const String DBTABLES::VOTES_TABLE = _S("os_votes");
const String DBTABLES::VOTES::ID = DBTABLES::ID;
const String DBTABLES::VOTES::AUTHOR = DBTABLES::AUTHOR;
const String DBTABLES::VOTES::SUBMIT_DATE = DBTABLES::SUBMIT_DATE;
const String DBTABLES::VOTES::SIGNATURE = DBTABLES::SIGNATURE;
const String DBTABLES::VOTES::REFERENCE = DBTABLES::REFERENCE;
const String DBTABLES::VOTES::SCORE = _S("score");

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
