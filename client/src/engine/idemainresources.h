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

#ifndef _IDE_MAIN_RESOURCES_H
#define _IDE_MAIN_RESOURCES_H

#include "iidemainpage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace main {

//////////////////////////////////////////////////////////////////////

class EngineExport Resources : public IMainPage
{
	typedef IMainPage PageBase;

// Construction
public:
	Resources(shared_ptr<HttpSession> session);
	virtual ~Resources();

// Operations
protected:
	bool transmitAvatar(const shared_ptr<IPortalDatabase> &database, const String &hash);
	bool transmitFile(const shared_ptr<IPortalDatabase> &database, const String &hash);

// IPage interface
public:
	virtual String getPageName() const;

	// Accesso MCP richiesto
	virtual bool isMcpModeRequired() const;

	// Accesso Isis consentito
	virtual bool isIsisAllowed() const;

// IPage overrides
public:
	virtual bool transmit();

public:
	static const std::wstring PARAM_AVATAR;
	static const std::wstring PARAM_FILE;
};

//////////////////////////////////////////////////////////////////////

} // main
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_MAIN_RESOURCES_H
