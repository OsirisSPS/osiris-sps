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
#include "ideeditpolleditor.h"

#include "algorithms.h"
#include "conversions.h"
#include "iportaldatabase.h"
#include "idesession.h"
#include "ideskin.h"
#include "objectspoll.h"
#include "css.h"
#include "htmlcombobox.h"
#include "htmlcontrols.h"
#include "htmlliteral.h"
#include "htmltextbox.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace edit {

//////////////////////////////////////////////////////////////////////

PollEditor::PollEditor(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent) : EditorBase(portalObjectTypePoll, entity, parent),
																				   m_title(OS_NEW HtmlTextBox()),
																				   m_type(OS_NEW HtmlComboBox())
{
	m_title->setID(_S("title"));
	m_title->setCss(_S("os_input_full"));
	m_type->setID(_S("type"));

	getTemplate()->addChildParam(m_title, _S("title"));
	getTemplate()->addChildParam(m_type, _S("type"));
}

PollEditor::~PollEditor()
{

}

String PollEditor::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("poll_editor.xsl"));
}

shared_ptr<ObjectsIRevisionable> PollEditor::createObject()
{
	shared_ptr<ObjectsPoll> poll(OS_NEW ObjectsPoll());
	poll->title = algorithms::trim_copy(m_title->getValue());
	poll->type = conversions::from_utf16<uint32>(m_type->getValue());

	return poll;
}

void PollEditor::onInit()
{
	EditorBase::onInit();

	m_type->addOption(getPage()->getText(_S("poll.editor.type.single_choice")), conversions::to_utf16<uint32>(ptSingleChoice));
	m_type->addOption(getPage()->getText(_S("poll.editor.type.multiple_choice")), conversions::to_utf16<uint32>(ptMultipleChoice));
}

void PollEditor::onPreRender()
{
	EditorBase::onPreRender();

	if(isRevision() && getPostBack() == false)
	{
		shared_ptr<ObjectsPoll> current = objects_poll_cast(getCurrent());
		if(current != null)
		{
			m_title->setValue(current->title);
			m_type->setValue(conversions::to_utf16<uint32>(current->type));
		}
	}
}

//////////////////////////////////////////////////////////////////////

} // edit
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
