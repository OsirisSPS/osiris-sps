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
#include "omlimg.h"

#include "entitiesentity.h"
#include "omlcontext.h"
#include "omlitem.h"
#include "iideportalpage.h"
#include "ideskin.h"
#include "objectsiobject.h"
#include "objectsirevisionable.h"

#include "options.h"
#include "idesession.h"
#include "portalsportal.h"
#include "css.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLImg::OMLImg(const String& tag) : OMLHtmlWrapper(tag, _S("img"), false, String::EMPTY, String::EMPTY, String::EMPTY)
{

}

OMLImg::~OMLImg()
{

}

String OMLImg::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{
	if(context->getMode() == omlRenderModeSearch) 
		return String::EMPTY;

	// Removed 0.14
	/*
	if(context->checkPortalPageAvailable(i) == false) 
		return String::EMPTY;
	*/

	mapDefaultParamTo(i, _S("src"));

	String src = i->getParam(_S("src"));
	if(src.empty())
		src = i->getSingleText();

	removeChilds(i);

	if(!i->hasParam(_S("alt")))
		i->setParam(_S("alt"), getText(context, _S("oml.img.alt")));

	if( (!i->hasParam(_S("alt"))) && (i->hasParam(_S("title"))) )
		i->setParam(_S("alt"), i->getParam(_S("title")));

	if( (!i->hasParam(_S("title"))) && (i->hasParam(_S("alt"))) )
		i->setParam(_S("title"), i->getParam(_S("alt")));

	// Converto il parametro 'src'
	String entityID;
	src = encodeResourceUrlEx(context, i, src, entityID);
	if(entityID.empty() == false)
		i->setParam(_S("data-os-entity"), entityID);

	i->setParam(_S("src"), src);

	if( (context->getMode() == omlRenderModeOsiris) && ( (i->hasParam(_S("tsrc"))) || (i->hasParam(_S("twidth"))) || (i->hasParam(_S("theight"))) ) )
	{
		// Lightbox mode
		if(i->hasParam(_S("tsrc")))
		{
			String tsrc = i->getParam(_S("tsrc"));
			tsrc = encodeResourceUrl(context, i, tsrc);
			i->setParam(_S("src"), tsrc);
			i->removeParam(_S("tsrc"));
		}
		if(i->hasParam(_S("twidth")))
		{
			i->setParam(_S("width"), i->getParam(_S("twidth")));
			i->removeParam(_S("twidth"));
		}
		if(i->hasParam(_S("theight")))
		{
			i->setParam(_S("height"), i->getParam(_S("theight")));
			i->removeParam(_S("theight"));
		}

		String imageSet = context->getContextID();
		if(i->hasParam(_S("imageset")))
			imageSet = i->getParam(_S("imageset"));
		
		return _S("<a rel=\"lightbox[") + encode(context, imageSet) + _S("]\" href=\"") + src + _S("\">") + OMLHtmlWrapper::processHtml(i, context) + _S("</a>");
	}
	else
	{
		return OMLHtmlWrapper::processHtml(i, context);
	}
}

std::wstring OMLImg::onEncodeParam(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context, const std::wstring& name, const std::wstring& value) const
{
	// Encoding html. La faccio qui per evitare che la base chiami la encodeUrl che trasformerebbe l'url con chiamata
	// alla pagina di redirect/external.	
	if(name == _W("src"))
		return encode(context, value).to_wide();

	return OMLHtmlWrapper::onEncodeParam(i, context, name, value);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
