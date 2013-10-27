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

#ifndef _OS_HTML_HTMLPAGER_H
#define _OS_HTML_HTMLPAGER_H

#include "htmldiv.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class WebExport HtmlPager : public HtmlDiv
{
	typedef HtmlDiv ControlBase;

// Enumerated types
public:
	enum Format
	{
		pfPagesNumbers		= 0,
		pfPagesCaptions		= 1,
	};

	enum Options
	{
		poShowFirstPage		= (uint32) 1 << 0,
		poShowPrevPage		= (uint32) 1 << 1,
		poShowNextPage		= (uint32) 1 << 2,
		poShowLastPage		= (uint32) 1 << 3,

		poShowAll			= (uint32) -1,
	};

public:
	class WebExport Entry : public Object
	{
	// Construction
	public:
		Entry(const String &href);
		~Entry();

	// Attributes
	public:
		inline const String & getCaption() const;
		inline void setCaption(const String &caption);

		inline const String & getHref() const;
		inline void setHref(const String &href);

	protected:
		String m_caption;
		String m_href;
	};

	typedef shared_ptr<Entry> entry_ptr;
	typedef deque<entry_ptr>::type Entries;

// Construction
public:
	HtmlPager();
	virtual ~HtmlPager();

// Attributes
public:
	inline const uint32 & getOptions() const;
	inline void setOptions(const uint32 &options);

	inline bool getOption(uint32 option) const;
	inline void setOption(uint32 option);

	inline const uint32 & getCurrent() const;
	inline void setCurrent(const uint32 &current);

	inline const uint32 & getCount() const;
	inline void setCount(const uint32 &count);

// Operations
public:
	entry_ptr addPage(const String &href = String::EMPTY);

private:
	void _insertPage(const String &caption, const String &href, const String &css);

// Overridables
protected:
	virtual String formatName(uint32 index, const entry_ptr &entry) const;

// IControl Overrides
protected:
	virtual void onLoad();

protected:
	Format m_format;		// Stile
	uint32 m_options;		// Opzioni
	Entries m_entries;		// Elementi del controllo
	uint32 m_current;		// Indice della pagina corrente
	uint32 m_count;			// Numero di pagine da visualizzare
};

//////////////////////////////////////////////////////////////////////

inline const String & HtmlPager::Entry::getCaption() const { return m_caption; }
inline void HtmlPager::Entry::setCaption(const String &caption) { m_caption = caption; }

inline const String & HtmlPager::Entry::getHref() const { return m_href; }
inline void HtmlPager::Entry::setHref(const String &href) { m_href = href; }

//////////////////////////////////////////////////////////////////////

inline const uint32 & HtmlPager::getOptions() const { return m_options; }
inline void HtmlPager::setOptions(const uint32 &options) { m_options = options; }

inline bool HtmlPager::getOption(uint32 option) const { return (m_options & option) != 0; }
inline void HtmlPager::setOption(uint32 option) { m_options |= option; }

inline const uint32 & HtmlPager::getCurrent() const { return m_current; }
inline void HtmlPager::setCurrent(const uint32 &current) { m_current = current; }

inline const uint32 & HtmlPager::getCount() const { return m_count; }
inline void HtmlPager::setCount(const uint32 &count) { m_count = count; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLPAGER_H
