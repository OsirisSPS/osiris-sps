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
#include "ideeditmodeleditor.h"

#include "algorithms.h"
#include "boost/bind.hpp"
#include "eventssource.h"
#include "iportaldatabase.h"
#include "ideomleditor.h"
#include "idesession.h"
#include "ideskin.h"

#include "objectsmodel.h"
#include "htmlattributes.h"
#include "htmlevent.h"
#include "css.h"
#include "htmlcontrols.h"
#include "htmltextarea.h"
#include "htmltextbox.h"
#include "xmlnode.h"
#include "xmlschema.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace edit {

//////////////////////////////////////////////////////////////////////

ModelEditor::ModelEditor(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent) : EditorBase(portalObjectTypeModel, entity, parent),
																					 m_title(OS_NEW HtmlTextBox()),
																					 m_description(OS_NEW HtmlTextBox()),
																					 m_schema(OS_NEW HtmlTextArea()),
																					 m_stylesheet(OS_NEW HtmlTextArea()),
																					 m_sample(OS_NEW IdeOMLEditor())
{
	m_title->setID(_S("title"));
	m_title->setCss(_S("os_input_full"));
	m_title->getAttributes()->set(_S("data-os-required"), _S("required")); // CLODO TEST
	m_title->setMaxLength(OS_CONTROLS_TITLE_MAXLENGTH);
	m_description->setID(_S("description"));
	m_description->setCss(_S("os_input_full"));
	m_description->setMaxLength(OS_CONTROLS_DESCRIPTION_MAXLENGTH);
	m_schema->setID(_S("schema"));
	m_stylesheet->setID(_S("stylesheet"));
	m_sample->setID(_S("sample"));
	m_sample->setPreviewCallback(boost::bind(&ModelEditor::onPreviewCallback, this, _1));

	m_schema->setRows(20);
	m_stylesheet->setRows(20);

	getTemplate()->addChildParam(m_title, _S("title"));
	getTemplate()->addChildParam(m_description, _S("description"));
	getTemplate()->addChildParam(m_schema, _S("schema"));
	getTemplate()->addChildParam(m_stylesheet, _S("stylesheet"));
	getTemplate()->addChildParam(m_sample, _S("sample"));
}

ModelEditor::~ModelEditor()
{

}

String ModelEditor::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("model_editor.xsl"));
}

shared_ptr<ObjectsIRevisionable> ModelEditor::createObject()
{
	shared_ptr<ObjectsModel> model(OS_NEW ObjectsModel());
	model->title = algorithms::trim_copy(m_title->getValue());
	model->description = algorithms::trim_copy(m_description->getValue());
	model->schema = m_schema->getValue();
	model->stylesheet = m_stylesheet->getValue();
	model->sample = m_sample->getValue();

	return model;
}

void ModelEditor::onPreRender()
{
	EditorBase::onPreRender();

	if(isRevision() && getPostBack() == false)
	{
		shared_ptr<ObjectsModel> current = objects_model_cast(getCurrent());
		if(current != null)
		{
			m_title->setValue(current->title);
     		m_description->setValue(current->description);
			m_schema->setValue(current->schema);
			m_stylesheet->setValue(current->stylesheet);
			m_sample->setValue(current->sample);
		}
	}
}

void ModelEditor::onPreviewCallback(String &preview)
{
	shared_ptr<XMLSchema> xsd(OS_NEW XMLSchema());
	xsd->parseString(m_schema->getValue());

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument(xsd));
	if(document->parseString(m_sample->getValue()) == false)
	{
		getPage()->showError(_S("model.editor.errors.invalid_sample"));
		return;
	}

	shared_ptr<XMLStylesheet> transformer(OS_NEW XMLStylesheet());
	if(transformer->parseString(m_stylesheet->getValue()) == false)
	{
		getPage()->showError(_S("model.editor.errors.invalid_stylesheet"));
		return;
	}

	if(transformer->applyToString(document, preview) == false)
	{
		getPage()->showError(_S("model.editor.errors.trasformation_error"));
		return;
	}
}

//////////////////////////////////////////////////////////////////////

} // edit
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
