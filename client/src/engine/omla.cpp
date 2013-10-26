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
#include "omla.h"

#include "entitiesentity.h"
#include "omlcontext.h"
#include "omlitem.h"
#include "ideskin.h"
#include "objectsiobject.h"
#include "objectsirevisionable.h"

#include "options.h"
#include "portalsportal.h"
#include "css.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLA::OMLA(const String& tag) : OMLHtmlWrapper(tag, _S("a"), false, String::EMPTY, String::EMPTY, String::EMPTY)
{	

}

OMLA::~OMLA()
{

}

String OMLA::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{	
	if(context->getMode() == omlRenderModeSearch) 
		return String::EMPTY;
	
	mapDefaultParamTo(i, _S("href"));	

	context->addUrl(i, i->getParam(_S("href")));
		
	return OMLHtmlWrapper::processHtml(i, context);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
