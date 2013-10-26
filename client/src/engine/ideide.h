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

#ifndef _IDE_IDE_H
#define _IDE_IDE_H

//////////////////////////////////////////////////////////////////////

#define OS_IDE_BLOCK_TYPE_NONE								0
#define OS_IDE_BLOCK_TYPE_HEADING							4
#define OS_IDE_BLOCK_TYPE_MINIMAL							3
#define OS_IDE_BLOCK_TYPE_SMALL								1
#define OS_IDE_BLOCK_TYPE_BIG								2

#define OS_IDE_SKIN_DEFAULT									_S("0000000000000000000000000000000000000000")
#define OS_IDE_SKIN_SCHEMA									_S("skin.xsd")

#define OS_XSL_NAMESPACE_DATE								_S("http://www.osiris-sps.org/xsl/date")
#define OS_XSL_NAMESPACE_LANG								_S("http://www.osiris-sps.org/xsl/lang")
#define OS_XSL_NAMESPACE_SYSTEM								_S("http://www.osiris-sps.org/xsl/system")

#define OS_PAGE_AREA_UNKNOWN								0
#define OS_PAGE_AREA_LEFT									1
#define OS_PAGE_AREA_TOP									2
#define OS_PAGE_AREA_RIGHT									3
#define OS_PAGE_AREA_BOTTOM									4
#define OS_PAGE_AREA_CONTENT_TOP							5
#define OS_PAGE_AREA_CONTENT_BOTTOM							6
#define OS_PAGE_AREA_CONTENT								7

#define OS_PAGE_AREANAME_UNKNOWN							_S("area_unknown")
#define OS_PAGE_AREANAME_LEFT								_S("area_left")
#define OS_PAGE_AREANAME_TOP								_S("area_top")
#define OS_PAGE_AREANAME_RIGHT								_S("area_right")
#define OS_PAGE_AREANAME_BOTTOM								_S("area_bottom")
#define OS_PAGE_AREANAME_CONTENT_TOP						_S("area_content_top")
#define OS_PAGE_AREANAME_CONTENT_BOTTOM						_S("area_content_bottom")
#define OS_PAGE_AREANAME_CONTENT							_S("area_content")

// Date functions
#define OS_XSL_FUNCTION_DATETIME_COMPARE					_S("compare")
#define OS_XSL_FUNCTION_DATETIME_CURRENTDATETIME			_S("current-datetime")
#define OS_XSL_FUNCTION_DATETIME_FORMATDATE					_S("format-date")
#define OS_XSL_FUNCTION_DATETIME_SHORTDATE					_S("short-date")
#define OS_XSL_FUNCTION_DATETIME_LONGDATE					_S("long-date")
#define OS_XSL_FUNCTION_DATETIME_SHORTTIME					_S("short-time")
#define OS_XSL_FUNCTION_DATETIME_LONGTIME					_S("long-time")
#define OS_XSL_FUNCTION_DATETIME_SHORTDATETIME				_S("short-datetime")
#define OS_XSL_FUNCTION_DATETIME_LONGDATETIME				_S("long-datetime")
#define OS_XSL_FUNCTION_DATETIME_USERFRIENDLYDATETIME		_S("userfriendly-datetime")
#define OS_XSL_FUNCTION_DATETIME_RFC822DATETIME				_S("rfc822-datetime")

// Language functions
#define OS_XSL_FUNCTION_LANGUAGE_TEXT						_S("text")
#define OS_XSL_FUNCTION_LANGUAGE_ENGLISHNAME				_S("english-name")
#define OS_XSL_FUNCTION_LANGUAGE_NATIVENAME					_S("native-name")

// System
#define OS_XSL_FUNCTION_SYSTEM_FORMATSIZE					_S("format-size")
#define OS_XSL_FUNCTION_SYSTEM_PARSE						_S("parse")
#define OS_XSL_FUNCTION_SYSTEM_PARSEREVERSE					_S("parse-reverse")
#define OS_XSL_FUNCTION_SYSTEM_JSENCODE						_S("js-encode")
#define OS_XSL_FUNCTION_SYSTEM_URLENCODE					_S("url-encode")
#define OS_XSL_FUNCTION_SYSTEM_URLDECODE					_S("url-decode")
#define OS_XSL_FUNCTION_SYSTEM_VERSIONMAJOR					_S("version-major")
#define OS_XSL_FUNCTION_SYSTEM_VERSIONMINOR					_S("version-minor")
#define OS_XSL_FUNCTION_SYSTEM_GETOPTION					_S("get-option")
#define OS_XSL_FUNCTION_SYSTEM_HTMLENCODE					_S("html-encode")
#define OS_XSL_FUNCTION_SYSTEM_HTMLDECODE					_S("html-decode")
#define OS_XSL_FUNCTION_SYSTEM_HELPLINK						_S("help-link")
#define OS_XSL_FUNCTION_SYSTEM_REFERENCEDATE				_S("reference-date")
#define OS_XSL_FUNCTION_SYSTEM_RESOURCEURL					_S("resource-url")
#define OS_XSL_FUNCTION_SYSTEM_REPLACE						_S("replace")

// Misc
#define OS_EMPTY_NAME										_S("empty")

// Commons Controls defaults values
#define OS_CONTROLS_TITLE_SIZE								50
#define OS_CONTROLS_TITLE_MAXLENGTH							128
#define OS_CONTROLS_DESCRIPTION_SIZE						50
#define OS_CONTROLS_DESCRIPTION_MAXLENGTH					-1

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

// Enumerated types

enum OMLRenderMode
{
	omlRenderModeOsiris		= 0,
	omlRenderModeSearch		= 1,
	omlRenderModeExternal	= 2,
};

enum OMLRenderUrlDestination
{
	omlRenderUrlDestinationSame			= 0,
	omlRenderUrlDestinationNewWindow	= 1,
};

enum BlockType
{
	blockTypeNone		= OS_IDE_BLOCK_TYPE_NONE,
	blockTypeHeading	= OS_IDE_BLOCK_TYPE_HEADING,
	blockTypeMinimal	= OS_IDE_BLOCK_TYPE_MINIMAL,
	blockTypeSmall		= OS_IDE_BLOCK_TYPE_SMALL,
	blockTypeBig		= OS_IDE_BLOCK_TYPE_BIG,
};

enum PageArea
{
	pageAreaUnknown			= OS_PAGE_AREA_UNKNOWN,
	pageAreaLeft			= OS_PAGE_AREA_LEFT,
	pageAreaTop				= OS_PAGE_AREA_TOP,
	pageAreaRight			= OS_PAGE_AREA_RIGHT,
	pageAreaBottom			= OS_PAGE_AREA_BOTTOM,
	pageAreaContentTop		= OS_PAGE_AREA_CONTENT_TOP,
	pageAreaContentBottom	= OS_PAGE_AREA_CONTENT_BOTTOM,
	pageAreaContent			= OS_PAGE_AREA_CONTENT,
};

//////////////////////////////////////////////////////////////////////

namespace ide {

//////////////////////////////////////////////////////////////////////

enum FormatFlags
{
	ffNone				= 0,
	ffSingleLine		= 1 << 0,
	ffTrim				= 1 << 1,
	ffNoBlank			= 1 << 2,
};

namespace edit
{
	// Interfaces
	class IObjectEditor;

	// Classes
	class CalendarEventEditor;
	class FileEditor;
	class ForumEditor;
	class InstanceEditor;
	class ModelEditor;
	class PollEditor;
	class PollOptionEditor;
	class PostEditor;
	class SectionEditor;
	class TagEditor;
	class TextEditor;
	class TopicEditor;

	// Types
	typedef shared_ptr<IObjectEditor> editor_ptr;
};

namespace functions
{
	// Interfaces
	class IFunction;

	// Classes
	class CompareDates;
	class CurrentDateTime;
	class EnglishName;
	class FormatDate;
	class FormatSize;
	class GetOption;
	class HelpLink;
	class HtmlEncode;
	class HtmlDecode;
	class JSEncode;
	class UrlEncode;
	class UrlDecode;
	class LongDate;
	class LongDateTime;
	class LongTime;
	class NativeName;
	class Parse;
	class PortalLink;	
	class ReferenceDate;
	class ResourceUrl;
	class Replace;
	class RFC822DateTime;		
	class ShortDate;
	class ShortDateTime;
	class ShortTime;
	class Text;
	class UserFriendlyDateTime;	
	class Version;	
};

namespace portal
{
	// Enumerated types
	enum PortalPage
	{
		ppUnknown			= 0,
		ppAccount			= 1,
		ppEdit				= 2,
		ppUser				= 3,
		ppUsers				= 4,
		ppView				= 5,		
		ppObjectDetail		= 6,
		ppAssistant			= 7,
		ppSearch			= 8,
		ppMessenger			= 9,
	};

	// Classes
	class Account;
	class Browse;
	class Changes;
	class Edit;
	class User;
	class Users;
	class View;
}

namespace main
{
	// Enumerated types
	enum MainPage
	{
		mpUnknown			= 0,
		mpAccounts			= 1,
		mpSettings			= 2,
		mpResources			= 3,
		mpMaintenance		= 4,
		mpSubscribe			= 5,
		mpIsis				= 6,		
		mpJobs				= 7,
		mpImport			= 8,
		mpExport			= 9,
		mpOptions			= 10,
		mpJobDetail			= 11,				
	};

	// Classes
	class Accounts;
	class Settings;
	class Resources;
}

namespace view
{
	// Interfaces
	class IObjectViewer;

	// Classes
	class CalendarEventViewer;
	class CommentableObjectViewer;
	class ForumViewer;
	class InstanceViewer;
	class ModelViewer;
	class PollViewer;
	class PollOptionViewer;
	class PostViewer;
	class SectionViewer;
	class TagViewer;
	class TextViewer;

	// Types
	typedef shared_ptr<IObjectViewer> viewer_ptr;
}

//////////////////////////////////////////////////////////////////////

// API
EngineExport String formatString(const String &str, uint32 flags);
EngineExport String encodeString(const String &str, uint32 flags = ffNone);

EngineExport String formatObjectTitle(const String &title);

//////////////////////////////////////////////////////////////////////

} // ide

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_IDE_H
