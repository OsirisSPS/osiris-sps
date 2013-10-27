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

#ifndef _OS_NET_BOOSTASIO_H
#define _OS_NET_BOOSTASIO_H

//////////////////////////////////////////////////////////////////////

#if OS_COMPILER	== OS_COMPILER_MSVC
	#pragma warning(disable : 4311)		// pointer truncation from 'type' to 'type'
	#pragma warning(disable : 4312)		// conversion from 'type' to 'type' of greater size
	#pragma warning(disable : 4267)
#endif

//////////////////////////////////////////////////////////////////////

#include "boost/asio.hpp"

//////////////////////////////////////////////////////////////////////

#if OS_COMPILER	== OS_COMPILER_MSVC
	#pragma warning(default : 4267)
	#pragma warning(default : 4311)
	#pragma warning(default : 4312)
#endif

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

NetExport bool isAsioEOF(const boost::system::error_code &e);
NetExport bool isAsioOperationAborted(const boost::system::error_code &e);

// N.B.: causa problemi di portabilità sotto Linux in caso di librerie dinamiche, è necessario che la creazione degli oggetti asio che operano su un io_service (creazione dell'io_service compresa) sia "centralizzata" in un solo modulo

NetExport shared_ptr<boost::asio::io_service> createAsioService();

template <typename T>
shared_ptr<T> createAsioObject(boost::asio::io_service &service);

//////////////////////////////////////////////////////////////////////

template <>
NetExport shared_ptr<boost::asio::deadline_timer> createAsioObject(boost::asio::io_service &service);

template <>
NetExport shared_ptr<boost::asio::ip::tcp::acceptor> createAsioObject(boost::asio::io_service &service);

template <>
NetExport shared_ptr<boost::asio::ip::tcp::resolver> createAsioObject(boost::asio::io_service &service);

template <>
NetExport shared_ptr<boost::asio::ip::udp::resolver> createAsioObject(boost::asio::io_service &service);

template <>
NetExport shared_ptr<boost::asio::ip::tcp::socket> createAsioObject(boost::asio::io_service &service);

template <>
NetExport shared_ptr<boost::asio::ip::udp::socket> createAsioObject(boost::asio::io_service &service);

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_BOOSTASIO_H
