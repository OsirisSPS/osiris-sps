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

#ifndef _OS_IRC_IRCPARSER_H
#define _OS_IRC_IRCPARSER_H

#include "irc.h"
#include "sealed.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IIRCCommand;
class IRCSession;

//////////////////////////////////////////////////////////////////////

class IRCExport IRCParser : Sealed<IRCParser>
{
// Construction
public:
	IRCParser();
	~IRCParser();

// Attributes
public:
	inline bool getAlive() const;

// Operations
public:
	shared_ptr<IIRCCommand> parse(shared_ptr<IRCSession> session, const std::string &str);

	static bool parseNickname(const std::string &str, std::string &name, IRCUserType &type);

private:
	shared_ptr<IIRCCommand> parseCommandChannel(shared_ptr<IRCSession> session, const std::string &content);
	shared_ptr<IIRCCommand> parseCommandEndOfNames(shared_ptr<IRCSession> session, const std::string &content);
	shared_ptr<IIRCCommand> parseCommandJoin(shared_ptr<IRCSession> session, const std::string &user, const std::string &content);
	shared_ptr<IIRCCommand> parseCommandKick(shared_ptr<IRCSession> session, const std::string &user, const std::string &content);
	shared_ptr<IIRCCommand> parseCommandMode(shared_ptr<IRCSession> session, const std::string &user, const std::string &content);
	shared_ptr<IIRCCommand> parseCommandNames(shared_ptr<IRCSession> session, const std::string &content);	
	shared_ptr<IIRCCommand> parseCommandNick(shared_ptr<IRCSession> session, const std::string &user, const std::string &content);		
	shared_ptr<IIRCCommand> parseCommandPart(shared_ptr<IRCSession> session, const std::string &user, const std::string &content);
	shared_ptr<IIRCCommand> parseCommandPing(shared_ptr<IRCSession> session, const std::string &content);
	shared_ptr<IIRCCommand> parseCommandPrivmsg(shared_ptr<IRCSession> session, const std::string &user, const std::string &content);		
	shared_ptr<IIRCCommand> parseCommandServerMessage(shared_ptr<IRCSession> session, const std::string &content);
	shared_ptr<IIRCCommand> parseCommandQuit(shared_ptr<IRCSession> session, const std::string &user, const std::string &content);
	
private:
	bool m_alive;
};

//////////////////////////////////////////////////////////////////////

inline bool IRCParser::getAlive() const { return m_alive; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IRC_IRCPARSER_H
