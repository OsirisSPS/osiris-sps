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
#include "omlsize.h"

#include "convert.h"
#include "conversions.h"
#include "omlcontext.h"
#include "omlitem.h"
#include "idesystem.h"
#include "unicode.h"
#include "css.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLSize::OMLSize(const String& tag) : IOMLCode(tag)
{	
}

OMLSize::~OMLSize()
{

}

String OMLSize::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{	
	//addReplace(_S("size"), 1, 1, _S("<span class=\"") OS_CSS_OML_SIZE _S("\" style=\"font-size:{$default}\">{$body}</span>"), _S(" {$body} "));

	String body = i->getHtmlChilds(context);
	if(context->getMode() == omlRenderModeSearch)
		return body;
	
	String value = i->getParam(OMLItem::ITEM_PARAM_DEFAULT);

	/*
	// 0.12
	// Per compatibilit con la sintassi dei bbcodes di altri forum, se  un numero,  inteso come percentuale.
	
	value.trim();
	if(conversions::to_utf16(conversions::from_utf16<int32>(value)) == value)
		value += _S("%");
	*/

	// 0.13
	// Se è un numero, è inteso come lo tratta Invision, vBulletin, SMF. No phpBB che ha una logica tutta sua.
	value.trim();
	if(value == _S("1"))
		value = _S("11px");
	else if(value == _S("2"))
		value = _S("13px");
	else if(value == _S("3"))
		value = _S("16px");
	else if(value == _S("4"))
		value = _S("19px");
	else if(value == _S("5"))
		value = _S("24px");
	else if(value == _S("6"))
		value = _S("32px");
	else if(value == _S("7"))
		value = _S("48px");
	
	return _S("<span class=\"") + OS_CSS_OML_SIZE + _S("\" style=\"font-size:") + encodeToAttribute(context, value) + _S("\">") + body + _S("</span>");
}


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
