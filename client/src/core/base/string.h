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

#ifndef _OS_CORE_STRING_H
#define _OS_CORE_STRING_H

#include "allocwrapper.h"
#include "boost/functional/hash.hpp"

//////////////////////////////////////////////////////////////////////

#ifndef _S
	#define _S(x)       OS_NAMESPACE_NAME::String(L##x)
#endif // _S

#ifndef _W
	#define _W(x)       L##x
#endif // _W

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport String : public AllocWrapper<String>
{
    typedef AllocWrapper<String> BaseClass;

public:
	typedef uchar char_type;
	typedef std::ustring string_type;

	typedef string_type::size_type size_type;
	typedef string_type::iterator iterator;
	typedef string_type::const_iterator const_iterator;
	typedef string_type::reverse_iterator reverse_iterator;
	typedef string_type::const_reverse_iterator const_reverse_iterator;

	typedef string_type::reference reference;
	typedef string_type::const_reference const_reference;

	typedef string_type::difference_type difference_type;
	typedef string_type::pointer pointer;
	typedef string_type::const_pointer const_pointer;
	typedef string_type::value_type value_type;

	enum { npos = size_type(-1) };

// Construction
public:
	inline String();
	inline String(const string_type &str);
	inline String(const String &str, size_type pos = 0, size_type n = npos);
	inline String(const char_type *str);
	inline String(const char_type *str, size_type len);
	inline String(size_type n, char_type c);
	String(const achar *str);
	String(const std::string &str);
#ifndef OS_HAS_NATIVE_UCHAR
	String(const wchar *str);
	String(const std::wstring &str);
#endif
	~String();

private:
	explicit String(const String &first, const String &second);
	explicit String(const char_type *first, const String &second);
	explicit String(const String &first, const char_type *second);
	explicit String(char_type first, const String &second);
	explicit String(const String &first, char_type second);

// Attributes
public:
	inline iterator begin();
	inline const_iterator begin() const;
	inline reverse_iterator rbegin();
	inline const_reverse_iterator rbegin() const;

	inline iterator end();
	inline const_iterator end() const;
	inline reverse_iterator rend();
	inline const_reverse_iterator rend() const;

	inline bool empty() const;

	inline const char_type * c_str() const;		// TODO: da abolire
	inline size_type length() const;			// TODO: da abolire

	inline const char_type * data() const;
	inline size_type size() const;

	inline reference at(size_type pos);
	inline const_reference at(size_type pos) const;

	inline const void * buffer() const;
	inline void * buffer();
	inline size_type buffer_size() const;

	inline const string_type & str() const;
	inline string_type & str();

// Operators
public:
	inline String & operator =(const String &second);
	inline String & operator =(const char_type *second);
	inline String & operator =(char_type second);

	inline String & operator +=(const String &second);
	inline String & operator +=(const char_type *second);
	inline String & operator +=(char_type second);

	// N.B.: essendo una stringa a dimensione variabile gli operatori [] non sono utilizzabili
//	inline reference operator [](size_type pos);
//	inline const_reference operator [](size_type pos) const;

//	inline operator const string_type &() const { return m_str; }

	String & operator =(const achar *str);
	String & operator =(const std::string &str);
#ifndef OS_HAS_NATIVE_UCHAR
	String & operator =(const wchar *str);
	String & operator =(const std::wstring &str);
#endif

// Operations
public:
	/*
	inline String & append(const String &v);
	inline String & append(const char_type *str, size_type len);
	inline String & append(const String &v, size_type pos, size_type n);

	inline String & assign(const String &v);
	inline String & assign(const char_type *str, size_type len);
	inline String & assign(const String &v, size_type pos, size_type n);
	*/

	inline String & append(const String &v);
	template <typename T1, typename T2>
	inline String & append(const T1 &v1, const T2 &v2);
	template <typename T1, typename T2, typename T3>
	inline String & append(const T1 &v1, const T2 &v2, const T3 &v3);

	inline String & assign(const String &v);
	template <typename T1, typename T2>
	inline String & assign(const T1 &v1, const T2 &v2);
	template <typename T1, typename T2, typename T3>
	inline String & assign(const T1 &v1, const T2 &v2, const T3 &v3);

	template <typename T>
	inline int compare(const T &v) const;
	template <typename T1, typename T2>
	inline int compare(const T1 &v1, const T2 &v2) const;
	template <typename T1, typename T2, typename T3>
	inline int compare(const T1 &v1, const T2 &v2, const T3 &v3) const;
	template <typename T1, typename T2, typename T3, typename T4>
	inline int compare(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4) const;

	template <typename T>
	inline String & insert(const T &v);
	template <typename T1, typename T2>
	inline String & insert(const T1 &v1, const T2 &v2);
	template <typename T1, typename T2, typename T3>
	inline String & insert(const T1 &v1, const T2 &v2, const T3 &v3);
	template <typename T1, typename T2, typename T3, typename T4>
	inline String & insert(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4);

	inline String & erase(size_type pos = 0, size_type n = npos);
	template <typename T>
	inline String & erase(const T &v);
	template <typename T1, typename T2>
	inline String & erase(const T1 &v1, const T2 &v2);

	template <typename T1, typename T2>
	inline String & replace(const T1 &v1, const T2 &v2);
	template <typename T1, typename T2, typename T3>
	inline String & replace(const T1 &v1, const T2 &v2, const T3 &v3);
	template <typename T1, typename T2, typename T3, typename T4>
	inline String & replace(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4);
	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	inline String & replace(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5);

    inline size_type copy(char_type *s, size_type n, size_type pos = 0) const;

	inline void swap(string_type &second);
	inline void swap(String &second);

	// URGENTSTRING: sono corrette le versioni con carattere?
	inline size_type find(const String &v, size_type pos = 0) const;
	inline size_type find(char_type v, size_type pos = 0) const;
	inline size_type rfind(const String &v, size_type pos = npos) const;
	inline size_type rfind(char_type v, size_type pos = npos) const;

	inline size_type find_first_of(const String &v, size_type pos = 0) const;
	inline size_type find_first_of(const String &v, size_type pos, size_type n) const;

	inline size_type find_last_of(const String &v, size_type pos = npos) const;
	inline size_type find_last_of(const String &v, size_type pos, size_type n) const;

	inline size_type find_first_not_of(const String &v, size_type pos = 0) const;
	inline size_type find_first_not_of(const String &v, size_type pos, size_type n) const;

	inline size_type find_last_not_of(const String &v, size_type pos = npos) const;
	inline size_type find_last_not_of(const String &v, size_type pos, size_type n) const;

	inline String substr(size_type pos = 0, size_type n = npos) const;

	inline void clear();
    inline void resize(size_type n, char_type c);
	inline void resize(size_type n);
	inline void reserve(size_type n = 0);

	inline void push_back(const char_type c);

	friend inline String operator +(const String &first, const String &second);
	friend inline String operator +(const String::char_type *first, const String &second);
	friend inline String operator +(const String &first, const String::char_type *second);
	friend inline String operator +(String::char_type first, const String &second);
	friend inline String operator +(const String &first, String::char_type second);

	friend inline bool operator ==(const String &first, const String &second);
	friend inline bool operator ==(const String::char_type *first, const String &second);
	friend inline bool operator ==(const String &first, const String::char_type *second);

	friend inline bool operator !=(const String &first, const String &second);
	friend inline bool operator !=(const String::char_type *first, const String &second);
	friend inline bool operator !=(const String &first, const String::char_type *second);

	friend inline bool operator <(const String &first, const String &second);
	friend inline bool operator <(const String::char_type *first, const String &second);
	friend inline bool operator <(const String &first, const String::char_type *second);

	friend inline bool operator <=(const String &first, const String &second);
	friend inline bool operator <=(const String::char_type *first, const String &second);
	friend inline bool operator <=(const String &first, const String::char_type *second);

	friend inline bool operator >(const String &first, const String &second);
	friend inline bool operator >(const String::char_type *first, const String &second);
	friend inline bool operator >(const String &first, const String::char_type *second);

	friend inline bool operator >=(const String &first, const String &second);
	friend inline bool operator >=(const String::char_type *first, const String &second);
	friend inline bool operator >=(const String &first, const String::char_type *second);

// Extensions
public:
	bool compare_no_case(const String &second) const;

	bool starts_with(const String &pattern, bool noCase = true) const;
	bool ends_with(const String &pattern, bool noCase = true) const;

	String & replace_all(const String &old_value, const String &new_value);
	String & remove(const String &value);

	String left(const size_type &count) const;
	String mid(const size_type &begin, const size_type &count = npos) const;
	String right(const size_type &count) const;

	String & trim(const String &delims = DEFAULT_TRIM);
	String & trim_left(const String &delims = DEFAULT_TRIM);
	String & trim_right(const String &delims = DEFAULT_TRIM);

	String & to_lower();
	String & to_upper();

	String find_next(const String &str, size_t &start) const;

	size_type scanf(const uchar *pattern, ...) const;

	static String format(const uchar *pattern, ...);
	static size_type sscanf(const String &str, const uchar *pattern, ...);

	String & from_ascii(const achar *str);
	String & from_ascii(const std::string &str);
	std::string to_ascii() const;

	String & from_utf8(const achar *str);
	String & from_utf8(const std::string &str);
	std::string to_utf8() const;

	String & from_wide(const wchar *str);
	String & from_wide(const std::wstring &str);
	std::wstring to_wide() const;

public:
	static const String EMPTY;
	static const String DEFAULT_TRIM;

private:
	string_type m_str;
};

//////////////////////////////////////////////////////////////////////

inline String::String()
{

}

inline String::String(const string_type &str) : m_str(str)
{

}

inline String::String(const String &str, size_type pos, size_type n) : BaseClass(str),
                                                                       m_str(str.m_str, pos, n)
{

}

inline String::String(const char_type *str) : m_str(str != null ? str : EMPTY.c_str())
{

}

inline String::String(const char_type *str, size_type len)
{
	if(str != null)
		m_str.assign(str, len);
}

inline String::String(size_type n, char_type c) : m_str(n, c)
{

}

inline String::iterator String::begin()
{
	return m_str.begin();
}

inline String::const_iterator String::begin() const
{
	return m_str.begin();
}

inline String::reverse_iterator String::rbegin()
{
	return m_str.rbegin();
}

inline String::const_reverse_iterator String::rbegin() const
{
	return m_str.rbegin();
}

inline String::iterator String::end()
{
	return m_str.end();
}

inline String::const_iterator String::end() const
{
	return m_str.end();
}

inline String::reverse_iterator String::rend()
{
	return m_str.rend();
}

inline String::const_reverse_iterator String::rend() const
{
	return m_str.rend();
}

inline bool String::empty() const
{
	return m_str.empty();
}

inline const String::char_type * String::c_str() const
{
	return m_str.c_str();
}

inline String::size_type String::length() const
{
	return m_str.length();
}

inline const String::char_type * String::data() const
{
	return m_str.data();
}

inline String::size_type String::size() const
{
	return m_str.size();
}

inline String::reference String::at(size_type pos)
{
	return m_str.at(pos);
}

inline String::const_reference String::at(size_type pos) const
{
	return m_str.at(pos);
}

inline const void * String::buffer() const
{
	return m_str.data();
}

inline void * String::buffer()
{
	return const_cast<char_type *>(m_str.data());
}

inline String::size_type String::buffer_size() const
{
	return (m_str.size() * sizeof(char_type));
}

inline const String::string_type & String::str() const
{
	return m_str;
}

inline String::string_type & String::str()
{
	return m_str;
}

inline String & String::operator =(const String &second)
{
	m_str = second.m_str;
	return *this;
}

inline String & String::operator =(const char_type *second)
{
	m_str = second;
	return *this;
}

inline String & String::operator =(char_type second)
{
	m_str = second;
	return *this;
}

inline String & String::operator +=(const String &second)
{
	m_str += second.m_str;
	return *this;
}

inline String & String::operator +=(const char_type *second)
{
	m_str += second;
	return *this;
}

inline String & String::operator +=(char_type second)
{
	m_str += second;
	return *this;
}
/*
inline String::reference String::operator [](size_type pos)
{
	return m_str[pos];
}

inline String::const_reference String::operator [](size_type pos) const
{
	return m_str[pos];
}
*/

/*
inline String & String::append(const String &v)
{
	m_str.append(v.m_str);
	return *this;
}

inline String & String::append(const char_type *str, size_type len)
{
	m_str.append(str, len);
	return *this;
}

inline String & String::append(const String &v, size_type pos, size_type n)
{
	m_str.append(v.m_str, pos, n);
	return *this;
}

inline String & String::assign(const String &v)
{
	m_str.assign(v.m_str);
	return *this;
}

inline String & String::assign(const char_type *str, size_type len)
{
	m_str.assign(str, len);
	return *this;
}

inline String & String::assign(const String &v, size_type pos, size_type n)
{
	m_str.assign(v.m_str, pos, n);
	return *this;
}
*/

inline String & String::append(const String &v)
{
	m_str.append(v.m_str);
	return *this;
}

template <typename T1, typename T2>
inline String & String::append(const T1 &v1, const T2 &v2)
{
	m_str.append(v1, v2);
	return *this;
}

template <typename T1, typename T2, typename T3>
inline String & String::append(const T1 &v1, const T2 &v2, const T3 &v3)
{
	m_str.append(v1, v2, v3);
	return *this;
}

inline String & String::assign(const String &v)
{
	m_str.assign(v.m_str);
	return *this;
}

template <typename T1, typename T2>
inline String & String::assign(const T1 &v1, const T2 &v2)
{
	m_str.assign(v1, v2);
	return *this;
}

template <typename T1, typename T2, typename T3>
inline String & String::assign(const T1 &v1, const T2 &v2, const T3 &v3)
{
	m_str.assign(v1, v2, v3);
	return *this;
}

template <typename T>
inline int String::compare(const T &v) const
{
	return m_str.compare(v);
}

template <typename T1, typename T2>
inline int String::compare(const T1 &v1, const T2 &v2) const
{
	return m_str.compare(v1, v2);
}

template <typename T1, typename T2, typename T3>
inline int String::compare(const T1 &v1, const T2 &v2, const T3 &v3) const
{
	return m_str.compare(v1, v2, v3);
}

template <typename T1, typename T2, typename T3, typename T4>
inline int String::compare(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4) const
{
	return m_str.compare(v1, v2, v3, v4);
}

template <typename T>
inline String & String::insert(const T &v)
{
	m_str.insert(v);
	return *this;
}

template <typename T1, typename T2>
inline String & String::insert(const T1 &v1, const T2 &v2)
{
	m_str.insert(v1, v2);
	return *this;
}

template <typename T1, typename T2, typename T3>
inline String & String::insert(const T1 &v1, const T2 &v2, const T3 &v3)
{
	m_str.insert(v1, v2, v3);
	return *this;
}

template <typename T1, typename T2, typename T3, typename T4>
inline String & String::insert(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4)
{
	m_str.insert(v1, v2, v3, v4);
	return *this;
}

inline String & String::erase(size_type pos, size_type n)
{
	m_str.erase(pos, n);
	return *this;
}

template <typename T>
inline String & String::erase(const T &v)
{
	m_str.erase(v);
	return *this;
}

template <typename T1, typename T2>
inline String & String::erase(const T1 &v1, const T2 &v2)
{
	m_str.erase(v1, v2);
	return *this;
}

template <typename T1, typename T2>
inline String & String::replace(const T1 &v1, const T2 &v2)
{
	m_str.replace(v1, v2);
	return *this;
}

template <typename T1, typename T2, typename T3>
inline String & String::replace(const T1 &v1, const T2 &v2, const T3 &v3)
{
	m_str.replace(v1, v2, v3);
	return *this;
}

template <typename T1, typename T2, typename T3, typename T4>
inline String & String::replace(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4)
{
	m_str.replace(v1, v2, v3, v4);
	return *this;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline String & String::replace(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5)
{
	m_str.replace(v1, v2, v3, v4, v5);
	return *this;
}

inline String::size_type String::copy(char_type *s, size_type n, size_type pos) const
{
	return m_str.copy(s, n, pos);
}

inline void String::swap(string_type &second)
{
	m_str.swap(second);
}

inline void String::swap(String &second)
{
	m_str.swap(second.m_str);
}

inline String::size_type String::find(const String &v, size_type pos) const
{
	return m_str.find(v.c_str(), pos);
}

inline String::size_type String::find(char_type v, size_type pos) const
{
	return m_str.find(v, pos);
}

inline String::size_type String::rfind(const String &v, size_type pos) const
{
	return m_str.rfind(v.c_str(), pos);
}

inline String::size_type String::rfind(char_type v, size_type pos) const
{
	return m_str.rfind(v, pos);
}

inline String::size_type String::find_first_of(const String &v, size_type pos) const
{
	return m_str.find_first_of(v.c_str(), pos);
}

inline String::size_type String::find_first_of(const String &v, size_type pos, size_type n) const
{
	return m_str.find_first_of(v.c_str(), pos, n);
}

inline String::size_type String::find_last_of(const String &v, size_type pos) const
{
	return m_str.find_last_of(v.c_str(), pos);
}

inline String::size_type String::find_last_of(const String &v, size_type pos, size_type n) const
{
	return m_str.find_last_of(v.c_str(), pos, n);
}

inline String::size_type String::find_first_not_of(const String &v, size_type pos) const
{
	return m_str.find_first_not_of(v.c_str(), pos);
}

inline String::size_type String::find_first_not_of(const String &v, size_type pos, size_type n) const
{
	return m_str.find_first_not_of(v.c_str(), pos, n);
}

inline String::size_type String::find_last_not_of(const String &v, size_type pos) const
{
	return m_str.find_last_not_of(v.c_str(), pos);
}

inline String::size_type String::find_last_not_of(const String &v, size_type pos, size_type n) const
{
	return m_str.find_last_not_of(v.c_str(), pos, n);
}

inline String String::substr(size_type pos, size_type n) const
{
	return m_str.substr(pos, n);
}

inline void String::resize(size_type n, char_type c)
{
	m_str.resize(n, c);
}

inline void String::resize(size_type n)
{
	m_str.resize(n);
}

inline void String::clear()
{
    m_str.clear();
}

inline void String::reserve(size_type n)
{
	m_str.reserve(n);
}

inline void String::push_back(const char_type c)
{
	m_str.push_back(c);
}

//////////////////////////////////////////////////////////////////////

typedef std::pair<String, String> StringPair;
typedef list<String>::type StringList;
typedef map<String, String>::type StringMap;
typedef set<String>::type StringSet;
typedef stack<String>::type StringStack;
typedef vector<String>::type StringVector;

//////////////////////////////////////////////////////////////////////

inline String operator +(const String &first, const String &second)
{
	return String(first, second);
}

inline String operator +(const String::char_type *first, const String &second)
{
	return String(first, second);
}

inline String operator +(const String &first, const String::char_type *second)
{
	return String(first, second);
}

inline String operator +(String::char_type first, const String &second)
{
	return String(first, second);
}

inline String operator +(const String &first, String::char_type second)
{
	return String(first, second);
}

inline bool operator ==(const String &first, const String &second)
{
	return first.m_str == second.m_str;
}

inline bool operator ==(const String::char_type *first, const String &second)
{
	return first == second.m_str;
}

inline bool operator ==(const String &first, const String::char_type *second)
{
	return first.m_str == second;
}

inline bool operator !=(const String &first, const String &second)
{
	return first.m_str != second.m_str;
}

inline bool operator !=(const String::char_type *first, const String &second)
{
	return first != second.m_str;
}

inline bool operator !=(const String &first, const String::char_type *second)
{
	return first.m_str != second;
}

inline bool operator <(const String &first, const String &second)
{
	return first.m_str < second.m_str;
}

inline bool operator <(const String::char_type *first, const String &second)
{
	return first < second.m_str;
}

inline bool operator <(const String &first, const String::char_type *second)
{
	return first.m_str < second;
}

inline bool operator <=(const String &first, const String &second)
{
	return first.m_str <= second.m_str;
}

inline bool operator <=(const String::char_type *first, const String &second)
{
	return first <= second.m_str;
}

inline bool operator <=(const String &first, const String::char_type *second)
{
	return first.m_str <= second;
}

inline bool operator >(const String &first, const String &second)
{
	return first.m_str > second.m_str;
}

inline bool operator >(const String::char_type *first, const String &second)
{
	return first > second.m_str;
}

inline bool operator >(const String &first, const String::char_type *second)
{
	return first.m_str > second;
}

inline bool operator >=(const String &first, const String &second)
{
	return first.m_str >= second.m_str;
}

inline bool operator >=(const String::char_type *first, const String &second)
{
	return first >= second.m_str;
}

inline bool operator >=(const String &first, const String::char_type *second)
{
	return first.m_str >= second;
}

//////////////////////////////////////////////////////////////////////

inline std::size_t hash_value(const String &v)
{
#ifdef OS_HAS_NATIVE_UCHAR
	return boost::hash<String::string_type>()(v.str());
#else
	return boost::hash<std::wstring>()(v.to_wide());
#endif
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_STRING_H
