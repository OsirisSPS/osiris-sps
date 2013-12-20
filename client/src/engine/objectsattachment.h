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

#ifndef _ENGINE_OBJECTSATTACHMENT_H
#define _ENGINE_OBJECTSATTACHMENT_H

#include "objectsdescriptor.h"
#include "objectsirevisionable.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsAttachment : public ObjectsIRevisionable
{
	typedef ObjectsIRevisionable RevisionableBase;

// Construction
public:
	ObjectsAttachment();
	virtual ~ObjectsAttachment();

// Attributes
public:
	String getMimeType() const;

// IRevisionable overrides
public:
	virtual bool requireParent() const;

	virtual bool validate(shared_ptr<IPortalDatabase> database) const;

	virtual void exportXML(shared_ptr<XMLPortalExporter> exporter);

protected:
	virtual bool onSign(const shared_ptr<ObjectsSigner> &signer) const;

	virtual bool onRead(const ObjectID &id, const DataTree &data);
	virtual bool onWrite(DataTree &data) const;

	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:
	DbValue<String> file_name;
	DbValue<Buffer> file_data;
	DbValue<String> file_type;
	DbValue<uint32> file_size;

public:
	static uint32 VERSION;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ObjectsAttachment> objects_attachment_cast(shared_ptr<ObjectsIObject> object) { return boost::dynamic_pointer_cast<ObjectsAttachment>(object); }

//////////////////////////////////////////////////////////////////////

class ObjectsAttachmentDescriptor : public ObjectsDescriptor<ObjectsAttachmentDescriptor>
{
	OS_DECLARE_DESCRIPTOR(ObjectsAttachmentDescriptor, portalObjectTypeAttachment);

	typedef ObjectsDescriptor<ObjectsAttachmentDescriptor> DescriptorBase;

// Construction
public:
	ObjectsAttachmentDescriptor();
	virtual ~ObjectsAttachmentDescriptor();

// IDescriptor interface
public:
	virtual shared_ptr<ObjectsIObject> createObject();
	virtual String getTableName();

	virtual String getTypeName();
	virtual String getGroupName();

// IDescriptor overrides
public:
	virtual bool isGroupable();
	virtual shared_ptr<ISearchOptions> getSearchOptions();

	virtual ide::edit::editor_ptr createEditControl(shared_ptr<EntitiesEntity> entity = nullptr, shared_ptr<EntitiesEntity> parent = nullptr);
	virtual ide::view::viewer_ptr createViewControl(shared_ptr<EntitiesEntity> entity = nullptr);
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_OBJECTSATTACHMENT_H
