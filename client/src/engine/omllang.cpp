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
#include "omllang.h"

#include "omlcontext.h"
#include "omlitem.h"
#include "omlmanager.h"
#include "iidepage.h"
#include "languageculture.h"
#include "languagemanager.h"
#include "portalsaccount.h"
#include "idesession.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLLang::OMLLang(const String &tag) : IOMLCode(tag)
{	

}

OMLLang::~OMLLang()
{

}

bool OMLLang::allowRowMode() const
{
	return true;
}

String OMLLang::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{	
	mapDefaultParamTo(i, _S("culture"));

	if(context->getMode() == omlRenderModeSearch)	
	{		
		return i->getHtmlChildsPlainWithAttributes(context);		
	}	
	else
	{
		// Removed 0.14
		/*
		if(context->checkPortalPageAvailable(i) == false)
			return String::EMPTY;		
		*/
	
		/* Versione 1:
		[lang=it]italiano[/lang]
		[lang=en]inglese[/lang]
		*/

		if(i->hasParam(_S("culture")))
		{	
			// TODO: Non è il massimo, questo pezzo lo fa per ogni [lang] quando basterebbe solo il primo giro.

			int32 bestDepth = 100;
			String bestCulture;
			shared_ptr<OMLItem> parent = i->getParent();		
			
			for(OMLManager::Items::const_iterator i2 = parent->getChilds().begin(); i2 != parent->getChilds().end(); ++i2)
			{
				shared_ptr<OMLItem> child = *i2;
				mapDefaultParamTo(child, _S("culture"));
				
				if( (child->getTagName() == _S("lang")) && (!child->getSkipped()) )
				{
					if(i->hasParam(_S("culture")))
					{
						const String &culture = child->getParam(_S("culture"));
						int32 currentDepth = getCultureDepth(context, culture);
						if( (currentDepth != -1) && (currentDepth < bestDepth) )
						{
							bestDepth = currentDepth;
							bestCulture = culture;					
						}				
					}
					else
					{
						// Non  possibile mischiare le sintassi V1 e V2.
						i->setSkipped(true);
					}
				}			
			}

			for(OMLManager::Items::const_iterator i2 = parent->getChilds().begin(); i2 != parent->getChilds().end(); ++i2)
			{
				shared_ptr<OMLItem> child = *i2;
				if( (child->getTagName() == _S("lang")) && (!child->getSkipped()) )
				{
					const String &culture = child->getParam(_S("culture"));
					child->setSkipped(culture != bestCulture);				
				}			
			}

			if(i->getSkipped())
				return String::EMPTY;
			else
				return i->getHtmlChilds(context);
		}
		else
		{
			/* Versione 2:
			[lang it=italiano en=inglese /]
			*/
			String bestText = String::EMPTY;
			int32 bestDepth = 100;
			
			for(StringCollection<String>::iterator i3 = i->getParams().begin(); i3 != i->getParams().end(); ++i3)
			{
				String culture = i3->first;
				int32 currentDepth = getCultureDepth(context, culture);
				if( (currentDepth != -1) && (currentDepth < bestDepth) )
				{
					bestDepth = currentDepth;
					bestText = *i3->second;
				}		
			}

			return encodeOML(context, bestText);
		}	
	}
}

int32 OMLLang::getCultureDepth(shared_ptr<OMLContext> context, const String &required) const
{
	int32 depth = 0;

	OS_ASSERT(context->getPage() != null);
	String current = context->getPage()->getLanguage();	
	shared_ptr<LanguageCulture> culture = LanguageManager::instance()->getCulture(current);
	while(culture != null)
	{
		if(culture->getID() == required)
		{
			return depth;
		}
		else
		{
			culture = culture->getParent();
			depth++;
		}
	}
	
	return -1;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
