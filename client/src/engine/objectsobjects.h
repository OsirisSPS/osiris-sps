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

#ifndef _OBJECTS_OBJECTS_H
#define _OBJECTS_OBJECTS_H

#include "ids.h"

//////////////////////////////////////////////////////////////////////

#define OS_DECLARE_DESCRIPTOR_INTERFACE()				public: virtual PortalObjectType getType() = 0;

#define OS_DECLARE_DESCRIPTOR(class_name, object_type)	public: virtual PortalObjectType getType() { return object_type; }\
														static ObjectsDescriptorClass<class_name> __descriptorclass_decl;

#define OS_IMPLEMENT_DESCRIPTOR(class_name)				OS_IMPLEMENT_SINGLETON(class_name)\
														OS_NAMESPACE_NAME::ObjectsDescriptorClass<class_name> class_name::__descriptorclass_decl;

//////////////////////////////////////////////////////////////////////

// Nota: non modificare mai i seguenti valori

#define OS_PORTAL_OBJECT_UNKNOWN_TYPENAME				_S("unknown")
#define OS_PORTAL_OBJECT_ROOT_TYPENAME					_S("root")
#define OS_PORTAL_OBJECT_OBJECT_TYPENAME				_S("object")
#define OS_PORTAL_OBJECT_OBJECT_GROUPNAME				_S("objects")
#define OS_PORTAL_OBJECT_INSTANCE_TYPENAME				_S("instance")
#define OS_PORTAL_OBJECT_INSTANCE_GROUPNAME				_S("instances")
#define OS_PORTAL_OBJECT_TOPIC_TYPENAME					_S("topic")
#define OS_PORTAL_OBJECT_TOPIC_GROUPNAME				_S("topics")
#define OS_PORTAL_OBJECT_POST_TYPENAME					_S("post")
#define OS_PORTAL_OBJECT_POST_GROUPNAME					_S("posts")
#define OS_PORTAL_OBJECT_TEXT_TYPENAME					_S("text")
#define OS_PORTAL_OBJECT_TEXT_GROUPNAME					_S("texts")
#define OS_PORTAL_OBJECT_USER_TYPENAME					_S("user")
#define OS_PORTAL_OBJECT_USER_GROUPNAME					_S("users")
#define OS_PORTAL_OBJECT_REPUTATION_TYPENAME			_S("reputation")
#define OS_PORTAL_OBJECT_REPUTATION_GROUPNAME			_S("reputations")
#define OS_PORTAL_OBJECT_AVATAR_TYPENAME				_S("avatar")
#define OS_PORTAL_OBJECT_AVATAR_GROUPNAME				_S("avatars")
#define OS_PORTAL_OBJECT_FILE_TYPENAME					_S("file")
#define OS_PORTAL_OBJECT_FILE_GROUPNAME					_S("files")
#define OS_PORTAL_OBJECT_TAG_TYPENAME					_S("tag")
#define OS_PORTAL_OBJECT_TAG_GROUPNAME					_S("tags")
#define OS_PORTAL_OBJECT_SECTION_TYPENAME				_S("section")
#define OS_PORTAL_OBJECT_SECTION_GROUPNAME				_S("sections")
#define OS_PORTAL_OBJECT_ATTRIBUTE_TYPENAME				_S("attribute")
#define OS_PORTAL_OBJECT_ATTRIBUTE_GROUPNAME			_S("attributes")
#define OS_PORTAL_OBJECT_PRIVATEMESSAGE_TYPENAME		_S("private_message")
#define OS_PORTAL_OBJECT_PRIVATEMESSAGE_GROUPNAME		_S("private_messages")
#define OS_PORTAL_OBJECT_POLL_TYPENAME					_S("poll")
#define OS_PORTAL_OBJECT_POLL_GROUPNAME					_S("polls")
#define OS_PORTAL_OBJECT_POLLOPTION_TYPENAME			_S("poll_option")
#define OS_PORTAL_OBJECT_POLLOPTION_GROUPNAME			_S("poll_options")
#define OS_PORTAL_OBJECT_VOTE_TYPENAME					_S("vote")
#define OS_PORTAL_OBJECT_VOTE_GROUPNAME					_S("votes")
#define OS_PORTAL_OBJECT_MODEL_TYPENAME					_S("model")
#define OS_PORTAL_OBJECT_MODEL_GROUPNAME				_S("models")
#define OS_PORTAL_OBJECT_CALENDAREVENT_TYPENAME			_S("calendar_event")
#define OS_PORTAL_OBJECT_CALENDAREVENT_GROUPNAME		_S("calendar_events")
#define OS_PORTAL_OBJECT_POLLVOTE_TYPENAME				_S("poll_vote")
#define OS_PORTAL_OBJECT_POLLVOTE_GROUPNAME				_S("poll_votes")
#define OS_PORTAL_OBJECT_ATTACHMENT_TYPENAME			_S("attachment")
#define OS_PORTAL_OBJECT_ATTACHMENT_GROUPNAME			_S("attachments")

#define OS_OBJECTID_FLAGS_NONE								0x00
#define	OS_OBJECTID_FLAGS_REVISION							0x01
#define	OS_OBJECTID_FLAGS_VIRTUAL							0x02

#define OS_PORTAL_OBJECT_TYPE_UNKNOWN						0
#define OS_PORTAL_OBJECT_TYPE_USER							1
#define OS_PORTAL_OBJECT_TYPE_REPUTATION					2
#define OS_PORTAL_OBJECT_TYPE_SECTION						3
#define OS_PORTAL_OBJECT_TYPE_INSTANCE						4
#define OS_PORTAL_OBJECT_TYPE_TEXT							5
#define OS_PORTAL_OBJECT_TYPE_POST							6
#define OS_PORTAL_OBJECT_TYPE_AVATAR						7
#define OS_PORTAL_OBJECT_TYPE_FILE							8
#define OS_PORTAL_OBJECT_TYPE_TAG							9
#define OS_PORTAL_OBJECT_TYPE_ATTRIBUTE						10
#define OS_PORTAL_OBJECT_TYPE_PRIVATEMESSAGE				11
#define OS_PORTAL_OBJECT_TYPE_POLL							12
#define OS_PORTAL_OBJECT_TYPE_POLLOPTION					13
#define OS_PORTAL_OBJECT_TYPE_VOTE							14
#define OS_PORTAL_OBJECT_TYPE_MODEL							15
#define OS_PORTAL_OBJECT_TYPE_CALENDAREVENT					16
#define OS_PORTAL_OBJECT_TYPE_POLLVOTE						17
#define OS_PORTAL_OBJECT_TYPE_ATTACHMENT					18

#define OS_PORTAL_OBJECT_USER_VERSION_0						0
#define OS_PORTAL_OBJECT_REPUTATION_VERSION_0				0
#define OS_PORTAL_OBJECT_SECTION_VERSION_0					0
#define OS_PORTAL_OBJECT_INSTANCE_VERSION_0					0
#define OS_PORTAL_OBJECT_TEXT_VERSION_0						0
#define OS_PORTAL_OBJECT_POST_VERSION_0						0
#define OS_PORTAL_OBJECT_AVATAR_VERSION_0					0
#define OS_PORTAL_OBJECT_FILE_VERSION_0						0
#define OS_PORTAL_OBJECT_TAG_VERSION_0						0
#define OS_PORTAL_OBJECT_ATTRIBUTE_VERSION_0				0
#define OS_PORTAL_OBJECT_PRIVATEMESSAGE_VERSION_0			0
#define OS_PORTAL_OBJECT_POLL_VERSION_0						0
#define OS_PORTAL_OBJECT_POLL_OPTION_VERSION_0				0
#define OS_PORTAL_OBJECT_VOTE_VERSION_0						0
#define OS_PORTAL_OBJECT_MODEL_VERSION_0					0
#define OS_PORTAL_OBJECT_CALENDAREVENT_VERSION_0			0
#define OS_PORTAL_OBJECT_POLLVOTE_VERSION_0					0
#define OS_PORTAL_OBJECT_ATTACHMENT_VERSION_0				0

#define OS_PORTAL_OBJECT_USER_VERSION						OS_PORTAL_OBJECT_USER_VERSION_0
#define OS_PORTAL_OBJECT_REPUTATION_VERSION					OS_PORTAL_OBJECT_REPUTATION_VERSION_0
#define OS_PORTAL_OBJECT_SECTION_VERSION					OS_PORTAL_OBJECT_SECTION_VERSION_0
#define OS_PORTAL_OBJECT_INSTANCE_VERSION					OS_PORTAL_OBJECT_INSTANCE_VERSION_0
#define OS_PORTAL_OBJECT_TEXT_VERSION						OS_PORTAL_OBJECT_TEXT_VERSION_0
#define OS_PORTAL_OBJECT_POST_VERSION						OS_PORTAL_OBJECT_POST_VERSION_0
#define OS_PORTAL_OBJECT_AVATAR_VERSION						OS_PORTAL_OBJECT_AVATAR_VERSION_0
#define OS_PORTAL_OBJECT_FILE_VERSION						OS_PORTAL_OBJECT_FILE_VERSION_0
#define OS_PORTAL_OBJECT_TAG_VERSION						OS_PORTAL_OBJECT_TAG_VERSION_0
#define OS_PORTAL_OBJECT_ATTRIBUTE_VERSION					OS_PORTAL_OBJECT_ATTRIBUTE_VERSION_0
#define OS_PORTAL_OBJECT_PRIVATEMESSAGE_VERSION				OS_PORTAL_OBJECT_PRIVATEMESSAGE_VERSION_0
#define OS_PORTAL_OBJECT_POLL_VERSION						OS_PORTAL_OBJECT_POLL_VERSION_0
#define OS_PORTAL_OBJECT_POLL_OPTION_VERSION				OS_PORTAL_OBJECT_POLL_OPTION_VERSION_0
#define OS_PORTAL_OBJECT_VOTE_VERSION						OS_PORTAL_OBJECT_VOTE_VERSION_0
#define OS_PORTAL_OBJECT_MODEL_VERSION						OS_PORTAL_OBJECT_MODEL_VERSION_0
#define OS_PORTAL_OBJECT_CALENDAREVENT_VERSION				OS_PORTAL_OBJECT_CALENDAREVENT_VERSION_0
#define OS_PORTAL_OBJECT_POLLVOTE_VERSION					OS_PORTAL_OBJECT_POLLVOTE_VERSION_0
#define OS_PORTAL_OBJECT_ATTACHMENT_VERSION					OS_PORTAL_OBJECT_ATTACHMENT_VERSION_0

#define OS_PORTAL_OBJECT_POSITION_NULL						0

#define OS_PORTAL_OBJECT_VOTE_MIN							0
#define OS_PORTAL_OBJECT_VOTE_MAX							10
#define OS_PORTAL_OBJECT_VOTE_DEFAULT						OS_PORTAL_OBJECT_VOTE_MAX

#define OS_CALENDAR_REPEAT_NONE					0
#define OS_CALENDAR_REPEAT_WEEK					1
#define OS_CALENDAR_REPEAT_MONTH				2
#define OS_CALENDAR_REPEAT_YEAR					3

#define OS_RENDER_MODE_VIEW						0
#define OS_RENDER_MODE_EDIT						1

#define OS_INSTANCE_INHERIT_NONE				0
#define OS_INSTANCE_INHERIT_PARENT				1
#define OS_INSTANCE_INHERIT_RECURSIVE			2

#define OS_INSTANCE_AREA_UNKNOWN				0
#define OS_INSTANCE_AREA_LEFT					1
#define OS_INSTANCE_AREA_TOP					2
#define OS_INSTANCE_AREA_RIGHT					3
#define OS_INSTANCE_AREA_BOTTOM					4
#define OS_INSTANCE_AREA_CONTENT_TOP			5
#define OS_INSTANCE_AREA_CONTENT_BOTTOM			6
#define OS_INSTANCE_AREA_CONTENT				7

#define OS_CONTAINER_STYLE_VERTICAL				0
#define OS_CONTAINER_STYLE_HORIZONTAL			1
#define OS_CONTAINER_STYLE_TABBEDLEFT			2
#define OS_CONTAINER_STYLE_TABBEDTOP			3
#define OS_CONTAINER_STYLE_TABBEDRIGHT			4
#define OS_CONTAINER_STYLE_TABBEDBOTTOM			5

#define OS_REPUTATION_THRESHOLD_ALL				0		// >= -1
#define OS_REPUTATION_THRESHOLD_NEGATIVE		1		// >  -1
#define OS_REPUTATION_THRESHOLD_NOT_NEGATIVE	2		// >=  0
#define OS_REPUTATION_THRESHOLD_POSITIVE		3		// >   0
#define OS_REPUTATION_THRESHOLD_INHERIT			4

#define OS_USER_GENDER_UNSPECIFIED				0
#define OS_USER_GENDER_MALE						1
#define OS_USER_GENDER_FEMALE					2

#define OS_MESSAGE_XML_ROOT						_S("message")
#define OS_MESSAGE_XML_SUBJECT					_S("subject")
#define OS_MESSAGE_XML_BODY						_S("body")

#define OS_PORTAL_OBJECT_POLL_SINGLE_CHOICE		0
#define OS_PORTAL_OBJECT_POLL_MULTIPLE_CHOICE	1

//////////////////////////////////////////////////////////////////////

//#define OS_PORTAL_OBJECT_MAXSIZE				(1024*128)
//#define OS_PORTAL_OBJECT_AVATAR_MAXWIDTH		90
//#define OS_PORTAL_OBJECT_AVATAR_MAXHEIGHT		90

#define OS_SCHEMA_CONTACTS						_S("contacts.xsd")

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

enum PortalObjectType
{
	portalObjectTypeUnknown			= OS_PORTAL_OBJECT_TYPE_UNKNOWN,
	portalObjectTypeUser			= OS_PORTAL_OBJECT_TYPE_USER,
	portalObjectTypeReputation		= OS_PORTAL_OBJECT_TYPE_REPUTATION,
	portalObjectTypeSection			= OS_PORTAL_OBJECT_TYPE_SECTION,
	portalObjectTypeInstance		= OS_PORTAL_OBJECT_TYPE_INSTANCE,
	portalObjectTypeText			= OS_PORTAL_OBJECT_TYPE_TEXT,
	portalObjectTypePost			= OS_PORTAL_OBJECT_TYPE_POST,
	portalObjectTypeAvatar			= OS_PORTAL_OBJECT_TYPE_AVATAR,
	portalObjectTypeFile			= OS_PORTAL_OBJECT_TYPE_FILE,
	portalObjectTypeTag				= OS_PORTAL_OBJECT_TYPE_TAG,
	portalObjectTypeAttribute		= OS_PORTAL_OBJECT_TYPE_ATTRIBUTE,
	portalObjectTypePrivateMessage	= OS_PORTAL_OBJECT_TYPE_PRIVATEMESSAGE,
	portalObjectTypePoll			= OS_PORTAL_OBJECT_TYPE_POLL,
	portalObjectTypePollOption		= OS_PORTAL_OBJECT_TYPE_POLLOPTION,
	portalObjectTypeVote			= OS_PORTAL_OBJECT_TYPE_VOTE,
	portalObjectTypeModel			= OS_PORTAL_OBJECT_TYPE_MODEL,
	portalObjectTypeCalendarEvent	= OS_PORTAL_OBJECT_TYPE_CALENDAREVENT,
	portalObjectTypePollVote		= OS_PORTAL_OBJECT_TYPE_POLLVOTE,
	portalObjectTypeAttachment		= OS_PORTAL_OBJECT_TYPE_ATTACHMENT,
};

//////////////////////////////////////////////////////////////////////

// Enumerated types
enum ObjectsCalendarRepeatMode
{
	cmRepeatNone		= OS_CALENDAR_REPEAT_NONE,
	cmRepeatWeek		= OS_CALENDAR_REPEAT_WEEK,
	cmRepeatMonth		= OS_CALENDAR_REPEAT_MONTH,
	cmRepeatYear		= OS_CALENDAR_REPEAT_YEAR,
};

enum ObjectsRenderMode
{
	rmView				= OS_RENDER_MODE_VIEW,
	rmEdit				= OS_RENDER_MODE_EDIT,	
};

enum ObjectsInstanceInherit
{
	iiNone				= OS_INSTANCE_INHERIT_NONE,
	iiParent			= OS_INSTANCE_INHERIT_PARENT,
	iiRecursive			= OS_INSTANCE_INHERIT_RECURSIVE,
};

enum ObjectsInstanceArea
{
	iaUnknown			= OS_INSTANCE_AREA_UNKNOWN,
	iaLeft				= OS_INSTANCE_AREA_LEFT,
	iaTop				= OS_INSTANCE_AREA_TOP,
	iaRight				= OS_INSTANCE_AREA_RIGHT,
	iaBottom			= OS_INSTANCE_AREA_BOTTOM,
	iaContentTop		= OS_INSTANCE_AREA_CONTENT_TOP,
	iaContentBottom		= OS_INSTANCE_AREA_CONTENT_BOTTOM,
	iaContent			= OS_INSTANCE_AREA_CONTENT,
};

enum ObjectsPollType
{
	ptSingleChoice		= OS_PORTAL_OBJECT_POLL_SINGLE_CHOICE,
	ptMultipleChoice	= OS_PORTAL_OBJECT_POLL_MULTIPLE_CHOICE,
};

enum ObjectsReputationThreshold
{
	rtAll				= OS_REPUTATION_THRESHOLD_ALL,
	rtNegative			= OS_REPUTATION_THRESHOLD_NEGATIVE,
	rtNotNegative		= OS_REPUTATION_THRESHOLD_NOT_NEGATIVE,
	rtPositive			= OS_REPUTATION_THRESHOLD_POSITIVE,
	rtInherit			= OS_REPUTATION_THRESHOLD_INHERIT,
};

enum ObjectsUserGender
{
	ugUnspecified		= OS_USER_GENDER_UNSPECIFIED,
	ugMale				= OS_USER_GENDER_MALE,
	ugFemale			= OS_USER_GENDER_FEMALE,
};

//////////////////////////////////////////////////////////////////////

// Interfaces
class ObjectsICommentable;
class ObjectsIDescriptor;
class ObjectsIEntry;
class ObjectsIObject;
class ObjectsIRevisionable;

// Classes
class ObjectsAttachment;
class ObjectsAttribute;
class ObjectsAvatar;
class ObjectsCalendarEvent;
class ObjectsFile;
class ObjectsInstance;
class ObjectsMessage;
class ObjectsModel;
class ObjectsObjectsCache;
class ObjectsPoll;
class ObjectsPollOption;
class ObjectsPollVote;
class ObjectsPost;
class ObjectsPrivateMessage;
class ObjectsReputation;
class ObjectsSection;
class ObjectsSigner;
class ObjectsTag;
class ObjectsText;
class ObjectsUser;
class ObjectsVote;

/*
// Types
typedef shared_ptr<ObjectsICommentable> commentable_ptr;
typedef shared_ptr<ObjectsIDescriptor> descriptor_ptr;
typedef shared_ptr<ObjectsIEntry> entry_ptr;
typedef shared_ptr<ObjectsIObject> object_ptr;
typedef list<shared_ptr<ObjectsIObject>>::type objects_list;
typedef shared_ptr<ObjectsIRevisionable> revisionable_ptr;
typedef list<shared_ptr<ObjectsIRevisionable>>::type revisionables_list;

typedef shared_ptr<ObjectsAttachment> attachment_ptr;
typedef shared_ptr<ObjectsAttribute> attribute_ptr;
typedef shared_ptr<ObjectsAvatar> avatar_ptr;
typedef shared_ptr<ObjectsCalendarEvent> calendar_event_ptr;
typedef shared_ptr<ObjectsFile> file_ptr;
typedef shared_ptr<ObjectsInstance> instance_ptr;
typedef shared_ptr<ObjectsPrivateMessage> private_message_ptr;
typedef shared_ptr<ObjectsMessage> message_ptr;
typedef shared_ptr<ObjectsModel> model_ptr;
typedef shared_ptr<ObjectsTag> tag_ptr;
typedef list<tag_ptr>::type tags_list;
typedef shared_ptr<ObjectsPoll> poll_ptr;
typedef shared_ptr<ObjectsPollOption> poll_option_ptr;
typedef list<poll_option_ptr>::type poll_options_list;
typedef shared_ptr<ObjectsPollVote> shared_ptr<ObjectsPollVote>;
typedef shared_ptr<ObjectsPost> shared_ptr<ObjectsPost>;
typedef list<shared_ptr<ObjectsPost>>::type posts_list;
typedef shared_ptr<ObjectsReputation> shared_ptr<ObjectsReputation>;
typedef list<shared_ptr<ObjectsReputation>>::type reputations_list;
typedef shared_ptr<ObjectsSection> shared_ptr<ObjectsSection>;
typedef shared_ptr<ObjectsText> text_ptr;
typedef shared_ptr<ObjectsUser> user_ptr;
typedef shared_ptr<ObjectsVote> vote_ptr;
*/

typedef list<shared_ptr<ObjectsPost> >::type objects_posts_list;
typedef list<shared_ptr<ObjectsReputation> >::type objects_reputations_list;

typedef vector<PortalObjectType>::type ObjectsTypes;
typedef map<PortalObjectType, shared_ptr<ObjectsIDescriptor> >::type ObjectsDescriptors;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OBJECTS_OBJECTS_H
