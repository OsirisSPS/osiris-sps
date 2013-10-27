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

#ifndef _OBJECTS_TEXT_H
#define _OBJECTS_TEXT_H

#include "objectsdescriptor.h"
#include "objectsicommentable.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsText : public ObjectsICommentable
{
	typedef ObjectsICommentable CommentableBase;

// Construction
public:
	ObjectsText();
	virtual ~ObjectsText();

public:
	static uint32 VERSION;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ObjectsText> objects_text_cast(shared_ptr<ObjectsIObject> object) { return boost::dynamic_pointer_cast<ObjectsText>(object); }

//////////////////////////////////////////////////////////////////////

class ObjectsTextDescriptor : public ObjectsDescriptor<ObjectsTextDescriptor, ObjectsICommentableDescriptor>
{
	OS_DECLARE_DESCRIPTOR(ObjectsTextDescriptor, portalObjectTypeText);

	typedef ObjectsDescriptor<ObjectsTextDescriptor, ObjectsICommentableDescriptor> DescriptorBase;

// Construction
public:
	ObjectsTextDescriptor();
	virtual ~ObjectsTextDescriptor();

// IDescriptor interface
public:
	virtual shared_ptr<ObjectsIObject> createObject();
	virtual String getTableName();

	virtual String getTypeName();
	virtual String getGroupName();

// IDescriptor overrides
public:
	virtual shared_ptr<ISearchOptions> getSearchOptions();

	virtual ide::edit::editor_ptr createEditControl(shared_ptr<EntitiesEntity> entity = null, shared_ptr<EntitiesEntity> parent = null);
	virtual ide::view::viewer_ptr createViewControl(shared_ptr<EntitiesEntity> entity = null);
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OBJECTS_TEXT_H
