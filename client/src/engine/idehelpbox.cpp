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
#include "idehelpbox.h"

#include "iidepage.h"
#include "ideskin.h"
#include "htmlpanel.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdeHelpBox::IdeHelpBox(const String &text, const String &helpID) : m_text(text),
														  m_helpID(helpID)
{	
}

IdeHelpBox::~IdeHelpBox()
{

}

String IdeHelpBox::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("help_box.xsl"));
}

void IdeHelpBox::onInit()
{
	ControlBase::onInit();

	shared_ptr<XMLDocument> document = getDocument();
	shared_ptr<XMLNode> root = document->create(_S("help_box"));

	root->setAttributeString(_S("text"), m_text);
	root->setAttributeString(_S("id"), m_helpID);	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
