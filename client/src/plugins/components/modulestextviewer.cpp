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
#include "modulestextviewer.h"

#include "omlmanager.h"
#include "ideviewiobjectviewer.h"
#include "modulestext.h"
#include "modulesmodulesconfig.h"
#include "objectsdescriptor.h"
#include "objectssystem.h"
#include "idesession.h"
#include "htmlliteral.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace modules {

//////////////////////////////////////////////////////////////////////

TextViewer::TextViewer()
{

}

TextViewer::~TextViewer()
{

}

void TextViewer::onLoad()
{
	ViewerBase::onLoad();

	shared_ptr<XMLNode> root = getModuleDocument()->getRoot();
	if(root != nullptr)
	{
		String text = root->getAttributeString(OS_MODULES_TEXT_TEXT);
		String style = root->getAttributeString(OS_MODULES_TEXT_STYLE);

		setCss(_S("os_modules_text"));
		setStyle(OMLManager::instance()->parseCss(style, getPage(), false, true));
		getControls()->add(shared_ptr<HtmlLiteral>(OS_NEW HtmlLiteral(getPage()->parseOml(text, true, false, false, omlRenderModeOsiris, getInstance().getString()))));
	}
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
