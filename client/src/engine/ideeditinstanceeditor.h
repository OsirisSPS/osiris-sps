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

#ifndef _IDE_EDIT_INSTANCEEDITOR_H
#define _IDE_EDIT_INSTANCEEDITOR_H

#include "extensionsextensions.h"
#include "ideeditiobjecteditor.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlComboBox;
class HtmlTextBox;
class IExtensionsModule;
class ExtensionsModuleEditor;

//////////////////////////////////////////////////////////////////////

namespace ide {
namespace edit {

//////////////////////////////////////////////////////////////////////

class EngineExport InstanceEditor : public IObjectEditor
{
	typedef IObjectEditor EditorBase;

// Construction
public:
	InstanceEditor(shared_ptr<EntitiesEntity> entity = nullptr, shared_ptr<EntitiesEntity> parent = nullptr);
	virtual ~InstanceEditor();

// IXSLRenderer interface
public:
	virtual String getTemplatePath();

// IObjectEditor interface
protected:
	virtual shared_ptr<ObjectsIRevisionable> createObject();

// IObjectEditor overrides
protected:
	virtual void onInit();
	virtual void onLoad();
	void renderActions(shared_ptr<XMLNode> nodeActions);

// Events
protected:
	void onModuleChanged();
	void onPreview();

private:
	shared_ptr<HtmlTextBox> m_title;
	shared_ptr<HtmlComboBox> m_area;
	shared_ptr<HtmlComboBox> m_inherit;
	shared_ptr<HtmlComboBox> m_modulesList;
	shared_ptr<HtmlComboBox> m_block;
    shared_ptr<HtmlTextBox> m_style;
	shared_ptr<ExtensionsModuleEditor> m_editor;
	shared_ptr<ObjectsInstance> m_current;
	shared_ptr<IExtensionsModule> m_module;

private:
	static const String EVENT_ONPREVIEW;
};

//////////////////////////////////////////////////////////////////////

} // edit
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_EDIT_INSTANCEEDITOR_H
