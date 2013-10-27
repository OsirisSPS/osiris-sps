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
#include "boostasio.h"

#include "boost/bind.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

// N.B.: l'operatore di confronto di boost::system::error_code effettua un confronto sulle "categorie di errore" tramite puntatore
// il che causa problemi quando l'errore viene generato in un modulo e testato in un altro (esistendo due istanze statiche della stessa categoria)

bool isAsioEOF(const boost::system::error_code &e)
{
	boost::system::error_code eof = boost::asio::error::eof;
	return e.value() == eof.value();
}

bool isAsioOperationAborted(const boost::system::error_code &e)
{
	boost::system::error_code operationAborted = boost::asio::error::operation_aborted;
	return e.value() == operationAborted.value();
}

//////////////////////////////////////////////////////////////////////

// URGENT: da verificare la necessità (per coerenza, visto il problema di portabilità, tutti gli oggetti dell'asio allocati qui, vengono deallocati nello stesso modulo

template <typename T>
static void deallocAsioObject(T *object)
{
	OS_DELETE_T(object);
}

//////////////////////////////////////////////////////////////////////

NetExport shared_ptr<boost::asio::io_service> createAsioService()
{
	return shared_ptr<boost::asio::io_service>(OS_NEW_T(boost::asio::io_service), boost::bind(&deallocAsioObject<boost::asio::io_service>, _1));
}

template <>
NetExport shared_ptr<boost::asio::deadline_timer> createAsioObject(boost::asio::io_service &service)
{
	return shared_ptr<boost::asio::deadline_timer>(OS_NEW_T(boost::asio::deadline_timer)(service), boost::bind(&deallocAsioObject<boost::asio::deadline_timer>, _1));
}

template <>
NetExport shared_ptr<boost::asio::ip::tcp::acceptor> createAsioObject(boost::asio::io_service &service)
{
	return shared_ptr<boost::asio::ip::tcp::acceptor>(OS_NEW_T(boost::asio::ip::tcp::acceptor)(service), boost::bind(&deallocAsioObject<boost::asio::ip::tcp::acceptor>, _1));
}

template <>
NetExport shared_ptr<boost::asio::ip::tcp::resolver> createAsioObject(boost::asio::io_service &service)
{
	return shared_ptr<boost::asio::ip::tcp::resolver>(OS_NEW_T(boost::asio::ip::tcp::resolver)(service), boost::bind(&deallocAsioObject<boost::asio::ip::tcp::resolver>, _1));
}

template <>
NetExport shared_ptr<boost::asio::ip::udp::resolver> createAsioObject(boost::asio::io_service &service)
{
	return shared_ptr<boost::asio::ip::udp::resolver>(OS_NEW_T(boost::asio::ip::udp::resolver)(service), boost::bind(&deallocAsioObject<boost::asio::ip::udp::resolver>, _1));
}

template <>
NetExport shared_ptr<boost::asio::ip::tcp::socket> createAsioObject(boost::asio::io_service &service)
{
    return shared_ptr<boost::asio::ip::tcp::socket>(OS_NEW_T(boost::asio::ip::tcp::socket)(service), boost::bind(&deallocAsioObject<boost::asio::ip::tcp::socket>, _1));
}

template <>
NetExport shared_ptr<boost::asio::ip::udp::socket> createAsioObject(boost::asio::io_service &service)
{
    return shared_ptr<boost::asio::ip::udp::socket>(OS_NEW_T(boost::asio::ip::udp::socket)(service), boost::bind(&deallocAsioObject<boost::asio::ip::udp::socket>, _1));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
