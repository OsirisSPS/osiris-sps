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
#include "ideblock.h"

#include "iidepage.h"
#include "ideskin.h"
#include "htmlpanel.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdeBlock::IdeBlock(const String &title) : m_title(title),
									m_body(OS_NEW HtmlPanel()),
									m_type(blockTypeBig),
									m_page(false)
{
	getTemplate()->addChildParam(m_body, _S("body"));
}

IdeBlock::~IdeBlock()
{

}

String IdeBlock::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("block.xsl"));
}

void IdeBlock::onInit()
{
	ControlBase::onInit();

	shared_ptr<XMLDocument> document = getDocument();
	shared_ptr<XMLNode> root = document->create(_S("block"));

	if(m_page)
		root->setAttributeBool(_S("page"), m_page);

	root->setAttributeString(_S("title"), m_title);
	if(isLink())
		root->setAttributeString(_S("href"), m_href);

	switch(m_type)
	{
	case blockTypeNone:		root->setAttributeString(_S("type"), _S("none"));
							break;

	case blockTypeHeading:	root->setAttributeString(_S("type"), _S("heading"));
							break;

	case blockTypeMinimal:	root->setAttributeString(_S("type"), _S("minimal"));
							break;

	case blockTypeSmall:	root->setAttributeString(_S("type"), _S("small"));
							break;

	case blockTypeBig:		root->setAttributeString(_S("type"), _S("big"));
							break;
	}	

	if(getInnerStyle().empty() == false)
		root->setAttributeString(_S("innerStyle"), getInnerStyle());	

	if(getOuterStyle().empty() == false)
		root->setAttributeString(_S("outerStyle"), getOuterStyle());	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
