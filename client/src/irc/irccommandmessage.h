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

#ifndef _OS_IRC_IRCCOMMANDMESSAGE_H
#define _OS_IRC_IRCCOMMANDMESSAGE_H

#include "iirccommand.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IRCExport IRCCommandMessage : public IIRCCommand
{
    typedef IIRCCommand CommandBase;

// Construction
public:
	IRCCommandMessage(shared_ptr<IRCSession> session);
	virtual ~IRCCommandMessage();

// Attributes
public:
	inline bool isLocal() const;

	inline const std::string & getSender() const;
	inline void setSender(const std::string &sender);

	inline const std::string & getTarget() const;
	inline void setTarget(const std::string &target);

	inline const String & getMessage() const;
	inline void setMessage(const String &message);

// IIRCCommand interface
public:
	virtual IRCCommandType getType() const;

// IIRCCommand overrides
public:
	virtual std::string toString() const;

private:
	std::string m_sender;
	std::string m_target;	
	String m_message;	
};

//////////////////////////////////////////////////////////////////////

inline bool IRCCommandMessage::isLocal() const { return m_sender.empty(); }

inline const std::string & IRCCommandMessage::getSender() const { return m_sender; }
inline void IRCCommandMessage::setSender(const std::string &sender) { m_sender = sender; }

inline const std::string & IRCCommandMessage::getTarget() const { return m_target; }
inline void IRCCommandMessage::setTarget(const std::string &target) { m_target = target; }

inline const String & IRCCommandMessage::getMessage() const { return m_message; }
inline void IRCCommandMessage::setMessage(const String &message) { m_message = message; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IRC_IRCCOMMANDMESSAGE_H
