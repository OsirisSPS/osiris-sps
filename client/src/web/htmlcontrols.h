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

#ifndef _OS_HTML_HTMLCONTROLS_H
#define _OS_HTML_HTMLCONTROLS_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IHtmlControl;

//////////////////////////////////////////////////////////////////////

class WebExport HtmlControls : public Object
{
public:
	typedef deque<shared_ptr<IHtmlControl> >::type Controls;
	typedef Controls::iterator iterator;
	typedef Controls::const_iterator const_iterator;
	typedef Controls::reverse_iterator reverse_iterator;
	typedef Controls::const_reverse_iterator const_reverse_iterator;

// Construction
public:
	HtmlControls(IHtmlControl &control);
	virtual ~HtmlControls();

// Attributes
public:
	inline IHtmlControl & getControl();
	inline const IHtmlControl & getControl() const;

	inline bool empty() const;
	inline size_t size() const;

	inline iterator begin();
	inline const_iterator begin() const;
	inline iterator end();
	inline const_iterator end() const;

	bool exists(shared_ptr<IHtmlControl> control) const;

// Operations
public:
	void add(shared_ptr<IHtmlControl> control);
	void remove(shared_ptr<IHtmlControl> control);
	void remove(const String &id);

	void push_back(shared_ptr<IHtmlControl> control);
	void push_front(shared_ptr<IHtmlControl> control);

	shared_ptr<IHtmlControl> find(const String &id) const;

private:
	void _insert(shared_ptr<IHtmlControl> control, bool back);

protected:
	IHtmlControl &m_control;	// Controllo di riferimento
	Controls m_controls;		// Figli del controllo
};

//////////////////////////////////////////////////////////////////////

inline IHtmlControl & HtmlControls::getControl() { return m_control; }
inline const IHtmlControl & HtmlControls::getControl() const { return m_control; }

inline bool HtmlControls::empty() const { return m_controls.empty(); }
inline size_t HtmlControls::size() const { return m_controls.size(); }

inline HtmlControls::iterator HtmlControls::begin() { return m_controls.begin(); }
inline HtmlControls::const_iterator HtmlControls::begin() const { return m_controls.begin(); }
inline HtmlControls::iterator HtmlControls::end() { return m_controls.end(); }
inline HtmlControls::const_iterator HtmlControls::end() const { return m_controls.end(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLCONTROLS_H
