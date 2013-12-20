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

#ifndef _OBJECTS_USER_H
#define _OBJECTS_USER_H

#include "objectsdescriptor.h"
#include "objectsiobject.h"
#include "xmloptions.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsUser : public ObjectsIObject
{
	typedef ObjectsIObject ObjectBase;
	
// Construction
public:
	ObjectsUser();
	virtual ~ObjectsUser();

// Attributes
public:
	// Restituisce l'avatar associato all'utente (non tiene conto della visibilit)
	shared_ptr<ObjectsAvatar> getAvatar(const shared_ptr<IPortalDatabase> &db) const;
	ObjectID getAvatarID() const;

	// Indica se l'utente ha un avatar (visibile)
	bool hasAvatar(const shared_ptr<IPortalDatabase> &db) const;
	// Indica se l'utente ha una firma (visibile)
	bool hasMark() const;

	//ReputationThreshold getAuthorsThreshold() const;
	//ReputationThreshold getEditorsThreshold() const;

// Operations
public:
	bool verify(const PortalID &portal) const;

// IObject Interface
public:
	virtual const ObjectID & getAuthor() const;

// IObject overrides
public:
	virtual void getDependencies(const shared_ptr<IPortalDatabase> &db, list<ObjectID>::type &dependencies);
	virtual bool validate(shared_ptr<IPortalDatabase> database) const;
	virtual LanguageResult acceptable(shared_ptr<IPortalDatabase> database) const;

	virtual void exportXML(shared_ptr<XMLPortalExporter> exporter);

protected:
	virtual bool onSign(const shared_ptr<ObjectsSigner> &signer) const;

	virtual bool onRead(const ObjectID &id, const DataTree &data);
	virtual bool onWrite(DataTree &data) const;

	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

private:
	bool _validatePublicKey() const;

public:
	DbValue<Buffer> public_key;
	DbValue<String> name;
	DbValue<String> description;
	DbValue<DateTime> join_date;
	DbValue<DateTime> birth_date;
	DbValue<uint32> gender;
	DbValue<String> misc;
	DbValue<String> mark;
	//DbValue<bool> show_mark; // TOCLEAN
	//DbValue<bool> show_avatar;
	DbValue<String> options;

public:
	static uint32 VERSION;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ObjectsUser> objects_user_cast(shared_ptr<ObjectsIObject> object) { return boost::dynamic_pointer_cast<ObjectsUser>(object); }

//////////////////////////////////////////////////////////////////////

class ObjectsUserDescriptor : public ObjectsDescriptor<ObjectsUserDescriptor>
{
	OS_DECLARE_DESCRIPTOR(ObjectsUserDescriptor, portalObjectTypeUser);

	typedef ObjectsDescriptor<ObjectsUserDescriptor> DescriptorBase;

// Construction
public:
	ObjectsUserDescriptor();
	virtual ~ObjectsUserDescriptor();

// IDescriptor interface
public:
	virtual shared_ptr<ObjectsIObject> createObject();
	virtual String getTableName();

	virtual String getTypeName();
	virtual String getGroupName();

// IDescriptor overrides
public:
	virtual bool hasStatistics();

	virtual void createStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object);
	virtual void computeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object);
	virtual void removeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object = nullptr);
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OBJECTS_USER_H
