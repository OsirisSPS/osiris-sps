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
#include "omlquote.h"

#include "omlcontext.h"
#include "omlitem.h"
#include "idesystem.h"
#include "iidepage.h"
#include "iideportalpage.h"
#include "ideskin.h"
#include "objectsirevisionable.h"

#include "objectsuser.h"
#include "portalsportal.h"
#include "entitiesentity.h"
#include "css.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLQuote::OMLQuote(const String& tag) : IOMLCode(tag)
{	
}

OMLQuote::~OMLQuote()
{

}

String OMLQuote::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{	
	mapDefaultParamTo(i, _S("title"));

	String title = encodeOML(context, i->getParam(_S("title")));

	if(context->getMode() != omlRenderModeSearch)
	{			
		if(i->hasParam(_S("id")))
		{
			shared_ptr<const IPortalPage> page = context->getPortalPage();
			if(page != nullptr)
			{
				String id = i->getParam(_S("id"));
				shared_ptr<EntitiesEntity> entity = page->getPortal()->getEntity(page->getDatabase(), id.to_ascii());
				if(entity != nullptr)
				{
					shared_ptr<ObjectsIRevisionable> current = entity->getCurrent();
					if(current != nullptr)
					{
						if(current->isVisible())
						{
							shared_ptr<ObjectsUser> author = objects_user_cast(page->getObject(current->getAuthor()));

							String authorNick = encode(context, author->name);
							String itemDate = encode(context, context->getPage()->formatDate(current->submit_date, DateTime::dpUserFriendly));

							String imageLinkUrl = context->getPage()->getSkin()->getResourceUrl(_S("images/oml/quote_link.gif"));
							imageLinkUrl = encode(context, imageLinkUrl);
							
							if(!title.empty())
								title += _S(" ");
							String entryHref = encode(context, entity->getViewLink(page->getPortal()));
							title += String::format(_S("(%S @ %S)<a href=\"%S\"><img src=\"%S\"></a>").c_str(), authorNick.c_str(), itemDate.c_str(), entryHref.c_str(), imageLinkUrl.c_str());
						}
						else
						{
							title += getText(context, _S("oml.quote.messages.hidden_id"));
						}
					}
					else
					{
						title += getText(context, _S("oml.quote.messages.skipped_id"));
					}
				}
				else
				{
					title += getText(context, _S("oml.quote.messages.unknown_id"));
				}
			}
		}	
	}

	String body = i->getHtmlChilds(context);
	
	if(context->getMode() == omlRenderModeSearch)
	{
		return _S(" ") + title + _S(" ") + body + _S(" ");
	}
	else
	{
		return _S("<div class=\"") + OS_CSS_OML_QUOTETOP + _S("\">") + title + _S("</div><div class=\"") + OS_CSS_OML_QUOTEMAIN + _S("\">") + body + _S("</div>");
	}
}

String OMLQuote::quoteObject(const EntityID &id, const String &content)
{
	OS_ASSERT(id.empty() == false);
	return String::format(_S("[quote id=%S]%S[/quote]").c_str(), id.toUTF16().c_str(), content.c_str());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
