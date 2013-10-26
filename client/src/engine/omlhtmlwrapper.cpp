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

/*
*/

#include "stdafx.h"
#include "omlhtmlwrapper.h"

#include "omlcontext.h"
#include "omlitem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLHtmlWrapper::OMLHtmlWrapper(const String& tag, const String& htmlTag, bool allowedInRowMode, const String &parentTag, const String &childsTag, const String &mapDefaultParamTo) : IOMLCode(tag)
{	
	m_htmlTag = htmlTag;	
	m_parentTag = parentTag;
	m_childsTag = childsTag;
	m_allowedInRowMode = allowedInRowMode;
	m_mapDefaultParamTo = mapDefaultParamTo;
}

OMLHtmlWrapper::~OMLHtmlWrapper()
{
}

std::wstring OMLHtmlWrapper::onEncodeParam(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context, const std::wstring& name, const std::wstring& value) const
{
	if(name == _W("style")) return encodeToCss(context, i, value).to_wide();
	if( (name == _W("href")) || (name == _W("src")) )
	{
		// TODO: dovrei aggiungere l'attributo "target"?
		OMLRenderUrlDestination omlRenderUrlDestination = omlRenderUrlDestinationSame;
		if(name == _W("href")) return encodeUrl(context, i, value, omlRenderUrlDestination, false).to_wide();
		if(name == _W("src")) return encodeUrl(context, i, value, omlRenderUrlDestination, false).to_wide();
	}
	
	
	// Not allowed
	// Al posto di mettere una lista (onclick, onmousemove etc..) filtro in generale tutti quelli "on*".
	// Questo perch mi risulta standard che gli eventi inizino per 'on', e una lista sarebbe problematico tenerla aggiornata.
	std::wstring::size_type posOn = name.find(_W("on"));
	//if(name.starts_with(_S("on")))
	if( (posOn != std::wstring::npos) && (posOn == 0) )	
	{
		context->addWarningInvalidParam(i, name);
		return String::EMPTY.to_wide();
	}
	
	// Default
	return encodeToAttribute(context, value).to_wide();
}

bool OMLHtmlWrapper::allowRowMode() const
{
	return m_allowedInRowMode;
}

String OMLHtmlWrapper::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{
	if(!m_childsTag.empty())
		allowedChildsTags(context, i, m_childsTag);

	if(!m_parentTag.empty())
	{		
		if(!allowedParentTags(context, i, m_parentTag))
			return String::EMPTY;
	}

	if(!m_mapDefaultParamTo.empty())
	{
		mapDefaultParamTo(i, m_mapDefaultParamTo);
	}	
	i->removeParam(OMLItem::ITEM_PARAM_DEFAULT);

	if(context->getMode() == omlRenderModeSearch)
	{
		return i->getHtmlChildsPlainWithAttributes(context);
	}
	else
	{	
		String childsHtml = i->getHtmlChilds(context);
		String attributes;

		for(StringCollection<String>::iterator i3 = i->getParams().begin(); i3 != i->getParams().end(); ++i3)
		{
			std::wstring paramName = i3->first.to_wide();
			std::wstring paramValue = i3->second->to_wide();

			paramValue = onEncodeParam(i, context, paramName, paramValue);
			
			if(!paramValue.empty())
				attributes+=_S(" ") + paramName + _S("=\"") + paramValue + _S("\"");
		}

		// Decommentare per output XHTML
		//if(childsHtml.empty())
		if( (m_htmlTag == _S("img")) )
		{
			return _S("<") + m_htmlTag + attributes + _S(">");
		}
		else if( (m_htmlTag == _S("br")) || (m_htmlTag == _S("hr")) )
		{
			return _S("<") + m_htmlTag + attributes + _S(" />");
		}
		else
		{
			return _S("<") + m_htmlTag + attributes + _S(">") + childsHtml + _S("</") + m_htmlTag + _S(">");
		}		
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
