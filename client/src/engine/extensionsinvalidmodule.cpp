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
#include "extensionsinvalidmodule.h"

#include "ideskin.h"
#include "htmlattributes.h"
#include "htmlcontrols.h"
#include "htmlimage.h"
#include "htmlliteral.h"
#include "htmltext.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ExtensionsInvalidModule::ExtensionsInvalidModule(const ExtensionsModuleID &module) : m_module(module)
{	
}

ExtensionsInvalidModule::~ExtensionsInvalidModule()
{

}

bool ExtensionsInvalidModule::getShowBlock() const
{
	return false;
}

void ExtensionsInvalidModule::onLoad()
{
	ViewerBase::onLoad();

	/*
	shared_ptr<HtmlImage> img(OS_NEW HtmlImage(getPage()->getSkin()->getImageUrl(_S("invalid_module.png"))));
	setCss(_S("os_modules_invalid"));
	img->getAttributes()->set(_S("data-os-tooltip"), getPage()->getText(_S("system.invalid_module")) + _S(": ") + m_module.toUTF16());
	getControls()->add(img);
	*/	
	setCss(_S("os_modules_invalid"));
	getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getPage()->getText(_S("system.invalid_module")))));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
