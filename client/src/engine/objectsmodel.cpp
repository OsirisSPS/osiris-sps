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
#include "objectsmodel.h"

#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "entitiessnapshotmanager.h"
#include "ideeditmodeleditor.h"
#include "ideviewmodelviewer.h"
#include "datatree.h"
#include "objectssigner.h"
#include "searchmodeloptions.h"
#include "xmlportalexporter.h"
#include "xmlmanager.h"
#include "xmlnode.h"
#include "xmlschema.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsModelDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsModel::VERSION = OS_PORTAL_OBJECT_MODEL_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsModel::ObjectsModel() : CommentableBase(portalObjectTypeModel, VERSION)
{

}

ObjectsModel::~ObjectsModel()
{

}

bool ObjectsModel::apply(const String &content, String &str)
{
	str.clear();

	shared_ptr<XMLSchema> xsd(OS_NEW XMLSchema());
	xsd->parseString(schema);

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument(xsd));
	if(document->parseString(content))
	{
		shared_ptr<XMLStylesheet> transformer(OS_NEW XMLStylesheet());
		if(transformer->parseString(stylesheet))
		{
			return transformer->applyToString(document, str);
		}
	}

	return false;
}

bool ObjectsModel::validate(shared_ptr<IPortalDatabase> database) const
{
	if(CommentableBase::validate(database) == false)
		return false;

	if(XMLManager::instance()->validateString(schema) == false)
		return false;

	if(XMLManager::instance()->validateString(stylesheet) == false)
		return false;

	if(sample->empty() == false)
	{
		shared_ptr<XMLSchema> sampleSchema(OS_NEW XMLSchema());
		sampleSchema->parseString(schema);

		shared_ptr<XMLDocument> sampleDocument(OS_NEW XMLDocument(sampleSchema));
		if(sampleDocument->parseString(sample) == false)
			return false;
	}

	return true;
}

void ObjectsModel::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	CommentableBase::exportXML(exporter);

	shared_ptr<XMLNode> node = exporter->getRoot();

	if(exporter->getMode() == XMLPortalExporter::emData)
	{
		node->setAttributeString(DBTABLES::MODELS::SCHEMA, schema);
		node->setAttributeString(DBTABLES::MODELS::STYLESHEET, stylesheet);
		node->setAttributeString(DBTABLES::MODELS::SAMPLE, sample);
	}

	if(exporter->getMode() == XMLPortalExporter::emFull)
	{
		if(sample->empty() == false)
		{
			String str;
			if(apply(sample, str))
				node->setAttributeString(DBTABLES::MODELS::SAMPLE, str);
		}
	}
}

bool ObjectsModel::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(CommentableBase::onSign(signer) == false)
		return false;

	signer->add(schema);
	signer->add(stylesheet);
	signer->add(sample);

	return true;
}

bool ObjectsModel::onRead(const ObjectID &id, const DataTree &data)
{
	if(CommentableBase::onRead(id, data) == false)
		return false;

	schema.read(data, DBTABLES::MODELS::SCHEMA);
	stylesheet.read(data, DBTABLES::MODELS::STYLESHEET);
	sample.read(data, DBTABLES::MODELS::SAMPLE);

	return true;
}

bool ObjectsModel::onWrite(DataTree &data) const
{
	if(CommentableBase::onWrite(data) == false)
		return false;

	schema.write(data, DBTABLES::MODELS::SCHEMA);
	stylesheet.write(data, DBTABLES::MODELS::STYLESHEET);
	sample.write(data, DBTABLES::MODELS::SAMPLE);

	return true;
}

bool ObjectsModel::onRead(const DataTableRow &row)
{
	if(CommentableBase::onRead(row) == false)
		return false;

	schema.read(row, DBTABLES::MODELS::SCHEMA);
	stylesheet.read(row, DBTABLES::MODELS::STYLESHEET);
	sample.read(row, DBTABLES::MODELS::SAMPLE);

	return true;
}

bool ObjectsModel::onWrite(DbSqlValues &values) const
{
	if(CommentableBase::onWrite(values) == false)
		return false;

	schema.write(values, DBTABLES::MODELS::SCHEMA);
	stylesheet.write(values, DBTABLES::MODELS::STYLESHEET);
	sample.write(values, DBTABLES::MODELS::SAMPLE);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsModelDescriptor::ObjectsModelDescriptor()
{

}

ObjectsModelDescriptor::~ObjectsModelDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsModelDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsModel());
}

String ObjectsModelDescriptor::getTableName()
{
	return DBTABLES::MODELS_TABLE;
}

String ObjectsModelDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_MODEL_TYPENAME;
}

String ObjectsModelDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_MODEL_GROUPNAME;
}

shared_ptr<ISearchOptions> ObjectsModelDescriptor::getSearchOptions()
{
	return shared_ptr<ISearchOptions>(OS_NEW SearchModelOptions());
}

ide::edit::editor_ptr ObjectsModelDescriptor::createEditControl(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent)
{
	return ide::edit::editor_ptr(OS_NEW ide::edit::ModelEditor(entity, parent));
}

ide::view::viewer_ptr ObjectsModelDescriptor::createViewControl(shared_ptr<EntitiesEntity> entity)
{
	return ide::view::viewer_ptr(OS_NEW ide::view::ModelViewer(entity));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
