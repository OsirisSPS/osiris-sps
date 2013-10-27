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

#ifndef _OS_IRC_IRCCOMMANDKICK_H
#define _OS_IRC_IRCCOMMANDKICK_H

#include "iirccommand.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IRCExport IRCCommandKick : public IIRCCommand
{
    typedef IIRCCommand CommandBase;

// Construction
public:
	IRCCommandKick(shared_ptr<IRCSession> session);
	virtual ~IRCCommandKick();

// Attributes
public:
	inline const std::string & getUser() const;
	inline void setUser(const std::string &user);

	inline const std::string & getChannel() const;
	inline void setChannel(const std::string &channel);

	inline const std::string & getTarget() const;
	inline void setTarget(const std::string &target);

	inline const String & getComment() const;
	inline void setComment(const String &comment);
	
// IIRCCommand interface
public:
	virtual IRCCommandType getType() const;

// IRCCommandMode overrides
public:
	virtual std::string toString() const;

private:
	std::string m_user;
	std::string m_channel;
	std::string m_target;
	String m_comment;
};

//////////////////////////////////////////////////////////////////////

inline const std::string & IRCCommandKick::getUser() const { return m_user; }
inline void IRCCommandKick::setUser(const std::string &user) { m_user = user; }

inline const std::string & IRCCommandKick::getChannel() const { return m_channel; }
inline void IRCCommandKick::setChannel(const std::string &channel) { m_channel = channel; }

inline const std::string & IRCCommandKick::getTarget() const { return m_target; }
inline void IRCCommandKick::setTarget(const std::string &target) { m_target = target; }

inline const String & IRCCommandKick::getComment() const { return m_comment; }
inline void IRCCommandKick::setComment(const String &comment) { m_comment = comment; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IRC_IRCCOMMANDKICK_H
