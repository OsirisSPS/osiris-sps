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

#ifndef _IDE_EDIT_TAGEDITOR_H
#define _IDE_EDIT_TAGEDITOR_H

#include "ideeditiobjecteditor.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlTextBox;

//////////////////////////////////////////////////////////////////////

namespace ide {
namespace edit {

//////////////////////////////////////////////////////////////////////

class EngineExport TagEditor : public IObjectEditor
{
	typedef IObjectEditor EditorBase;

// Construction
public:
	TagEditor(shared_ptr<EntitiesEntity> entity = null, shared_ptr<EntitiesEntity> parent = null);
	virtual ~TagEditor();

// IXSLRenderer interface
public:
	virtual String getTemplatePath();

// IObjectEditor interface
protected:
	virtual shared_ptr<ObjectsIRevisionable> createObject();

// IObjectEditor overrides
protected:
	virtual void onPreRender();

private:
	shared_ptr<HtmlTextBox> m_name;
};

//////////////////////////////////////////////////////////////////////

} // edit
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_EDIT_TAGEDITOR_H
