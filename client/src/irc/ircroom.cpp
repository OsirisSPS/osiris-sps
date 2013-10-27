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
#include "ircroom.h"

#include "iirctarget.h"
#include "irccommandkick.h"
#include "irccommandmessage.h"
#include "irccommandmode.h"
#include "irccommandraw.h"
#include "ircsession.h"
#include "ircuser.h"
#include "lock.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IRCRoom::UserDetails::UserDetails(shared_ptr<IRCRoom> room, shared_ptr<boost::recursive_mutex> cs) : m_room(room),
																					m_cs(cs),
																					m_type(ircUserTypeNormal),
																					m_voice(false),
																					m_invisible(false),
																					m_banned(false)
{
	OS_ASSERT(room != null);
	OS_ASSERT(cs != null);
}

IRCRoom::UserDetails::~UserDetails()
{

}

bool IRCRoom::UserDetails::hasOperatorPrivileges() const
{
	OS_LOCK(*m_cs);

	switch(m_type)
	{
	case ircUserTypeOperator:
	case ircUserTypeHalfOperator:
									return true;
	}

	return false;
}

IRCUserType IRCRoom::UserDetails::getType() const
{
	OS_LOCK(*m_cs);
	return m_type;
}

void IRCRoom::UserDetails::setType(IRCUserType type)
{
	OS_LOCK(*m_cs);

	if(m_type != type)
	{
		m_type = type;
		markModified();
	}
}

bool IRCRoom::UserDetails::getVoice() const
{
	OS_LOCK(*m_cs);
	return m_voice;
}

void IRCRoom::UserDetails::setVoice(bool voice)
{
	OS_LOCK(*m_cs);

	if(m_voice != voice)
	{
		m_voice = voice;
		markModified();
	}
}

bool IRCRoom::UserDetails::getInvisible() const
{
	OS_LOCK(*m_cs);
	return m_invisible;
}

void IRCRoom::UserDetails::setInvisible(bool invisible)
{
	OS_LOCK(*m_cs);

	if(m_invisible != invisible)
	{
		m_invisible = invisible;
		markModified();
	}
}

bool IRCRoom::UserDetails::getBanned() const
{
	OS_LOCK(*m_cs);
	return m_banned;
}

void IRCRoom::UserDetails::setBanned(bool banned)
{
	OS_LOCK(*m_cs);

	if(m_banned != banned)
	{
		m_banned = banned;
		markModified();
	}
}

void IRCRoom::UserDetails::markModified()
{
	shared_ptr<IRCRoom> room = getRoom();
	if(room != null)
		room->markModified();
}

//////////////////////////////////////////////////////////////////////

IRCRoom::IRCRoom(shared_ptr<IIRCTarget> target) : m_target(target),
												  m_cs(OS_NEW_T(boost::recursive_mutex), os_delete_t()),
												  m_modified(true)		// Notifica lo stato iniziale
{
	OS_ASSERT(target != null);
}

IRCRoom::~IRCRoom()
{

}

std::string IRCRoom::getName() const
{
	return getTarget()->getName();
}

IRCTargetType IRCRoom::getType() const
{
	return getTarget()->getType();
}

shared_ptr<IRCSession> IRCRoom::getSession() const
{
	return getTarget()->getSession();
}

Locked<const IRCRoom::Users>::unique IRCRoom::getUsers() const
{
	return Locked<const Users>::unique(m_users, *m_cs);
}

shared_ptr<IRCUser> IRCRoom::getUser(uint32 userID) const
{
	OS_LOCK(*m_cs);

	Users::const_iterator i = m_users.find(userID);
	if(i != m_users.end())
		return i->second.first;
	
	return null;
}

shared_ptr<IRCRoom::UserDetails> IRCRoom::getUserDetails(uint32 userID) const
{
	OS_LOCK(*m_cs);

	Users::const_iterator i = m_users.find(userID);
	if(i != m_users.end())
		return i->second.second;
	
	return null;
}

shared_ptr<IRCRoom::UserDetails> IRCRoom::getLocalUserDetails()
{
	shared_ptr<IRCSession> session = getSession();
	if(session == null)
		return null;

	shared_ptr<IRCUser> user = session->getLocalUser();
	if(user == null)
	{
		OS_ASSERTFALSE();
		return null;
	}

	return ensureUser(user);
}

void IRCRoom::notifyModified(const boost::signal<void (shared_ptr<IRCRoom>)> &callback, bool force)
{
	OS_LOCK(*m_cs);

	if(m_modified || force)
	{
		m_modified = false;
		callback(get_this_ptr());
	}
}

bool IRCRoom::sendMessage(const String &message)
{
	shared_ptr<IRCSession> session = getSession();
	if(session == null)
		return false;

	shared_ptr<IRCCommandMessage> command(OS_NEW IRCCommandMessage(session));	
	command->setTarget(getTarget()->getName());
	command->setMessage(message);
	return session->addRequest(command, true);
}

bool IRCRoom::changeUserMode(uint32 userID, IRCModeType mode, bool enabled)
{
	OS_ASSERT(getLocalUserDetails()->hasOperatorPrivileges());

	shared_ptr<IRCUser> user = getUser(userID);
	if(user == null)
		return false;

	shared_ptr<IRCSession> session = getSession();
	if(session == null)
		return false;

	shared_ptr<IRCCommandMode> command(OS_NEW IRCCommandMode(session));
	command->setTarget(getName());
	command->addMode(mode, enabled);
	command->setArg(user->getName());
	return session->addRequest(command, true);
}

bool IRCRoom::kickUser(uint32 userID, const String &comment)
{
	OS_ASSERT(getLocalUserDetails()->hasOperatorPrivileges());

	shared_ptr<IRCUser> user = getUser(userID);
	if(user == null)
		return false;

	shared_ptr<IRCSession> session = getSession();
	if(session == null)
		return false;

	shared_ptr<IRCCommandKick> command(OS_NEW IRCCommandKick(session));
	command->setChannel(getName());
	command->setTarget(user->getName());
	command->setComment(comment);
	return session->addRequest(command, true);	
}

shared_ptr<IRCRoom::UserDetails> IRCRoom::ensureUser(shared_ptr<IRCUser> user)
{
	if(user == null)
	{
		OS_ASSERTFALSE();
		return null;
	}

	OS_LOCK(*m_cs);

	shared_ptr<IRCRoom::UserDetails> userDetails;

	Users::iterator i = m_users.find(user->getID());
	if(i != m_users.end())
	{
		userDetails = i->second.second;		
	}
	else
	{
		userDetails.reset(OS_NEW UserDetails(get_this_ptr(), m_cs));
		m_users[user->getID()] = std::make_pair(user, userDetails);
		m_modified = true;
	}

	OS_ASSERT(userDetails != null);
	return userDetails;
}

void IRCRoom::removeUser(shared_ptr<IRCUser> user)
{
	if(user == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	OS_LOCK(*m_cs);

	Users::iterator i = m_users.find(user->getID());
	if(i == m_users.end())
		return;

	m_users.erase(i);
	m_modified = true;
}

void IRCRoom::leave()
{
	if(getType() != ircTargetTypeChannel)
		return;

	shared_ptr<IRCSession> session = getSession();
	if(session != null)
		session->leaveChannel(getName());
}

void IRCRoom::markModified()
{
	OS_LOCK(*m_cs);
	m_modified = true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
