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

#ifndef _DB_TABLES_H
#define _DB_TABLES_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

struct EngineExport DBTABLES
{
	// Campi comuni
	static const String AUTHOR;
	static const String COMMENT;
	static const String CONTENT;
	static const String DESCRIPTION;
#ifdef OS_TODOCIP
	static const String ENTITY;
	static const String ENTITY_AUTHOR;
	static const String ENTITY_DATE;
	static const String ENTITY_PARENT;
	static const String ENTITY_SIGNATURE;
#endif
	static const String ID;
	static const String POV;
	static const String MODEL;
	static const String PARENT;
	static const String PINNED;
	static const String CLOSED_DATE;
	static const String POSITION;
	static const String REVISION;
	static const String REFERENCE;
	static const String SIGNATURE;
	static const String SUBMIT_DATE;
	static const String TITLE;
	static const String VERSION;
	static const String VISIBLE;

	static const String ACCOUNTS_TABLE;
	struct EngineExport ACCOUNTS
	{
		static const String USERNAME;
		static const String DESCRIPTION;
		static const String PASSWORD;
		static const String ID;
		static const String REAL_PASSWORD;
		static const String PUBLIC_KEY;
		static const String PRIVATE_KEY;
		static const String SECRET_QUESTION;
		static const String SECRET_RESPONSE;
		static const String LAST_ACCESS_DATE;
		static const String REFERENCE_DATE;
		static const String OPTIONS;
		static const String IDENTITY;
	};

	static const String ATTACHMENTS_TABLE;
	struct EngineExport ATTACHMENTS
	{
		static const String ID;
		static const String PARENT;
		static const String REVISION;
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String VISIBLE;
		static const String POSITION;
		static const String COMMENT;
		static const String SIGNATURE;
		static const String FILE_NAME;
		static const String FILE_DATA;
		static const String FILE_TYPE;
		static const String FILE_SIZE;
	};

	static const String ATTRIBUTES_TABLE;
	struct EngineExport ATTRIBUTES
	{
		static const String ID;
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String SIGNATURE;
		static const String REFERENCE;
		static const String TYPE;
		static const String VALUE;
	};

	static const String AVATARS_TABLE;
	struct EngineExport AVATARS
	{
		static const String ID;
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String SIGNATURE;
		static const String FILE_NAME;
		static const String FILE_DATA;
		static const String FILE_TYPE;
		static const String FILE_SIZE;
	};

	static const String CALENDAR_EVENTS_TABLE;
	struct EngineExport CALENDAR_EVENTS
	{
		static const String ID;
		static const String PARENT;
		static const String REVISION;
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String VISIBLE;
		static const String POSITION;
		static const String COMMENT;
		static const String SIGNATURE;
		static const String TITLE;
		static const String DESCRIPTION;
		static const String CONTENT;
		static const String MODEL;
		static const String PINNED;
		static const String CLOSED_DATE;
		static const String START_DATE;
		static const String END_DATE;
		static const String REPEAT_MODE;
		static const String EXPIRATION_DATE;
	};

	static const String DISCUSSIONS_STATS_TABLE;
	struct EngineExport DISCUSSIONS_STATS
	{
		static const String REFERENCE;
		static const String PARENT;
		static const String POSTS;
		static const String VOTES;
		static const String VOTES_AVERAGE;
		static const String LAST_ENTRY;
		static const String LAST_ENTRY_DATE;
		static const String PINNED;
	};

	static const String FILES_TABLE;
	struct EngineExport FILES
	{
		static const String ID;
		static const String PARENT;
		static const String REVISION;
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String VISIBLE;
		static const String POSITION;
		static const String COMMENT;
		static const String SIGNATURE;
		static const String TITLE;
		static const String DESCRIPTION;
		static const String CONTENT;
		static const String MODEL;
		static const String PINNED;
		static const String CLOSED_DATE;
		static const String FILE_NAME;
		static const String FILE_DATA;
		static const String FILE_TYPE;
		static const String FILE_SIZE;
	};

	static const String ENTRIES_TABLE;
	struct EngineExport ENTRIES
	{
		static const String ID;
		static const String PARENT;
#ifdef OS_TODOCIP
		static const String ENTITY;
		static const String ENTITY_AUTHOR;
		static const String ENTITY_DATE;		
#else
		static const String REVISION;
#endif
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String INSERT_DATE;
		static const String TYPE;
		static const String RANK;
	};

	static const String EXTENSIONS_TABLE;
	struct EngineExport EXTENSIONS
	{
		static const String ID;
		static const String VERSION;
	};

	static const String INSTANCES_TABLE;
	struct EngineExport INSTANCES
	{
		static const String ID;
		static const String PARENT;
		static const String REVISION;
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String VISIBLE;
		static const String POSITION;
		static const String COMMENT;
		static const String SIGNATURE;
		static const String TITLE;
		static const String AREA;
		static const String INHERIT;
		static const String MODULE;
		static const String DATA;
		static const String BLOCK;
		static const String STYLE;
	};

	static const String INSTANCES_CACHE_TABLE;
	struct EngineExport INSTANCES_CACHE
	{
		static const String ENTITY;
		static const String CACHE;
		static const String CACHE_DATE;
	};

	static const String LOCAL_MESSAGES_TABLE;
	struct EngineExport LOCAL_MESSAGES
	{
		static const String PROFILE;
		static const String ID;
		static const String AUTHOR;
		static const String SESSION;
		static const String CONTENT;
		static const String SUBMIT_DATE;
		static const String READ_DATE;
		static const String STATUS;
		static const String FOLDER;
	};

	static const String MODELS_TABLE;
	struct EngineExport MODELS
	{
		static const String ID;
		static const String PARENT;
		static const String REVISION;
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String VISIBLE;
		static const String POSITION;
		static const String COMMENT;
		static const String SIGNATURE;
		static const String TITLE;
		static const String DESCRIPTION;
		static const String CONTENT;
		static const String MODEL;
		static const String PINNED;
		static const String CLOSED_DATE;
		static const String SCHEMA;
		static const String STYLESHEET;
		static const String SAMPLE;
	};

	static const String PEERS_TABLE;
	struct EngineExport PEERS
	{
		static const String IP;
		static const String PORT;		
	};

	static const String POLLS_TABLE;
	struct EngineExport POLLS
	{
		static const String ID;
		static const String PARENT;
		static const String REVISION;
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String VISIBLE;
		static const String POSITION;
		static const String COMMENT;
		static const String SIGNATURE;
		static const String TITLE;
		static const String DESCRIPTION;
		static const String CONTENT;
		static const String MODEL;
		static const String PINNED;
		static const String CLOSED_DATE;
		static const String TYPE;
	};

	static const String POLLS_STATS_TABLE;
	struct EngineExport POLLS_STATS
	{
		static const String REFERENCE;
		static const String POLL_VOTES;
	};

	static const String POLLS_OPTIONS_TABLE;
	struct EngineExport POLLS_OPTIONS
	{
		static const String ID;
		static const String PARENT;
		static const String REVISION;
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String VISIBLE;
		static const String POSITION;
		static const String COMMENT;
		static const String SIGNATURE;
		static const String TITLE;
	};

	static const String POLLS_OPTIONS_STATS_TABLE;
	struct EngineExport POLLS_OPTIONS_STATS
	{
		static const String REFERENCE;
		static const String VOTES;
		static const String VOTES_AVERAGE;
	};

	static const String POLLS_VOTES_TABLE;
	struct EngineExport POLLS_VOTES
	{
		static const String ID;
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String SIGNATURE;
		static const String PARENT;
		static const String REFERENCE;
		static const String SCORE;
	};

	static const String PRIVATE_MESSAGES_TABLE;
	struct EngineExport PRIVATE_MESSAGES
	{
		static const String ID;
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String SIGNATURE;
		static const String ADRESSEE;
		static const String SESSION;
		static const String CONTENT;
	};

	static const String POSTS_TABLE;
	struct EngineExport POSTS
	{
		static const String ID;
		static const String PARENT;
		static const String REVISION;
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String VISIBLE;
		static const String POSITION;
		static const String COMMENT;
		static const String SIGNATURE;
		static const String TITLE;
		static const String CONTENT;
		static const String REFERENCE;
	};

	static const String REPUTATIONS_TABLE;
	struct EngineExport REPUTATIONS
	{
		static const String ID;
		static const String AUTHOR;
		static const String REFERENCE;
		static const String SCORE;
		static const String DESCRIPTION;
		static const String FOLLOW;
		static const String SUBMIT_DATE;
		static const String SIGNATURE;
	};

	static const String SECTIONS_TABLE;
	struct EngineExport SECTIONS
	{
		static const String ID;
		static const String PARENT;
		static const String REVISION;
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String VISIBLE;
		static const String POSITION;
		static const String COMMENT;
		static const String SIGNATURE;
		static const String TITLE;
		static const String DESCRIPTION;
		static const String COMPONENT;
		static const String DATA;
	};

	static const String SNAPSHOT_OBJECTS_TABLE;
	struct EngineExport SNAPSHOT_OBJECTS
	{
		static const String ENTITY;
		static const String TYPE;
		static const String SUBMIT_DATE;
		static const String PARENT;
		static const String CURRENT;
		static const String VISIBLE;
		static const String POSITION;
		static const String TITLE;
		static const String SCORE;
		static const String INSERT_DATE;
		static const String STABILITY_DATE;		
	};

	static const String SNAPSHOT_USERS_TABLE;
	struct EngineExport SNAPSHOT_USERS
	{
		static const String ID;
		static const String SCORE;
		static const String DESCRIPTION;
		static const String FOLLOW;
		static const String STABILITY_DATE;
		static const String STATS;
	};

	static const String TAGS_TABLE;
	struct EngineExport TAGS
	{
		static const String ID;
		static const String PARENT;
		static const String REVISION;
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String VISIBLE;
		static const String POSITION;
		static const String COMMENT;
		static const String SIGNATURE;
		static const String NAME;
	};

	static const String TEXTS_TABLE;
	struct EngineExport TEXTS
	{
		static const String ID;
		static const String PARENT;
		static const String REVISION;
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String VISIBLE;
		static const String POSITION;
		static const String COMMENT;
		static const String SIGNATURE;
		static const String TITLE;
		static const String DESCRIPTION;
		static const String CONTENT;
		static const String MODEL;
		static const String PINNED;
		static const String CLOSED_DATE;
	};

	static const String USERS_TABLE;
	struct EngineExport USERS
	{
		static const String ID;
		static const String SUBMIT_DATE;
		static const String SIGNATURE;
		static const String PUBLIC_KEY;
		static const String NAME;
		static const String DESCRIPTION;
		static const String AUTHORS_THRESHOLD;
		static const String EDITORS_THRESHOLD;
		static const String JOIN_DATE;
		static const String BIRTH_DATE;
		static const String GENDER;
		static const String LOCATION;
		static const String TIME_OFFSET;
		static const String LANGUAGE;
		static const String MISC;
		static const String MARK;
		static const String SHOW_MARK;
		static const String SHOW_AVATAR;
		static const String EMAIL;
		static const String WEBSITE;
		static const String CONTACTS;
		static const String OPTIONS;
	};

	static const String USERS_STATS_TABLE;
	struct EngineExport USERS_STATS
	{
		static const String REFERENCE;
		static const String OBJECTS;
		static const String REVISIONS;
		static const String PROGRESSIVE_NUMBER;
	};

	static const String VOTES_TABLE;
	struct EngineExport VOTES
	{
		static const String ID;
		static const String AUTHOR;
		static const String SUBMIT_DATE;
		static const String SIGNATURE;
		static const String REFERENCE;
		static const String SCORE;
	};
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_TABLES_H
