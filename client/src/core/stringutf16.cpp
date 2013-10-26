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
#include "stringutf16.h"

#include "iculayer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const StringUTF16 StringUTF16::EMPTY;

//////////////////////////////////////////////////////////////////////

StringUTF16::StringUTF16()
{
	
}

StringUTF16::StringUTF16(const StringUTF16 &second) : m_str(second.m_str)
{

}

StringUTF16::StringUTF16(const uchar *text, size_type length) : m_str(text, length)
{

}

StringUTF16::StringUTF16(const achar *text, size_type length, const char *codepage) :  m_str(text, length, codepage)
{
	
}

StringUTF16::StringUTF16(const std::string &text, const char *codepage) :  m_str(text.c_str(), static_cast<size_type>(text.length()), codepage)
{

}

#ifndef OS_HAS_NATIVE_UCHAR
StringUTF16::StringUTF16(const wchar *text, size_type length)
{
	std::ustring str = icu_wtou(text, length);
	assign(str.c_str(), static_cast<size_type>(str.size()));
}
#endif

StringUTF16::StringUTF16(const std::wstring &text)
{
	std::ustring str = icu_wtou(text);
	assign(str.c_str(), static_cast<size_type>(str.size()));
}

StringUTF16::~StringUTF16()
{
	
}

StringUTF16::StringUTF16(const UnicodeString &str, size_type offset, size_type count) : m_str(str, offset, count)
{

}

const uchar * StringUTF16::data() const 
{ 
	// Non "inline" per evitare errori di link
	return m_str.getBuffer(); 
}

const uchar * StringUTF16::c_str() const 
{ 
	// Non "inline" per evitare errori di link
	return m_str.getTerminatedBuffer(); 
}

StringUTF16 & StringUTF16::append(const StringUTF16 &second, size_type offset, size_type count)
{
	m_str.append(second.m_str, offset, count);
	return *this;
}

StringUTF16 & StringUTF16::append(const uchar *text, size_type length)
{
	m_str.append(text, length);
	return *this;
}

StringUTF16 & StringUTF16::append(const uchar c)
{
	m_str.append(c);
	return *this;
}

StringUTF16 & StringUTF16::assign(const StringUTF16 &second, size_type offset, size_type count)
{
	m_str.setTo(second.m_str, offset, count);
	return *this;
}

StringUTF16 & StringUTF16::assign(const uchar *text, size_type length)
{
	m_str.setTo(text, length);
	return *this;
}

int32 StringUTF16::compare(const StringUTF16 &second) const
{
	return m_str.compare(second.m_str);
}

int32 StringUTF16::compare(size_type offset, size_type count, const StringUTF16 &second) const
{
	return m_str.compare(offset, count, second.m_str);
}

int32 StringUTF16::compare(size_type offset, size_type count, const StringUTF16 &second, size_type secondOffset, size_type secondCount) const
{
	return m_str.compare(offset, count, second.m_str, secondOffset, secondCount);
}

int32 StringUTF16::compare(const uchar *text, size_type length) const
{
	return m_str.compare(text, length);
}

int32 StringUTF16::compare(size_type offset, size_type count, const uchar *text) const
{
	return m_str.compare(offset, count, text);
}

int32 StringUTF16::compare(size_type offset, size_type count, const uchar *text, size_type textOffset, size_type textCount) const
{
	return m_str.compare(offset, count, text, textOffset, textCount);
}

void StringUTF16::clear()
{
	m_str.remove();
}

StringUTF16 & StringUTF16::insert(size_type position, const StringUTF16 &second, size_type offset, size_type count)
{
	// N.B.: per conformità con lo standard se la posizione è oltre la dimensione lancia un'eccezione (l'ICU invece effettua comunque l'inserimento in coda)
	if(position > size())
		throw std::out_of_range("invalid string position");

	m_str.insert(position, second.m_str, offset, count);
	return *this;
}

StringUTF16 & StringUTF16::insert(size_type position, const uchar *text, size_type length)
{
	// N.B.: per conformità con lo standard se la posizione è oltre la dimensione lancia un'eccezione (l'ICU invece effettua comunque l'inserimento in coda)
	if(position > size())
		throw std::out_of_range("invalid string position");

	m_str.insert(position, text, length);
	return *this;
}

StringUTF16 StringUTF16::substr(size_type offset, size_type count) const
{
	return StringUTF16(m_str, offset, count);
}

void StringUTF16::push_back(const uchar c)
{
	m_str.append(c);
}

void StringUTF16::swap(StringUTF16 &second)
{
	std::swap(m_str, second.m_str);
}

StringUTF16 & StringUTF16::operator =(const StringUTF16 &second)
{
	m_str = second.m_str;
	return *this;
}

StringUTF16 & StringUTF16::operator =(const UnicodeString &str)
{
	m_str = str;
	return *this;
}

CoreExport StringUTF16 operator +(const StringUTF16 &first, const StringUTF16 &second)
{
	return StringUTF16(first.m_str + second.m_str);
}

CoreExport StringUTF16 operator +(const uchar *first, const StringUTF16 &second)
{
	return StringUTF16(StringUTF16(first).m_str + second.m_str);		// Richiama il costruttore di StringUTF16 per gestire la costruzione a partire da un uchar *
}

CoreExport StringUTF16 operator +(const StringUTF16 &first, const uchar *second)
{
	return StringUTF16(first.m_str + StringUTF16(second).m_str);		// Richiama il costruttore di StringUTF16 per gestire la costruzione a partire da un uchar *
}

CoreExport bool operator ==(const StringUTF16 &first, const StringUTF16 &second)
{
	return OS_BOOL_CAST(first.m_str.operator ==(second.m_str));
}

CoreExport bool operator ==(const uchar *first, const StringUTF16 &second)
{
	return OS_BOOL_CAST(second.m_str.operator ==(first));
}

CoreExport bool operator ==(const StringUTF16 &first, const uchar *second)
{
	return OS_BOOL_CAST(first.m_str.operator ==(second));
}

CoreExport bool operator !=(const StringUTF16 &first, const StringUTF16 &second)
{
	return OS_BOOL_CAST(first.m_str.operator !=(second.m_str));
}

CoreExport bool operator !=(const uchar *first, const StringUTF16 &second)
{
	return OS_BOOL_CAST(second.m_str.operator !=(first));
}

CoreExport bool operator !=(const StringUTF16 &first, const uchar *second)
{
	return OS_BOOL_CAST(first.m_str.operator !=(second));
}

CoreExport bool operator <(const StringUTF16 &first, const StringUTF16 &second)
{
	return OS_BOOL_CAST(first.m_str.operator <(second.m_str));
}

CoreExport bool operator <(const uchar *first, const StringUTF16 &second)
{
	return OS_BOOL_CAST(second.m_str.operator >(first));
}

CoreExport bool operator <(const StringUTF16 &first, const uchar *second)
{
	return OS_BOOL_CAST(first.m_str.operator <(second));
}

CoreExport bool operator <=(const StringUTF16 &first, const StringUTF16 &second)
{
	return OS_BOOL_CAST(first.m_str.operator <=(second.m_str));
}

CoreExport bool operator <=(const uchar *first, const StringUTF16 &second)
{
	return OS_BOOL_CAST(second.m_str.operator >=(first));
}

CoreExport bool operator <=(const StringUTF16 &first, const uchar *second)
{
	return OS_BOOL_CAST(first.m_str.operator <=(second));
}

CoreExport bool operator >(const StringUTF16 &first, const StringUTF16 &second)
{
	return OS_BOOL_CAST(first.m_str > second.m_str);
}

CoreExport bool operator >(const uchar *first, const StringUTF16 &second)
{
	return OS_BOOL_CAST(second.m_str.operator <(first));
}

CoreExport bool operator >(const StringUTF16 &first, const uchar *second)
{
	return OS_BOOL_CAST(first.m_str.operator >(second));
}

CoreExport bool operator >=(const StringUTF16 &first, const StringUTF16 &second)
{
	return OS_BOOL_CAST(first.m_str.operator >=(second.m_str));
}

CoreExport bool operator >=(const uchar *first, const StringUTF16 &second)
{
	return OS_BOOL_CAST(second.m_str.operator <=(first));
}

CoreExport bool operator >=(const StringUTF16 &first, const uchar *second)
{
	return OS_BOOL_CAST(first.m_str.operator >=(second));
}

//////////////////////////////////////////////////////////////////////

CoreExport std::size_t hash_value(const OS_NAMESPACE_NAME::StringUTF16 &v)
{
	return static_cast<std::size_t>(v.str().hashCode());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((algorithms))

//////////////////////////////////////////////////////////////////////

template<> 
CoreExport void to_lower<StringUTF16>(StringUTF16 &v)
{
	v.str().toLower();
}

template<>
CoreExport void to_upper<StringUTF16>(StringUTF16 &v)
{
	v.str().toUpper();
}

template<>
CoreExport bool compare_no_case<StringUTF16, StringUTF16>(const StringUTF16 &v1, const StringUTF16 &v2)
{
	return v1.str().caseCompare(v2.str(), 0) == 0;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((algorithms))

//////////////////////////////////////////////////////////////////////
