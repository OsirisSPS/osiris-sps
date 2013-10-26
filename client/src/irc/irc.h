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

#ifndef _OS_IRC_IRC_H
#define _OS_IRC_IRC_H

//////////////////////////////////////////////////////////////////////

#define OS_IRC_DEFAULT_TIMEOUT						300000	// 5 minuti: solitamente il server ping i client ogni 2/3 minuti, quindi 5 è un compromesso valido anche per la "lettura" in caso di inattività

#define OS_IRC_COMMANDS_TERMINATOR   				"\r\n"

#define OS_IRC_REPLY_WELCOME						001
#define OS_IRC_REPLY_YOURHOST						002
#define OS_IRC_REPLY_CREATED						003
#define OS_IRC_REPLY_MYINFO							004
#define OS_IRC_REPLY_BOUNCE							005

#define OS_IRC_REPLY_STATSDLINE						250
#define OS_IRC_REPLY_LUSERCLIENT					251
#define OS_IRC_REPLY_LUSEROP						252
#define OS_IRC_REPLY_LUSERUNKNOWN					253
#define OS_IRC_REPLY_LUSERCHANNELS					254
#define OS_IRC_REPLY_LUSERME						255
#define OS_IRC_REPLY_ADMINME						256
#define OS_IRC_REPLY_ADMINLOC1						257
#define OS_IRC_REPLY_ADMINLOC2						258
#define OS_IRC_REPLY_ADMINEMAIL						259
#define OS_IRC_REPLY_TRYAGAIN						263
#define OS_IRC_REPLY_LOCALUSERS						265
#define OS_IRC_REPLY_GLOBALUSERS					266

#define OS_IRC_REPLY_AWAY							301
#define OS_IRC_REPLY_USERHOST						302
#define OS_IRC_REPLY_ISON							303
#define OS_IRC_REPLY_UNAWAY							305
#define OS_IRC_REPLY_NOWAWAY						306
#define OS_IRC_REPLY_WHOISUSER						311
#define OS_IRC_REPLY_WHOISSERVER					312
#define OS_IRC_REPLY_WHOISOPERATOR					313
#define OS_IRC_REPLY_WHOWASUSER						314
#define OS_IRC_REPLY_ENDOFWHO						315
#define OS_IRC_REPLY_WHOISIDLE						317
#define OS_IRC_REPLY_ENDOFWHOIS						318
#define OS_IRC_REPLY_WHOISCHANNELS					319
#define OS_IRC_REPLY_LISTSTART						321
#define OS_IRC_REPLY_LIST							322
#define OS_IRC_REPLY_LISTEND						323
#define OS_IRC_REPLY_CHANNELMODEIS					324
#define OS_IRC_REPLY_UNIQOPIS						325
#define OS_IRC_REPLY_NOTOPIC						331
#define OS_IRC_REPLY_TOPIC							332
#define OS_IRC_REPLY_INVITING						341
#define OS_IRC_REPLY_SUMMONING						342
#define OS_IRC_REPLY_INVITELIST						346
#define OS_IRC_REPLY_ENDOFINVITELIST				347
#define OS_IRC_REPLY_EXCEPTLIST						348
#define OS_IRC_REPLY_ENDOFEXCEPTLIST				349
#define OS_IRC_REPLY_VERSION						351
#define OS_IRC_REPLY_WHOREPLY						352
#define OS_IRC_REPLY_NAMES							353
#define OS_IRC_REPLY_LINKS							364
#define OS_IRC_REPLY_ENDOFLINKS						365
#define OS_IRC_REPLY_ENDOFNAMES						366
#define OS_IRC_REPLY_BANLIST						367
#define OS_IRC_REPLY_ENDOFBANLIST					368
#define OS_IRC_REPLY_ENDOFWHOWAS					369
#define OS_IRC_REPLY_INFO							371
#define OS_IRC_REPLY_MOTD							372
#define OS_IRC_REPLY_ENDOFINFO						374
#define OS_IRC_REPLY_MOTDSTART						375
#define OS_IRC_REPLY_ENDOFMOTD						376
#define OS_IRC_REPLY_YOUREOPER						381
#define OS_IRC_REPLY_REHASHING						382
#define OS_IRC_REPLY_YOURESERVICE					383
#define OS_IRC_REPLY_TIME							391
#define OS_IRC_REPLY_USERSSTART						392
#define OS_IRC_REPLY_USERS							393
#define OS_IRC_REPLY_ENDOFUSERS						394
#define OS_IRC_REPLY_NOUSERS						395

#define OS_IRC_ERROR_NOSUCHNICK						401
#define OS_IRC_ERROR_NOSUCHSERVER					402
#define OS_IRC_ERROR_NOSUCHCHANNEL					403
#define OS_IRC_ERROR_CANNOTSENDTOCHAN				404
#define OS_IRC_ERROR_TOOMANYCHANNELS				405
#define OS_IRC_ERROR_WASNOSUCHNICK					406
#define OS_IRC_ERROR_TOOMANYTARGETS					407
#define OS_IRC_ERROR_NOSUCHSERVICE					408
#define OS_IRC_ERROR_NOORIGIN						409
#define OS_IRC_ERROR_NORECIPIENT					411
#define OS_IRC_ERROR_NOTEXTTOSEND					412
#define OS_IRC_ERROR_NOTOPLEVEL						413
#define OS_IRC_ERROR_WILDTOPLEVEL					414
#define OS_IRC_ERROR_BADMASK						415
#define OS_IRC_ERROR_UNKNOWNCOMMAND					421
#define OS_IRC_ERROR_NOMOTD							422
#define OS_IRC_ERROR_NOADMININFO					423
#define OS_IRC_ERROR_FILEERROR						424
#define OS_IRC_ERROR_NONICKNAMEGIVEN				431
#define OS_IRC_ERROR_ERRONEUSNICKNAME				432
#define OS_IRC_ERROR_NICKNAMEINUSE					433
#define OS_IRC_ERROR_NICKCOLLISION					436
#define OS_IRC_ERROR_UNAVAILRESOURCE				437
#define OS_IRC_ERROR_USERNOTINCHANNEL				441
#define OS_IRC_ERROR_NOTONCHANNEL					442
#define OS_IRC_ERROR_USERONCHANNEL					443
#define OS_IRC_ERROR_NOLOGIN						444
#define OS_IRC_ERROR_SUMMONDISABLED					445
#define OS_IRC_ERROR_USERSDISABLED					446
#define OS_IRC_ERROR_NOTREGISTERED					451
#define OS_IRC_ERROR_NEEDMOREPARAMS					461
#define OS_IRC_ERROR_ALREADYREGISTRED				462
#define OS_IRC_ERROR_NOPERMFORHOST					463
#define OS_IRC_ERROR_PASSWDMISMATCH					464
#define OS_IRC_ERROR_YOUREBANNEDCREEP				465
#define OS_IRC_ERROR_YOUWILLBEBANNED				466
#define OS_IRC_ERROR_KEYSET							467
#define OS_IRC_ERROR_CHANNELISFULL					471
#define OS_IRC_ERROR_UNKNOWNMODE					472
#define OS_IRC_ERROR_INVITEONLYCHAN					473
#define OS_IRC_ERROR_BANNEDFROMCHAN					474
#define OS_IRC_ERROR_BADCHANNELKEY					475
#define OS_IRC_ERROR_BADCHANMASK					476
#define OS_IRC_ERROR_NOCHANMODES					477
#define OS_IRC_ERROR_BANLISTFULL					478
#define OS_IRC_ERROR_NOPRIVILEGES					481
#define OS_IRC_ERROR_CHANOPRIVSNEEDED				482
#define OS_IRC_ERROR_CANTKILLSERVER					483
#define OS_IRC_ERROR_RESTRICTED						484
#define OS_IRC_ERROR_UNIQOPPRIVSNEEDED				485
#define OS_IRC_ERROR_NOOPERHOST						491

#define OS_IRC_ERROR_UMODEUNKNOWNFLAG				501
#define OS_IRC_ERROR_USERSDONTMATCH					502

#define OS_IRC_USER_OPERATOR_PREFIX					"@"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

enum IRCConnectionStatus
{
	ircConnectionStatusDisconnected	= 0,
	ircConnectionStatusConnecting	= 1,
	ircConnectionStatusConnected	= 2,
};

enum IRCCommandType
{
	ircCommandTypeUnknown		   = 0,
	ircCommandTypePing			   = 1,
	ircCommandTypePong			   = 2,
	ircCommandTypeMessage		   = 3,
	ircCommandTypeServerMessage	   = 4,
	ircCommandTypeTopic			   = 5,
	ircCommandTypeRaw			   = 6,
	ircCommandTypeChannelListStart = 7,
	ircCommandTypeChannel		   = 8,
	ircCommandTypeChannelListEnd   = 9,
	ircCommandTypeJoin			   = 10,
	ircCommandTypePart			   = 11,
	ircCommandTypeNames			   = 12,
	ircCommandTypeEndOfNames	   = 13,	
	ircCommandTypeQuit			   = 14,
	ircCommandTypeNick			   = 15,
	ircCommandTypeMode			   = 16,
	ircCommandTypeKick			   = 17,	
};

enum IRCTargetType
{
	ircTargetTypeUser	 = 0,
	ircTargetTypeChannel = 1,
};

enum IRCUserType
{
	ircUserTypeNormal		= 0,
	ircUserTypeOperator		= 1,
	ircUserTypeHalfOperator	= 2
};

enum IRCModeType
{
	ircModeTypeUnknown		= 0,
	ircModeTypeOperator		= 1,
	ircModeTypeHalfOperator	= 2,
	ircModeTypeVoice		= 3,
	ircModeTypeInvisible 	= 4,
	ircModeTypeBan			= 5,
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IRC_IRC_H
