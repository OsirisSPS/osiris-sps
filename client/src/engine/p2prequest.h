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

#ifndef _OS_P2P_REQUEST_H
#define _OS_P2P_REQUEST_H

#include "base/object.h"
#include "datadata.h"
#include "p2pp2p.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {

//////////////////////////////////////////////////////////////////////

class EngineExport Request : public Object
{
// Construction
public:
	Request(RequestType type);
	virtual ~Request();

// Attributes
public:
	inline RequestType getType() const;
	inline const StringSet & getObjects() const;

	bool objectExists(const String &id) const;
	bool hasObjects() const;

// Operations
public:
	void addObject(const String &id);
	void removeObject(const String &id);

	void getObjects(StringList &objects) const;

protected:
	const RequestType m_type;
	StringSet m_objects;
};

//////////////////////////////////////////////////////////////////////

inline RequestType Request::getType() const { return m_type; }

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_P2P_REQUEST_H
