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

#ifndef _OS_UPNP_UPNPMANAGER_H
#define _OS_UPNP_UPNPMANAGER_H

#include "constants.h"
#include "pimpl_ptr.h"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

#define OS_UPNP_DEFAULT_DISCOVER_TIMEOUT			2000

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class UPnPExport UPnPManager : public StaticSingleton<UPnPManager>
{	
// Construction
protected:
	UPnPManager();

public:
	virtual ~UPnPManager();

// Attributes
public:
	bool getServiceAvailable() const;

	uint32 getDiscoverTimeout() const;
	void setDiscoverTimeout(uint32 discoverTimeout);

// Operations
public:
	bool addPortMapping(uint32 port, const std::string &protocol, const std::string &description = constants::empty_string);
	bool removePortMapping(uint32 port, const std::string &protocol);

private:
	pimpl_of<UPnPManager>::type m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UPNP_UPNPMANAGER_H€àè
