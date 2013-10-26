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
#include "boostasio_ssl.h"

#include "boost/bind.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

// N.B.: vedere il commento di deallocAsioObject in asio.cpp

template <typename T>
static void deallocAsioSSLObject(T *object)
{
	OS_DELETE_T(object);
}

//////////////////////////////////////////////////////////////////////

template <>
NetExport shared_ptr<boost::asio::ssl::context> createAsioSSLObject<boost::asio::ssl::context, boost::asio::ssl::context_base::method>(boost::asio::io_service &service, boost::asio::ssl::context_base::method param)
{
	return shared_ptr<boost::asio::ssl::context>(OS_NEW_T(boost::asio::ssl::context)(service, param), boost::bind(&deallocAsioSSLObject<boost::asio::ssl::context>, _1));
}

template <>
NetExport shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> > createAsioSSLObject<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>, boost::asio::ssl::context &>(boost::asio::io_service &service, boost::asio::ssl::context &param)
{
	return shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> >(OS_NEW_T(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>)(service, param), boost::bind(&deallocAsioSSLObject<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> >, _1));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
