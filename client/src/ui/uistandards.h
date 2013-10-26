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

#ifndef _OS_UI_UISTANDARDS_H
#define _OS_UI_UISTANDARDS_H

//////////////////////////////////////////////////////////////////////

#define OS_CMDLINE_BACKGROUND							_S("background")
#define OS_CMDLINE_INSTALL								_S("install")
#define OS_CMDLINE_UNINSTALL							_S("uninstall")
#define OS_CMDLINE_RECOVERY								_S("recovery")
#define OS_CMDLINE_VERBOSE								_S("verbose")

#define OS_SHELL_EXECUTE_COMMAND						0

#define OS_UI_ROOT_PATH									_S("ui")
#define OS_UI_RESOURCES_PATH							_S("res")
#define OS_UI_TEMPLATES_PATH							_S("templates")

#define OS_UI_MAX_LOGS_MESSAGES							2000
#define OS_UI_MAX_IRC_MESSAGES							1000

#define OS_UI_CONNECTIONS_PENDING_REFRESH_TIMER			250		// Definisce il timer di aggiornamento delle connessioni
#define OS_UI_CONNECTIONS_REMOVED_COLOUR				wxColour(180, 180, 180)
#define OS_UI_CONNECTIONS_TTL_MIN						1000	// Definisce il tempo minimo di visualizzazione di una connessione

#define OS_UI_TEXTBOX_BACKGROUND_STANDARD				wxColour(255, 255, 255)
#define OS_UI_TEXTBOX_BACKGROUND_INVALID				wxColour(255, 0, 0)

#define OS_IRC_OPTION_SERVER_AUTOMATIC_CONNECTION		_S("irc.server.automatic_connection")
#define OS_IRC_OPTION_SERVER_ADDRESS					_S("irc.server.address")
#define OS_IRC_OPTION_SERVER_PORT						_S("irc.server.port")
#define OS_IRC_OPTION_SERVER_PORT_SSL					_S("irc.server.port_ssl")
#define OS_IRC_OPTION_SERVER_ENABLE_SSL					_S("irc.server.enable_ssl")
#define OS_IRC_OPTION_SERVER_TIMEOUT					_S("irc.server.timeout")
#define OS_IRC_OPTION_NICKNAME							_S("irc.nickname")
#define OS_IRC_OPTION_ALTERNATE_NICKNAME				_S("irc.alternate_nickname")
#define OS_IRC_OPTION_USERNAME							_S("irc.username")
#define OS_IRC_OPTION_EMAIL								_S("irc.email")
#define OS_IRC_OPTION_DEFAULT_CHANNEL					_S("irc.default_channel")
#define OS_IRC_OPTION_SOUNDS_MODE						_S("irc.sounds.mode")

//#define OS_SYSTEM_OPTION_REGISTER_PROTOCOL				_S("system.register_protocol")
//#define OS_SYSTEM_OPTION_REGISTER_FILES_ASSOCIATIONS	_S("system.register_files_associations")

#define OS_IRC_OPTION_SOUNDS_MODE_NONE					0
#define OS_IRC_OPTION_SOUNDS_MODE_ALWAYS				1
#define OS_IRC_OPTION_SOUNDS_MODE_NOTIFY				2

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_UISTANDARDS_H
