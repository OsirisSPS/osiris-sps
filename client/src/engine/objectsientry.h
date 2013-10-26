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

#ifndef _ENGINE_OBJECTSIENTRY_H
#define _ENGINE_OBJECTSIENTRY_H

#include "objectsiobject.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsIEntry : public ObjectsIObject
{	
	typedef ObjectsIObject ObjectBase;

// Construction
public:
	ObjectsIEntry(PortalObjectType object_type, uint32 object_version);
	virtual ~ObjectsIEntry();

// IObject Interface
public:
	virtual const ObjectID & getAuthor() const;

// IObject overrides
public:
	virtual bool validate(shared_ptr<IPortalDatabase> database) const;	
	virtual LanguageResult acceptable(shared_ptr<IPortalDatabase> database) const;

	virtual void exportXML(shared_ptr<XMLPortalExporter> exporter);
	virtual void exportAuthor(shared_ptr<XMLPortalExporter> exporter);

protected:
	virtual bool onCreate(const shared_ptr<IPortalDatabase> &db, const Buffer &privateKey);
	virtual bool onSign(const shared_ptr<ObjectsSigner> &signer) const;

	virtual bool onRead(const ObjectID &id, const DataTree &data);
	virtual bool onWrite(DataTree &data) const;

	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:
	DbValue<ObjectID> author;	
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ObjectsIEntry> objects_entry_cast(shared_ptr<ObjectsIObject> object) { return boost::dynamic_pointer_cast<ObjectsIEntry>(object); } // CLODOVERYURGENT

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
#endif // _ENGINE_OBJECTSIENTRY_H
