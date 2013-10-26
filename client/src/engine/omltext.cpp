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
#include "omltext.h"

#include "conversions.h"
#include "log.h"
#include "omlcontext.h"
#include "omlitem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLText::OMLText(const String& tag) : IOMLCode(tag)
{	
}

OMLText::~OMLText()
{

}

bool OMLText::allowRowMode() const
{
	return true;
}

String OMLText::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{
	// Da notare che gli emoticons sono permessi in RowMode.

	String text = i->getParam(OMLItem::ITEM_PARAM_TEXT);
	
	// Conversione 	
	return encodeBody(context, text, i->getPostProcess(), false);
}


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
