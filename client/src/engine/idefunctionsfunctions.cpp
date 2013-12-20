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
#include "idefunctionsfunctions.h"

#include "conversions.h"
#include "datetime.h"
#include "engine.h"
#include "htmlparser.h"
#include "httpparser.h"
#include "omlmanager.h"
#include "ideide.h"
#include "iidepage.h"
#include "iideportalpage.h"
#include "idesession.h"
#include "ideskin.h"
#include "languageculture.h"
#include "languagemanager.h"
#include "log.h"
#include "options.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace functions {

//////////////////////////////////////////////////////////////////////

IFunction::IFunction(shared_ptr<IPage> page, const String &name, const String &ns, uint32 required_args, uint32 optionals_args) : FunctionBase(name, ns, required_args, optionals_args)
{
	m_page = page;
}

IFunction::~IFunction()
{

}

//////////////////////////////////////////////////////////////////////

CompareDates::CompareDates(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_DATETIME_COMPARE, OS_XSL_NAMESPACE_DATE, 2)
{

}

CompareDates::~CompareDates()
{

}

bool CompareDates::execute(IContext &context, const Arguments &args, DataItem &result)
{
	DateTime dt1;
	if(dt1.fromXML(args[0]) == false)
		return false;

	DateTime dt2;
	if(dt2.fromXML(args[1]) == false)
		return false;

	if(dt1 < dt2)
		result = static_cast<int32>(-1);
	else if(dt1 == dt2)
		result = static_cast<int32>(0);
	else
		result = static_cast<int32>(1);

	return true;
}

//////////////////////////////////////////////////////////////////////

CurrentDateTime::CurrentDateTime(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_DATETIME_CURRENTDATETIME, OS_XSL_NAMESPACE_DATE)
{

}

CurrentDateTime::~CurrentDateTime()
{

}

bool CurrentDateTime::execute(IContext &context, const Arguments &args, DataItem &result)
{
	result = DateTime::now().toXML();
	return true;
}

//////////////////////////////////////////////////////////////////////

FormatDate::FormatDate(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_DATETIME_FORMATDATE, OS_XSL_NAMESPACE_DATE, 2)
{

}

FormatDate::~FormatDate()
{

}

bool FormatDate::execute(IContext &context, const Arguments &args, DataItem &result)
{
	DateTime dt;
	if(dt.fromXML(args[0]) == false)
		return false;

	result = getPage()->formatDate(dt, args[1]);
	return true;
}

//////////////////////////////////////////////////////////////////////

FormatSize::FormatSize(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_SYSTEM_FORMATSIZE, OS_XSL_NAMESPACE_SYSTEM, 1)
{

}

FormatSize::~FormatSize()
{

}

bool FormatSize::execute(IContext &context, const Arguments &args, DataItem &result)
{
	result = utils::formatSize(args[0]);
	return true;
}

//////////////////////////////////////////////////////////////////////

ShortDate::ShortDate(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_DATETIME_SHORTDATE, OS_XSL_NAMESPACE_DATE, 1)
{

}

ShortDate::~ShortDate()
{

}

bool ShortDate::execute(IContext &context, const Arguments &args, DataItem &result)
{
	DateTime dt;
	if(dt.fromXML(args[0]) == false)
		return false;

	result = getPage()->formatDate(dt, DateTime::dpShortDatePattern);

	return true;
}

//////////////////////////////////////////////////////////////////////

LongDate::LongDate(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_DATETIME_LONGDATE, OS_XSL_NAMESPACE_DATE, 1)
{

}

LongDate::~LongDate()
{

}

bool LongDate::execute(IContext &context, const Arguments &args, DataItem &result)
{
	DateTime dt;
	if(dt.fromXML(args[0]) == false)
		return false;

	result = getPage()->formatDate(dt, DateTime::dpLongDatePattern);
	return true;
}

//////////////////////////////////////////////////////////////////////

ShortTime::ShortTime(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_DATETIME_SHORTTIME, OS_XSL_NAMESPACE_DATE, 1)
{

}

ShortTime::~ShortTime()
{

}

bool ShortTime::execute(IContext &context, const Arguments &args, DataItem &result)
{
	DateTime dt;
	if(dt.fromXML(args[0]) == false)
		return false;

	result = getPage()->formatDate(dt, DateTime::dpShortTimePattern);
	return true;
}

//////////////////////////////////////////////////////////////////////

LongTime::LongTime(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_DATETIME_LONGTIME, OS_XSL_NAMESPACE_DATE, 1)
{

}

LongTime::~LongTime()
{

}

bool LongTime::execute(IContext &context, const Arguments &args, DataItem &result)
{
	DateTime dt;
	if(dt.fromXML(args[0]) == false)
		return false;

	result = getPage()->formatDate(dt, DateTime::dpLongTimePattern);
	return true;
}

//////////////////////////////////////////////////////////////////////

ShortDateTime::ShortDateTime(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_DATETIME_SHORTDATETIME, OS_XSL_NAMESPACE_DATE, 1)
{

}

ShortDateTime::~ShortDateTime()
{

}

bool ShortDateTime::execute(IContext &context, const Arguments &args, DataItem &result)
{
	DateTime dt;
	if(dt.fromXML(args[0]) == false)
		return false;

	result = getPage()->formatDate(dt, DateTime::dpShortDateTimePattern);
	return true;
}

//////////////////////////////////////////////////////////////////////

LongDateTime::LongDateTime(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_DATETIME_LONGDATETIME, OS_XSL_NAMESPACE_DATE, 1)
{

}

LongDateTime::~LongDateTime()
{

}

bool LongDateTime::execute(IContext &context, const Arguments &args, DataItem &result)
{
	DateTime dt;
	if(dt.fromXML(args[0]) == false)
		return false;

	result = getPage()->formatDate(dt, DateTime::dpLongDateTimePattern);
	return true;
}

//////////////////////////////////////////////////////////////////////

UserFriendlyDateTime::UserFriendlyDateTime(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_DATETIME_USERFRIENDLYDATETIME, OS_XSL_NAMESPACE_DATE, 1)
{

}

UserFriendlyDateTime::~UserFriendlyDateTime()
{

}

bool UserFriendlyDateTime::execute(IContext &context, const Arguments &args, DataItem &result)
{
	DateTime dt;
	if(dt.fromXML(args[0]) == false)
		return false;

	result = getPage()->formatDate(dt, DateTime::dpUserFriendly);
	return true;
}

//////////////////////////////////////////////////////////////////////

RFC822DateTime::RFC822DateTime(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_DATETIME_RFC822DATETIME, OS_XSL_NAMESPACE_DATE, 1)
{

}

RFC822DateTime::~RFC822DateTime()
{

}

bool RFC822DateTime::execute(IContext &context, const Arguments &args, DataItem &result)
{
	DateTime dt;
	if(dt.fromXML(args[0]) == false)
		return false;

	result = getPage()->formatDate(dt, DateTime::dpRFC822);
	return true;
}

//////////////////////////////////////////////////////////////////////

Parse::Parse(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_SYSTEM_PARSE, OS_XSL_NAMESPACE_SYSTEM, 1, 6)
{

}

Parse::~Parse()
{

}

bool Parse::execute(IContext &context, const Arguments &args, DataItem &result)
{
	// In vista di semplificare codice in giro, farei una args->get(int32 index, DataItem def);
	// Questa diventa:
	// result = getPage()->parse(args[0], args->get(1, omHtml);

	bool full = true;
	if(args.size() > 1)
		full = static_cast<bool>(args[1]);

	bool secure = false;
	if(args.size() > 2)
		secure = static_cast<bool>(args[2]);

	bool row = false;
	if(args.size() > 3)
		row = static_cast<bool>(args[3]);

	uint32 mode = 0;
	if(args.size() > 4)
		mode = static_cast<uint32>(args[4]);

	String refID;
	if(args.size() > 5)
		refID = static_cast<String>(args[5]);

	String fullUrl;
	if(args.size() > 6)
		fullUrl = static_cast<String>(args[6]);
	
	result = getPage()->parseOml(args[0], full, secure, row, (OMLRenderMode) mode, refID, fullUrl);
	return true;
}

//////////////////////////////////////////////////////////////////////

ParseReverse::ParseReverse(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_SYSTEM_PARSEREVERSE, OS_XSL_NAMESPACE_SYSTEM, 1, 1)
{

}

ParseReverse::~ParseReverse()
{

}

bool ParseReverse::execute(IContext &context, const Arguments &args, DataItem &result)
{
	if(args.size() > 1)
		result = OMLManager::instance()->reverseParse(args[0], String(args[1]).to_ascii());
	else
		result = OMLManager::instance()->reverseParse(args[0], "html");
	return true;
}

//////////////////////////////////////////////////////////////////////

VersionMajor::VersionMajor(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_SYSTEM_VERSIONMAJOR, OS_XSL_NAMESPACE_SYSTEM)
{

}

VersionMajor::~VersionMajor()
{

}

bool VersionMajor::execute(IContext &context, const Arguments &args, DataItem &result)
{
	result = Engine::getVersionMajor();
	return true;
}

//////////////////////////////////////////////////////////////////////

VersionMinor::VersionMinor(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_SYSTEM_VERSIONMINOR, OS_XSL_NAMESPACE_SYSTEM)
{

}

VersionMinor::~VersionMinor()
{

}

bool VersionMinor::execute(IContext &context, const Arguments &args, DataItem &result)
{
	result = Engine::getVersionMinor();
	return true;
}

//////////////////////////////////////////////////////////////////////

GetOption::GetOption(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_SYSTEM_GETOPTION, OS_XSL_NAMESPACE_SYSTEM, 1)
{

}

GetOption::~GetOption()
{

}

bool GetOption::execute(IContext &context, const Arguments &args, DataItem &result)
{
	//result = Options::instance()->getOption(args[0]);
	result = getPage()->getOption(args[0]);
	return true;
}

//////////////////////////////////////////////////////////////////////

HtmlEncode::HtmlEncode(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_SYSTEM_HTMLENCODE, OS_XSL_NAMESPACE_SYSTEM, 1, 1)
{

}

HtmlEncode::~HtmlEncode()
{

}

bool HtmlEncode::execute(IContext &context, const Arguments &args, DataItem &result)
{
	result = HtmlParser::instance()->encodeEx(args[0]);
	return true;
}

//////////////////////////////////////////////////////////////////////

HtmlDecode::HtmlDecode(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_SYSTEM_HTMLDECODE, OS_XSL_NAMESPACE_SYSTEM, 1, 1)
{

}

HtmlDecode::~HtmlDecode()
{

}

bool HtmlDecode::execute(IContext &context, const Arguments &args, DataItem &result)
{
	result = HtmlParser::instance()->decode(args[0]);
	return true;
}

//////////////////////////////////////////////////////////////////////

JSEncode::JSEncode(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_SYSTEM_JSENCODE, OS_XSL_NAMESPACE_SYSTEM, 1, 1)
{

}

JSEncode::~JSEncode()
{

}

bool JSEncode::execute(IContext &context, const Arguments &args, DataItem &result)
{
	String text = args[0];
	text.replace_all(_S("\\"), _S("\\\\")); // 0.14 RC2
	text.replace_all(_S("'"), _S("\\'"));	
	text.replace_all(_S("%27"), _S("\\%27")); // Se è url-encoded
	text.replace_all(_S("&#039;"), _S("\\&#039;")); // Se è html-encoded
	result = text;
	return true;
}

//////////////////////////////////////////////////////////////////////

UrlEncode::UrlEncode(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_SYSTEM_URLENCODE, OS_XSL_NAMESPACE_SYSTEM, 1, 1)
{

}

UrlEncode::~UrlEncode()
{

}

bool UrlEncode::execute(IContext &context, const Arguments &args, DataItem &result)
{
	String text = args[0];
	result = HttpParser::urlEncodeW(text.to_wide());
	return true;
}

//////////////////////////////////////////////////////////////////////

UrlDecode::UrlDecode(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_SYSTEM_URLDECODE, OS_XSL_NAMESPACE_SYSTEM, 1, 1)
{

}

UrlDecode::~UrlDecode()
{

}

bool UrlDecode::execute(IContext &context, const Arguments &args, DataItem &result)
{
	String text = args[0];
	result = HttpParser::urlDecodeW(text.to_ascii());
	return true;
}

//////////////////////////////////////////////////////////////////////

HelpLink::HelpLink(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_SYSTEM_HELPLINK, OS_XSL_NAMESPACE_SYSTEM, 1)
{

}

HelpLink::~HelpLink()
{

}

bool HelpLink::execute(IContext &context, const Arguments &args, DataItem &result)
{
	String id = static_cast<String>(args[0]);

	result = getPage()->helpLink(id);
	return true;
}
/*
//////////////////////////////////////////////////////////////////////

PortalLink::PortalLink(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_SYSTEM_PORTALLINK, OS_XSL_NAMESPACE_SYSTEM, 1)
{

}

PortalLink::~PortalLink()
{

}

bool PortalLink::execute(IContext &context, const Arguments &args, DataItem &result)
{
	PortalID id = static_cast<String>(args[0]).to_ascii();
	if(id.validate(false) == false)
	{
		context.reportError(_S("invalid id"));
		return false;
	}

	result = ShellLinkPortal::generate(id);
	return true;
}
*/

//////////////////////////////////////////////////////////////////////

ReferenceDate::ReferenceDate(shared_ptr<IPortalPage> page) : FunctionBase(page, OS_XSL_FUNCTION_SYSTEM_REFERENCEDATE, OS_XSL_NAMESPACE_SYSTEM)
{

}

ReferenceDate::~ReferenceDate()
{

}

bool ReferenceDate::execute(IContext &context, const Arguments &args, DataItem &result)
{
	shared_ptr<IPortalPage> page = boost::dynamic_pointer_cast<IPortalPage>(getPage());
	if(page == nullptr)
	{
		context.reportError(_S("internal error"));
		return false;
	}

	shared_ptr<IdeSession> ideSession = page->getSessionAccount();
	if(ideSession != nullptr)
	{
		result = ideSession->getReferenceDate().toXML();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////

ResourceUrl::ResourceUrl(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_SYSTEM_RESOURCEURL, OS_XSL_NAMESPACE_SYSTEM, 1)
{

}

ResourceUrl::~ResourceUrl()
{

}

bool ResourceUrl::execute(IContext &context, const Arguments &args, DataItem &result)
{
	result = getPage()->getSkin()->getResourceUrl(args[0]);
	return true;
}

//////////////////////////////////////////////////////////////////////

Replace::Replace(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_SYSTEM_REPLACE, OS_XSL_NAMESPACE_SYSTEM, 3)
{

}

Replace::~Replace()
{

}

bool Replace::execute(IContext &context, const Arguments &args, DataItem &result)
{
	String from = args[0];
	String pattern = args[1];
	String replace = args[2];

	result = from.replace_all(pattern, replace);	
	return true;
}

//////////////////////////////////////////////////////////////////////

Text::Text(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_LANGUAGE_TEXT, OS_XSL_NAMESPACE_LANG, 1)
{

}

Text::~Text()
{

}

bool Text::execute(IContext &context, const Arguments &args, DataItem &result)
{
	result = getPage()->getText(args[0]);

	return true;
}

//////////////////////////////////////////////////////////////////////

EnglishName::EnglishName(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_LANGUAGE_ENGLISHNAME, OS_XSL_NAMESPACE_LANG, 1)
{

}

EnglishName::~EnglishName()
{

}

bool EnglishName::execute(IContext &context, const Arguments &args, DataItem &result)
{
	shared_ptr<LanguageCulture> culture = LanguageManager::instance()->getCulture(args[0]);
	if(culture == nullptr)
	{
		context.reportError(_S("invalid culture"));
		return false;
	}

	result = culture->getEnglishName();
	return true;
}

//////////////////////////////////////////////////////////////////////

NativeName::NativeName(shared_ptr<IPage> page) : FunctionBase(page, OS_XSL_FUNCTION_LANGUAGE_NATIVENAME, OS_XSL_NAMESPACE_LANG, 1)
{

}

NativeName::~NativeName()
{

}

bool NativeName::execute(IContext &context, const Arguments &args, DataItem &result)
{
	shared_ptr<LanguageCulture> culture = LanguageManager::instance()->getCulture(args[0]);
	if(culture == nullptr)
	{
		context.reportError(_S("invalid culture"));
		return false;
	}

	result = culture->getNativeName();
	return true;
}

//////////////////////////////////////////////////////////////////////

} // functions
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
