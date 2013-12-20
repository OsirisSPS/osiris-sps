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
#include "ircsession.h"

#include "connectionsmanager.h"
#include "ircchannel.h"
#include "irccommandchannel.h"
#include "irccommandendofnames.h"
#include "irccommandjoin.h"
#include "irccommandkick.h"
#include "irccommandmessage.h"
#include "irccommandmode.h"
#include "irccommandnames.h"
#include "irccommandnick.h"
#include "irccommandpart.h"
#include "irccommandquit.h"
#include "irccommandraw.h"
#include "ircconnection.h"
#include "ircparser.h"
#include "ircroom.h"
#include "ircuser.h"
#include "lock.h"
#include "log.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IRCSession::IRCSession(uint32 id, shared_ptr<ConnectionsManager> connectionsManager) : m_id(id),
																					   m_connectionStatus(ircConnectionStatusDisconnected),
																					   m_connectionsManager(connectionsManager),
																					   m_port(0),
																					   m_enableSSL(false),
																					   m_timeout(OS_IRC_DEFAULT_TIMEOUT),
																					   m_usersID(0)
{
	OS_ASSERT(connectionsManager != nullptr);
}

IRCSession::~IRCSession()
{

}

uint32 IRCSession::getID() const
{
	OS_LOCK(m_dataCS);
	return m_id;
}

shared_ptr<IRCConnection> IRCSession::getConnection() const
{
	OS_LOCK(m_dataCS);
	return m_connection.lock();
}

IRCConnectionStatus IRCSession::getConnectionStatus() const
{
	OS_LOCK(m_dataCS);
	return m_connectionStatus;
}

std::string IRCSession::getServer() const
{
	OS_LOCK(m_dataCS);
	return m_server;
}

void IRCSession::setServer(const std::string &server)
{
	OS_ASSERT(getConnection() == nullptr);

	OS_LOCK(m_dataCS);
	m_server = server;
}

uint32 IRCSession::getPort() const
{
	OS_LOCK(m_dataCS);
	return m_port;
}

void IRCSession::setPort(uint32 port)
{
	OS_ASSERT(getConnection() == nullptr);

	OS_LOCK(m_dataCS);
	m_port = port;
}

bool IRCSession::getEnableSSL() const
{
	OS_LOCK(m_dataCS);
	return m_enableSSL;
}

void IRCSession::setEnableSSL(bool enableSSL)
{
	OS_ASSERT(getConnection() == nullptr);

	OS_LOCK(m_dataCS);
	m_enableSSL = enableSSL;
}

uint32 IRCSession::getTimeout() const
{
	OS_LOCK(m_dataCS);
	return m_timeout;
}

void IRCSession::setTimeout(uint32 timeout)
{
	OS_ASSERT(getConnection() == nullptr);

	OS_LOCK(m_dataCS);
	m_timeout = timeout;
}

std::string IRCSession::getClientName() const
{
	OS_LOCK(m_dataCS);
	return m_clientName;
}

void IRCSession::setClientName(const std::string &clientName)
{
	OS_LOCK(m_dataCS);
	m_clientName = clientName;
}

std::string IRCSession::getUser() const
{
	OS_LOCK(m_dataCS);
	return m_user;
}

void IRCSession::setUser(const std::string &user)
{
	OS_ASSERT(getConnection() == nullptr);

	OS_LOCK(m_dataCS);
	m_user = user;
}

std::string IRCSession::getNick() const
{
	OS_LOCK(m_dataCS);
	return m_nick;
}

void IRCSession::setNick(const std::string &nick)
{
	OS_ASSERT(getConnection() == nullptr);

	OS_LOCK(m_dataCS);
	m_nick = nick;
}

std::string IRCSession::getUserName(uint32 id) const
{
	OS_LOCK(m_dataCS);

	for(Users::const_iterator i = m_users.begin(); i != m_users.end(); ++i)
	{
		if(i->second->getID() == id)
			return i->first;
	}

	return constants::empty_string;
}

shared_ptr<IRCUser> IRCSession::getLocalUser()
{
	OS_LOCK(m_dataCS);
	return ensureUser(m_nick);
}

bool IRCSession::isLocalUser(const std::string &name) const
{
	OS_LOCK(m_dataCS);
	return m_nick == name;
}

Locked<const IRCSession::Channels>::unique IRCSession::getChannels() const
{
	return Locked<const Channels>::unique(m_channels, m_dataCS); 	
}

shared_ptr<IRCConnection> IRCSession::openConnection(shared_ptr<TCPSocket> socket)
{
	if(socket == nullptr)
	{
		OS_ASSERTFALSE();
		return nullptr;
	}

	OS_LOCK(m_dataCS);

	shared_ptr<IRCConnection> connection = m_connection.lock();
	if(connection != nullptr)
		return connection;

	shared_ptr<ConnectionsManager> connectionsManager = m_connectionsManager.lock();
	if(connectionsManager == nullptr)
		return nullptr;

	clear();
	
	initLoginRequest();

	connection.reset(OS_NEW IRCConnection(get_this_ptr(), socket, connectionsManager));
	connectionsManager->addConnection(connection, true);
	m_connection = connection;

	return connection;
}

void IRCSession::closeConnection()
{
	OS_LOCK(m_dataCS);

	shared_ptr<IRCConnection> connection = m_connection.lock();
	if(connection == nullptr)
	{
		OS_ASSERT(m_connectionStatus == ircConnectionStatusDisconnected);
		return;
	}

	m_connection.reset();

	shared_ptr<ConnectionsManager> connectionsManager = m_connectionsManager.lock();
	if(connectionsManager != nullptr)
		connectionsManager->removeConnection(connection, true);
}

bool IRCSession::handleResponse(const std::string &command)
{
	IRCParser parser;
	shared_ptr<IIRCCommand> response = parser.parse(get_this_ptr(), command);
	if(response != nullptr)
	{
		processResponse(response);
		notifyCommand(response);

	    shared_ptr<IIRCCommand> reply = response->getReply();
		if(reply != nullptr)
			addRequest(reply, false);
	}
	else
	{
		OS_LOG_DEBUG(_S("Unknown IRC command '") + command + _S("'"));
	}

	return parser.getAlive();
}

bool IRCSession::addRequest(shared_ptr<IIRCCommand> request, bool update)
{
	if(request == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	{
		OS_LOCK(m_dataCS);
		m_requests.push_back(request);
	}

	if(update)
	{
		shared_ptr<IRCConnection> connection = m_connection.lock();
		if(connection != nullptr)
			connection->poll();
	}

	return true;
}

shared_ptr<IIRCCommand> IRCSession::popRequest()
{
	OS_LOCK(m_dataCS);

	if(m_requests.empty())
		return nullptr;

	return utils::pop_front(m_requests);
}

bool IRCSession::joinChannel(const std::string &channel)
{
	if(channel.empty())
	{
		OS_ASSERTFALSE();
		return false;
	}

	std::string command;
	command.append("JOIN " + channel);
	command.append(OS_IRC_COMMANDS_TERMINATOR);

	return addRequest(shared_ptr<IIRCCommand>(OS_NEW IRCCommandRaw(get_this_ptr(), command)), true);
}

bool IRCSession::leaveChannel(const std::string &channel)
{
	if(channel.empty())
	{
		OS_ASSERTFALSE();
		return false;
	}

	std::string command;
	command.append("PART " + channel);
	command.append(OS_IRC_COMMANDS_TERMINATOR);

	return addRequest(shared_ptr<IIRCCommand>(OS_NEW IRCCommandRaw(get_this_ptr(), command)), true);	
}

bool IRCSession::requestChannelsList()
{
	std::string command;
	command.append("LIST");
	command.append(OS_IRC_COMMANDS_TERMINATOR);

	return addRequest(shared_ptr<IIRCCommand>(OS_NEW IRCCommandRaw(get_this_ptr(), command)), true);
}

void IRCSession::notifyConnectionStatusChanged(IRCConnectionStatus connectionStatus)
{
	bool notify = false;

	{
		OS_LOCK(m_dataCS);

		if(m_connectionStatus != connectionStatus)
		{
			m_connectionStatus = connectionStatus;
			notify = true;
		}
	}

	if(notify)
	{
		OS_LOCK(m_signalsCS);
		m_notifySignal(statusChanged);
	}
}

void IRCSession::notifyCommand(shared_ptr<IIRCCommand> command)
{
	OS_ASSERT(command != nullptr);
	OS_ASSERT(command->getSession() == get_this_ptr());

	OS_LOCK(m_signalsCS);
	m_commandsSignal(command);
}

shared_ptr<IRCRoom> IRCSession::findRoom(const std::string &name) const
{
	OS_LOCK(m_dataCS);

	Rooms::const_iterator i = m_rooms.find(name);
	if(i != m_rooms.end())
		return i->second;

	return nullptr;
}

bool IRCSession::leaveRoom(shared_ptr<IRCRoom> room)
{
	if(room == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	OS_LOCK(m_dataCS);

	Rooms::iterator i = m_rooms.find(room->getName());
	if(i == m_rooms.end())
		return false;

	m_rooms.erase(i);

	if(room->getType() == ircTargetTypeChannel)
		leaveChannel(room->getName());	

	return true;
}

shared_ptr<IRCUser> IRCSession::findUser(const std::string &name) const
{
	OS_LOCK(m_dataCS);

	Users::const_iterator i = m_users.find(name);
	if(i != m_users.end())
		return i->second;

	return nullptr;
}

shared_ptr<IRCUser> IRCSession::findUser(uint32 id) const
{
	OS_LOCK(m_dataCS);

	for(Users::const_iterator i = m_users.begin(); i != m_users.end(); ++i)
	{
		if(i->second->getID() == id)
			return i->second;
	}

	return nullptr;
}

shared_ptr<IRCRoom> IRCSession::ensureRoom(shared_ptr<IIRCTarget> target, bool update)
{
	if(target == nullptr)
	{
		OS_ASSERTFALSE();
		return nullptr;
	}

	OS_LOCK(m_dataCS);

	const std::string &name = target->getName();
	bool needUpdate = false;

	shared_ptr<IRCRoom> room;

	Rooms::const_iterator i = m_rooms.find(name);
	if(i != m_rooms.end())
	{
		room = i->second;
	}
	else
	{
		room.reset(OS_NEW IRCRoom(target));
		m_rooms[name] = room;				
		needUpdate = true;
	}

	OS_ASSERT(room != nullptr);

	if(needUpdate && update)
		updateRoom(room, false);

	return room;
}

boost::signals::connection IRCSession::registerNotifyCallback(const boost::signal<void (NotifyType)>::slot_type &slot)
{
	OS_LOCK(m_signalsCS);
	return m_notifySignal.connect(slot);
}

boost::signals::connection IRCSession::registerCommandsCallback(const boost::signal<void (shared_ptr<IIRCCommand>)>::slot_type &slot)
{
	OS_LOCK(m_signalsCS);
	return m_commandsSignal.connect(slot);
}

boost::signals::connection IRCSession::registerRoomsCallback(const boost::signal<void (shared_ptr<IRCRoom>)>::slot_type &slot)
{
	OS_LOCK(m_signalsCS);
	return m_roomsSignal.connect(slot);
}

void IRCSession::processResponse(shared_ptr<IIRCCommand> command)
{
	OS_ASSERT(command != nullptr);
	
	switch(command->getType())
	{
	case ircCommandTypeMessage:				processCommandMessage(boost::dynamic_pointer_cast<IRCCommandMessage>(command));
											break;

	case ircCommandTypeChannelListStart:	invalidateChannels();
											break;

	case ircCommandTypeChannel:				{
												shared_ptr<IRCCommandChannel> commandChannel = boost::dynamic_pointer_cast<IRCCommandChannel>(command);
												OS_ASSERT(commandChannel != nullptr);
												validateChannel(commandChannel->getChannelName(), commandChannel->getChannelDescription(), commandChannel->getChannelUsers());
											}

											break;

	case ircCommandTypeChannelListEnd:		updateChannels(true);
											break;

	case ircCommandTypeJoin:				processCommandJoin(boost::dynamic_pointer_cast<IRCCommandJoin>(command));
											break;

	case ircCommandTypePart:				processCommandPart(boost::dynamic_pointer_cast<IRCCommandPart>(command));
											break;

	case ircCommandTypeNames:				processCommandNames(boost::dynamic_pointer_cast<IRCCommandNames>(command));
											break;

	case ircCommandTypeEndOfNames:			processCommandEndOfNames(boost::dynamic_pointer_cast<IRCCommandEndOfNames>(command));
											break;

	case ircCommandTypeQuit:				processCommandQuit(boost::dynamic_pointer_cast<IRCCommandQuit>(command));
											break;

	case ircCommandTypeNick:				processCommandNick(boost::dynamic_pointer_cast<IRCCommandNick>(command));
											break;

	case ircCommandTypeMode:				processCommandMode(boost::dynamic_pointer_cast<IRCCommandMode>(command));
											break;

	case ircCommandTypeKick:				processCommandKick(boost::dynamic_pointer_cast<IRCCommandKick>(command));
											break;
	}
}

void IRCSession::clear()
{
	OS_LOCK(m_dataCS);

	m_requests.clear();
	m_rooms.clear();
	m_channels.clear();	
	m_usersID = 0;
	m_users.clear();
}

bool IRCSession::initLoginRequest()
{
	std::string nick = getNick();
	std::string user = getUser();
	if(user.empty())
		user = nick;
	std::string clientName = getClientName();

	OS_ASSERT(nick.empty() == false);
	OS_ASSERT(user.empty() == false);
	OS_ASSERT(clientName.empty() == false);

	std::string command;
	command.append("PASS NOPASS");
	command.append(OS_IRC_COMMANDS_TERMINATOR);	
	command.append("NICK " + nick);
	command.append(OS_IRC_COMMANDS_TERMINATOR);			
	command.append("USER " + user + " USING " + clientName + " :" + nick);
	command.append(OS_IRC_COMMANDS_TERMINATOR);

	return addRequest(shared_ptr<IIRCCommand>(OS_NEW IRCCommandRaw(get_this_ptr(), command)), false);
}

void IRCSession::invalidateChannels()
{
	OS_LOCK(m_dataCS);

	for(Channels::iterator i = m_channels.begin(); i != m_channels.end(); i->second->setAvailable(false), ++i);
}

shared_ptr<IRCChannel> IRCSession::validateChannel(const std::string &name, const std::string &description, uint32 users)
{
	shared_ptr<IRCChannel> channel = ensureChannel(name);
	if(channel == nullptr)
	{
		OS_ASSERTFALSE();
		return nullptr;
	}

	channel->setAvailable(true);
	channel->setDescription(description);
	channel->setUsers(users);

	return channel;
}

void IRCSession::updateChannels(bool notify)
{
	{
		OS_LOCK(m_dataCS);

		for(Channels::iterator i = m_channels.begin(); i != m_channels.end();)
		{
			Channels::iterator y = i;
			++i;
		
			if(y->second->getAvailable() == false)
				m_channels.erase(y);	
		}
	}

	if(notify)
	{
		OS_LOCK(m_signalsCS);
		m_notifySignal(channelsUpdated);
	}
}

void IRCSession::updateRoom(shared_ptr<IRCRoom> room, bool force)
{
	OS_ASSERT(room != nullptr);

	OS_LOCK(m_signalsCS);
	room->notifyModified(m_roomsSignal, force);
}

void IRCSession::updateAllRooms(bool force)
{
	OS_LOCK(m_dataCS);
	
	for(Rooms::const_iterator i = m_rooms.begin(); i != m_rooms.end(); ++i)
	{
		updateRoom(i->second, force);
	}
}

shared_ptr<IRCChannel> IRCSession::ensureChannel(const std::string &name)
{
	OS_LOCK(m_dataCS);

	shared_ptr<IRCChannel> channel;

	Channels::const_iterator i = m_channels.find(name);
	if(i != m_channels.end())
	{
		channel = i->second;
	}
	else
	{
		channel.reset(OS_NEW IRCChannel(get_this_ptr(), name));		
		m_channels[name] = channel;
	}

	OS_ASSERT(channel != nullptr);
	return channel;
}

shared_ptr<IRCChannel> IRCSession::findChannel(const std::string &name) const
{
	OS_LOCK(m_dataCS);

	Channels::const_iterator i = m_channels.find(name);
	if(i != m_channels.end())
		return i->second;

	return nullptr;
}

shared_ptr<IRCUser> IRCSession::ensureUser(const std::string &name)
{
	OS_LOCK(m_dataCS);

	shared_ptr<IRCUser> user;

	Users::const_iterator i = m_users.find(name);
	if(i != m_users.end())
	{
		user = i->second;
	}
	else
	{
		user.reset(OS_NEW IRCUser(get_this_ptr(), m_usersID));
		m_users[name] = user;
		m_usersID++;
	}

	OS_ASSERT(user != nullptr);
	return user;
}

bool IRCSession::renameUser(const std::string &oldName, const std::string &newName)
{
	OS_LOCK(m_dataCS);

	OS_ASSERT(m_users.find(newName) == m_users.end());
	
	Users::iterator i = m_users.find(oldName);
	if((i == m_users.end()) || (m_users.find(newName) != m_users.end()))
		return false;

	shared_ptr<IRCUser> user = i->second;
	m_users.erase(i);
	m_users[newName] = user;	

	return true;
}

void IRCSession::processCommandMessage(shared_ptr<IRCCommandMessage> command)
{
	OS_ASSERT(command != nullptr);

	OS_LOCK(m_dataCS);

	if(isLocalUser(command->getTarget()))
	{
		std::string name;
		IRCUserType type = ircUserTypeNormal;
		if(IRCParser::parseNickname(command->getSender(), name, type))
		{
			OS_ASSERT(type == ircUserTypeNormal);
			ensureRoom(ensureUser(name), true);			
		}
	}
}

void IRCSession::processCommandJoin(shared_ptr<IRCCommandJoin> command)
{
	OS_ASSERT(command != nullptr);

	shared_ptr<IRCRoom> room;

	{
		OS_LOCK(m_dataCS);

		const std::string &channelName = command->getChannel();

		shared_ptr<IRCChannel> channel;

		Rooms::const_iterator i = m_rooms.find(channelName);
		
		if(isLocalUser(command->getNick()))
			channel = ensureChannel(channelName);
		else
			channel = findChannel(channelName);
		
		if(channel != nullptr)
		{
			room = ensureRoom(channel, false);
			OS_ASSERT(room != nullptr);

			std::string nickname;
			IRCUserType userType = ircUserTypeNormal;
			if(IRCParser::parseNickname(command->getNick(), nickname, userType))
			{
				shared_ptr<IRCRoom::UserDetails> userDetails = room->ensureUser(ensureUser(nickname));				
				OS_ASSERT(userDetails != nullptr);
				if(userDetails != nullptr)
					userDetails->setType(userType);
			}
		}		
	}

	if(room != nullptr)
		updateRoom(room, false);	
}

void IRCSession::processCommandKick(shared_ptr<IRCCommandKick> command)
{
	removeUserFromRoom(command->getChannel(), command->getTarget());
}

void IRCSession::processCommandMode(shared_ptr<IRCCommandMode> command)
{
	OS_ASSERT(command != nullptr);

	if(command->getTargetType() != ircTargetTypeChannel)
		return;

	shared_ptr<IRCRoom> room = findRoom(command->getTarget());
	if(room == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	shared_ptr<IRCUser> user;
	if(command->hasArg())
		user = findUser(command->getArg());

	shared_ptr<IRCRoom::UserDetails> userDetails;
	if(user != nullptr)
		userDetails = room->getUserDetails(user->getID());

	const IRCCommandMode::ModeOptions &modes = command->getModeOptions();
	for(IRCCommandMode::ModeOptions::const_iterator i = modes.begin(); i != modes.end(); ++i)
	{
		switch(i->first)
		{
		case ircModeTypeUnknown:		
										break;

		case ircModeTypeOperator:		if(userDetails != nullptr)
											userDetails->setType(i->second ? ircUserTypeOperator : ircUserTypeNormal);

										break;

		case ircModeTypeHalfOperator:	if(userDetails != nullptr)
											userDetails->setType(i->second ? ircUserTypeHalfOperator : ircUserTypeNormal);

										break;


		case ircModeTypeVoice:			if(userDetails != nullptr)
											userDetails->setVoice(i->second);

										break;

		case ircModeTypeInvisible:		if(userDetails != nullptr)
											userDetails->setInvisible(i->second);

										break;

		case ircModeTypeBan:			if(userDetails != nullptr)
											userDetails->setBanned(i->second);

										break;

		default:						OS_ASSERTFALSE();
										break;
		}
	}

	updateRoom(room, false);
}

void IRCSession::processCommandPart(shared_ptr<IRCCommandPart> command)
{
	removeUserFromRoom(command->getChannel(), command->getNick());
}

void IRCSession::processCommandNames(shared_ptr<IRCCommandNames> command)
{
	OS_ASSERT(command != nullptr);

	OS_LOCK(m_dataCS);	

	Rooms::const_iterator r = m_rooms.find(command->getChannel());
	if(r == m_rooms.end())
	{
		OS_ASSERTFALSE();
		return;
	}

	shared_ptr<IRCRoom> room = r->second;
	OS_ASSERT(room != nullptr);
	
	const IRCCommandNames::Users &users = command->getUsers();
	for(IRCCommandNames::Users::const_iterator i = users.begin(); i != users.end(); ++i)
	{
		std::string nickname;
		IRCUserType userType = ircUserTypeNormal;
		if(IRCParser::parseNickname(*i, nickname, userType))
		{	
			shared_ptr<IRCRoom::UserDetails> userDetails = room->ensureUser(ensureUser(nickname));				
			OS_ASSERT(userDetails != nullptr);
			if(userDetails != nullptr)
				userDetails->setType(userType);
		}
	}	
}

void IRCSession::processCommandEndOfNames(shared_ptr<IRCCommandEndOfNames> command)
{
	OS_ASSERT(command != nullptr);

	shared_ptr<IRCRoom> room;

	{
		OS_LOCK(m_dataCS);	

		Rooms::const_iterator i = m_rooms.find(command->getChannel());
		if(i == m_rooms.end())
		{
			OS_ASSERTFALSE();
			return;
		}

		room = i->second;
	}

	OS_ASSERT(room != nullptr);
	updateRoom(room, false);
}

void IRCSession::processCommandQuit(shared_ptr<IRCCommandQuit> command)
{
	OS_ASSERT(command != nullptr);

	shared_ptr<IRCUser> user = findUser(command->getNick());
	if(user == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	{
		OS_LOCK(m_dataCS);	

		for(Rooms::const_iterator i = m_rooms.begin(); i != m_rooms.end(); ++i)
		{
			i->second->removeUser(user);
		}
	}

	updateAllRooms(false);
}

void IRCSession::processCommandNick(shared_ptr<IRCCommandNick> command)
{
	OS_ASSERT(command != nullptr);

	if(renameUser(command->getOldNick(), command->getNewNick()) == false)
	{
		OS_ASSERTFALSE();
		return;
	}

	updateAllRooms(true);
}

void IRCSession::removeUserFromRoom(const std::string &roomName, const std::string &userName)
{
	shared_ptr<IRCRoom> room;

	bool force = false;

	{
		OS_LOCK(m_dataCS);		

		Rooms::iterator r = m_rooms.find(roomName);
		if(r == m_rooms.end())
			return;

		room = r->second;

		if(isLocalUser(userName))
		{
			m_rooms.erase(r);		
			force = true;
		}
		else
		{		
			Users::const_iterator u = m_users.find(userName);
			if(u == m_users.end())
			{
				OS_ASSERTFALSE();
				return;
			}

			OS_ASSERT(room != nullptr);
			room->removeUser(u->second);
		}
	}

	OS_ASSERT(room != nullptr);
	updateRoom(room, force);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
