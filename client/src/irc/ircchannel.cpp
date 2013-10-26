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
#include "ircchannel.h"

#include "lock.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IRCChannel::IRCChannel(shared_ptr<IRCSession> session, const std::string &name, bool available) : BaseClass(session),
																								  m_name(name),
																								  m_available(available),
																								  m_users(0)																								
{
	OS_ASSERT(name.empty() == false);
}

IRCChannel::~IRCChannel()
{

}

std::string IRCChannel::getDescription() const
{
	OS_LOCK(m_cs);
	return m_description;
}

void IRCChannel::setDescription(const std::string &description)
{
	OS_LOCK(m_cs);
	m_description = description;
}

uint32 IRCChannel::getUsers() const
{
	OS_LOCK(m_cs);
	return m_users;
}

void IRCChannel::setUsers(uint32 users)
{
	OS_LOCK(m_cs);
	m_users = users;
}

bool IRCChannel::getAvailable() const
{
	OS_LOCK(m_cs);
	return m_available;
}

void IRCChannel::setAvailable(bool available)
{
	OS_LOCK(m_cs);
	m_available = available;
}

std::string IRCChannel::getName() const
{
	return m_name;
}

IRCTargetType IRCChannel::getType() const
{
	return ircTargetTypeChannel;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
