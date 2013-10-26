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
#include "omldate.h"

#include "omlcontext.h"
#include "omlitem.h"
#include "idesystem.h"
#include "iidepage.h"
#include "httpparser.h"
#include "unicode.h"
#include "css.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLDate::OMLDate(const String& tag) : IOMLCode(tag)
{	
}

OMLDate::~OMLDate()
{

}

String OMLDate::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{	
	mapDefaultParamTo(i, _S("mode"));	

	String strDate = i->getHtmlChilds(context);
	String mode = i->getParam(_S("mode"));

	if(context->getMode() == omlRenderModeSearch)
	{
		return strDate;
	}
	else
	{
		DateTime dt = DateTime::now();
		bool result = false;
		bool showTime = false;
		bool showDate = false;
		size_t len = strDate.length();

		if(len==10)
		{
			// Scan only date
			int32 year = 0, month = 0, day = 0;
			
			showDate = true;
			result = (strDate.scanf(_S("%d-%d-%dT%d:%d:%d").c_str(), &year, &month, &day) == 3);
			if(result)
				dt.setDate(year, month, day);
		}
		else if(len==8)
		{
			// Scan only time
			int32 hours = 0, minutes = 0, seconds = 0;

			showTime = true;
			result = (strDate.scanf(_S("%d:%d:%d").c_str(), &hours, &minutes, &seconds) == 3);
			if(result)
				dt.setTime(hours, minutes, seconds);
		}
		else 
		{
			showTime = true;
			showDate = true;
			boost::posix_time::ptime time;
			result = HttpParser::parseDate(strDate.to_ascii(), time);
			if(result)
				dt.fromPosixTime(time);
		}

		if(!result)
		{
			context->addWarning(_S("Tag [date] have an unknown format date."));
		}
		else
		{
			bool use_pattern = true;
			DateTime::DateTimePattern pattern;
			if(mode == String::EMPTY)
			{
				if( (showDate) && (showTime) )
					pattern = DateTime::dpLongDateTimePattern;
				else if (showDate)
					pattern = DateTime::dpLongDatePattern;
				else
					pattern = DateTime::dpLongTimePattern;
			}
			else if(mode == _S("friendly"))
				pattern = DateTime::dpUserFriendly;
			else if(mode == _S("long_date"))
				pattern = DateTime::dpLongDatePattern;
			else if(mode == _S("long_time"))
				pattern = DateTime::dpLongTimePattern;
			else if(mode == _S("long_datetime"))
				pattern = DateTime::dpLongDateTimePattern;
			else if(mode == _S("short_date"))
				pattern = DateTime::dpShortDatePattern;
			else if(mode == _S("short_time"))
				pattern = DateTime::dpShortTimePattern;
			else if(mode == _S("short_datetime"))
				pattern = DateTime::dpShortDateTimePattern;
			else if(mode == _S("RFC822"))
				pattern = DateTime::dpRFC822;
			else
				use_pattern = false;

			String output;
			if(use_pattern)
			{			
				output = context->getPage()->formatDate(dt, pattern);
			}
			else
			{
				output = context->getPage()->formatDate(dt, mode);
			}		

			return output;
		}	

		return String::EMPTY;
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
