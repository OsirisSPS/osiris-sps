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
#include "utils.h"

#include "boost/date_time/c_local_time_adjustor.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/thread/xtime.hpp"
#include "boost/tokenizer.hpp"
#include "charset.h"
#include "conversions.h"
#include "log.h"
#include "platformmanager.h"

#if OS_PLATFORM == OS_PLATFORM_WIN
#include <windows.h>
#include <atlbase.h> // ATL smart pointers
#include <objidl.h>   /* For IPersistFile */
#include <shlobj.h>   /* For IShellLink */
#endif

//////////////////////////////////////////////////////////////////////

#define OS_SIZE_KB ((uint64)1024)
#define OS_SIZE_MB (OS_SIZE_KB * OS_SIZE_KB)
#define OS_SIZE_GB (OS_SIZE_KB * OS_SIZE_KB * OS_SIZE_KB)
#define OS_SIZE_TB (OS_SIZE_KB * OS_SIZE_KB * OS_SIZE_KB * OS_SIZE_KB)
#define OS_SIZE_PB (OS_SIZE_KB * OS_SIZE_KB * OS_SIZE_KB * OS_SIZE_KB * OS_SIZE_KB)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((utils))

//////////////////////////////////////////////////////////////////////

bool equal(const double &a, const double &b)
{
	return (abs(a - b) <= DBL_EPSILON);
}

bool equal(const float &a, const float &b)
{
	return (abs(a - b) <= FLT_EPSILON);
}

uint32 split(const String &str, const String &separators, StringVector &v, bool skipEmpty)
{
	v.clear();

	typedef boost::tokenizer<boost::char_separator<uchar>, String::const_iterator, String> tokenizer_type;

	tokenizer_type tokenizer(str, boost::char_separator<uchar>(separators.c_str()));
	for(tokenizer_type::iterator i = tokenizer.begin(); i  != tokenizer.end(); ++i)
	{
		if(skipEmpty && (*i).empty())
			continue;

		v.push_back(*i);
	}

	return static_cast<uint32>(v.size());
}

String desplit(const StringVector &v, const String &separator, size_t start)
{
	String str;
	for(size_t i = start; i < v.size(); i++)
	{
		if(i != start)
			str += separator;
		str += v[i];
	}
	return str;
}

String safeString(const uchar *str)
{
	if(str != null)
		return str;

	return String::EMPTY;
}

String standardisePath(const String &path, bool terminator)
{
	if(path.empty())
		return path;

    String standard = path;
/*
    std::replace(standard.begin(), standard.end(), chBackSlash, chForwardSlash);
    if(terminator && standard[standard.length() - 1] != chForwardSlash)
        standard += chForwardSlash;
*/

	standard.replace_all(_S("\\"), _S("/"));
	if(terminator && standard.ends_with(_S("/")) == false)
		standard.append(_S("/"));

    return standard;
}

String standardiseEnvPath(const String &path)
{
	return standardisePath(path, false).trim_right(_S("/"));
}

String makeFilePath(const String &folder, String filename)
{
	return standardisePath(folder) + filename.trim_left(_S("\\/"));
}

String makeFolderPath(const String &folder, String second)
{
	return standardisePath(standardisePath(folder) + second.trim_left(_S("\\/")));
}

String makeUrlFilePath(const String &url, const String &filename)
{
	return makeFilePath(url, filename);
}

String makeUrlFolderPath(const String &url, const String &folder)
{
	return makeFolderPath(url, folder);
}

boost::xtime xtime_seconds(size_t seconds)
{
	boost::xtime xt;
	boost::xtime_get(&xt, boost::TIME_UTC_);
	xt.sec += static_cast<boost::xtime::xtime_sec_t>(seconds);

	return xt;
}

boost::xtime xtime_millisec(size_t millisec)
{
	boost::xtime xt;
	boost::xtime_get(&xt, boost::TIME_UTC_);
	xt.sec += static_cast<boost::xtime::xtime_sec_t>(millisec / 1000);
	xt.nsec += static_cast<boost::xtime::xtime_nsec_t>(millisec % 1000) * 1000000;

	return xt;
}

boost::posix_time::ptime local_time()
{
	return boost::posix_time::microsec_clock::local_time();
}

boost::posix_time::ptime utc_time()
{
	return boost::posix_time::microsec_clock::universal_time();
}

boost::posix_time::ptime system_time()
{
	// L'ora di sistema è espressa in utc
	return utc_time();
}

boost::posix_time::ptime utc_to_local(const boost::posix_time::ptime &t)
{
	return boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(t);
}

boost::posix_time::ptime ptime_create(uint16 year, uint16 month, uint16 day, uint16 hours, uint16 minutes, uint16 seconds, uint16 milliseconds)
{
	return boost::posix_time::ptime(boost::gregorian::date(year, month, day), boost::posix_time::time_duration(hours, minutes, seconds, milliseconds*1000));
}

uint16 ptime_milliseconds(const boost::posix_time::ptime &t)
{
	boost::posix_time::time_duration duration = t.time_of_day();
	uint64 milliseconds = duration.fractional_seconds() / 1000;
	OS_ASSERT(milliseconds < 1000);
	return static_cast<uint16>(milliseconds);
}

boost::posix_time::ptime ptime_truncate_milliseconds(const boost::posix_time::ptime &t)
{
	if(t.is_not_a_date_time())
		return t;

	boost::posix_time::time_duration time = t.time_of_day();
	return boost::posix_time::ptime(t.date(), boost::posix_time::time_duration(time.hours(), time.minutes(), time.seconds()));
}

bool ptime_is_valid(const boost::posix_time::ptime &t)
{
	return (t.is_not_a_date_time() == false);
}

String formatSize(uint64 size)
{
	static const struct FormatSizeInfo
	{
		uint64 limit;
		double divisor;
		double normaliser;
		int decimals;
		const char *prefix;
	}
	formats[] =
	{
		{ 10*OS_SIZE_KB, 10.24, 100.0, 2, " KB" },			// 10 KB
		{ 100*OS_SIZE_KB, 102.4, 10.0, 1, " KB" },			// 100 KB
		{ 1000*OS_SIZE_KB, 1024.0, 1.0, 0, " KB" },			// 1000 KB
		{ 10*OS_SIZE_MB, 10485.76, 100.0, 2, " MB" },		// 10 MB
		{ 100*OS_SIZE_MB, 104857.6, 10.0, 1, " MB" },		// 100 MB
		{ 1000*OS_SIZE_MB, 1048576.0, 1.0, 0, " MB" },		// 1000 MB
		{ 10*OS_SIZE_GB, 10737418.24, 100.0, 2, " GB" },	// 10 GB
		{ 100*OS_SIZE_GB, 107374182.4, 10.0, 1, " GB" },	// 100 GB
		{ 1000*OS_SIZE_GB, 1073741824.0, 1.0, 0, " GB" },	// 1000 GB
		{ 10*OS_SIZE_TB, 10485.76, 100.0, 2, " TB" },		// 10 TB
		{ 100*OS_SIZE_TB, 104857.6, 10.0, 1, " TB" },		// 100 TB
		{ 1000*OS_SIZE_TB, 1048576.0, 1.0, 0, " TB" },		// 1000 TB
		{ 10*OS_SIZE_PB, 10737418.24, 100.00, 2, " PB" },	// 10 PB
		{ 100*OS_SIZE_PB, 107374182.4, 10.00, 1, " PB" },	// 100 PB
		{ 1000*OS_SIZE_PB, 1073741824.0, 1.00, 0, " PB" },	// 1000 PB
		{ 0, 10995116277.76, 100.00, 2, " EB" }				// EB's, catch all
	};

	if(size < OS_SIZE_KB)  // 1K
		return conversions::to_utf16(size) + _S(" byte");

	uint32 i = 0;
	while(i < (sizeof(formats) / sizeof(FormatSizeInfo) - 1))
	{
		if(size < formats[i].limit)
			break;

		i++;
	}

	double bytes = 0;
	if(i > 8)
		bytes = (double)(size >> 20) + 0.001;
	else
		bytes = (double)size + 0.00001;

	bytes = floor(bytes / formats[i].divisor) / formats[i].normaliser;

	String str = String::format(String::format(_S("%%.%df").c_str(), formats[i].decimals).c_str(), bytes);
	str += formats[i].prefix;
	return str;
}

std::string wtoa(std::wstring s)
{
	return String(s).to_ascii();
}

std::wstring atow(std::string s)
{
	return String(s).to_wide();
}

std::string wtou8(std::wstring s)
{
	return String(s).to_utf8();
}

std::wstring u8tow(std::string s)
{
	String x;
	x.from_utf8(s);
	return x.to_wide();
}

std::string atou8(std::string s)
{
	return String(s).to_utf8();
}

std::string u8toa(std::string s)
{
	String x;
	x.from_utf8(s);
	return x.to_ascii();
}

std::wstring generateHashMergeW(std::wstring hash1, std::wstring hash2, bool add)
{
	if( (hash1.empty()) && add)
		return hash2;

	if(hash1.length() == 0)
		return String::EMPTY.to_wide();

	if(hash1.length() != hash2.length())
		return String::EMPTY.to_wide();

	std::wstring result = hash1;
	for(size_t i=0;i<hash1.length();i++)
	{
		int32 a = conversions::hexToInt(hash1[i]);
		int32 b = conversions::hexToInt(hash2[i]);

		int32 c = 0;
		if(add)
			c = (a + b)%16;
		else
			c = (16 + a - b)%16;

		result[i] = conversions::intToHex(c);
	}

	return result;
}

std::string generateHashMerge(std::string hash1, std::string hash2, bool add)
{
	if( (hash1.empty()) && add)
		return hash2;

	if(hash1.length() == 0)
		return "";

	if(hash1.length() != hash2.length())
		return "";

	std::string result = hash1;
	for(size_t i=0;i<hash1.length();i++)
	{
		int32 a = conversions::hexToInt(hash1[i]);
		int32 b = conversions::hexToInt(hash2[i]);

		int32 c = 0;
		if(add)
			c = (a + b)%16;
		else
			c = (16 + a - b)%16;

		result[i] = char(conversions::intToHex(c));
	}

	return result;
}

#if OS_PLATFORM == OS_PLATFORM_WIN
 HRESULT CreateWindowsShortcut(/*in*/ LPCTSTR lpszFileName,
					/*in*/ LPCTSTR lpszArguments,
                    /*in*/ LPCTSTR lpszDesc,
                    /*in*/ LPCTSTR lpszShortcutPath)
{
    HRESULT hRes = E_FAIL;
    DWORD dwRet = 0;
    CComPtr<IShellLink> ipShellLink;
        // buffer that receives the null-terminated string

        // for the drive and path

    TCHAR szPath[MAX_PATH];
        // buffer that receives the address of the final

        //file name component in the path

    LPTSTR lpszFilePart;
    WCHAR wszTemp[MAX_PATH];

    // Retrieve the full path and file name of a specified file

    dwRet = GetFullPathName(lpszFileName,
                       sizeof(szPath) / sizeof(TCHAR),
                       szPath, &lpszFilePart);
    if (!dwRet)
        return hRes;

    // Get a pointer to the IShellLink interface

    hRes = CoCreateInstance(CLSID_ShellLink,
                            NULL,
                            CLSCTX_INPROC_SERVER,
                            IID_IShellLink,
                            (void**)&ipShellLink);

    if (SUCCEEDED(hRes))
    {
        // Get a pointer to the IPersistFile interface

        CComQIPtr<IPersistFile> ipPersistFile(ipShellLink);

        // Set the path to the shortcut target and add the description

        hRes = ipShellLink->SetPath(szPath);
        if (FAILED(hRes))
            return hRes;


		hRes = ipShellLink->SetArguments(lpszArguments);
        if (FAILED(hRes))
            return hRes;

        hRes = ipShellLink->SetDescription(lpszDesc);
        if (FAILED(hRes))
            return hRes;

        // IPersistFile is using LPCOLESTR, so make sure

                // that the string is Unicode

#if !defined _UNICODE
        MultiByteToWideChar(CP_ACP, 0,
                       lpszShortcutPath, -1, wszTemp, MAX_PATH);
#else
        wcsncpy(wszTemp, lpszShortcutPath, MAX_PATH);
#endif

        // Write the shortcut to disk

        hRes = ipPersistFile->Save(wszTemp, TRUE);
    }

    return hRes;
}
#endif


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((utils))

//////////////////////////////////////////////////////////////////////
