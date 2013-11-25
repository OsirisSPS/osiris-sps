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

#ifndef _OS_COMMON_DATETIME_H
#define _OS_COMMON_DATETIME_H

#include "pimpl_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport DateTime
{
public:
	typedef boost::posix_time::ptime posix_time;

// Enumerated types
public:
	enum DateTimePattern
	{
		dpLongDatePattern			= 0,
		dpShortDatePattern			= 1,
		dpLongTimePattern			= 2,
		dpShortTimePattern			= 3,
        dpLongDateTimePattern		= 4,
        dpShortDateTimePattern		= 5,
		dpUserFriendly				= 6,
		dpRFC822					= 7,
	};

// Internal
private:
	enum Part
	{
		dtYear				= 0,
		dtMonth				= 1,
		dtDay				= 2,
		dtHour				= 3,
		dtMinutes			= 4,
		dtSeconds			= 5,
	};

// Construction
public:
	DateTime();
	DateTime(uint32 year, uint32 month, uint32 day, uint32 hour = 0, uint32 minutes = 0, uint32 seconds = 0);
	DateTime(const DateTime &dt);
	DateTime(const posix_time &pt);	
	DateTime(const String &str);
	DateTime(const uint32 &unix_time);
	~DateTime();

private:
	void _construct();

// Attributes
public:
	bool isValid() const;
	bool isNull() const;

	uint32 getYear() const;
	void setYear(uint32 year);

	uint32 getMonth() const;
	void setMonth(uint32 month);

	uint32 getDay() const;
	void setDay(uint32 day);

	uint32 getHours() const;
	void setHours(uint32 hours);

	uint32 getMinutes() const;
	void setMinutes(uint32 minutes);

	uint32 getSeconds() const;
	void setSeconds(uint32 seconds);

	uint32 getDayOfWeek() const;

	void setDate(uint32 year, uint32 month, uint32 day);
	void setTime(uint32 hour, uint32 minutes, uint32 seconds);

	// Restituisce l'ora corrente in utc
	static DateTime now();

	// Converte da data da locale a UTC o viceversa, in base al timeOffset e DST (espressi in secondi)
	void applyTimeOffset(const int32 &timeOffset, const int32 &timeDST);

// Operators
public:
	bool operator ==(const DateTime &dt) const;
	bool operator ==(const String &str) const;
	bool operator !=(const DateTime &dt) const;
	bool operator !=(const String &str) const;

	DateTime & operator =(const DateTime &dt);
	DateTime & operator =(const posix_time &pt);
	DateTime & operator =(const String &str);	
	DateTime & operator =(const uint32 &unix_time);	

	bool operator <(const DateTime &dt) const;
	bool operator <=(const DateTime &dt) const;
	bool operator >(const DateTime &dt) const;
	bool operator >=(const DateTime &dt) const;

// Operations
public:
    void clear();

	String toString() const;
	bool fromString(const String &str);

	String toXML() const;
	bool fromXML(const String &str);

	tm toTm() const;
	bool fromTm(const tm &dt);

	time_t toTime_t() const;
	bool fromTime_t(const time_t &t);

	posix_time toPosixTime() const;
	bool fromPosixTime(const posix_time &time);

	uint32 toUnix() const; // Use Time_t
	bool fromUnix(const uint32 &unix_time);

private:
	static void _info(Part part, uint32 &min, uint32 &max, uint32 &length, uint32 &offset);
	static uint32 _extract(const String &str, Part part);

	uint32 _extract(Part part) const;
	void _write(uint32 value, Part part);

protected:
	char m_value[14];	// year/month/day/hour/minutes/seconds (yyyymmddhhmmss)

public:
	static const DateTime EMPTY;
};

//////////////////////////////////////////////////////////////////////

class CoreExport TimeDuration
{
public:
	typedef boost::posix_time::time_duration time_duration;

// Construction
public:
	TimeDuration();
	TimeDuration(int32 hours, int32 minutes, int32 seconds);
	TimeDuration(const time_duration &duration);
	TimeDuration(const TimeDuration &second);
	TimeDuration(const DateTime &first, const DateTime &second);
	~TimeDuration();

// Attributes
public:
	bool isNull() const;
	const time_duration & getDuration() const;

	int32 getHours() const;
	int32 getMinutes() const;
	int32 getSeconds() const;

	int32 getTotalSeconds() const;
	int32 getTotalMilliseconds() const;

// Operations
public:
	static TimeDuration days(int32 count);
	static TimeDuration hours(int32 count);
	static TimeDuration minutes(int32 count);
	static TimeDuration seconds(int32 count);
	static TimeDuration millisec(int32 count);

// Operators
public:
	TimeDuration & operator =(const TimeDuration &second);
	operator DateTime() const;

	bool operator <(const TimeDuration &second) const;
	bool operator <=(const TimeDuration &second) const;
	bool operator >(const TimeDuration &second) const;
	bool operator >=(const TimeDuration &second) const;
	bool operator ==(const TimeDuration &second) const;
	bool operator !=(const TimeDuration &second) const;

public:
	struct impl;
	pimpl_ptr<impl> m_data;

public:
	static const TimeDuration ZERO;
};

//////////////////////////////////////////////////////////////////////

// Operators

CoreExport DateTime operator +(const DateTime &first, const TimeDuration &second);
CoreExport DateTime operator -(const DateTime &first, const TimeDuration &second);
CoreExport TimeDuration operator -(const DateTime &first, const DateTime &second);

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_COMMON_DATETIME_H
