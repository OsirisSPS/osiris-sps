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

#ifndef _OS_OSIRISLINK_H
#define _OS_OSIRISLINK_H

#include "base/object.h"
#include "collections.h"
#include "ordered_map.h"
#include "portalsportals.h"

#define OS_OSIRISLINK_TYPE_PORTAL		_W("portal")
#define OS_OSIRISLINK_TYPE_ISIS			_W("isis")
#define OS_OSIRISLINK_TYPE_URL			_W("url")
#define OS_OSIRISLINK_TYPE_FILE			_W("file")
#define OS_OSIRISLINK_TYPE_RES			_W("res")

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport OsirisLink : public Object
{
public:
	typedef ordered_map<std::string, std::wstring>::type OsirisLinkParams;

// Enumerated types
public:
	enum LinkType
	{
		linkInvalid		= 0,
		linkPortal		= 1,
		linkIsis		= 2,		
		linkUrl			= 3,
		linkFile		= 4,
		linkResource	= 5,
	};
	

// Construction
public:
	OsirisLink(const std::string &link = "");
	virtual ~OsirisLink();

// Attributes
public:
	bool empty() const;

	inline const OsirisLinkParams & getParams() const;
	ordered_map<std::wstring, std::wstring> getParams2() const;

	bool hasParam(const std::string &name) const;
	std::wstring getParam(const std::string &name) const;
	void setParam(const std::string &name, const std::wstring &value);	
	void setParam(const std::string &name, const std::string &value);	

	LinkType getType() const;

	// Temp
	PortalID getPortal() const;
	PovID getPov() const;
	String getName() const;
	String getDescription() const;

// Operations
public:
	void clear();
	bool isValid() const;
	bool parse(const std::string &link);	
	std::string generate() const;

	String toString() const;

private:

	OsirisLinkParams m_params;
};

//////////////////////////////////////////////////////////////////////

const OsirisLink::OsirisLinkParams & OsirisLink::getParams() const { return m_params; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_OSIRISLINK_H
