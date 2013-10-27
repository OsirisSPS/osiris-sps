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

#ifndef _SEARCH_SEARCH_H
#define _SEARCH_SEARCH_H

//////////////////////////////////////////////////////////////////////

#define OS_SEARCH_FIELD_ID				_S("id")
#define OS_SEARCH_FIELD_CURRENT			_S("current")
#define OS_SEARCH_FIELD_TITLE			_S("title")
#define OS_SEARCH_FIELD_CONTENT			_S("content")
#define OS_SEARCH_FIELD_PARENT			_S("parent")
#define OS_SEARCH_FIELD_SUBMITDATE		_S("submit_date")
#define OS_SEARCH_FIELD_INSERTDATE		_S("insert_date")
#define OS_SEARCH_FIELD_SCORE			_S("score")
#define OS_SEARCH_FIELD_VISIBLE			_S("visible")
#define OS_SEARCH_FIELD_SECTION			_S("section")

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

// Enumerated types
enum SearchTextMode
{
	searchTextModeAnyWords		= 0,
	searchTextModeAllWords		= 1,
	searchTextModeExactPhrase	= 2,
};

enum SearchTextWhere
{
	searchTextWhereAllFields	= 0,
	searchTextWhereOnlyTitle	= 1,
	searchTextWhereCustom		= 2,
};

enum SearchOrderMode
{
	searchOrderModeRelevance	= 0,
	searchOrderModeReputation	= 1,
	searchOrderModeSubmitDate	= 2,
	searchOrderModeInsertDate	= 3,
	searchOrderModePosition		= 4,
	searchOrderModeTitle		= 5,
	searchOrderModeLastObject	= 6,
	searchOrderModeVotesTotal	= 7,
	searchOrderModeVotesAverage	= 8,	
};

enum SearchOrderDirection
{
	searchOrderDirectionDescendent	= 0,
	searchOrderDirectionAscendent	= 1,
};

enum SearchDateTime	
{
	searchDateTimeNotSpecified		= 0,
	searchDateTimeSpecifiedDate		= 1,
	searchDateTimeNow				= 2,
	searchDateTimeReference			= 3,
	searchDateTimeLast24Hour		= 4,
	searchDateTimeLastWeek			= 5,
	searchDateTimeLastTwoWeek		= 6,
	searchDateTimeLastMonth			= 7,
	searchDateTimeLastThreeMonth	= 8,
	searchDateTimeLastYear			= 9,	
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _SEARCH_SEARCH_H
