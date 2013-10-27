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

#ifndef _OS_IRC_IRCROOM_H
#define _OS_IRC_IRCROOM_H

#include "base/object.h"
#include "boost/signals.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "enable_this_ptr.h"
#include "irc.h"
#include "locked.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IIRCTarget;
class IRCSession;
class IRCUser;

//////////////////////////////////////////////////////////////////////

class IRCExport IRCRoom : public Object,
						  public enable_this_ptr<IRCRoom>
{	
public:
	class IRCExport UserDetails : public Object
	{
	// Construction
	public:
		UserDetails(shared_ptr<IRCRoom> room, shared_ptr<boost::recursive_mutex> cs);
		virtual ~UserDetails();

	// Attributes
	public:
		inline shared_ptr<IRCRoom> getRoom() const;

		bool hasOperatorPrivileges() const;

		IRCUserType getType() const;
		void setType(IRCUserType type);

		bool getVoice() const;
		void setVoice(bool voice);

		bool getInvisible() const;
		void setInvisible(bool invisible);

		bool getBanned() const;
		void setBanned(bool banned);

	// Operations
	private:
		void markModified();
		
	private:
		weak_ptr<IRCRoom> m_room;
		shared_ptr<boost::recursive_mutex> m_cs;
		IRCUserType m_type;
		bool m_voice;
		bool m_invisible;
		bool m_banned;
	};

	typedef unordered_map<uint32, std::pair<shared_ptr<IRCUser>, shared_ptr<UserDetails> > >::type Users;

// Construction
public:
	IRCRoom(shared_ptr<IIRCTarget> target);
	virtual ~IRCRoom();

// Attributes
public:
	inline shared_ptr<IIRCTarget> getTarget() const;

	std::string getName() const;
	IRCTargetType getType() const;

	shared_ptr<IRCSession> getSession() const;

	Locked<const Users>::unique getUsers() const;
	
	shared_ptr<IRCUser> getUser(uint32 userID) const;
	shared_ptr<UserDetails> getUserDetails(uint32 userID) const;

	shared_ptr<UserDetails> getLocalUserDetails();

// Operations
public:
	void notifyModified(const boost::signal<void (shared_ptr<IRCRoom>)> &callback, bool force);
	bool sendMessage(const String &message);
	bool changeUserMode(uint32 userID, IRCModeType mode, bool enabled);
	bool kickUser(uint32 userID, const String &comment = String::EMPTY);
	
	shared_ptr<UserDetails> ensureUser(shared_ptr<IRCUser> user);
	void removeUser(shared_ptr<IRCUser> user);

	void leave();

	void markModified();

private:
	mutable shared_ptr<boost::recursive_mutex> m_cs;
	shared_ptr<IIRCTarget> m_target;
	uint32 m_id;
	Users m_users;
	mutable bool m_modified;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IRCRoom> IRCRoom::UserDetails::getRoom() const { return m_room.lock(); }

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IIRCTarget> IRCRoom::getTarget() const { return m_target; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IRC_IRCROOM_H
