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
#include "omllist.h"

#include "omlcontext.h"
#include "omlitem.h"
#include "css.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLList::OMLList(const String& tag) : IOMLCode(tag)
{	
}

OMLList::~OMLList()
{
}

String OMLList::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{		
	mapDefaultParamTo(i, _S("mode"));

	allowedChildsTags(context, i, _S("li;*"));
	/*
	// Toglie i figli diversi da [li] e [*] 
	for(Items::const_iterator i2 = i->getChilds().begin(); i2 != i->getChilds().end(); ++i2)
	{
		shared_ptr<OMLItem> child = *i2;
		if( (child->getTagName() != _S("li")) && (child->getTagName() != _S("*")) )
		{
			context->addWarningCantBeHere(child,i);				
			child->clear();
		}			
	}
	*/

	String mode = i->getParam(_S("mode"));
	if(mode == String::EMPTY)
	{
		return _S("<ul class=\"") + OS_CSS_OML_LIST_UL + _S("\">") + i->getHtmlChilds(context) + _S("</ul>");
	}
	else
	{
		String listStyle;
		if(mode == _S("1")) listStyle = _S("decimal");
		else if(mode == _S("a")) listStyle = _S("lower-alpha");
		else if(mode == _S("A")) listStyle = _S("upper-alpha");
		else if(mode == _S("i")) listStyle = _S("lower-roman");
		else if(mode == _S("I")) listStyle = _S("upper-roman");
		else
		{
			// Warning
			context->addWarning(_S("List mode not supported."));
			return String::EMPTY;
		}
		return _S("<ol class=\"") + OS_CSS_OML_LIST_OL + _S("\" style=\"list-style-type:") + listStyle + _S("\">") + i->getHtmlChilds(context) + _S("</ol>");
	}	
}


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
