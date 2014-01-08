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
#include "thirdpartylibrariesreporter.h"

#include "boost/format.hpp"
#include "boost/version.hpp"
#include "crypto++.h"
#include "freeimage.h"
#include "openssl/opensslv.h"
#include "unicode/uversion.h" 
#include "thirdpartylibrary.h"
#include "zlib/zlib.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ThirdPartyLibrariesReporter::ThirdPartyLibrariesReporter()
{
	registerCommonLibraries();
}

ThirdPartyLibrariesReporter::~ThirdPartyLibrariesReporter()
{

}

Locked<const ThirdPartyLibrariesReporter::Libraries>::unique ThirdPartyLibrariesReporter::getLibraries() const 
{ 
	return Locked<const Libraries>::unique(m_libraries, m_cs); 
}

shared_ptr<ThirdPartyLibrary> ThirdPartyLibrariesReporter::addLibrary()
{
	OS_LOCK(m_cs);

	shared_ptr<ThirdPartyLibrary> library(OS_NEW ThirdPartyLibrary());
	m_libraries.push_back(library);
	return library;
}

void ThirdPartyLibrariesReporter::registerCommonLibraries()
{
	{
		shared_ptr<ThirdPartyLibrary> boostLibrary = addLibrary();
		boostLibrary->setName("Boost");
		boostLibrary->setVersion(BOOST_LIB_VERSION);
		boostLibrary->setDescription("Boost provides free peer-reviewed portable C++ source libraries");
		boostLibrary->setUrl("http://www.boost.org/");
		boostLibrary->setLicense("Boost Software License");
		boostLibrary->setLicenseUrl("http://www.boost.org/LICENSE_1_0.txt");
	}

	{
		shared_ptr<ThirdPartyLibrary> cryptoppLibrary = addLibrary();
		cryptoppLibrary->setName("Crypto++");
		cryptoppLibrary->setVersion((boost::format("%d.%d.%d") % (CRYPTOPP_VERSION / 100) % ((CRYPTOPP_VERSION % 100) / 10) % (CRYPTOPP_VERSION % 10)).str());
		cryptoppLibrary->setDescription("Crypto++ Library is a free C++ class library of cryptographic schemes");
		cryptoppLibrary->setUrl("http://www.cryptopp.com");
		cryptoppLibrary->setLicense("Public domain");
		cryptoppLibrary->setLicenseUrl("http://en.wikipedia.org/wiki/Public_domain");	
	}	

	{
		shared_ptr<ThirdPartyLibrary> freeimageLibrary = addLibrary();
		freeimageLibrary->setName("FreeImage");
		freeimageLibrary->setVersion((boost::format("%d.%d.%d") % FREEIMAGE_MAJOR_VERSION % FREEIMAGE_MINOR_VERSION % FREEIMAGE_RELEASE_SERIAL).str());
		freeimageLibrary->setDescription("FreeImage is an Open Source library project for developers who would like to support popular graphics image formats like PNG, BMP, JPEG, TIFF and others as needed by today's multimedia applications.");
		freeimageLibrary->setUrl("http://freeimage.sourceforge.net/");
		freeimageLibrary->setLicense("FreeImage Public License");
		freeimageLibrary->setLicenseUrl("http://freeimage.sourceforge.net/freeimage-license.txt");
	}		

	{
		shared_ptr<ThirdPartyLibrary> icuLibrary = addLibrary();
		icuLibrary->setName("ICU");
		icuLibrary->setVersion(U_ICU_VERSION);
		icuLibrary->setDescription("International Components for Unicode (ICU) is an open source project of mature C/C++ and Java libraries for Unicode support, software internationalization and software globalization");
		icuLibrary->setUrl("http://www.icu-project.org/");
		icuLibrary->setLicense("ICU License");
		icuLibrary->setLicenseUrl("http://source.icu-project.org/repos/icu/icu/trunk/license.html");
	}

	{
		shared_ptr<ThirdPartyLibrary> opensslLibrary = addLibrary();
		opensslLibrary->setName("OpenSSL");
		opensslLibrary->setVersion(SHLIB_VERSION_NUMBER);
		opensslLibrary->setDescription("OpenSSL is an open source implementation of the SSL and TLS protocols");
		opensslLibrary->setUrl("http://www.openssl.org/");
		opensslLibrary->setLicense("OpenSSL License");
		opensslLibrary->setLicenseUrl("http://www.openssl.org/source/license.html");
	}	

	{
		shared_ptr<ThirdPartyLibrary> zlibLibrary = addLibrary();
		zlibLibrary->setName("zlib");
		zlibLibrary->setVersion(ZLIB_VERSION);
		zlibLibrary->setDescription("zlib is a software library used for data compression");
		zlibLibrary->setUrl("http://www.zlib.net/");
		zlibLibrary->setLicense("zlib license");
		zlibLibrary->setLicenseUrl("http://www.zlib.net/zlib_license.html");
	}	 
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
