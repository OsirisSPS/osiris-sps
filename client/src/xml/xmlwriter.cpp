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
#include "xmlwriter.h"

#include "file.h"
#include "log.h"
#include "xmlattribute.h"
#include "xmlattributes.h"
#include "xmldetails.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<XMLWriter>
{
// Internal
private:
	class IndentScope
	{
	public:
		IndentScope(pimpl<XMLWriter> *writer)
		{
			OS_ASSERT(writer != nullptr);
			m_writer = writer;
			m_writer->increaseIndent();
		}

		~IndentScope()
		{
			m_writer->decreaseIndent();
		}

	private:
		pimpl<XMLWriter> *m_writer;
	};

// Construction
public:
	pimpl(bool enableFormat);
	virtual ~pimpl();

// Attributes
public:
	bool getEnableFormat() const;
	void setEnableFormat(bool enableFormat);

	String getIndent() const;
	void setIndent(const String &indent);

// Operations
public:
	bool openFile(const String &filename);
	bool openStream(shared_ptr<IStream> stream);
	void close();

	bool writeDeclaration();
	bool writeDocument(shared_ptr<XMLDocument> document, bool declaration);
	bool writeNode(shared_ptr<XMLNode> node);
	bool writeOpen(shared_ptr<XMLNode> node, bool selfClose);
	bool writeContent(shared_ptr<XMLNode> node, bool childs);
	bool writeClose(shared_ptr<XMLNode> node);

	void increaseIndent();
	void decreaseIndent();
	void resetIndent();

private:
	void writeIndent();
	void writeLine();

private:
	bool m_enableFormat;
	String m_indent;
	uint32 m_indentLevel;	
	shared_ptr<XMLStreamFormatTarget> m_formatTarget;
	shared_ptr<xerces::XMLFormatter> m_formatter;

	static const XMLCh m_angleFeed[];
	static const XMLCh m_equalsQuote[];
	static const XMLCh m_angleSlash[];	
	static const XMLCh m_declarationVersionInfo[];
	static const XMLCh m_declarationSeparator[];
	static const XMLCh m_declarationEncoding[];
	static const XMLCh m_declarationStandalone[];
	static const XMLCh m_declarationEndTag[];
};

//////////////////////////////////////////////////////////////////////

const XMLCh pimpl<XMLWriter>::m_angleFeed[] = { xerces::chCloseAngle, xerces::chNull };
const XMLCh pimpl<XMLWriter>::m_equalsQuote[] = { xerces::chEqual, xerces::chDoubleQuote, xerces::chNull };
const XMLCh pimpl<XMLWriter>::m_angleSlash[] = { xerces::chOpenAngle, xerces::chForwardSlash, xerces::chNull };
const XMLCh pimpl<XMLWriter>::m_declarationVersionInfo[] = { xerces::chOpenAngle, xerces::chQuestion, xerces::chLatin_x, xerces::chLatin_m, xerces::chLatin_l, xerces::chSpace, xerces::chLatin_v, xerces::chLatin_e, xerces::chLatin_r, xerces::chLatin_s, xerces::chLatin_i, xerces::chLatin_o, xerces::chLatin_n, xerces::chEqual, xerces::chDoubleQuote, xerces::chNull };
const XMLCh pimpl<XMLWriter>::m_declarationSeparator[] = { xerces::chDoubleQuote, xerces::chSpace, xerces::chNull };
const XMLCh pimpl<XMLWriter>::m_declarationEncoding[] = { xerces::chLatin_e, xerces::chLatin_n, xerces::chLatin_c, xerces::chLatin_o, xerces::chLatin_d, xerces::chLatin_i, xerces::chLatin_n, xerces::chLatin_g, xerces::chEqual, xerces::chDoubleQuote, xerces::chNull };
const XMLCh pimpl<XMLWriter>::m_declarationStandalone[] = { xerces::chLatin_s, xerces::chLatin_t, xerces::chLatin_a, xerces::chLatin_n, xerces::chLatin_d, xerces::chLatin_a, xerces::chLatin_l, xerces::chLatin_o, xerces::chLatin_n, xerces::chLatin_e, xerces::chEqual, xerces::chDoubleQuote, xerces::chNull };
const XMLCh pimpl<XMLWriter>::m_declarationEndTag[] = { xerces::chQuestion, xerces::chCloseAngle, xerces::chNull };

//////////////////////////////////////////////////////////////////////

pimpl<XMLWriter>::pimpl(bool enableFormat) : m_enableFormat(enableFormat),
											 m_indent("  "),
											 m_indentLevel(0)											 
{

}

pimpl<XMLWriter>::~pimpl()
{
	close();
}

bool pimpl<XMLWriter>::getEnableFormat() const
{
	return m_enableFormat;
}

void pimpl<XMLWriter>::setEnableFormat(bool enableFormat)
{
	m_enableFormat = enableFormat;
}

String pimpl<XMLWriter>::getIndent() const
{
	return m_indent;
}

void pimpl<XMLWriter>::setIndent(const String &indent)
{
	m_indent = indent;
}

bool pimpl<XMLWriter>::openFile(const String &filename)
{
	close();

	shared_ptr<File> file(OS_NEW File());
	if(file->open(filename, File::ofWrite) == false)
		return false;

	return openStream(file);	
}

bool pimpl<XMLWriter>::openStream(shared_ptr<IStream> stream)
{
	close();

	if(stream == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	shared_ptr<XMLStreamFormatTarget> formatTarget;
	shared_ptr<xerces::XMLFormatter> formatter;
	try
	{
		formatTarget.reset(OS_XERCES_NEW XMLStreamFormatTarget(stream));
		formatter.reset(OS_XERCES_NEW xerces::XMLFormatter(String(OS_XML_ENCODING_UTF8).c_str(), formatTarget.get(), xerces::XMLFormatter::NoEscapes, xerces::XMLFormatter::UnRep_Fail));
	}
	catch(const xerces::XMLException &e)
	{
		OS_LOG_ERROR(e.getMessage());
		return false;
	}

	OS_ASSERT(formatTarget != nullptr);
	m_formatTarget = formatTarget;

	OS_ASSERT(formatter != nullptr);
	m_formatter = formatter;

	return true;
}

void pimpl<XMLWriter>::close()
{
	OS_ASSERT(m_indentLevel == 0);

	if(m_formatTarget != nullptr)
	{
		m_formatTarget->flush();
		m_formatTarget.reset();
	}

	m_formatter.reset();
}

bool pimpl<XMLWriter>::writeDeclaration()
{
	try
	{
		OS_ASSERT(m_formatter != nullptr);

		*m_formatter << xerces::XMLFormatter::NoEscapes;
        *m_formatter << m_declarationVersionInfo << xerces::XMLUni::fgVersion1_0 << m_declarationSeparator;
        *m_formatter << m_declarationEncoding << String(OS_XML_ENCODING_UTF8).c_str() << m_declarationSeparator;
		*m_formatter << m_declarationStandalone << xerces::XMLUni::fgNoString << m_declarationSeparator;      
        *m_formatter << m_declarationEndTag;

		writeLine();
	}
	catch(const xerces::XMLException &e)
	{
		OS_LOG_ERROR(e.getMessage());
		return false;
	}

	return true;
}

bool pimpl<XMLWriter>::writeDocument(shared_ptr<XMLDocument> document, bool declaration)
{
	if(document == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	if(declaration && writeDeclaration() == false)
		return false;

	return writeNode(document->getRoot());
}

bool pimpl<XMLWriter>::writeNode(shared_ptr<XMLNode> node)
{
	if(node == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	bool selfClose = true;
	if(node->hasData() || node->hasNodes())
		selfClose = false;

	if(writeOpen(node, selfClose) == false)
		return false;

	if(selfClose == false)
	{
		{
			IndentScope indent(this);

			if(writeContent(node, true) == false)
				return false;
		}

		if(writeClose(node) == false)
			return false;
	}

	return true;
}

bool pimpl<XMLWriter>::writeOpen(shared_ptr<XMLNode> node, bool selfClose)
{
	if(node == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	try
	{
		writeIndent();

		OS_ASSERT(m_formatter != nullptr);

		*m_formatter << xerces::XMLFormatter::NoEscapes << xerces::chOpenAngle << node->getName().c_str();

		shared_ptr<XMLAttributes> attributes = node->getAttributes();
		OS_ASSERT(attributes != nullptr);
		for(XMLAttributes::const_iterator i = attributes->begin(); i != attributes->end(); ++i)
		{
			shared_ptr<XMLAttribute> attribute = *i;
			*m_formatter << xerces::XMLFormatter::NoEscapes << xerces::chSpace << attribute->getName().c_str() << m_equalsQuote << xerces::XMLFormatter::AttrEscapes << attribute->getValue().c_str() << xerces::XMLFormatter::NoEscapes << xerces::chDoubleQuote;
		}
		
		if(selfClose)
			*m_formatter << xerces::chForwardSlash;

		*m_formatter << m_angleFeed;

		writeLine();
	}
	catch(const xerces::XMLException &e)
	{
		OS_LOG_ERROR(e.getMessage());
		return false;
	}	
	
	return true;
}

bool pimpl<XMLWriter>::writeContent(shared_ptr<XMLNode> node, bool childs)
{
	if(node == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	if(node->hasData())
	{
		try
		{
			writeIndent();

			OS_ASSERT(m_formatter != nullptr);

			*m_formatter << xerces::XMLFormatter::CharEscapes << node->getData().c_str();		

			writeLine();
		}
		catch(const xerces::XMLException &e)
		{
			OS_LOG_ERROR(e.getMessage());
			return false;
		}
	}

	if(childs)
	{
		shared_ptr<XMLNodes> nodes = node->getNodes();
		for(XMLNodes::const_iterator i = nodes->begin(); i != nodes->end(); ++i)
		{
			if(writeNode(*i) == false)
				return false;
		}
	}

	return true;
}

bool pimpl<XMLWriter>::writeClose(shared_ptr<XMLNode> node)
{
	if(node == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	try
	{
		writeIndent();

		OS_ASSERT(m_formatter != nullptr);

		*m_formatter << xerces::XMLFormatter::NoEscapes << m_angleSlash << node->getName().c_str() << m_angleFeed;

		writeLine();
	}
	catch(const xerces::XMLException &e)
	{
		OS_LOG_ERROR(e.getMessage());
		return false;
	}

	return true;
}

void pimpl<XMLWriter>::increaseIndent()
{
	m_indentLevel++;
}

void pimpl<XMLWriter>::decreaseIndent()
{
	if(m_indentLevel == 0)
	{
		OS_ASSERTFALSE();
		return;
	}
	
	m_indentLevel--;
}

void pimpl<XMLWriter>::resetIndent()
{
	m_indentLevel = 0;
}

void pimpl<XMLWriter>::writeIndent()
{
	if(m_enableFormat == false)
		return;
	
	uint32 indentLevel = m_indentLevel;
	while(indentLevel > 0)
	{
		OS_ASSERT(m_formatter != nullptr);
		*m_formatter << m_indent.c_str();
		indentLevel--;
	}		
}

void pimpl<XMLWriter>::writeLine()
{
	if(m_enableFormat == false)
		return;

	OS_ASSERT(m_formatter != nullptr);
	*m_formatter << xerces::chLF;	
}

//////////////////////////////////////////////////////////////////////

XMLWriter::XMLWriter(bool enableFormat) : m_impl(enableFormat)
{

}

XMLWriter::~XMLWriter()
{
	
}

bool XMLWriter::getEnableFormat() const
{
	return m_impl->getEnableFormat();
}

void XMLWriter::setEnableFormat(bool enableFormat)
{
	return m_impl->setEnableFormat(enableFormat);
}

String XMLWriter::getIndent() const
{
	return m_impl->getIndent();
}

void XMLWriter::setIndent(const String &indent)
{
	m_impl->setIndent(indent);
}

bool XMLWriter::openFile(const String &filename)
{
	return m_impl->openFile(filename);
}

bool XMLWriter::openStream(shared_ptr<IStream> stream)
{
	return m_impl->openStream(stream);
}

void XMLWriter::close()
{
	return m_impl->close();
}

bool XMLWriter::writeDeclaration()
{
	return m_impl->writeDeclaration();
}

bool XMLWriter::writeDocument(shared_ptr<XMLDocument> document, bool declaration)
{
	return m_impl->writeDocument(document, declaration);
}

bool XMLWriter::writeNode(shared_ptr<XMLNode> node)
{
	return m_impl->writeNode(node);
}

bool XMLWriter::writeOpen(shared_ptr<XMLNode> node)
{
	return m_impl->writeOpen(node, false);
}

bool XMLWriter::writeContent(shared_ptr<XMLNode> node, bool childs)
{
	return m_impl->writeContent(node, childs);
}

bool XMLWriter::writeClose(shared_ptr<XMLNode> node)
{
	return m_impl->writeClose(node);
}

void XMLWriter::increaseIndent()
{
	m_impl->increaseIndent();
}

void XMLWriter::decreaseIndent()
{
	m_impl->decreaseIndent();
}

void XMLWriter::resetIndent()
{
	m_impl->resetIndent();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
