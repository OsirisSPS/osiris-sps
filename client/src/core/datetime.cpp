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
#include "datetime.h"

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/format.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "charset.h"
#include "conversions.h"
#include "constants.h"
#include "lock.h"
#include "platformmanager.h"
#include "unicode.h"
#include <stdio.h>
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const DateTime DateTime::EMPTY;

//////////////////////////////////////////////////////////////////////

DateTime::DateTime()
{
	_construct();
}

DateTime::DateTime(uint32 year, uint32 month, uint32 day, uint32 hour, uint32 minutes, uint32 seconds)
{
	_construct();

	setDate(year, month, day);
	setTime(hour, minutes, seconds);
}

DateTime::DateTime(const DateTime &dt)
{
	_construct();
    *this = dt;
}

DateTime::DateTime(const posix_time &pt)
{
	_construct();
	*this = pt;
}

DateTime::DateTime(const String &str)
{
	_construct();
    *this = str;
}

DateTime::DateTime(const uint32 &unix_time)
{
	_construct();
    *this = unix_time;
}

DateTime::~DateTime()
{

}

void DateTime::_construct()
{
	memset(m_value, '0', 14);
}

bool DateTime::isValid() const
{
	return *this != EMPTY;
}

bool DateTime::isNull() const
{
	return *this == EMPTY;
}

uint32 DateTime::getYear() const
{
	return _extract(dtYear);
}

void DateTime::setYear(uint32 year)
{
	_write(year, dtYear);
}

uint32 DateTime::getMonth() const
{
	return _extract(dtMonth);
}

void DateTime::setMonth(uint32 month)
{
	_write(month, dtMonth);
}

uint32 DateTime::getDay() const
{
	return _extract(dtDay);
}

void DateTime::setDay(uint32 day)
{
	_write(day, dtDay);
}

uint32 DateTime::getHours() const
{
	return _extract(dtHour);
}

void DateTime::setHours(uint32 hours)
{
	_write(hours, dtHour);
}

uint32 DateTime::getMinutes() const
{
	return _extract(dtMinutes);
}

void DateTime::setMinutes(uint32 minutes)
{
	_write(minutes, dtMinutes);
}

uint32 DateTime::getSeconds() const
{
	return _extract(dtSeconds);
}

uint32 DateTime::getDayOfWeek() const
{
	return toPosixTime().date().day_of_week();
}

void DateTime::setSeconds(uint32 seconds)
{
	_write(seconds, dtSeconds);
}

void DateTime::setDate(uint32 year, uint32 month, uint32 day)
{
	setYear(year);
	setMonth(month);
	setDay(day);
}

void DateTime::setTime(uint32 hour, uint32 minutes, uint32 seconds)
{
	setHours(hour);
	setMinutes(minutes);
	setSeconds(seconds);
}

DateTime DateTime::now()
{
	static boost::recursive_mutex cs;
	OS_LOCK(cs);			// gmtime non è garantita essere thread-safe su tutti i sistemi operativi

	time_t t = time(null);
	tm *ct = gmtime(&t);
	if(ct == null)
		return EMPTY;

	DateTime dt;
	dt.fromTm(*ct);
	return dt;
}

void DateTime::applyTimeOffset(const int32 &timeOffset, const int32 &timeDST)
{
	if(isValid())
	{
		*this = *this + TimeDuration::seconds(timeOffset) + TimeDuration::seconds(timeDST);
	}
}

bool DateTime::operator ==(const DateTime &dt) const
{
	return memcmp(m_value, dt.m_value, 14) == 0;
}

bool DateTime::operator ==(const String &str) const
{
	DateTime dt(str);
	return *this == dt;
}

bool DateTime::operator !=(const DateTime &dt) const
{
	return (*this == dt) == false;
}

bool DateTime::operator !=(const String &str) const
{
	return (*this == str) == false;
}

DateTime & DateTime::operator =(const DateTime &dt)
{
	memcpy(m_value, dt.m_value, 14);
	return *this;
}

DateTime & DateTime::operator =(const posix_time &pt)
{
	fromPosixTime(pt);
	return *this;
}

DateTime & DateTime::operator =(const String &str)
{
	fromString(str);
	return *this;
}

DateTime & DateTime::operator =(const uint32 &unix_time)
{
	if(unix_time == 0)
		clear();
	else
		fromUnix(unix_time);
	return *this;
}

bool DateTime::operator <(const DateTime &dt) const
{
	return toString() < dt.toString();
}

bool DateTime::operator <=(const DateTime &dt) const
{
	return toString() <= dt.toString();
}

bool DateTime::operator >(const DateTime &dt) const
{
	return toString() > dt.toString();
}

bool DateTime::operator >=(const DateTime &dt) const
{
	return toString() >= dt.toString();
}

void DateTime::clear()
{
    *this = EMPTY;
}

String DateTime::toString() const
{
	std::string str(m_value, 14);
	return str;
}

bool DateTime::fromString(const String &str)
{
	if(str.empty() || str == EMPTY.toString())
	{
		clear();
	}
	else
	{
		if(str.length() != 14)
		{
			OS_ASSERTFALSE();
			return false;
		}

		setDate(_extract(str, dtYear), _extract(str, dtMonth), _extract(str, dtDay));
		setTime(_extract(str, dtHour), _extract(str, dtMinutes), _extract(str, dtSeconds));
	}

	return true;
}

String DateTime::toXML() const
{
	if(isValid())
		return String::format(_S("%04d-%02d-%02dT%02d:%02d:%02d").c_str(), getYear(), getMonth(), getDay(), getHours(), getMinutes(), getSeconds());

	return String::EMPTY;
}

bool DateTime::fromXML(const String &str)
{
	if(str.empty())
	{
		clear();
	}
	else
	{
		int year = 0, month = 0, day = 0;
		int hours = 0, minutes = 0, seconds = 0;

		if(str.scanf(_S("%d-%d-%dT%d:%d:%d").c_str(), &year, &month, &day, &hours, &minutes, &seconds) != 6)
		{
			OS_ASSERTFALSE();
			return false;
		}

		setDate(year, month, day);
		setTime(hours, minutes, seconds);
	}

	return true;
}

tm DateTime::toTm() const
{
	return boost::posix_time::to_tm(toPosixTime());
}

bool DateTime::fromTm(const tm &dt)
{
	setDate(dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday);
	setTime(dt.tm_hour, dt.tm_min, dt.tm_sec);

	return true;
}

time_t DateTime::toTime_t() const
{
	OS_ASSERT(getYear() >= 1970);

	tm t = toTm();

	register short month, year;
	register time_t	result;
	static const int m_to_d[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

	month = t.tm_mon;
	year = t.tm_year + month / 12 + 1900;
	month %= 12;
	if(month < 0)
	{
		year -= 1;
		month += 12;
	}
	result = (year - 1970) * 365 + (year - 1969) / 4 + m_to_d[month];
	result = (year - 1970) * 365 + m_to_d[month];
	if(month <= 1)
		year -= 1;
	result += (year - 1968) / 4;
	result -= (year - 1900) / 100;
	result += (year - 1600) / 400;
	result += t.tm_mday;
	result -= 1;
	result *= 24;
	result += t.tm_hour;
	result *= 60;
	result += t.tm_min;
	result *= 60;
	result += t.tm_sec;

	return result;
}

bool DateTime::fromTime_t(const time_t &t)
{
	return fromPosixTime(boost::posix_time::from_time_t(t));
}

DateTime::posix_time DateTime::toPosixTime() const
{
	if(isNull())
		return boost::posix_time::ptime(boost::date_time::not_a_date_time);

	return posix_time(boost::gregorian::date(static_cast<boost::gregorian::date::year_type>(getYear()), static_cast<boost::gregorian::date::month_type>(getMonth()), static_cast<boost::gregorian::date::day_type>(getDay())), boost::posix_time::time_duration(getHours(), getMinutes(), getSeconds()));
}

bool DateTime::fromPosixTime(const posix_time &pt)
{
	if(utils::ptime_is_valid(pt))
	{
		boost::gregorian::date date = pt.date();
		boost::posix_time::time_duration time = pt.time_of_day();

		setDate(date.year(), date.month(), date.day());
		setTime(time.hours(), time.minutes(), time.seconds());
	}
	else
	{
		clear();
	}	

	return true;
}

uint32 DateTime::toUnix() const
{
	return uint32(toTime_t());
}

bool DateTime::fromUnix(const uint32 &unix_time)
{
	return fromTime_t(unix_time);
}

/*
String DateTime::toHttp() const
{
	// RFC 822

	static const uchar Jan[] = { chLatin_J, chLatin_a, chLatin_n, chNull };
	static const uchar Feb[] = { chLatin_F, chLatin_e, chLatin_b, chNull };
	static const uchar Mar[] = { chLatin_M, chLatin_a, chLatin_r, chNull };
	static const uchar Apr[] = { chLatin_A, chLatin_p, chLatin_r, chNull };
	static const uchar May[] = { chLatin_M, chLatin_a, chLatin_y, chNull };
	static const uchar Jun[] = { chLatin_J, chLatin_u, chLatin_n, chNull };
	static const uchar Jul[] = { chLatin_J, chLatin_u, chLatin_l, chNull };
	static const uchar Aug[] = { chLatin_A, chLatin_u, chLatin_g, chNull };
	static const uchar Sep[] = { chLatin_S, chLatin_e, chLatin_p, chNull };
	static const uchar Oct[] = { chLatin_O, chLatin_c, chLatin_t, chNull };
	static const uchar Nov[] = { chLatin_N, chLatin_o, chLatin_v, chNull };
	static const uchar Dec[] = { chLatin_D, chLatin_e, chLatin_c, chNull };
	static const uchar *months[] = { Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec };

	static const uchar Sun[] = { chLatin_S, chLatin_u, chLatin_n, chNull };
	static const uchar Mon[] = { chLatin_M, chLatin_o, chLatin_n, chNull };
	static const uchar Tue[] = { chLatin_T, chLatin_u, chLatin_e, chNull };
	static const uchar Wed[] = { chLatin_W, chLatin_e, chLatin_d, chNull };
	static const uchar Thu[] = { chLatin_T, chLatin_h, chLatin_u, chNull };
	static const uchar Fri[] = { chLatin_F, chLatin_r, chLatin_i, chNull };
	static const uchar Sat[] = { chLatin_S, chLatin_a, chLatin_t, chNull };
	static const uchar *days[] = { Sun, Mon, Tue, Wed, Thu, Fri, Sat };

	OS_ASSERT(getDayOfWeek() < OS_COUNTOF(days));
	OS_ASSERT((getMonth() - 1) < OS_COUNTOF(months));

    return String::format(_S("%S, %02d %S %04d %02d:%02d:%02d GMT").c_str(), days[getDayOfWeek()], getDay(), months[getMonth() - 1], getYear(), getHours(), getMinutes(), getSeconds());
}
*/
void DateTime::_info(Part part, uint32 &min, uint32 &max, uint32 &length, uint32 &offset)
{
	switch(part)
	{
	case dtYear:		min = 1400;		// Il boost non supporta date antecedenti al 1400
						max = 9999;
						length = 4;
						offset = 0;

						break;

	case dtMonth:		min = 1;
						max = 12;
						length = 2;
						offset = 4;

						break;

	case dtDay:			min = 1;
						max = 31;
						length = 2;
						offset = 6;

						break;

	case dtHour:		min = 0;
						max = 23;
						length = 2;
						offset = 8;

						break;

	case dtMinutes:		min = 0;
						max = 59;
						length = 2;
						offset = 10;

						break;

	case dtSeconds:		min = 0;
						max = 59;
						length = 2;
						offset = 12;

						break;

	default:			OS_ASSERTFALSE();
						break;
	}
}

uint32 DateTime::_extract(const String &str, Part part)
{
	uint32 min = 0;
	uint32 max = 0;
	uint32 length = 0;
	uint32 offset = 0;
	_info(part, min, max, length, offset);

	uint32 value =  conversions::from_utf16<uint32>(str.mid(offset, length));
	return utils::limit(value, min, max);
}

uint32 DateTime::_extract(Part part) const
{
	return _extract(toString(), part);
}

void DateTime::_write(uint32 value, Part part)
{
	uint32 min = 0;
	uint32 max = 0;
	uint32 length = 0;
	uint32 offset = 0;
	_info(part, min, max, length, offset);

	if(value < min || value > max)
	{
		OS_ASSERTFALSE();
		return;
	}

	OS_ASSERT(conversions::to_utf16(value).length() <= length);

	scoped_array<char, os_deallocate_t> buff(OS_ALLOCATE_T(char, length + 1));

	std::string tmp = (boost::format("%%0%dd") % length).str();
	sprintf(buff.get(), tmp.c_str(), value);
	memcpy(m_value + offset, buff.get(), length);
}

//////////////////////////////////////////////////////////////////////

const TimeDuration TimeDuration::ZERO(0, 0, 0);

//////////////////////////////////////////////////////////////////////

struct TimeDuration::impl
{
	impl() { }
	impl(int32 hours, int32 minutes, int32 seconds) : duration(hours, minutes, seconds) { }
	impl(const time_duration &td) : duration(td) { }

	time_duration duration;
};

//////////////////////////////////////////////////////////////////////

TimeDuration::TimeDuration()
{

}

TimeDuration::TimeDuration(int32 hours, int32 minutes, int32 seconds) : m_data(hours, minutes, seconds)
{

}

TimeDuration::TimeDuration(const time_duration &duration) : m_data(duration)
{

}

TimeDuration::TimeDuration(const TimeDuration &second) : m_data(second.m_data->duration)
{

}

TimeDuration::~TimeDuration()
{

}

TimeDuration::TimeDuration(const DateTime &first, const DateTime &second)
{
	*this = first - second;
}

bool TimeDuration::isNull() const
{
	return *this == ZERO;
}

const TimeDuration::time_duration & TimeDuration::getDuration() const
{
	return m_data->duration;
}

int32 TimeDuration::getHours() const
{
	return m_data->duration.hours();
}

int32 TimeDuration::getMinutes() const
{
	return m_data->duration.minutes();
}

int32 TimeDuration::getSeconds() const
{
	return m_data->duration.seconds();
}

int32 TimeDuration::getTotalSeconds() const
{
	return m_data->duration.total_seconds();
}

int32 TimeDuration::getTotalMilliseconds() const
{
	return static_cast<int32>(m_data->duration.total_milliseconds());
}

TimeDuration & TimeDuration::operator =(const TimeDuration &second)
{
	m_data->duration = second.m_data->duration;
	return *this;
}

TimeDuration::operator DateTime() const
{
	DateTime dt;
	dt.fromTm(boost::posix_time::to_tm(m_data->duration));
	return dt;
}

bool TimeDuration::operator <(const TimeDuration &second) const
{
	return m_data->duration < second.m_data->duration;
}

bool TimeDuration::operator <=(const TimeDuration &second) const
{
	return m_data->duration <= second.m_data->duration;
}

bool TimeDuration::operator >(const TimeDuration &second) const
{
	return m_data->duration > second.m_data->duration;
}

bool TimeDuration::operator >=(const TimeDuration &second) const
{
	return m_data->duration >= second.m_data->duration;
}

bool TimeDuration::operator ==(const TimeDuration &second) const
{
	return m_data->duration == second.m_data->duration;
}

bool TimeDuration::operator !=(const TimeDuration &second) const
{
	return m_data->duration != second.m_data->duration;
}

TimeDuration TimeDuration::days(int32 count)
{
	return hours(count * 24);
}

TimeDuration TimeDuration::hours(int32 count)
{
	return boost::posix_time::hours(count);
}

TimeDuration TimeDuration::minutes(int32 count)
{
	return boost::posix_time::minutes(count);
}

TimeDuration TimeDuration::seconds(int32 count)
{
	return boost::posix_time::seconds(count);
}

TimeDuration TimeDuration::millisec(int32 count)
{
	return boost::posix_time::millisec(count);
}

//////////////////////////////////////////////////////////////////////

CoreExport DateTime operator +(const DateTime &first, const TimeDuration &second)
{
	DateTime dt;
	dt.fromPosixTime(first.toPosixTime() + second.getDuration());
	return dt;
}

CoreExport DateTime operator -(const DateTime &first, const TimeDuration &second)
{
	DateTime dt;
	dt.fromPosixTime(first.toPosixTime() - second.getDuration());
	return dt;
}

CoreExport TimeDuration operator -(const DateTime &first, const DateTime &second)
{
	return first.toPosixTime() - second.toPosixTime();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
