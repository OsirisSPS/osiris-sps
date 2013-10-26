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
#include "idesearchparam.h"

#include "ideblock.h"
#include "css.h"
#include "htmlcheckbox.h"
#include "htmllink.h"
#include "htmlliteral.h"
#include "htmltable.h"
#include "htmltablecell.h"
#include "htmltablerow.h"
#include "htmltextbox.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdeSearchParam::IdeSearchParam(shared_ptr<ISearchOptions> options) : m_active(OS_NEW HtmlCheckBox())
{
	m_options = options;
}

IdeSearchParam::~IdeSearchParam()
{

}

void IdeSearchParam::onLoad()
{
	ControlBase::onLoad();	

	shared_ptr<IdeBlock> block(OS_NEW IdeBlock(_S("Parametri ricerca...")));
	getControls()->add(block);

	m_active->setCaption(_S("active"));
	m_active->setID(_S("active"));
	block->getBody()->getControls()->add(m_active);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
