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
-----------------------------------------------------------------------------
Un tag 'replace':

- Pu avere un solo parametro, ma  facoltativo
- Pu contenere figli

Nel costruttore viene passata una stringa di formattazione, che sostituisce il tag. 
I figli vanno al posto di $1, l'eventuale parametro al posto di $p.

-----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "omlreplace.h"

#include "omlcontext.h"
#include "omlitem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLReplace::OMLReplace(const String& tag, int32 minParams, int32 maxParams, const String &replaceHtml, const String &replacePlain, const bool allowedInRowMode, const String &mapDefaultParamTo) : IOMLCode(tag)
{	
	m_minParams = minParams;
	m_maxParams = maxParams;
	m_replaceHtml = replaceHtml;
	m_replacePlain = replacePlain;	
	m_allowedInRowMode = allowedInRowMode;
	m_mapDefaultParamTo = mapDefaultParamTo;
}

OMLReplace::~OMLReplace()
{
}

bool OMLReplace::allowRowMode() const
{
	return m_allowedInRowMode;
}

String OMLReplace::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{
	if( (m_maxParams == 0) && (i->hasParams()) )
	{
		context->addWarningCantHaveParams(i);		
	}

	if( (m_minParams == 1) && (!i->hasParams()) )
	{
		context->addWarningParamRequired(i);
		return String::EMPTY;
	}

	if(!m_mapDefaultParamTo.empty())
	{
		mapDefaultParamTo(i, m_mapDefaultParamTo);
	}	

	String childsHtml = i->getHtmlChilds(context);

	String out;
	if(context->getMode() == omlRenderModeSearch)
		out = m_replacePlain;		
	else
		out = m_replaceHtml;
	
	out = out.replace_all(_S("{$body}"), childsHtml);
	for(StringCollection<String>::iterator i3 = i->getParams().begin(); i3 != i->getParams().end(); ++i3)
	{
		String paramName = i3->first;
		String paramValue = *i3->second;

		out = out.replace_all(_S("{$") + paramName + _S("}"), encodeToAttribute(context, paramValue));
	}

	// 11/05/2009 - Da abolire insieme alle varie "replace", in favore di classi specifiche.
	out = out.replace_all(_S("{$title}"), String::EMPTY);
	
	return out;
}


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
