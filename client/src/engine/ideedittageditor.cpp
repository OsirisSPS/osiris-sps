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
#include "ideedittageditor.h"

#include "algorithms.h"
#include "iportaldatabase.h"
#include "idesession.h"
#include "ideskin.h"
#include "objectstag.h"
#include "css.h"
#include "htmlcontrols.h"
#include "htmltextbox.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace edit {

//////////////////////////////////////////////////////////////////////

TagEditor::TagEditor(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent) : EditorBase(portalObjectTypeTag, entity, parent),
																				 m_name(OS_NEW HtmlTextBox())
{
	m_name->setID(_S("name"));
	m_name->setCss(_S("os_input_full"));
	m_name->setMaxLength(OS_CONTROLS_TITLE_MAXLENGTH);

	getTemplate()->addChildParam(m_name, _S("name"));
}

TagEditor::~TagEditor()
{

}

String TagEditor::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("tag_editor.xsl"));
}

shared_ptr<ObjectsIRevisionable> TagEditor::createObject()
{
	shared_ptr<ObjectsTag> tag(OS_NEW ObjectsTag());
	tag->name = algorithms::trim_copy(m_name->getValue());

	return tag;
}

void TagEditor::onPreRender()
{
	EditorBase::onPreRender();

	if(isRevision() && getPostBack() == false)
	{
		shared_ptr<ObjectsTag> current = objects_tag_cast(getCurrent());
		if(current != null)
			m_name->setValue(current->name);
	}
}

//////////////////////////////////////////////////////////////////////

} // edit
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
