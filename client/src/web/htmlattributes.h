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

#ifndef _OS_HTML_HTMLATTRIBUTES_H
#define _OS_HTML_HTMLATTRIBUTES_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlAttribute;
class IHtmlControl;

//////////////////////////////////////////////////////////////////////

class WebExport HtmlAttributes : public Object
{
public:
	typedef deque<shared_ptr<HtmlAttribute> >::type AttributesList;
	typedef AttributesList::iterator iterator;
	typedef AttributesList::const_iterator const_iterator;
	typedef AttributesList::reverse_iterator reverse_iterator;
	typedef AttributesList::const_reverse_iterator const_reverse_iterator;

// Construction
public:
	HtmlAttributes(IHtmlControl &control);
	virtual ~HtmlAttributes();

// Attributes
public:
	inline IHtmlControl & getControl();
	inline const IHtmlControl & getControl() const;

	inline bool empty() const;
	inline uint32 size() const;

	inline iterator begin();
	inline const_iterator begin() const;
	inline iterator end();
	inline const_iterator end() const;

	bool exists(const String &name) const;

// Operations
public:
	void clear();
	shared_ptr<HtmlAttribute> get(const String &name) const;
	shared_ptr<HtmlAttribute> get(size_t index) const;
	shared_ptr<HtmlAttribute> set(const String &name, const String &value);

	void remove(const String &name);

	const String & value_of(const String &name);

private:
	iterator _findByName(const String &name) const;
	shared_ptr<HtmlAttribute> _findByIndex(size_t index) const;

protected:
	IHtmlControl &m_control;
	mutable AttributesList m_attributes;
};

//////////////////////////////////////////////////////////////////////

inline IHtmlControl & HtmlAttributes::getControl() { return m_control; }
inline const IHtmlControl & HtmlAttributes::getControl() const { return m_control; }

inline bool HtmlAttributes::empty() const { return m_attributes.empty(); }
inline uint32 HtmlAttributes::size() const { return static_cast<uint32>(m_attributes.size()); }

inline HtmlAttributes::iterator HtmlAttributes::begin() { return m_attributes.begin(); }
inline HtmlAttributes::const_iterator HtmlAttributes::begin() const { return m_attributes.begin(); }
inline HtmlAttributes::iterator HtmlAttributes::end() { return m_attributes.end(); }
inline HtmlAttributes::const_iterator HtmlAttributes::end() const { return m_attributes.end(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLATTRIBUTES_H
