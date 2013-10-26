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
#include "thirdpartylibrary.h"

#include "lock.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ThirdPartyLibrary::ThirdPartyLibrary()
{

}

ThirdPartyLibrary::~ThirdPartyLibrary()
{

}

std::string ThirdPartyLibrary::getName() const
{
	OS_LOCK(m_cs);
	return m_name;
}

void ThirdPartyLibrary::setName(const std::string &name)
{
	OS_LOCK(m_cs);
	m_name = name;
}

std::string ThirdPartyLibrary::getVersion() const
{
	OS_LOCK(m_cs);
	return m_version;
}

void ThirdPartyLibrary::setVersion(const std::string &version)
{
	OS_LOCK(m_cs);
	m_version = version;
}

std::string ThirdPartyLibrary::getDescription() const
{
	OS_LOCK(m_cs);
	return m_description;
}

void ThirdPartyLibrary::setDescription(const std::string &description)
{
	OS_LOCK(m_cs);
	m_description = description;
}

std::string ThirdPartyLibrary::getUrl() const
{
	OS_LOCK(m_cs);
	return m_url;
}

void ThirdPartyLibrary::setUrl(const std::string &url)
{
	OS_LOCK(m_cs);
	m_url = url;
}

std::string ThirdPartyLibrary::getLicense() const
{
	OS_LOCK(m_cs);
	return m_license;
}

void ThirdPartyLibrary::setLicense(const std::string &license)
{
	OS_LOCK(m_cs);
	m_license = license;
}

std::string ThirdPartyLibrary::getLicenseUrl() const
{
	OS_LOCK(m_cs);
	return m_licenseUrl;
}

void ThirdPartyLibrary::setLicenseUrl(const std::string &licenseUrl)
{
	OS_LOCK(m_cs);
	m_licenseUrl = licenseUrl;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
