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

#ifndef _OS_CORE_CONFIGPLATFORMLINUX_H
#define _OS_CORE_CONFIGPLATFORMLINUX_H

//////////////////////////////////////////////////////////////////////

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdint.h>

#define INVALID_SOCKET					(socket_t)(~0)
#define SOCKET_ERROR					(-1)
#define SD_RECEIVE						SHUT_RD
#define SD_SEND							SHUT_WR
#define SD_BOTH							SHUT_RDWR

#define OS_MODULE_ENTRY_POINT()
										// Di default sotto linux la visibilità è "globale" ed era stato modificato a "nascosta" (tramite -fvisibility=hidden), ma causa problemi con alcune librerie (asio in particolare)
#define OS_LIBRARY_EXPORT				// __attribute__ ((visibility("default")))
#define OS_LIBRARY_IMPORT				// __attribute__ ((visibility("default")))

#define OS_SHARED_LIBRARY_EXTENSION		_S("so")

#define OS_MAX_PATH						256

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_CONFIGPLATFORMLINUX_H
