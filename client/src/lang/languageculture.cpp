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
#include "languageculture.h"

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/format.hpp"
#include "constants.h"
#include "conversions.h"
#include "datetime.h"
#include "languagemanager.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

LanguageCulture::LanguageCulture(shared_ptr<LanguageCulture> parent) : m_parent(parent),
																	   m_isNeutralCulture(false),
																	   m_lcid(0),
																	   m_translationAvaible(false),
																	   m_defaultTimeOffset(0)
{

}

LanguageCulture::~LanguageCulture()
{

}

bool LanguageCulture::getTranslationDerivedAvaible()
{
	shared_ptr<LanguageCulture> current = get_this_ptr();
	for(;;)
	{
		if( (current == nullptr) || (current->getID() == _S("inv")) )
			return false;

		if(current->getTranslationAvaible())
			return true;

		current = current->getParent();
	}

	return false;
}

bool LanguageCulture::isParentOf(const String &id)
{
	//shared_ptr<LanguageCulture> current = LanguageManager::instance()->getCultures()[id];
	shared_ptr<LanguageCulture> current = LanguageManager::instance()->getCulture(id);
	for(;;)
	{
		if(current == nullptr)
			return false;

		if(current->getID() == getID())
			return true;

		current = current->getParent();
	}

	return false;
}

shared_ptr<LanguageCulture> LanguageCulture::addCulture(const String &id)
{
	Cultures::const_iterator i = m_cultures.find(String(id).to_lower());
	if(i != m_cultures.end())
		return i->second;

	shared_ptr<LanguageCulture> culture(OS_NEW LanguageCulture(get_this_ptr()));
	m_cultures[String(id).to_lower()] = culture;

	return culture;
}

shared_ptr<LanguageCulture> LanguageCulture::getCulture(const String &id) const
{
	Cultures::const_iterator i = m_cultures.find(String(id).to_lower());
	if(i == m_cultures.end())
		return nullptr;

	return i->second;
}

void LanguageCulture::parse(shared_ptr<XMLNode> node)
{
	if(node->getName() == _S("culture"))
	{
		m_id = node->getAttributeString(_S("id"));

		m_englishName = node->getAttributeString(_S("englishName"));
		m_nativeName = node->getAttributeString(_S("nativeName"));
		m_isNeutralCulture = conversions::from_utf16<bool>(node->getAttributeString(_S("isNeutralCulture")));
		m_defaultNotNeutral = node->getAttributeString(_S("defaultNotNeutral"));
		m_isoName = node->getAttributeString(_S("isoName"));
		m_lcid = conversions::from_utf16<uint32>(node->getAttributeString(_S("LCID")));

		m_defaultTimeOffset = conversions::from_utf16<int32>(node->getAttributeString(_S("defaultTimeOffset")));

		LanguageManager::instance()->getCultures()[String(getID()).to_lower()] = get_this_ptr();

		shared_ptr<XMLNode> nodeCultures = node->getNode(_S("cultures"));
		if(nodeCultures != nullptr)
		{
			shared_ptr<XMLNodes> childs = nodeCultures->getNodes();
			for(XMLNodes::const_iterator i = childs->begin(); i != childs->end(); ++i)
			{
				shared_ptr<XMLNode> subNode = *i;

				OS_ASSERT(subNode->getName() == _S("culture"));

				shared_ptr<LanguageCulture> subCulture(OS_NEW LanguageCulture(get_this_ptr()));
				subCulture->parse(subNode);
				m_cultures[String(subCulture->getID()).to_lower()] = subCulture;
			}
		}

		shared_ptr<XMLNode> nodeDataTimeFormatInfo = node->getNode(_S("dateTimeFormatInfo"));
		if(nodeDataTimeFormatInfo != nullptr)
		{
			//m_calendarWeekRule = nodeDataTimeFormatInfo->getAttributeString(_S("calendarWeekRule"));
			//m_dateSeparator = nodeDataTimeFormatInfo->getAttributeString(_S("dateSeparator"));
			//m_timeSeparator = nodeDataTimeFormatInfo->getAttributeString(_S("timeSeparator"));
			m_amDesignator = nodeDataTimeFormatInfo->getAttributeString(_S("amDesignator"));
			m_pmDesignator = nodeDataTimeFormatInfo->getAttributeString(_S("pmDesignator"));

			m_longDatePattern = nodeDataTimeFormatInfo->getAttributeString(_S("longDatePattern"));
			m_shortDatePattern = nodeDataTimeFormatInfo->getAttributeString(_S("shortDatePattern"));
			m_longTimePattern = nodeDataTimeFormatInfo->getAttributeString(_S("longTimePattern"));
			m_shortTimePattern = nodeDataTimeFormatInfo->getAttributeString(_S("shortTimePattern"));

			shared_ptr<XMLNode> nodeDayNames = nodeDataTimeFormatInfo->getNode(_S("dayNames"));
			if(nodeDayNames != nullptr)
			{
				shared_ptr<XMLNodes> childs = nodeDayNames->getNodes();
				for(XMLNodes::const_iterator i = childs->begin(); i != childs->end(); ++i)
				{
					shared_ptr<XMLNode> nodeDayName = *i;

					uint32 index = conversions::from_utf16<uint32>(nodeDayName->getAttributeString(_S("index")));
					String name = nodeDayName->getAttributeString(_S("name"));
					String abbreviated = nodeDayName->getAttributeString(_S("abbreviated"));

					OS_ASSERT(index < 7);
					m_dayNamesLong[index] = name;
					m_dayNamesShort[index] = abbreviated;
				}
			}

			shared_ptr<XMLNode> nodeMonthNames = nodeDataTimeFormatInfo->getNode(_S("monthNames"));
			{
				shared_ptr<XMLNodes> childs = nodeMonthNames->getNodes();
				for(XMLNodes::const_iterator i = childs->begin(); i != childs->end(); ++i)
				{
					shared_ptr<XMLNode> nodeMonthName = *i;

					uint32 index = conversions::from_utf16<uint32>(nodeMonthName->getAttributeString(_S("index")));
					String name = nodeMonthName->getAttributeString(_S("name"));
					String abbreviated = nodeMonthName->getAttributeString(_S("abbreviated"));

					OS_ASSERT(index < 12);
					m_monthNamesLong[index] = name;
					m_monthNamesShort[index] = abbreviated;
				}
			}

		}
	}
}

String LanguageCulture::formatDate(const DateTime &dt, const DateTime::DateTimePattern &pattern, int32 timeOffset, int32 timeDST) const
{
	// I pattern restituiti dal .net non contengono mai i nomi dei mesi abbreviati... a parte l' RFC, non usato qui...
	// per usarli dovremmo creare 3 nuovi pattern 'm_medium'... ne vale la pena?
	
	if(pattern == DateTime::dpUserFriendly)
	{
		DateTime now = DateTime::now();
		if(dt > now)
		{
			return formatDate(dt, m_longDatePattern + _S(" ") + m_longTimePattern, timeOffset, timeDST);
		}

		TimeDuration delta = now - dt;

		uint32 days = delta.getHours()/24;

		if(days > 4)
			return formatDate(dt, m_longDatePattern + _S(" ") + m_longTimePattern, timeOffset, timeDST);

		if(days > 1)
			return conversions::to_utf16(days) + _S(" ") + LanguageManager::instance()->getText(getID(),_S("datetime.days_ago"));

		if(days == 1)
			return LanguageManager::instance()->getText(getID(),_S("datetime.yesterday_at")) + _S(" ") + formatDate(dt, m_longTimePattern, timeOffset, timeDST);

		if(delta.getHours() > 1)
			return conversions::to_utf16(delta.getHours()) + _S(" ") + LanguageManager::instance()->getText(getID(),_S("datetime.hours_ago"));

		if(delta.getHours() == 1)
			return conversions::to_utf16(delta.getHours()) + _S(" ") + LanguageManager::instance()->getText(getID(),_S("datetime.hour_ago"));
		
		if(delta.getMinutes() > 1)
			return conversions::to_utf16(delta.getMinutes()) + _S(" ") + LanguageManager::instance()->getText(getID(),_S("datetime.minutes_ago"));

		if(delta.getMinutes() == 1)
			return conversions::to_utf16(delta.getMinutes()) + _S(" ") + LanguageManager::instance()->getText(getID(),_S("datetime.minute_ago"));

		if(delta.getSeconds() > 5)
			return conversions::to_utf16(delta.getSeconds()) + _S(" ") + LanguageManager::instance()->getText(getID(),_S("datetime.seconds_ago"));

		return LanguageManager::instance()->getText(getID(),_S("datetime.now_ago"));
	}
	else
	{
		String sPattern;

		switch(pattern)
		{
		case DateTime::dpLongDatePattern:		sPattern = m_longDatePattern;
												break;

		case DateTime::dpShortDatePattern:		sPattern = m_shortDatePattern;
												break;

		case DateTime::dpLongTimePattern:		sPattern = m_longTimePattern;
												break;

		case DateTime::dpShortTimePattern:		sPattern = m_shortTimePattern;
												break;

		case DateTime::dpLongDateTimePattern:	sPattern = m_longDatePattern + _S(" ") + m_longTimePattern;
												break;

		case DateTime::dpShortDateTimePattern:	sPattern = m_shortDatePattern + _S(" ") + m_shortTimePattern;
												break;

		case DateTime::dpUserFriendly:			sPattern = m_shortDatePattern + _S(" ") + m_shortTimePattern;
												break;

		case DateTime::dpRFC822:				
			{
				// Copiata da HttpParser::toString
				static const char *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
				static const char *days[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

				DateTime::posix_time time = dt.toPosixTime();

				OS_ASSERT(time.date().day_of_week() < OS_COUNTOF(days));
				OS_ASSERT(static_cast<uint32>(time.date().month() - 1) < OS_COUNTOF(months));

				return (boost::format("%s, %02d %s %04d %02d:%02d:%02d GMT") % days[time.date().day_of_week()] % time.date().day() % months[time.date().month() - 1] % time.date().year() % time.time_of_day().hours() % time.time_of_day().minutes() % time.time_of_day().seconds()).str();

			} break;
		}

		return formatDate(dt, sPattern, timeOffset, timeDST);
	}
}

String LanguageCulture::formatDate(const DateTime &dt, const String &pattern, int32 timeOffset, int32 timeDST) const
{
	if(!dt.isValid())
		return String::EMPTY;

	DateTime dtl = dt;
	dtl.applyTimeOffset(timeOffset, timeDST); 

	String output = pattern;

	uint32 dayOfWeek = dtl.getDayOfWeek();

	output.replace_all(_S("{dddd}"), m_dayNamesLong[dayOfWeek]);
	output.replace_all(_S("{MMMM}"), m_monthNamesLong[dtl.getMonth()-1]);

	output.replace_all(_S("{ddd}"), m_dayNamesShort[dayOfWeek]);
	output.replace_all(_S("{MMM}"), m_monthNamesShort[dtl.getMonth() - 1]);

	output.replace_all(_S("{yyyy}"), String::format(_S("%04d").c_str(), dtl.getYear()));
	output.replace_all(_S("{yy}"), String::format(_S("%02d").c_str(), dtl.getYear()%100));
	output.replace_all(_S("{dd}"), String::format(_S("%02d").c_str(), dtl.getDay()));
	output.replace_all(_S("{MM}"), String::format(_S("%02d").c_str(), dtl.getMonth()));

	uint32 hours = dtl.getHours();
	bool PM = hours > 12;
	output.replace_all(_S("{tt}"), PM ? m_pmDesignator : m_amDesignator);
	output.replace_all(_S("{HH}"), String::format(_S("%02d").c_str(), hours));
	output.replace_all(_S("{H}"), String::format(_S("%d").c_str(), hours));
	output.replace_all(_S("{hh}"), String::format(_S("%02d").c_str(), hours%12));
	output.replace_all(_S("{h}"), String::format(_S("%d").c_str(), hours % 12));
	output.replace_all(_S("{mm}"), String::format(_S("%02d").c_str(), dtl.getMinutes()));
	output.replace_all(_S("{ss}"), String::format(_S("%02d").c_str(), dtl.getSeconds()));

	output.replace_all(_S("{d}"), String::format(_S("%d").c_str(), dtl.getDay()));
	output.replace_all(_S("{M}"), String::format(_S("%d").c_str(), dtl.getMonth()));

	output.replace_all(_S("{H}"), String::format(_S("%d").c_str(), dtl.getHours()));
	output.replace_all(_S("{m}"), String::format(_S("%d").c_str(), dtl.getMinutes()));
	output.replace_all(_S("{s}"), String::format(_S("%d").c_str(), dtl.getSeconds()));

	return output;
}

String LanguageCulture::formatTime(double time) const
{
	// TODO: Andrebbero inserite nel multilingua

	uint32 total = static_cast<uint32>(time / 1000);

	uint32 days = static_cast<uint32>(floor(double(total / (60 * 60 * 24))));
	uint32 hours = static_cast<uint32>(floor(double(total / (60 * 60)))) % (24);
	uint32 minutes = static_cast<uint32>(floor(double(total / 60))) % 60;
	uint32 seconds = total % 60;

	String output = _S("");
	if(days == 1)
		output += _S("1 ") + LanguageManager::instance()->getText(getID(),_S("datetime.timespan.day"));
	else if(days >1)
		output += conversions::to_utf16(days) + LanguageManager::instance()->getText(getID(),_S("datetime.timespan.days"));

	if( (hours != 0) && (output.empty() == false) )
		output += _S(", ");
	if(hours == 1)
		output += _S("1 ") + LanguageManager::instance()->getText(getID(),_S("datetime.timespan.hour"));
	else if(hours>1)
		output += conversions::to_utf16(hours) + _S(" ") + LanguageManager::instance()->getText(getID(),_S("datetime.timespan.hours"));

	if( (minutes != 0) && (output.empty() == false) )
		output += _S(", ");
	if(minutes == 1)
		output += _S("1 ") + LanguageManager::instance()->getText(getID(),_S("datetime.timespan.minute"));
	else if(minutes >1)
		output += conversions::to_utf16(minutes) + _S(" ") + LanguageManager::instance()->getText(getID(),_S("datetime.timespan.minutes"));

	if( (seconds != 0) && (output.empty() == false) )
		output += _S(", ");
	if(seconds == 1)
		output += _S("1 ") + LanguageManager::instance()->getText(getID(),_S("datetime.timespan.second"));
	else if(seconds >1)
		output += conversions::to_utf16(seconds) + _S(" ") + LanguageManager::instance()->getText(getID(),_S("datetime.timespan.seconds"));

	return output;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
