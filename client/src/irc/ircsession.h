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

#ifndef _OS_IRC_IRCSESSION_H
#define _OS_IRC_IRCSESSION_H

#include "base/object.h"
#include "boost/signals.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "constants.h"
#include "enable_this_ptr.h"
#include "irc.h"
#include "locked.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ConnectionsManager;
class IIRCCommand;
class IIRCTarget;
class IRCChannel;
class IRCCommandEndOfNames;
class IRCCommandJoin;
class IRCCommandKick;
class IRCCommandMessage;
class IRCCommandMode;
class IRCCommandNames;
class IRCCommandNick;
class IRCCommandPart;
class IRCCommandQuit;
class IRCConnection;
class IRCUser;
class IRCRoom;
class TCPSocket;

//////////////////////////////////////////////////////////////////////

class IRCExport IRCSession : public Object,
							 public enable_this_ptr<IRCSession>
{	
	typedef Object BaseClass;

public:
	typedef deque<shared_ptr<IIRCCommand> >::type Requests;
	typedef unordered_map<std::string, shared_ptr<IRCChannel> >::type Channels;
	typedef unordered_map<std::string, shared_ptr<IRCUser> >::type Users; 
	typedef unordered_map<std::string, shared_ptr<IRCRoom> >::type Rooms;

// Enumerated types
public:
	enum NotifyType
	{
		statusChanged	= 0,
		channelsUpdated	= 1,
	};

// Construction
public:
	IRCSession(uint32 id, shared_ptr<ConnectionsManager> connectionsManager);
	virtual ~IRCSession();

// Attributes
public:
	uint32 getID() const;
	
	shared_ptr<IRCConnection> getConnection() const;
	IRCConnectionStatus getConnectionStatus() const;	

	std::string getServer() const;
	void setServer(const std::string &server);

	uint32 getPort() const;
	void setPort(uint32 port);

	bool getEnableSSL() const;
	void setEnableSSL(bool enableSSL);

	uint32 getTimeout() const;
	void setTimeout(uint32 timeout);

	std::string getClientName() const;
	void setClientName(const std::string &clientName);

	std::string getUser() const;
	void setUser(const std::string &user);

	std::string getNick() const;
	void setNick(const std::string &nick);

	std::string getUserName(uint32 id) const;

	shared_ptr<IRCUser> getLocalUser();
	bool isLocalUser(const std::string &name) const;

	Locked<const Channels>::unique getChannels() const;

// Operations
public:
	shared_ptr<IRCConnection> openConnection(shared_ptr<TCPSocket> socket);
	void closeConnection();

	bool handleResponse(const std::string &command);

	bool addRequest(shared_ptr<IIRCCommand> request, bool update);
	shared_ptr<IIRCCommand> popRequest();	

	bool joinChannel(const std::string &channel);
	bool leaveChannel(const std::string &channel);
	bool requestChannelsList();	

	void notifyConnectionStatusChanged(IRCConnectionStatus connectionStatus);
	void notifyCommand(shared_ptr<IIRCCommand> command);

	shared_ptr<IRCRoom> findRoom(const std::string &name) const;
	bool leaveRoom(shared_ptr<IRCRoom> room);

	shared_ptr<IRCUser> findUser(const std::string &name) const;
	shared_ptr<IRCUser> findUser(uint32 id) const;
	
	shared_ptr<IRCRoom> ensureRoom(shared_ptr<IIRCTarget> target, bool update);
	
	boost::signals::connection registerNotifyCallback(const boost::signal<void (NotifyType)>::slot_type &slot);
	boost::signals::connection registerCommandsCallback(const boost::signal<void (shared_ptr<IIRCCommand>)>::slot_type &slot);	
	boost::signals::connection registerRoomsCallback(const boost::signal<void (shared_ptr<IRCRoom>)>::slot_type &slot);	

private:
	void processResponse(shared_ptr<IIRCCommand> command);

	void clear();
	
	bool initLoginRequest();

	void invalidateChannels();
	shared_ptr<IRCChannel> validateChannel(const std::string &name, const std::string &description, uint32 users);
	void updateChannels(bool notify);

	void updateRoom(shared_ptr<IRCRoom> room, bool force);
	void updateAllRooms(bool force);

	shared_ptr<IRCChannel> ensureChannel(const std::string &name);
	shared_ptr<IRCChannel> findChannel(const std::string &name) const;

	shared_ptr<IRCUser> ensureUser(const std::string &name);	
	bool renameUser(const std::string &oldName, const std::string &newName);	

	void processCommandMessage(shared_ptr<IRCCommandMessage> command);
	void processCommandJoin(shared_ptr<IRCCommandJoin> command);
	void processCommandKick(shared_ptr<IRCCommandKick> command);
	void processCommandPart(shared_ptr<IRCCommandPart> command);	
	void processCommandMode(shared_ptr<IRCCommandMode> command);
	void processCommandNames(shared_ptr<IRCCommandNames> command);
	void processCommandEndOfNames(shared_ptr<IRCCommandEndOfNames> command);
	void processCommandQuit(shared_ptr<IRCCommandQuit> command);
	void processCommandNick(shared_ptr<IRCCommandNick> command);

	void removeUserFromRoom(const std::string &roomName, const std::string &userName);

private:
	mutable boost::recursive_mutex m_dataCS;
	uint32 m_id;
	weak_ptr<ConnectionsManager> m_connectionsManager;
	weak_ptr<IRCConnection> m_connection;
	IRCConnectionStatus m_connectionStatus;	
	std::string m_server;
	uint32 m_port;
	bool m_enableSSL;
	uint32 m_timeout;
	std::string m_clientName;
	std::string m_user;
	std::string m_nick;
	Channels m_channels;
	Rooms m_rooms;
	Requests m_requests;		
	boost::recursive_mutex m_signalsCS;
	boost::signal<void (NotifyType)> m_notifySignal;
	boost::signal<void (shared_ptr<IIRCCommand>)> m_commandsSignal;		
	boost::signal<void (shared_ptr<IRCRoom>)> m_roomsSignal;	
	uint32 m_usersID;
	Users m_users;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IRC_IRCSESSION_H
