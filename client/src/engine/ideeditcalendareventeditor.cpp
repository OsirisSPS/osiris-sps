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
#include "ideeditcalendareventeditor.h"

#include "algorithms.h"
#include "conversions.h"
#include "css.h"
#include "iportaldatabase.h"
#include "htmlcombobox.h"
#include "htmlcontrols.h"
#include "htmlevent.h"
#include "htmltext.h"
#include "htmltextbox.h"
#include "ideomleditor.h"
#include "idepickerdatetime.h"
#include "idesession.h"
#include "ideskin.h"

#include "objectscalendarevent.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace edit {

//////////////////////////////////////////////////////////////////////

CalendarEventEditor::CalendarEventEditor(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent) : EditorBase(portalObjectTypeCalendarEvent, entity, parent),
																									 m_title(OS_NEW HtmlTextBox()),
																									 m_description(OS_NEW HtmlTextBox()),
																									 m_content(OS_NEW IdeOMLEditor()),
																									 m_startDate(OS_NEW IdePickerDateTime()),
																									 m_endDate(OS_NEW IdePickerDateTime()),
																									 m_repeatMode(OS_NEW HtmlComboBox()),
																									 m_expirationDate(OS_NEW IdePickerDateTime())
{
	m_title->setID(_S("title"));
	m_title->setCss(_S("os_input_full"));
	m_title->setMaxLength(OS_CONTROLS_TITLE_MAXLENGTH);
	m_description->setID(_S("description"));
	m_description->setCss(_S("os_input_full"));
	m_description->setMaxLength(OS_CONTROLS_DESCRIPTION_MAXLENGTH);
	m_content->setID(_S("content"));
	m_startDate->setID(_S("startDate"));
	m_endDate->setID(_S("endDate"));

	m_repeatMode->setID(_S("repeatMode"));
	m_repeatMode->addOption(_S("repeat_none"), conversions::to_utf16<uint32>(cmRepeatNone));
	m_repeatMode->addOption(_S("repeat_week"), conversions::to_utf16<uint32>(cmRepeatWeek));
	m_repeatMode->addOption(_S("repeat_month"), conversions::to_utf16<uint32>(cmRepeatMonth));
	m_repeatMode->addOption(_S("repeat_year"), conversions::to_utf16<uint32>(cmRepeatYear));

	m_expirationDate->setID(_S("expirationDate"));

	getTemplate()->addChildParam(m_title, _S("title"));
	getTemplate()->addChildParam(m_description, _S("description"));
	getTemplate()->addChildParam(m_content, _S("content"));
	getTemplate()->addChildParam(m_startDate, _S("start_date"));
	getTemplate()->addChildParam(m_endDate, _S("end_date"));
	getTemplate()->addChildParam(m_repeatMode, _S("repeat_mode"));
	getTemplate()->addChildParam(m_expirationDate, _S("expiration_date"));
}

CalendarEventEditor::~CalendarEventEditor()
{

}

String CalendarEventEditor::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("calendar_event_editor.xsl"));
}

shared_ptr<ObjectsIRevisionable> CalendarEventEditor::createObject()
{
	shared_ptr<ObjectsCalendarEvent> calendar_event(OS_NEW ObjectsCalendarEvent());
	calendar_event->title = algorithms::trim_copy(m_title->getValue());
	calendar_event->description = algorithms::trim_copy(m_description->getValue());
	calendar_event->content = algorithms::trim_copy(m_content->getValue());
	calendar_event->start_date = m_startDate->getValue();
	calendar_event->end_date = m_endDate->getValue();
	calendar_event->repeat_mode = conversions::from_utf16<uint32>(m_repeatMode->getValue());
	calendar_event->expiration_date = m_expirationDate->getValue();

	return calendar_event;
}

void CalendarEventEditor::onPreRender()
{
	EditorBase::onPreRender();

	if(isRevision() && getPostBack() == false)
	{
		shared_ptr<ObjectsCalendarEvent> current = objects_calendar_event_cast(getCurrent());
		if(current != null)
		{
			m_title->setValue(current->title);
			m_description->setValue(current->description);
			m_startDate->setValue(current->start_date);
			m_endDate->setValue(current->end_date);
			m_repeatMode->setValue(conversions::to_utf16<uint32>(current->repeat_mode));
			m_expirationDate->setValue(current->expiration_date);
		}
	}
}

//////////////////////////////////////////////////////////////////////

} // edit
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
