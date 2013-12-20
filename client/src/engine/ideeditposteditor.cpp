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
#include "ideeditposteditor.h"

#include "algorithms.h"
#include "conversions.h"
#include "iportaldatabase.h"
#include "ideomleditor.h"
#include "idesession.h"
#include "ideskin.h"
#include "objectspost.h"
#include "css.h"
#include "htmltextbox.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace edit {

//////////////////////////////////////////////////////////////////////

PostEditor::PostEditor(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent) : EditorBase(portalObjectTypePost, entity, parent),
																				   m_title(OS_NEW HtmlTextBox()),
																				   m_content(OS_NEW IdeOMLEditor())
{
	m_title->setID(_S("title"));
	m_title->setCss(_S("os_input_full"));
	m_title->setMaxLength(OS_CONTROLS_TITLE_MAXLENGTH);
	m_content->setID(_S("content"));

	getTemplate()->addChildParam(m_title, _S("title"));
	getTemplate()->addChildParam(m_content, _S("content"));
}

PostEditor::~PostEditor()
{

}

const String & PostEditor::getTitle() const
{
	return m_title->getValue();
}

void PostEditor::setTitle(const String &title)
{
	m_title->setValue(title);
}

const String & PostEditor::getContent() const
{
	return m_content->getValue();
}

void PostEditor::setContent(const String &content)
{
	m_content->setValue(content);
}

const EntityID & PostEditor::getReference() const
{
	return m_reference;
}

void PostEditor::setReference(const EntityID &reference)
{
	m_reference = reference;
}

String PostEditor::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("post_editor.xsl"));
}

shared_ptr<ObjectsIRevisionable> PostEditor::createObject()
{
	shared_ptr<ObjectsPost> post(OS_NEW ObjectsPost());
	post->title = algorithms::trim_copy(m_title->getValue());
	post->content = algorithms::trim_copy(m_content->getValue());
	if(m_reference.empty() == false)
		post->reference = m_reference;

	return post;
}

void PostEditor::onPreRender()
{
	EditorBase::onPreRender();

	if(isRevision() && getPostBack() == false)
	{
		shared_ptr<ObjectsPost> current = objects_post_cast(getCurrent());
		if(current != nullptr)
		{
			m_title->setValue(current->title);
     		m_content->setValue(current->content);
		}
	}
}

//////////////////////////////////////////////////////////////////////

} // edit
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
