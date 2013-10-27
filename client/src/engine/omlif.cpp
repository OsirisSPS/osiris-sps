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
#include "omlif.h"

#include "conversions.h"
#include "omlcontext.h"
#include "omlitem.h"
#include "omlmanager.h"
#include "iidepage.h"
#include "iideportalpage.h"
#include "languageculture.h"
#include "languagemanager.h"
#include "portalsaccount.h"
#include "idesession.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLIf::OMLIf(const String &tag) : IOMLCode(tag)
{	

}

OMLIf::~OMLIf()
{

}

bool OMLIf::allowRowMode() const
{
	return false;
}

bool OMLIf::matchCondition(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context, const String &condition) const
{
	if(condition == _S("isis"))
	{
		if(context->getPage()->getRequestSource() == IPage::rsIsis)
			return true;
	} 
	else if(condition == _S("osiris"))
	{
		if(context->getPage()->getRequestSource() == IPage::rsOsiris)
			return true;
	}
	else if(condition == _S("guest"))
	{
		shared_ptr<IPortalPage> portalPage = context->getPortalPage();
		if(portalPage)
		{
			shared_ptr<IdeSession> session = portalPage->getSessionAccount();
			if(session)
			{
				if(session->isPortalGuest(portalPage->getDatabase()))
					return true;
			}
		}
	}
	else if(condition == _S("member"))
	{
		shared_ptr<IPortalPage> portalPage = context->getPortalPage();
		if(portalPage)
		{
			shared_ptr<IdeSession> session = portalPage->getSessionAccount();
			if(session)
			{
				if(session->isPortalGuest(portalPage->getDatabase()) == false)
					return true;
			}
		}
	}

	return false;
}

String OMLIf::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{	
	mapDefaultParamTo(i, _S("condition"));
	
	String condition = i->getParam(_S("condition"));
	bool message = true;
	if(i->hasParam(_S("message")))
		message = conversions::from_utf16<bool>(i->getParam(_S("message")));
		
	StringVector itemsCondition;
	utils::split(condition, _S(" "), itemsCondition);

	bool show = false;
	size_t nItems = itemsCondition.size();
	for(size_t iItem=0; iItem<nItems; iItem++)
	{
		String item = itemsCondition[iItem];

		if(matchCondition(i, context, item))
		{
			show = true;
			break;
		}
	}

	

	// Custom behavour:
	if(message)
	{
		if( (condition == _S("member")) && (show == false) )
			return encodeOML(context, getText(context, _S("oml.if.no_member")));			
		if( (condition == _S("osiris")) && (show == false) )
			return encodeOML(context, getText(context, _S("oml.if.no_osiris")));
	}


	if(show == false)
		return String::EMPTY;
	else
		return i->getHtmlChilds(context);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
