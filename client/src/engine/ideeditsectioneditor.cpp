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
#include "ideeditsectioneditor.h"

#include "algorithms.h"
#include "constants.h"
#include "convert.h"
#include "conversions.h"
#include "iportaldatabase.h"
#include "extensionscomponenteditor.h"
#include "extensionssystem.h"
#include "iextensionscomponent.h"
#include "ideomleditor.h"
#include "idesession.h"
#include "ideskin.h"

#include "objectssystem.h"
#include "objectssection.h"
#include "htmlevent.h"
#include "css.h"
#include "htmlcombobox.h"
#include "htmlcontrols.h"
#include "htmlliteral.h"
#include "htmloption.h"
#include "htmltable.h"
#include "htmltablecell.h"
#include "htmltablerow.h"
#include "htmltextbox.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace edit {

//////////////////////////////////////////////////////////////////////

SectionEditor::SectionEditor(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent) : EditorBase(portalObjectTypeSection, entity, parent),
																						 m_title(OS_NEW HtmlTextBox()),
																						 m_description(OS_NEW IdeOMLEditor()),
																						 m_componentsList(OS_NEW HtmlComboBox())
{
	m_title->setID(_S("title"));
	m_title->setCss(_S("os_input_full"));
	m_title->setMaxLength(OS_CONTROLS_TITLE_MAXLENGTH);
	m_description->setID(_S("description"));
	m_componentsList->setID(_S("components"));
	m_componentsList->setAutoPostBack(true);

	getTemplate()->addChildParam(m_title, _S("title"));
	getTemplate()->addChildParam(m_description, _S("description"));
	getTemplate()->addChildParam(m_componentsList, _S("component"));
}

SectionEditor::~SectionEditor()
{

}

String SectionEditor::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("section_editor.xsl"));
}

shared_ptr<ObjectsIRevisionable> SectionEditor::createObject()
{
	shared_ptr<ObjectsSection> section(OS_NEW ObjectsSection());
	section->title = algorithms::trim_copy(m_title->getValue());
	section->description = algorithms::trim_copy(m_description->getValue());

	ExtensionsComponentID component = static_cast<ExtensionsComponentID>(m_componentsList->getValue().to_ascii());
	if(component.empty() == false)
	{
		section->component = component;

		String data;
		if(m_editor != nullptr)
		{
			if(m_editor->createDocument() == false)
				return nullptr;

			// Questo perch potrebbe aver creato la vista di edit, ma comunque non valorizza i parametri.
			if(m_editor->getComponentDocument()->getRoot() != nullptr)
			{
				if(m_editor->getComponentDocument()->writeString(data) == false)
					return nullptr;
			}
		}
		if(data.empty() == false)
			section->data = data;
	}

	return section;
}

void SectionEditor::onInit()
{
	EditorBase::onInit();

	m_componentsList->addOption(getPage()->getText(_S("section.components.none")), ExtensionsComponentID::EMPTY.toUTF16())->setSelected(true);

	const ExtensionsSystem::Components &components = ExtensionsSystem::instance()->getComponents();
	for(ExtensionsSystem::Components::const_iterator i = components.begin(); i != components.end(); ++i)
	{
		shared_ptr<IExtensionsComponent> component = i->second;
		m_componentsList->addOption(component->getName(), component->getID().toUTF16());
	}

	m_current = objects_section_cast(getCurrent());
	if(m_current != nullptr)
	{
		if(isRevision() && getPostBack() == false)
		{
			m_title->setValue(m_current->title);
			m_description->setValue(m_current->description);
			m_componentsList->setValue(m_current->component->toUTF16());
		}
	}
}

void SectionEditor::onLoad()
{
	EditorBase::onLoad();

	ExtensionsComponentID selectedComponentID(m_componentsList->getValue().to_ascii());
	if(selectedComponentID.empty() == false)
	{
		const ExtensionsSystem::Components &components = ExtensionsSystem::instance()->getComponents();
		for(ExtensionsSystem::Components::const_iterator i = components.begin(); i != components.end(); ++i)
		{
			shared_ptr<IExtensionsComponent> component = i->second;
			if(component->getID() == selectedComponentID)
			{
				m_component = component;

				m_editor = ExtensionsSystem::instance()->createComponentEditor(component->getID(), getEntity());
				if(m_editor != nullptr)
				{
					getTemplate()->addChildParam(m_editor, _S("editor"));

					/*
					// Carica l'xml della base solo nel caso in cui si tratti dello stesso modulo
					if(m_current != nullptr && m_current->component == selectedComponentID)
					{
						m_editor->init(m_current->title, getPrimaryID(), m_current->data);
					}
					*/

					m_editor->init(ExtensionsSystem::instance()->getComponent(selectedComponentID), m_current->getEntityID(), m_current->data);
				}

				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

} // edit
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
