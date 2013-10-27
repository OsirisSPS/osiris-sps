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

#ifndef _OS_NET_NTPTIME_H
#define _OS_NET_NTPTIME_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

struct TimeStamp
{
    uint32 integer;
	uint32 fractional;
};

class NetExport NTPTime
{
// Construction
public:
    NTPTime();
	NTPTime(const NTPTime &time);
	NTPTime(const TimeStamp &time);
//	NTPTime(const SYSTEMTIME &time);
	NTPTime(const boost::posix_time::ptime &pt);

// Attributes
public:
    uint32 getSeconds() const;
	uint32 getFraction() const;

// Operators
public:
    NTPTime & operator=(const NTPTime &time);
	double operator-(const NTPTime &time) const;
	NTPTime operator+(const double &timespan) const;

//	operator SYSTEMTIME() const;
	operator boost::posix_time::ptime() const;
	operator TimeStamp() const;
	operator uint64() const;

// Operations
public:
    static NTPTime GetCurrentTime();

protected:
    static uint32 MsToNtpFraction(uint16 wMilliSeconds);
	static uint16 NtpFractionToMs(uint32 dwFraction);
	static double NtpFractionToSecond(uint32 dwFraction);

protected:
	static long GetJulianDay(uint16 Year, uint16 Month, uint16 Day);
	static void GetGregorianDate(long JD, uint16 &Year, uint16 &Month, uint16 &Day);

	static uint32 m_MsToNTP[1000];
	static const double NTP_FRACTIONAL_TO_MS;
	static const double NTP_TO_SECOND;
	static const long JAN_1ST_1900;

protected:
    uint64 m_time;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_NTPTIME_H
