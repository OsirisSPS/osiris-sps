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
#include "objectsdescriptor.h"

#include "convert.h"
#include "constants.h"
#include "dataitem.h"
#include "iportaldatabase.h"
#include "idbconnection.h"
#include "dbsqlselect.h"
#include "dbtables.h"
#include "entitiesentity.h"
#include "isearchoptions.h"
#include "objectsiobject.h"
#include "objectsirevisionable.h"
#include "objectssystem.h"
#include "objectsuser.h"
#include "idesession.h"
#include "portalsportal.h"
#include "portalsprofile.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ObjectsIDescriptor::ObjectsIDescriptor()
{

}

ObjectsIDescriptor::~ObjectsIDescriptor()
{

}

bool ObjectsIDescriptor::allowChild(PortalObjectType type) const
{
	ObjectsTypes allowed_childs;
	getAllowedChilds(allowed_childs);

	return utils::exists(allowed_childs, type);
}

/*
uint32 IDescriptor::getMaxObjectSize() const
{
	return OS_PORTAL_OBJECT_MAXSIZE;
}
*/

bool ObjectsIDescriptor::objectExists(shared_ptr<IPortalDatabase> db, const ObjectID &id)
{
	return db->recordExists(getTableName(), DBTABLES::ID, Convert::toSQL(id));
}

void ObjectsIDescriptor::getAllowedChilds(ObjectsTypes &allowed_childs) const
{
	// Di default l'oggetto base non ammette figli
}

ObjectsIDescriptor::OrderMode ObjectsIDescriptor::getOrderMode() const
{
	// Di default l'ordinamento  per submitdate
	return omSubmitDate;
}

shared_ptr<ObjectsIObject> ObjectsIDescriptor::loadObject(shared_ptr<IPortalDatabase> db, const ObjectID &id)
{
	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(getTableName()));
	select->where.add(DBTABLES::ID, Convert::toSQL(id));
	select->limit.setCount(1);

	DataTable result;
	if(db->execute(select, result) == false)
		return null;

	if(result.rows() != 1)
		return null;

	shared_ptr<ObjectsIObject> object = createObject();
	object->read(result[0]);
	return object;
}

bool ObjectsIDescriptor::isGroupable()
{
	return false;
}

bool ObjectsIDescriptor::hasStatistics()
{
	return false;
}

shared_ptr<ISearchOptions> ObjectsIDescriptor::getSearchOptions()
{
	return null;
}

ide::edit::editor_ptr ObjectsIDescriptor::createEditControl(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent)
{
	return null;
}

ide::view::viewer_ptr ObjectsIDescriptor::createViewControl(shared_ptr<EntitiesEntity> entity)
{
	return null;
}

void ObjectsIDescriptor::createStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{

}

void ObjectsIDescriptor::computeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{

}

void ObjectsIDescriptor::removeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{

}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
