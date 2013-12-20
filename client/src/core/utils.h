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

#ifndef _OS_CORE_UTILS_H
#define _OS_CORE_UTILS_H

#include <algorithm>
#include "boost/thread/locks.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

struct CoreExport null_deleter
{
	template <typename T>
	inline void operator()(T *p) const { }
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((utils))

//////////////////////////////////////////////////////////////////////

bool CoreExport equal(const double &a, const double &b);
bool CoreExport equal(const float &a, const float &b);

uint32 CoreExport split(const String &str, const String &separators, StringVector &v, bool skipEmpty = false);
String CoreExport desplit(const StringVector &v, const String &separator, size_t start = 0);

String CoreExport safeString(const uchar *str);

String CoreExport standardisePath(const String &path, bool terminator = true);
String CoreExport standardiseEnvPath(const String &path);

String CoreExport makeFilePath(const String &folder, String filename);
String CoreExport makeFolderPath(const String &folder, String second);

String CoreExport makeUrlFilePath(const String &url, const String &filename);
String CoreExport makeUrlFolderPath(const String &url, const String &folder);

boost::xtime CoreExport xtime_seconds(size_t seconds);
boost::xtime CoreExport xtime_millisec(size_t millisec);

CoreExport boost::posix_time::ptime local_time();
CoreExport boost::posix_time::ptime utc_time();
CoreExport boost::posix_time::ptime system_time();
CoreExport boost::posix_time::ptime utc_to_local(const boost::posix_time::ptime &t);
CoreExport boost::posix_time::ptime ptime_create(uint16 year, uint16 month, uint16 day, uint16 hours = 0, uint16 minutes = 0, uint16 seconds = 0, uint16 milliseconds = 0);
CoreExport uint16 ptime_milliseconds(const boost::posix_time::ptime &t);
CoreExport boost::posix_time::ptime ptime_truncate_milliseconds(const boost::posix_time::ptime &t);
CoreExport bool ptime_is_valid(const boost::posix_time::ptime &t);

CoreExport String formatSize(uint64 size);

CoreExport std::wstring generateHashMergeW(std::wstring hash1, std::wstring hash2, bool add);
CoreExport std::string generateHashMerge(std::string hash1, std::string hash2, bool add);

CoreExport std::string wtoa(std::wstring s); // Wide to Ascii
CoreExport std::wstring atow(std::string s); // Ascii to Wide
CoreExport std::string wtou8(std::wstring s); // Wide to UTF8
CoreExport std::wstring u8tow(std::string s); // UTF8 to Wide
CoreExport std::string atou8(std::string s); // Ascii to UTF8
CoreExport std::string u8toa(std::string s); // UTF8 to Ascii

#if OS_PLATFORM == OS_PLATFORM_WIN
CoreExport HRESULT CreateWindowsShortcut(/*in*/ LPCTSTR lpszFileName, 
					/*in*/ LPCTSTR lpszArguments, 
                    /*in*/ LPCTSTR lpszDesc, 
                    /*in*/ LPCTSTR lpszShortcutPath);
#endif

CoreExport void test_unicode();

//////////////////////////////////////////////////////////////////////

inline static bool
notdigit(char c)
{
	return !isdigit(c);
}

inline static bool
notxdigit(char c)
{
	return !isxdigit(c);
}

inline static bool
notalnum(char c)
{
	return !isalnum(c);
}

//////////////////////////////////////////////////////////////////////

template <typename T1, typename T2>
inline bool safe_convert(T1 from, T2 &to)
{
	to = static_cast<T2>(from);
	if(from != to || (from > 0) != (to > 0))
		return false;

	return true;
}

template <typename T1, typename T2>
inline bool convertible(T1 value)
{
	T2 to = 0;
	return safe_convert(value, to);
}

template <typename T>
inline T safe_string(typename T::const_pointer str)
{
	if(str != nullptr)
		return T(str);

	return T();
}

template <typename T>
inline T round(double v) 
{
	return static_cast<T>(floor(v + 0.5));
}

template <typename T>
inline T abs(const T &a) { return (a < 0) ? -a : a; }

template <typename T>
inline void swap(T &a, T &b) { T c(a); a = b; b = c; }

template <typename T> 
inline T min(const T &left, const T &right) { return left < right ? left : right; }

template <typename T> 
inline T max(const T &left, const T &right) { return left > right ? left : right; }

template <typename T>
inline T limit(const T &v, const T &lowerBound, const T &upperBound) { return max<T>(min<T>(v, upperBound), lowerBound); }

template <typename T>
void add(T &dest, const T &src)
{
	for(typename T::const_iterator i = src.begin(); i != src.end(); dest.push_back(*i), ++i);
}

template <typename T, typename K, typename P, typename A>
void add(std::map<T, K, P, A> &dest, const std::map<T, K, P, A> &src)
{
	for(typename std::map<T, K, P, A>::const_iterator i = src.begin(); i != src.end(); dest[i->first] = i->second, ++i);
}

template <typename T>
void copy(T &dest, const T &src)
{
	dest.clear();
	add<T>(dest, src);
}

template <typename T, typename K, typename P, typename A>
void copy(std::map<T, K, P, A> &dest, const std::map<T, K, P, A> &src)
{
	dest.clear();
	add<T, K>(dest, src);
}

template <typename T, typename V>
inline bool exists(T &array, const V &value)
{
	return std::find(array.begin(), array.end(), value) != array.end();
}

template <typename T, typename K, typename P, typename A>
inline bool exists(std::map<T, K, P, A> &map, const T &value)
{
	return map.find(value) != map.end();
}

template <typename T, typename V>
inline void remove(T &array, const V &value)
{
	typename T::iterator i = std::find(array.begin(), array.end(), value);
	if(i != array.end())
		array.erase(i);
}

template <typename T>
inline void flush_front(T &v)
{
	while(v.empty() == false)
		v.pop_front();
}

template <typename T>
inline void flush_back(T &v)
{
	while(v.empty() == false)
		v.pop_back();
}

template <typename T, typename Y>
inline T find_index(T first, T last, const Y &i)
{
	for(Y y = 0; first != last && y != i; ++first, ++y);
	return first;
}

template <typename T, typename Y>
inline Y for_each(const T &v, const Y &y)
{
	return std::for_each(v.begin(), v.end(), y);
}

template <typename T>
inline typename T::value_type pop_front(T &container)
{
	OS_ASSERT(container.empty() == false);
	typename T::value_type front = container.front();
	container.pop_front();
	return front;
}

template <typename T>
inline typename T::value_type pop_back(T &container)
{
	OS_ASSERT(container.empty() == false);
	typename T::value_type back = container.back();
	container.pop_back();
	return back;
}

template <typename T>
inline void pop_front_until(T &container, typename T::size_type size)
{
	while(container.size() > size)
		container.pop_front();
}

template <typename T>
inline void pop_back_until(T &container, typename T::size_type size)
{
	while(container.size() > size)
		container.pop_back();	
}

template <typename T>
shared_ptr<typename T::scoped_lock> acquire_mutex(T &mutex, bool locked)
{
	shared_ptr<typename T::scoped_lock> lock(OS_NEW_T(typename T::scoped_lock)(mutex, boost::defer_lock), os_delete_t());
	if(locked)
		lock->lock();

	return lock;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((utils))

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_UTILS_H
