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

#ifndef _OS_CORE_THIRDPARTYLIBRARY_H
#define _OS_CORE_THIRDPARTYLIBRARY_H

#include "base/object.h"
#include "boost/thread/recursive_mutex.hpp"
#include "sealed.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLNode;

//////////////////////////////////////////////////////////////////////

class CoreExport ThirdPartyLibrary : /*Sealed<ThirdPartyLibrary>,*/
									 public Object
{
// Construction
public:
	ThirdPartyLibrary();
	virtual ~ThirdPartyLibrary();

// Attributes
public:
    std::string getName() const;
    void setName(const std::string &name);

    std::string getVersion() const;
    void setVersion(const std::string &version);

    std::string getDescription() const;
    void setDescription(const std::string &description);

    std::string getUrl() const;
    void setUrl(const std::string &url);

    std::string getLicense() const;
    void setLicense(const std::string &license);

	std::string getLicenseUrl() const;
	void setLicenseUrl(const std::string &licenseUrl);

private:
	mutable boost::recursive_mutex m_cs;
    std::string m_name;          // Nome della libreria
    std::string m_version;       // Versione della libreria
    std::string m_description;   // Descrizione della libreria
	std::string m_url;			 // Homepage della libreria
    std::string m_license;       // Licenza di utilizzo della libreria
	std::string m_licenseUrl;    // Link alle informazioni sulla licenza
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_THIRDPARTYLIBRARY_H
