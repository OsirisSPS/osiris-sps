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

#ifndef _OS_HTML_HTMLUNIT_H
#define _OS_HTML_HTMLUNIT_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class WebExport HtmlUnit : public Object
{
// Enumerated types
public:
	enum Type
	{
		utPure		= 0,
		utPercent	= 1,
		utPixels	= 2,
	};

// Construction
public:
	HtmlUnit(int32 value = 0, Type type = utPure);
	HtmlUnit(const HtmlUnit &second);
	virtual ~HtmlUnit();

// Attributes
public:
	inline bool empty() const;
	inline int32 getValue() const;
	inline Type getType() const;

// Operations
public:
	String toString() const;
	static HtmlUnit fromString(String value);

	static HtmlUnit units(int32 value);
	static HtmlUnit percent(int32 value);
	static HtmlUnit pixels(int32 value);

// Operators
public:
	operator int32() const;
	operator String() const;

	HtmlUnit & operator =(const HtmlUnit &second);

protected:
	int32 m_value;
	Type m_type;
};

//////////////////////////////////////////////////////////////////////

inline bool HtmlUnit::empty() const { return (m_type == HtmlUnit::utPure); }
inline int32 HtmlUnit::getValue() const { return m_value; }
inline HtmlUnit::Type HtmlUnit::getType() const { return m_type; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLUNIT_H
