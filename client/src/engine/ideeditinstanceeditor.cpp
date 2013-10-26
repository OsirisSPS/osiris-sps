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
#include "ideeditinstanceeditor.h"

#include "algorithms.h"
#include "boost/bind.hpp"
#include "iportaldatabase.h"
#include "convert.h"
#include "conversions.h"
#include "constants.h"
#include "iextensionsmodule.h"
#include "extensionsmoduleeditor.h"
#include "extensionsmoduleviewer.h"
#include "omlmanager.h"
#include "ideomleditor.h"
#include "ideblock.h"
#include "idesession.h"
#include "ideskin.h"
#include "eventssource.h"
#include "extensionssystem.h"
#include "objectsinstance.h"

#include "htmlevent.h"
#include "css.h"
#include "htmlcheckbox.h"
#include "htmlcombobox.h"
#include "htmlcontrols.h"
#include "htmlliteral.h"
#include "htmloption.h"
#include "htmltable.h"
#include "htmltablecell.h"
#include "htmltablerow.h"
#include "htmltext.h"
#include "htmltextbox.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace edit {

//////////////////////////////////////////////////////////////////////

const String InstanceEditor::EVENT_ONPREVIEW = _S("onPreview");

//////////////////////////////////////////////////////////////////////

InstanceEditor::InstanceEditor(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent) :	EditorBase(portalObjectTypeInstance, entity, parent),
																							m_title(OS_NEW HtmlTextBox()),
																							m_area(OS_NEW HtmlComboBox()),
																							m_inherit(OS_NEW HtmlComboBox()),
																							m_modulesList(OS_NEW HtmlComboBox()),
																							m_block(OS_NEW HtmlComboBox()),
																							m_style(OS_NEW HtmlTextBox())
{
	m_title->setID(_S("title"));
	m_title->setCss(_S("os_input_full"));
	m_title->setMaxLength(OS_CONTROLS_TITLE_MAXLENGTH);

	m_area->setID(_S("area"));
	m_inherit->setID(_S("inherit"));
	m_modulesList->setID(_S("modules"));
	m_modulesList->getEventChange()->connect(boost::bind(&InstanceEditor::onModuleChanged, this));
	m_block->setID(_S("blockType"));
	m_style->setID(_S("blockStyle"));
	m_style->setCss(_S("os_input_full"));

	getTemplate()->addChildParam(m_title, _S("title"));
	getTemplate()->addChildParam(m_area, _S("area"));
	getTemplate()->addChildParam(m_inherit, _S("inherit"));
	getTemplate()->addChildParam(m_modulesList, _S("module"));
	getTemplate()->addChildParam(m_block, _S("block"));
	getTemplate()->addChildParam(m_style, _S("style"));

	getEvents()->get(EVENT_ONPREVIEW)->connect(boost::bind(&InstanceEditor::onPreview, this));
}

InstanceEditor::~InstanceEditor()
{

}

String InstanceEditor::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("instance_editor.xsl"));
}

shared_ptr<ObjectsIRevisionable> InstanceEditor::createObject()
{
	shared_ptr<ObjectsInstance> instance(OS_NEW ObjectsInstance());
	instance->title = algorithms::trim_copy(m_title->getValue());
	instance->area = conversions::from_utf16<uint32>(m_area->getValue());
	instance->inherit = conversions::from_utf16<uint32>(m_inherit->getValue());
	instance->module = static_cast<ExtensionsModuleID>(m_modulesList->getValue().to_ascii());
	instance->block = conversions::from_utf16<uint32>(m_block->getValue());
	instance->style = algorithms::trim_copy(m_style->getValue());

	String data;
	if(m_editor != null)
	{
		if(m_editor->createDocument() == false)
			return null;

		// Questo perch potrebbe aver creato la vista di edit, ma comunque non valorizza i parametri.
		if(m_editor->getModuleDocument()->getRoot() != null)
		{
			if(m_editor->getModuleDocument()->writeString(data) == false)
				return null;
		}
	}
	if(data.empty() == false)
		instance->data = data;

	return instance;
}

void InstanceEditor::onInit()
{
	EditorBase::onInit();

	const ExtensionsSystem::Modules &modules = ExtensionsSystem::instance()->getModules();
	for(ExtensionsSystem::Modules::const_iterator i = modules.begin(); i != modules.end(); ++i)
	{
		shared_ptr<IExtensionsModule> module = i->second;
		OS_ASSERT(module != null);

		shared_ptr<HtmlOption> option = m_modulesList->addOption(getPage()->parseOml(module->getName(), false, false, true), module->getID().toUTF16());

		// VERYURGENT: prendere da un'opzione
		if(module->getID().toUTF16() == _S("F81C7ADA024CA4CB38201B82D5370C0392770773"))
			option->setSelected(true);
	}

	m_area->addOption(getPage()->getText(_S("instance.editor.area.left")), Convert::toString(iaLeft));
	m_area->addOption(getPage()->getText(_S("instance.editor.area.top")), Convert::toString(iaTop));
	m_area->addOption(getPage()->getText(_S("instance.editor.area.right")), Convert::toString(iaRight));
	m_area->addOption(getPage()->getText(_S("instance.editor.area.bottom")), Convert::toString(iaBottom));
	m_area->addOption(getPage()->getText(_S("instance.editor.area.content_top")), Convert::toString(iaContentTop));
	m_area->addOption(getPage()->getText(_S("instance.editor.area.content_bottom")), Convert::toString(iaContentBottom));
	m_area->addOption(getPage()->getText(_S("instance.editor.area.content")), Convert::toString(iaContent));
	m_area->setValue(Convert::toString(iaContent));

	m_inherit->addOption(getPage()->getText(_S("instance.editor.inherit.none")), Convert::toString(iiNone));
	m_inherit->addOption(getPage()->getText(_S("instance.editor.inherit.parent")), Convert::toString(iiParent));
	m_inherit->addOption(getPage()->getText(_S("instance.editor.inherit.recursive")), Convert::toString(iiRecursive));
	m_inherit->setValue(Convert::toString(iiParent));

	m_block->addOption(getPage()->getText(_S("instance.editor.block.none")), conversions::to_utf16<uint32>(blockTypeNone));
	m_block->addOption(getPage()->getText(_S("instance.editor.block.heading")), conversions::to_utf16<uint32>(blockTypeHeading));
	m_block->addOption(getPage()->getText(_S("instance.editor.block.minimal")), conversions::to_utf16<uint32>(blockTypeMinimal));
	m_block->addOption(getPage()->getText(_S("instance.editor.block.small")), conversions::to_utf16<uint32>(blockTypeSmall));
	m_block->addOption(getPage()->getText(_S("instance.editor.block.big")), conversions::to_utf16<uint32>(blockTypeBig));
	m_block->setValue(conversions::to_utf16<uint32>(blockTypeSmall));

	m_current = objects_instance_cast(getCurrent());

	if(isRevision() && getPostBack() == false)
	{
		m_title->setValue(m_current->title);
		m_area->setValue(conversions::to_utf16<uint32>(m_current->area));
		m_inherit->setValue(conversions::to_utf16<uint32>(m_current->inherit));
		m_modulesList->setValue(m_current->module->toUTF16());
		m_block->setValue(conversions::to_utf16<uint32>(m_current->block));
		m_style->setValue(m_current->style);
	}
}

void InstanceEditor::onLoad()
{
	EditorBase::onLoad();

	ExtensionsModuleID selectedModuleID(m_modulesList->getValue().to_ascii());
	if(selectedModuleID.empty() == false)
	{
		const ExtensionsSystem::Modules &modules = ExtensionsSystem::instance()->getModules();
		for(ExtensionsSystem::Modules::const_iterator i = modules.begin(); i != modules.end(); ++i)
		{
			shared_ptr<IExtensionsModule> module = i->second;
			if(module->getID() == selectedModuleID)
			{
				m_module = module;

				m_editor = ExtensionsSystem::instance()->createModuleEditor(module->getID());
				if(m_editor != null)
				{
					getTemplate()->addChildParam(m_editor, _S("editor"));

					/*
					// Carica l'xml della base solo nel caso in cui si tratti dello stesso modulo
					if(m_current != null && m_current->module == selectedModuleID)
					{
						m_editor->init(m_module, m_current->title, getPrimaryID(), m_current->data);
					}
					*/

					// 0.12 - Se non è lo stesso modulo, resetta il documento.
					String title;
					String data;
					if(m_current != null && m_current->module == selectedModuleID)
					{
						title = m_current->title;
						data = m_current->data;
					}
					m_editor->init(m_module, title, getEntityID(), data);
				}

				break;
			}
		}
	}
}

void InstanceEditor::onModuleChanged()
{
	if(m_module != null && isRevision() == false)
		m_style->setValue(m_module->getDefaultStyle());
}

void InstanceEditor::renderActions(shared_ptr<XMLNode> nodeActions)
{
	EditorBase::renderActions(nodeActions);

	shared_ptr<XMLNode> actionCreate = nodeActions->addChild(_S("action"));
	actionCreate->setAttributeString(_S("name"), _S("preview"));
	actionCreate->setAttributeString(_S("href"), getEventCommand(EVENT_ONPREVIEW));
}

void InstanceEditor::onPreview()
{
	shared_ptr<ObjectsInstance> instance = objects_instance_cast(createObject());

	if(instance == null)
		return;

	shared_ptr<IExtensionsModule> module = ExtensionsSystem::instance()->getModule(instance->module);
	OS_EXCEPT_IF(module == null, "Invalid instance module");
	shared_ptr<ExtensionsModuleViewer> viewer = ExtensionsSystem::instance()->createModuleViewer(instance->module, false);
	OS_EXCEPT_IF(viewer == null, "Invalid instance module");
	viewer->init(module, instance->title, constants::empty_string, instance->data);

	shared_ptr<HtmlDiv> instance_ctrl(OS_NEW HtmlDiv());
	// chiamata alla 'encodeCss' per evitare javascript-injection
	instance_ctrl->setStyle(OMLManager::instance()->parseCss(instance->style, getPage(), false, true));

	shared_ptr<IdeBlock> block(OS_NEW IdeBlock(instance->title));
	block->setType(instance->getBlockType());
	block->getBody()->getControls()->add(viewer);

	instance_ctrl->getControls()->add(block);

	// Inserisce il controllo nell'area di riferimento dell'istanza
	getPage()->getArea(static_cast<PageArea>(instance->getArea()))->getControls()->add(instance_ctrl);
}

//////////////////////////////////////////////////////////////////////

} // edit
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
