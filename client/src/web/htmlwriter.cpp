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
#include "htmlwriter.h"

#include "htmlparser.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String HtmlWriter::TAG_LEFT = _S("<");
const String HtmlWriter::TAG_RIGHT = _S(">");
const String HtmlWriter::TAG_END_LEFT = _S("</");
const String HtmlWriter::TAG_SELF_CLOSE = _S("/>");

const String HtmlWriter::INDENT = _S("\t");
const String HtmlWriter::BR = _S("<br />");
const String HtmlWriter::NEW_LINE = _S("\r\n");
const String HtmlWriter::BLANK = _S(" ");
const String HtmlWriter::SPACE = _S("&nbsp");

//////////////////////////////////////////////////////////////////////

HtmlWriter::HtmlWriter()
{
	m_indent = 0;
}

HtmlWriter::~HtmlWriter()
{

}

String HtmlWriter::makeTagBegin(const String &name)
{
	return TAG_LEFT + name + TAG_RIGHT;
}

String HtmlWriter::makeTagEnd(const String &name)
{
	return TAG_END_LEFT + name + TAG_RIGHT;
}

void HtmlWriter::add(const String &html)
{
	_add(html);
}

void HtmlWriter::write(const String &html)
{
	_writeIndent();
	_add(html);
}

void HtmlWriter::writeLine()
{
	_add(NEW_LINE);
}

void HtmlWriter::writeLine(const String &html)
{
	write(html);
	writeLine();
}

void HtmlWriter::writeAttribute(const String &name, const String &value)
{
	_add(BLANK + name + _S("=\"") + HtmlParser::instance()->encode(value) + _S("\""));
}

void HtmlWriter::writeText(const String &tag, const String &text)
{
	if(tag.empty() == false)
		writeLine(makeTagBegin(tag) + text + makeTagEnd(tag));
}

void HtmlWriter::br()
{
    _add(BR);
}

void HtmlWriter::indent(uint16 level)
{
	m_indent += level;
}

void HtmlWriter::unindent(uint16 level)
{
	if(m_indent >= level)
		m_indent -= level;
}

void HtmlWriter::beginTag(const String &name, bool indentate)
{
	if(name.empty() == false)
	{
		m_tags.push(name);
		writeLine(makeTagBegin(name));

		if(indentate)
			indent();
	}
}

void HtmlWriter::endTag(bool unindentate)
{
	if(m_tags.empty() == false)
	{
		if(unindentate)
			unindent();

		writeLine(makeTagEnd(m_tags.top()));
		m_tags.pop();
	}
}

void HtmlWriter::openTag(const String &name, bool indentated)
{
	if(name.empty() == false)
	{
		if(indentated)
			write(TAG_LEFT + name);
		else
			_add(TAG_LEFT + name);
	}
}

void HtmlWriter::closeTag(const String &name, bool indentated)
{
	if(name.empty() == false)
	{
		if(indentated)
			write(makeTagEnd(name));
		else
			_add(makeTagEnd(name));
	}
}

void HtmlWriter::closeTag()
{
	_add(TAG_RIGHT);
}

void HtmlWriter::selfCloseTag()
{
	_add(BLANK + TAG_SELF_CLOSE);
}

void HtmlWriter::_add(const String &html)
{
	m_html += html;
}

void HtmlWriter::_writeIndent()
{
#ifdef OS_DEBUG
	if(m_indent > 0)
	{
		String indent;
		for(uint16 i = 0; i < m_indent; indent += INDENT, i++);
		_add(indent);
	}
#endif // OS_DEBUG
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
