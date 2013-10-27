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

#ifndef _ENGINE_DATADATA_H
#define _ENGINE_DATADATA_H

//////////////////////////////////////////////////////////////////////

#define OS_SCHEMA_ACCOUNT						_S("account.xsd")

#define OS_MESSAGE_FOLDER_NONE				   -1
#define OS_MESSAGE_FOLDER_INBOX					0
#define OS_MESSAGE_FOLDER_TRASH					1
#define OS_MESSAGE_FOLDER_SENT_ITEMS			2

#define OS_MESSAGE_STATUS_NONE				   -1
#define OS_MESSAGE_STATUS_NEW					0
#define OS_MESSAGE_STATUS_UNREAD				1
#define OS_MESSAGE_STATUS_READ					2
#define OS_MESSAGE_STATUS_DELETED				3

#define OS_RECORD_UNKNOWN						0
#define OS_RECORD_ACCOUNT						1
#define OS_RECORD_ENTRY							5
#define OS_RECORD_OBJECT						6
#define OS_RECORD_PEER							7
#define OS_RECORD_STATISTICS					8
#define OS_RECORD_LOCALMESSAGE					9
#define OS_RECORD_INSTANCECACHE					10

#define	OS_STATISTICS_NONE						0
#define OS_STATISTICS_USER						1
#define OS_STATISTICS_POLL						2
#define OS_STATISTICS_POLL_OPTION				3
#define OS_STATISTICS_DISCUSSION				4

#define OS_ACCOUNT_VIEWMODE_NORMAL				0		// La modalità di default deve essere = 0 per il cast del valore di default
#define OS_ACCOUNT_VIEWMODE_EDITOR				1

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

enum MessageFolder
{
	messageFolderNone				= OS_MESSAGE_FOLDER_NONE,			// Non specificata
	messageFolderInbox				= OS_MESSAGE_FOLDER_INBOX,			// Folder per i messaggi in arrivo
	messageFolderTrash				= OS_MESSAGE_FOLDER_TRASH,			// Folder per i messaggi cancellati
	messageFolderSentItems			= OS_MESSAGE_FOLDER_SENT_ITEMS,		// Folder per i messaggi inviati
};

enum MessageStatus
{
	messageStatusNone				= OS_MESSAGE_STATUS_NONE,			// Non specificato
	messageStatusNew				= OS_MESSAGE_STATUS_NEW,			// Nuovo messaggio
	messageStatusUnread				= OS_MESSAGE_STATUS_UNREAD,			// Messaggio non ancora letto
	messageStatusRead				= OS_MESSAGE_STATUS_READ,			// Messaggio archiviato
	messageStatusDeleted			= OS_MESSAGE_STATUS_DELETED,		// Messaggio cancellati definitivamente
};

enum DataRecordType
{
	dataRecordTypeUnknown			= OS_RECORD_UNKNOWN,
	dataRecordTypeAccount			= OS_RECORD_ACCOUNT,
	dataRecordTypeEntry				= OS_RECORD_ENTRY,
	dataRecordTypeObject			= OS_RECORD_OBJECT,
	dataRecordTypePeer				= OS_RECORD_PEER,
	dataRecordTypeStatistics		= OS_RECORD_STATISTICS,
	dataRecordTypeLocalMessage		= OS_RECORD_LOCALMESSAGE,
	dataRecordTypeInstanceCache		= OS_RECORD_INSTANCECACHE,
};

enum DataViewMode
{
	dataViewModeNormal			= OS_ACCOUNT_VIEWMODE_NORMAL,
	dataViewModeEditor			= OS_ACCOUNT_VIEWMODE_EDITOR,
};

//////////////////////////////////////////////////////////////////////

// Interfaces
class DataIRecord;
class DataIStatistics;

// Classes
class DataAccount;
class DataDiscussionStats;
class DataEntry;
class DataInstanceCache;
class DataLocalMessage;
class DataPeer;
class DataPollStats;
class DataPollOptionStats;
class DataUserStats;

/*
typedef shared_ptr<DataIRecord> record_ptr;
typedef shared_ptr<DataIStatistics> statistics_ptr;

typedef shared_ptr<DataAccount> account_ptr;
typedef shared_ptr<DataDiscussionStats> discussion_stats_ptr;
typedef shared_ptr<DataInstanceCache> instance_cache_ptr;
typedef shared_ptr<DataLocalMessage> local_message_ptr;
typedef list<shared_ptr<DataLocalMessage> >::type local_messages_list;
typedef shared_ptr<DataPeer> peer_ptr;
typedef list<peer_ptr>::type peers_list;
typedef shared_ptr<DataPollStats> poll_stats_ptr;
typedef shared_ptr<DataPollOptionStats> poll_option_stats_ptr;
typedef shared_ptr<DataUserStats> user_stats_ptr;
*/

//////////////////////////////////////////////////////////////////////

//typedef shared_ptr<DataEntry> shared_ptr<DataEntry>;
//typedef list<shared_ptr<DataEntry> >::type DataEntry::DataEntryList;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_DATADATA_H
