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
#include "ideeditpolloptioneditor.h"

#include "algorithms.h"
#include "iportaldatabase.h"
#include "idesession.h"
#include "ideskin.h"
#include "objectspolloption.h"
#include "css.h"
#include "htmlcontrols.h"
#include "htmltextbox.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace edit {

//////////////////////////////////////////////////////////////////////

PollOptionEditor::PollOptionEditor(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent) : EditorBase(portalObjectTypePollOption, entity, parent),
																							   m_title(OS_NEW HtmlTextBox())
{
	m_title->setID(_S("title"));
	m_title->setCss(_S("os_input_full"));
	m_title->setMaxLength(OS_CONTROLS_TITLE_MAXLENGTH);

	getTemplate()->addChildParam(m_title, _S("title"));
}

PollOptionEditor::~PollOptionEditor()
{

}

String PollOptionEditor::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("poll_option_editor.xsl"));
}

shared_ptr<ObjectsIRevisionable> PollOptionEditor::createObject()
{
	shared_ptr<ObjectsPollOption> poll_option(OS_NEW ObjectsPollOption());
	poll_option->title = algorithms::trim_copy(m_title->getValue());

	return poll_option;
}

void PollOptionEditor::onPreRender()
{
	EditorBase::onPreRender();

	if(isRevision() && getPostBack() == false)
	{
		shared_ptr<ObjectsPollOption> current = objects_poll_option_cast(getCurrent());
		if(current != nullptr)
			m_title->setValue(current->title);
	}
}

//////////////////////////////////////////////////////////////////////

} // edit
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
