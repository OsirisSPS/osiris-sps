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

#ifndef _ENTITIES_SNAPSHOTOPTIONS_H
#define _ENTITIES_SNAPSHOTOPTIONS_H

#include "base/object.h"
#include "boost/thread/recursive_mutex.hpp"
#include "collections.h"
#include "dbdb.h"
#include "ids.h"
#include "locked.h"
#include "version.h"
#include "xmloptions.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ConnectionsManager;
class DateTime;
class IDbOptions;
class IsisEndpoint;
class XMLDocument;
class XMLNode;

//////////////////////////////////////////////////////////////////////

namespace entities {

//////////////////////////////////////////////////////////////////////

class EngineExport SnapshotOptions : public Object
{
public:
	

// Construction
public:
	SnapshotOptions(const PortalID &portalID = PortalID::EMPTY);
	virtual ~SnapshotOptions();

// Attributes
public:

	inline const PortalID & getPortalID() const;

	String getName() const;
	void setName(const String &name);

	String getDescription() const;
	void setDescription(const String &description);

// Operations
public:
	void clear();

	bool readFile(const String &filename);	
	bool writeFile(const String &filename) const;
	
	bool validate() const;

	void unload();

private:
	bool _readXML(shared_ptr<XMLDocument> document);
	bool _writeXML(shared_ptr<XMLDocument> document) const;
	
public:
	static const String NAME;
	static const String DESCRIPTION;	

	struct EngineExport options
	{
		static const String name;
		static const String description;		
	};

private:
	PortalID m_portalID;
	mutable XMLOptions m_snapshotOptions;	
};

//////////////////////////////////////////////////////////////////////

inline const PortalID & SnapshotOptions::getPortalID() const { return m_portalID; }

//////////////////////////////////////////////////////////////////////

} // entities
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENTITIES_SNAPSHOTOPTIONS_H
