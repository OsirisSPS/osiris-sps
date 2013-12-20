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
#include "ideedittexteditor.h"

#include "algorithms.h"
#include "boost/bind.hpp"
#include "constants.h"
#include "iportaldatabase.h"
#include "ideomleditor.h"
#include "ideskin.h"

#include "objectsmodel.h"
#include "objectstext.h"
#include "portalsportal.h"
#include "htmlevent.h"
#include "css.h"
#include "htmlcontrols.h"
#include "htmlcheckbox.h"
#include "htmltextbox.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace edit {

//////////////////////////////////////////////////////////////////////

TextEditor::TextEditor(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent) :  EditorBase(portalObjectTypeText, entity, parent),
																					m_title(OS_NEW HtmlTextBox()),
																					m_description(OS_NEW HtmlTextBox()),
																					m_content(OS_NEW IdeOMLEditor()),
																					m_model(OS_NEW HtmlTextBox()),
																					m_pinned(OS_NEW HtmlCheckBox())
{
	m_title->setID(_S("title"));
	m_title->setCss(_S("os_input_full"));
	m_title->setMaxLength(OS_CONTROLS_TITLE_MAXLENGTH);
	m_description->setID(_S("description"));
	m_description->setCss(_S("os_input_full"));
	m_description->setMaxLength(OS_CONTROLS_DESCRIPTION_MAXLENGTH);
	m_content->setID(_S("content"));
	m_content->setPreviewCallback(boost::bind(&TextEditor::onPreviewCallback, this, _1));
	m_model->setID(_S("model"));
	m_model->setCss(_S("os_input_full")); // In realt ci andrebbe un object-picker
	m_pinned->setID(_S("pinned"));

	getTemplate()->addChildParam(m_title, _S("title"));
	getTemplate()->addChildParam(m_description, _S("description"));
	getTemplate()->addChildParam(m_content, _S("content"));
	getTemplate()->addChildParam(m_model, _S("model"));
	getTemplate()->addChildParam(m_pinned, _S("pinned"));
}

TextEditor::~TextEditor()
{

}

String TextEditor::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("text_editor.xsl"));
}

shared_ptr<ObjectsIRevisionable> TextEditor::createObject()
{
	shared_ptr<ObjectsText> text(OS_NEW ObjectsText());
	text->title = algorithms::trim_copy(m_title->getValue());
	text->description = algorithms::trim_copy(m_description->getValue());
	text->content = algorithms::trim_copy(m_content->getValue());
	text->model = EntityID(static_cast<String>(m_model->getValue()).to_ascii());
	text->pinned = m_pinned->getCheck();

	return text;
}

void TextEditor::onPreRender()
{
	EditorBase::onPreRender();

	if(isRevision() && getPostBack() == false)
	{
		shared_ptr<ObjectsText> current = objects_text_cast(getCurrent());
		if(current != nullptr)
		{
			m_title->setValue(current->title);
			m_description->setValue(current->description);
			m_content->setValue(current->content);
			m_model->setValue(current->model->toUTF16());
			m_pinned->setCheck(current->pinned);
		}
	}
}

void TextEditor::onPreviewCallback(String &preview)
{
	EntityID model = m_model->getValue().to_ascii();
	if(model.empty() == false)
	{
		shared_ptr<EntitiesEntity> model_entity = getPortal()->getEntity(getDatabase(), model);
		shared_ptr<ObjectsModel> model_object = model_entity != nullptr ? objects_model_cast(model_entity->getCurrent()) : nullptr;
		if(model_object == nullptr)
		{
			getPage()->showError(getPage()->getText(_S("text.editor.errors.model_not_found")));
			return;
		}

		String transformed;
		if(model_object->apply(preview, transformed))
		{
			preview = transformed;
		}
		else
		{
			getPage()->showError(getPage()->getText(_S("text.editor.errors.model_transformation_error")));
		}
	}
}

//////////////////////////////////////////////////////////////////////

} // edit
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
