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

#ifndef _OS_CORE_STRINGUTF16_H
#define _OS_CORE_STRINGUTF16_H

#include "algorithms.h"
#include "unicode/unistr.h" 

//////////////////////////////////////////////////////////////////////

#ifndef _U
	#define _U(x)       OS_NAMESPACE_NAME::StringUTF16(L##x)
#endif // _U

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport StringUTF16
{
public:
	typedef int32 size_type;

	enum { npos = size_type(INT32_MAX) };

	typedef uchar value_type;

// Construction
public:
	StringUTF16();
	StringUTF16(const StringUTF16 &second);
	// Costruisce la stringa da un puntatore UTF-16 e la relativa dimensione in caratteri
	StringUTF16(const uchar *text, size_type length = -1);
	// Costruisce una stringa effettuando la conversione da char *
	StringUTF16(const achar *text, size_type length = -1, const char *codepage = nullptr);
	StringUTF16(const std::string &text, const char *codepage = nullptr);
#ifndef OS_HAS_NATIVE_UCHAR
	StringUTF16(const wchar *text, size_type length = -1);
#endif
	StringUTF16(const std::wstring &text);
	~StringUTF16();

private:
	StringUTF16(const UnicodeString &str, size_type offset = 0, size_type count = npos);

// Attributes
public:
	inline bool empty() const;

	inline size_type size() const;
	inline size_type length() const;

	inline UnicodeString & str();
	inline const UnicodeString & str() const;

	const uchar * data() const;
	const uchar * c_str() const;

// Operations
public:
	StringUTF16 & append(const StringUTF16 &second, size_type offset = 0, size_type count = npos);
	StringUTF16 & append(const uchar *text, size_type length = -1);
	StringUTF16 & append(const uchar c);

	StringUTF16 & assign(const StringUTF16 &second, size_type offset = 0, size_type count = npos);
	StringUTF16 & assign(const uchar *text, size_type length = -1);

	int32 compare(const StringUTF16 &second) const;
	int32 compare(size_type offset, size_type count, const StringUTF16 &second) const;
	int32 compare(size_type offset, size_type count, const StringUTF16 &second, size_type secondOffset, size_type secondCount) const;
	int32 compare(const uchar *text, size_type length = -1) const;
	int32 compare(size_type offset, size_type count, const uchar *text) const;
	int32 compare(size_type offset, size_type count, const uchar *text, size_type textOffset, size_type textCount) const;

	void clear();

	StringUTF16 & insert(size_type position, const StringUTF16 &second, size_type offset = 0, size_type count = npos);
	StringUTF16 & insert(size_type position, const uchar *text, size_type length = -1);

	StringUTF16 substr(size_type offset = 0, size_type count = npos) const;

	void push_back(const uchar c);

	void swap(StringUTF16 &second);

// Operators
public:
	StringUTF16 & operator =(const StringUTF16 &second);
	StringUTF16 & operator =(const UnicodeString &str);

	friend CoreExport StringUTF16 operator +(const StringUTF16 &first, const StringUTF16 &second);
	friend CoreExport StringUTF16 operator +(const uchar *first, const StringUTF16 &second);
	friend CoreExport StringUTF16 operator +(const StringUTF16 &first, const uchar *second);

	friend CoreExport bool operator ==(const StringUTF16 &first, const StringUTF16 &second);
	friend CoreExport bool operator ==(const uchar *first, const StringUTF16 &second);
	friend CoreExport bool operator ==(const StringUTF16 &first, const uchar *second);

	friend CoreExport bool operator !=(const StringUTF16 &first, const StringUTF16 &second);
	friend CoreExport bool operator !=(const uchar *first, const StringUTF16 &second);
	friend CoreExport bool operator !=(const StringUTF16 &first, const uchar *second);

	friend CoreExport bool operator <(const StringUTF16 &first, const StringUTF16 &second);
	friend CoreExport bool operator <(const uchar *first, const StringUTF16 &second);
	friend CoreExport bool operator <(const StringUTF16 &first, const uchar *second);

	friend CoreExport bool operator <=(const StringUTF16 &first, const StringUTF16 &second);
	friend CoreExport bool operator <=(const uchar *first, const StringUTF16 &second);
	friend CoreExport bool operator <=(const StringUTF16 &first, const uchar *second);

	friend CoreExport bool operator >(const StringUTF16 &first, const StringUTF16 &second);
	friend CoreExport bool operator >(const uchar *first, const StringUTF16 &second);
	friend CoreExport bool operator >(const StringUTF16 &first, const uchar *second);

	friend CoreExport bool operator >=(const StringUTF16 &first, const StringUTF16 &second);
	friend CoreExport bool operator >=(const uchar *first, const StringUTF16 &second);
	friend CoreExport bool operator >=(const StringUTF16 &first, const uchar *second);

public:
	static const StringUTF16 EMPTY;

private:
	mutable U_ICU_NAMESPACE::UnicodeString m_str;
};

//////////////////////////////////////////////////////////////////////

inline bool StringUTF16::empty() const { return OS_BOOL_CAST(m_str.isEmpty()); }

inline StringUTF16::size_type StringUTF16::size() const { return static_cast<size_type>(m_str.length()); }
inline StringUTF16::size_type StringUTF16::length() const { return static_cast<size_type>(m_str.length()); }

inline UnicodeString & StringUTF16::str() { return m_str; }
inline const UnicodeString & StringUTF16::str() const { return m_str; }

//////////////////////////////////////////////////////////////////////

CoreExport std::size_t hash_value(const StringUTF16 &v);

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((algorithms))

//////////////////////////////////////////////////////////////////////

template<>
CoreExport void to_lower<StringUTF16>(StringUTF16 &v);

template<>
CoreExport void to_upper<StringUTF16>(StringUTF16 &v);

template<>
CoreExport bool compare_no_case<StringUTF16, StringUTF16>(const StringUTF16 &v1, const StringUTF16 &v2);

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((algorithms))

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_STRINGUTF16_H
