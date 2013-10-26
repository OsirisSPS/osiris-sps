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
#include "omldebug.h"

#include "convert.h"
#include "conversions.h"
#include "omlcontext.h"
#include "omlitem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLDebug::OMLDebug(const String& tag) : IOMLCode(tag)
{	
}

OMLDebug::~OMLDebug()
{

}

String OMLDebug::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{	
	if(context->getMode() == omlRenderModeSearch)
		return String::EMPTY;

	if(i->hasParam(_S("mode")))
		context->setMode((OMLRenderMode) conversions::from_utf16<uint32>(i->getParam(_S("mode"))));
	if(i->hasParam(_S("row")))
		context->setRow(conversions::from_utf16<bool>(i->getParam(_S("row"))));
	if(i->hasParam(_S("secure")))
		context->setSecure(conversions::from_utf16<bool>(i->getParam(_S("secure"))));
	if(i->hasParam(_S("full")))
		context->setSecure(conversions::from_utf16<bool>(i->getParam(_S("full"))));

	return String::EMPTY;	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
