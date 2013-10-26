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

#ifndef _OS_HTML_HTMLWRITER_H
#define _OS_HTML_HTMLWRITER_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class WebExport HtmlWriter : public Object
{
// Construction
public:
	HtmlWriter();
	virtual ~HtmlWriter();

// Attributes
public:
	inline const String & getHtml() const;
	inline void setHtml(const String &html) ;

	inline const String & getURL() const;
	inline void setURL(const String &url);

	inline const String & getError() const;
	inline void setError(const String &error);

protected:
	static String makeTagBegin(const String &name);
	static String makeTagEnd(const String &name);

// Operations
public:
	void add(const String &html);

	void write(const String &html);
	void writeLine();
	void writeLine(const String &html);

	void writeAttribute(const String &name, const String &value);
	void writeText(const String &tag, const String &text);

	void br();

	void indent(uint16 level = 1);
	void unindent(uint16 level = 1);

	void beginTag(const String &name, bool indentate = true);
	void endTag(bool unindentate = true);

	void openTag(const String &name, bool indentated = true);
	void closeTag(const String &name, bool indentated = true);

	void closeTag();
	void selfCloseTag();

protected:
	void _add(const String &html);
	void _writeIndent();

protected:
	String m_html;
	String m_url;
	uint16 m_indent;
	String m_error;
	StringStack m_tags;

public:
	static const String TAG_LEFT;
	static const String TAG_RIGHT;
	static const String TAG_END_LEFT;
	static const String TAG_SELF_CLOSE;

	static const String BR;
	static const String INDENT;
	static const String NEW_LINE;
	static const String BLANK;
	static const String SPACE;
};

//////////////////////////////////////////////////////////////////////

inline const String & HtmlWriter::getHtml() const { return m_html; }
inline void HtmlWriter::setHtml(const String &html) { m_html = html; }

inline const String & HtmlWriter::getURL() const { return m_url; }
inline void HtmlWriter::setURL(const String &url) { m_url = url; }

inline const String & HtmlWriter::getError() const { return m_error; }
inline void HtmlWriter::setError(const String &error) { m_error = error; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLWRITER_H
