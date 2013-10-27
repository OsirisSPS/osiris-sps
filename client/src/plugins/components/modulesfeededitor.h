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

#ifndef _COMPONENTS_MODULES_FEEDEDITOR_H
#define _COMPONENTS_MODULES_FEEDEDITOR_H

#include "extensionsmoduleeditor.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IdeOMLEditor;
class HtmlTextBox;
class HtmlComboBox;
class IdePickerBool;

//////////////////////////////////////////////////////////////////////

namespace components {
namespace modules {

//////////////////////////////////////////////////////////////////////

class FeedEditor : public ExtensionsModuleEditor
{
	typedef ExtensionsModuleEditor EditorBase;

// Construction
public:
	FeedEditor();
	virtual ~FeedEditor();

// ModuleEditor overrides
public:
	virtual bool createDocument();

protected:
	virtual void onInit();
	virtual void onLoad();

private:
	shared_ptr<HtmlTextBox> m_url;
	shared_ptr<IdePickerBool> m_showTitle;
	shared_ptr<IdePickerBool> m_showDescription;
	shared_ptr<IdePickerBool> m_showImage;
	shared_ptr<HtmlTextBox> m_nItems;
	shared_ptr<HtmlComboBox> m_showItemDescription;
	shared_ptr<HtmlComboBox> m_templateModel;
	shared_ptr<IdeOMLEditor> m_templateCustom;
};

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _COMPONENTS_MODULES_FEEDEDITOR_H
