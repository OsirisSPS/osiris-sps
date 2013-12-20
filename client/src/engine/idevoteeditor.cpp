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
#include "idevoteeditor.h"

#include "conversions.h"
#include "htmlcombobox.h"
#include "htmlcontrols.h"
#include "htmllinkbutton.h"
#include "htmlliteral.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String IdeVoteEditor::EVENT_ONCHANGE = _S("onChange");

//////////////////////////////////////////////////////////////////////

IdeVoteEditor::IdeVoteEditor() : m_values(OS_NEW HtmlComboBox()),
						   m_update(OS_NEW HtmlLinkButton())				
{

}

IdeVoteEditor::~IdeVoteEditor()
{

}

uint32 IdeVoteEditor::getValue() const
{
	return conversions::from_utf16<uint32>(m_values->getValue());
}

void IdeVoteEditor::setValue(uint32 value)
{
	m_values->setValue(conversions::to_utf16(value));
}

bool IdeVoteEditor::isNull() const
{
	return m_values->getValue().empty();
}

void IdeVoteEditor::setNull()
{
	m_values->setValue(String::EMPTY);
}

shared_ptr<EventSource> IdeVoteEditor::getEventUpdate() const
{
	return m_update->getEventClick();
}

void IdeVoteEditor::onInit()
{
	ControlBase::onInit();

	m_values->setID(_S("values"));

	m_values->addOption(getPage()->getText(_S("vote_editor.votes.nullptr")), _S(""));
	for(uint32 i = OS_PORTAL_OBJECT_VOTE_MIN; i <= OS_PORTAL_OBJECT_VOTE_MAX; i+=2)
	{
		String value = conversions::to_utf16<uint32>(i);
		m_values->addOption(getPage()->getText(_S("vote_editor.votes.") + value), value);
	}
	
	m_update->setCaption(getPage()->getText(_S("vote_editor.vote.caption")));
	m_update->setID(_S("update"));

	getControls()->add(m_values);
	getControls()->add(m_update);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
