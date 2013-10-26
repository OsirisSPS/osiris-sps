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

#ifndef _OS_HTML_HTMLEVENT_H
#define _OS_HTML_HTMLEVENT_H

#include "ievent.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class WebExport HtmlEvent : public IEvent
{
	typedef IEvent EventBase;

// Construction
public:
	HtmlEvent();
	HtmlEvent(const String &value);
	virtual ~HtmlEvent();

// Attributes
public:
	inline bool empty() const;
	inline size_t size() const;

	inline const StringVector & getArgs() const;

	const String & get(size_t index) const;

// Operations
public:
	String encode() const;
	void decode(const String &args);

	void add(const String &arg);

// Operators
public:
	const String & operator[](size_t index) const;

public:
	static const HtmlEvent EMPTY;

protected:
	StringVector m_args;	
};

//////////////////////////////////////////////////////////////////////

inline bool HtmlEvent::empty() const { return m_args.empty(); }
inline size_t HtmlEvent::size() const { return m_args.size(); }

inline const StringVector & HtmlEvent::getArgs() const { return m_args; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLEVENT_H
