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
#include "modulespathwayeditor.h"

#include "conversions.h"
#include "htmlcombobox.h"
#include "ideomleditor.h"
#include "modulespathway.h"
#include "modulesmodulesconfig.h"

#include "htmlcontrols.h"
#include "htmltextbox.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

extern OS_NAMESPACE_NAME::String g_componentsPath;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {

//////////////////////////////////////////////////////////////////////

namespace modules {

//////////////////////////////////////////////////////////////////////

PathwayEditor::PathwayEditor()
{	
}

PathwayEditor::~PathwayEditor()
{

}

bool PathwayEditor::createDocument()
{
	if(EditorBase::createDocument() == false)
		return false;
	
	return true;
}

void PathwayEditor::onInit()
{
	EditorBase::onInit();	
}

void PathwayEditor::onLoad()
{
	EditorBase::onLoad();	
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
