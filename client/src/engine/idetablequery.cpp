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
#include "idetablequery.h"

#include "objectsdescriptor.h"
#include "objectssystem.h"
#include "iportaldatabase.h"
#include "idesession.h"
#include "ideskin.h"
#include "datatable.h"

#include "objectsiobject.h"
#include "objectsuser.h"
#include "platformmanager.h"
#include "portalsprofile.h"
#include "htmlcontrols.h"
#include "htmllink.h"
#include "htmlliteral.h"
#include "htmlspan.h"
#include "htmltable.h"
#include "htmltablecell.h"
#include "htmltablerow.h"
#include "htmlxslcontrol.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdeTableQuery::IdeTableQuery()
{

}

IdeTableQuery::~IdeTableQuery()
{

}

String IdeTableQuery::getColumnTitle(const uint32 &index) const
{
	ColumnsTitles::const_iterator i = m_columnsTitles.find(index);
	return i == m_columnsTitles.end() ? String::EMPTY : i->second;
}

void IdeTableQuery::setColumnTitle(const uint32 &index,const String &title)
{
	m_columnsTitles[index] = title;
}

IdeTableQuery::ColumnType IdeTableQuery::getColumnType(const uint32 &index) const
{
	ColumnsTypes::const_iterator i = m_columnsTypes.find(index);
	return i == m_columnsTypes.end() ? IdeTableQuery::ctString : i->second;
}

void IdeTableQuery::setColumnType(const uint32 &index, const ColumnType &type)
{
	m_columnsTypes[index] = type;
}

String IdeTableQuery::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("table_query.xsl"));
}

void IdeTableQuery::onPreRender()
{
	ControlBase::onPreRender();
	
	shared_ptr<XMLDocument> document = getDocument();
	shared_ptr<XMLNode> node_root = document->create(_S("table_query"));

	DataTable result;
	if(getPage()->getDatabase()->execute(m_sql,result))
	{
		shared_ptr<XMLNode> node_header = node_root->addChild(_S("header"));

		for(uint32 c = 0; c < result.columns(); c++)
		{
			String columnTitle = getColumnTitle(c);
			if(columnTitle.empty())
				columnTitle = result.getColumnName(c);

			shared_ptr<XMLNode> node_column = node_header->addChild(_S("column"));

			node_column->setAttributeString(_S("name"), columnTitle);

			switch(getColumnType(c))
			{
			case IdeTableQuery::ctString:
				{
					node_column->setAttributeString(_S("type"), _S("string"));
				} break;
			case IdeTableQuery::ctScore:
				{
					node_column->setAttributeString(_S("type"), _S("score"));
				} break;
			case IdeTableQuery::ctEntityID:
				{
					node_column->setAttributeString(_S("type"), _S("entity-id"));
				} break;
			case IdeTableQuery::ctObjectID:
				{
					node_column->setAttributeString(_S("type"), _S("object-id"));
				} break;
			case IdeTableQuery::ctUserID:
				{
					node_column->setAttributeString(_S("type"), _S("user-id"));
				} break;
			case IdeTableQuery::ctShortDateTime:
				{
					node_column->setAttributeString(_S("type"), _S("short-datetime"));
				} break;
			case IdeTableQuery::ctLongDateTime:
				{
					node_column->setAttributeString(_S("type"), _S("long-datetime"));
				} break;
			}
		}

		shared_ptr<XMLNode> node_data = node_root->addChild(_S("data"));

		for(uint32 r=0;r<result.rows();r++)
		{
			shared_ptr<XMLNode> node_data_row = node_data->addChild(_S("row"));

			for(uint32 c=0;c<result.columns();c++)
			{
				shared_ptr<XMLNode> node_data_row_item = node_data_row->addChild(_S("item"));

				String value = result.get(r,c);

				switch(getColumnType(c))
				{
				case IdeTableQuery::ctScore:
					{
						node_data_row_item->setAttributeString(_S("value"), String::format(_S("%1.1f").c_str(), double(result.get(r,c))));
					} break;
				case IdeTableQuery::ctEntityID:
					{
						// Sbagliata
						shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(node_data_row_item, getPage(), XMLPortalExporter::emLite));
						shared_ptr<ObjectsIObject> object = getPage()->getObject(value.to_ascii());
						if(object != null)
						{
							object->exportXML(exporter);
						}
					} break;
				case IdeTableQuery::ctObjectID:
					{
						shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(node_data_row_item, getPage(), XMLPortalExporter::emLite));
						shared_ptr<ObjectsIObject> object = getPage()->getObject(value.to_ascii());
						if(object != null)
						{
							object->exportXML(exporter);
						}
					} break;
				case IdeTableQuery::ctUserID:
					{
						shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(node_data_row_item, getPage(), XMLPortalExporter::emLite));
						shared_ptr<ObjectsIObject> object = getPage()->getObject(value.to_ascii());
						if(object != null)
						{
							object->exportXML(exporter);
						}
					} break;
				case IdeTableQuery::ctShortDateTime:
				case IdeTableQuery::ctLongDateTime:
					{
						DateTime dt;
						dt.fromString(value);
						node_data_row_item->setAttributeDateTime(_S("value"), dt);
					} break;
				default:
					{
						node_data_row_item->setAttributeString(_S("value"), value);
					} break;
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
