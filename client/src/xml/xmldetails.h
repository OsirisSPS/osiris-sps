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

#ifndef _OS_XML_XMLDETAILS_H
#define _OS_XML_XMLDETAILS_H

#include "ixmlhandler.h"
#include "xmlschema.h"

//////////////////////////////////////////////////////////////////////

#define OS_XERCES_NEW						new		// Xerces ha già un memory manager per le classi derivate da XMemory (vedere l'aggancio in XMLPlatformUtils::Initialize) per cui non serve richiamare le macro di allocazione/deallocazione
#undef null

#if OS_COMPILER	== OS_COMPILER_MSVC
	#pragma warning(disable : 4244)		// 'conversion from <type> to <type>, possible loss of data'
	#pragma warning(disable : 4267)		// 'conversion from <type> to <type>, possible loss of data'
#endif

//////////////////////////////////////////////////////////////////////

#include "xalanc/Include/XalanVersion.hpp"
#include "xalanc/XalanDOM/XalanDocumentFragment.hpp"
#include "xalanc/XalanTransformer/XalanTransformer.hpp"
#include "xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp"
#include "xalanc/XercesParserLiaison/XercesDOMSupport.hpp"
#include "xalanc/XercesParserLiaison/XercesParserLiaison.hpp"
#include "xalanc/XPath/Function.hpp"
#include "xalanc/XPath/XObjectFactory.hpp"
#include "xercesc/dom/DOMDocument.hpp"
#include "xercesc/dom/DOMElement.hpp"
#include "xercesc/dom/DOMException.hpp"
#include "xercesc/dom/DOMImplementation.hpp"
#include "xercesc/dom/DOMImplementationRegistry.hpp"
#include "xercesc/dom/DOMLSOutput.hpp"
#include "xercesc/dom/DOMLSSerializer.hpp"
#include "xercesc/dom/DOMText.hpp"
#include "xercesc/framework/LocalFileFormatTarget.hpp"
#include "xercesc/framework/LocalFileInputSource.hpp"
#include "xercesc/framework/MemBufFormatTarget.hpp"
#include "xercesc/framework/MemBufInputSource.hpp"
#include "xercesc/framework/URLInputSource.hpp"
#include "xercesc/parsers/XercesDOMParser.hpp"
#include "xercesc/util/XercesDefs.hpp"
#include "xercesc/util/XMLString.hpp"
#include "xercesc/sax/EntityResolver.hpp"
#include "xercesc/sax/InputSource.hpp"
#include "xercesc/sax2/Attributes.hpp"
#include "xercesc/sax2/DefaultHandler.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"
#include "xercesc/util/BinInputStream.hpp"
#include "xercesc/util/IOException.hpp"
#include "xercesc/validators/common/Grammar.hpp"

//////////////////////////////////////////////////////////////////////

#if OS_COMPILER	== OS_COMPILER_MSVC
	#pragma warning(default : 4244)
	#pragma warning(default : 4267)
#endif

#define null OS_NULL

//////////////////////////////////////////////////////////////////////

namespace OS_NAMESPACE_NAME
{
	class IStream;
	class XMLDocument;
	class XMLNode;
}

namespace XERCES_CPP_NAMESPACE
{
	class DOMDocument;
	class DOMElement;
	class DOMException;
	class DOMImplementation;
	class SAXException;
	class XMLException;	
}

namespace XALAN_CPP_NAMESPACE
{
	class XSLTResultTarget;
	class XalanTransformer;
}

namespace xalan = XALAN_CPP_NAMESPACE;
namespace xerces = XERCES_CPP_NAMESPACE;

//////////////////////////////////////////////////////////////////////

namespace XERCES_CPP_NAMESPACE {

//////////////////////////////////////////////////////////////////////

XMLExport DOMImplementation * getImplementation();
XMLExport bool parseSource(const xerces::InputSource &source, OS_NAMESPACE_NAME::IXMLHandler *handler, InputSource *xsd);

bool exportToString(const OS_NAMESPACE_NAME::XMLDocument &document, OS_NAMESPACE_NAME::String &str);
bool exportToTarget(const OS_NAMESPACE_NAME::XMLDocument &document, xerces::XMLFormatTarget *target, const OS_NAMESPACE_NAME::String &encoding);

bool convertDocument(const OS_NAMESPACE_NAME::XMLDocument &document, xerces::DOMDocument *doc);
bool convertNode(OS_NAMESPACE_NAME::shared_ptr<OS_NAMESPACE_NAME::XMLNode> node, xerces::DOMElement *element);

//////////////////////////////////////////////////////////////////////

template <typename T>
class AutoPtr
{
	typedef T value_type;
	typedef value_type * pointer_type;
	typedef value_type & reference_type;

// Construction
public:
	AutoPtr(T *ptr)
	{
		OS_ASSERT(ptr != null);
		m_ptr = ptr;
	}

	~AutoPtr()
	{
		m_ptr->release();
		m_ptr = null;
	}

// Operators
public:
	inline operator pointer_type() { return m_ptr; }
	inline operator reference_type() { return *m_ptr; }

	inline reference_type operator *() { return *m_ptr; }
	inline pointer_type operator ->() { return m_ptr; }

private:
	T *m_ptr;
};

//////////////////////////////////////////////////////////////////////

} // xerces

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IXMLHandler::impl : public xerces::DefaultHandler
{
	typedef xerces::DefaultHandler HandlerBase;

// Construction
public:
	impl(IXMLHandler *handler, shared_ptr<XMLSchema> schema);
	virtual ~impl();

// Attributes
public:
	inline shared_ptr<XMLSchema> getSchema() const;
	inline void setSchema(shared_ptr<XMLSchema> schema);

	inline bool getStopParser() const;
	inline void setStopParser(bool stopParser);

// xerces::DefaultHandler overrides
public:
	virtual void startDocument();
	virtual void endDocument();

	virtual void startElement(const XMLCh * const uri, const XMLCh * const localname, const XMLCh* const qname, const xerces::Attributes &attrs);
    virtual void endElement(const XMLCh * const uri, const XMLCh * const localname, const XMLCh* const qname);
	virtual void characters(const XMLCh * const chars, const XMLSize_t length);

	virtual void warning(const xerces::SAXParseException &e);
	virtual void error(const xerces::SAXParseException &e);
	virtual void fatalError(const xerces::SAXParseException &e);

private:
	IXMLHandler *m_handler;
	shared_ptr<XMLSchema> m_schema;
	bool m_stopParser;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<XMLSchema> IXMLHandler::impl::getSchema() const { return m_schema; }
inline void IXMLHandler::impl::setSchema(shared_ptr<XMLSchema> schema) { m_schema = schema; }

inline bool IXMLHandler::impl::getStopParser() const { return m_stopParser; }
inline void IXMLHandler::impl::setStopParser(bool stopParser) { m_stopParser = stopParser; }

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<XMLSchema>
{
// Construction
public:
	pimpl();
	~pimpl();

// Attributes
public:
	inline xerces::InputSource * getSource() const;

// Operations
public:
	void parseFile(const String &filename);
	void parseString(const String &str);
	void parseBuffer(const Buffer &buffer);

private:
	scoped_ptr<xerces::InputSource> m_source;
};

//////////////////////////////////////////////////////////////////////

inline xerces::InputSource * pimpl<XMLSchema>::getSource() const { return m_source.get(); }

//////////////////////////////////////////////////////////////////////

class XMLStreamInput : public xerces::BinInputStream
{
// Construction
public:
	XMLStreamInput(shared_ptr<IStream> stream);
	virtual ~XMLStreamInput();

// Attributes
public:
	inline shared_ptr<IStream> getStream() const;

// BinInputStream interface
public:
	virtual XMLFilePos curPos() const;
	virtual XMLSize_t readBytes(XMLByte * const toFill, const XMLSize_t maxToRead);
	virtual const XMLCh * getContentType() const;

private:
	shared_ptr<IStream> m_stream;
};

//////////////////////////////////////////////////////////////////////

class XMLStreamInputSource : public xerces::InputSource
{
// Construction
public:
	XMLStreamInputSource(shared_ptr<IStream> stream);
	virtual ~XMLStreamInputSource();

// Attributes
public:
	inline shared_ptr<IStream> getStream() const;

// InputSource interface
public:
	virtual xerces::BinInputStream * makeStream() const;

private:
	shared_ptr<IStream> m_stream;
};

//////////////////////////////////////////////////////////////////////

class XMLStreamFormatTarget : public xerces::XMLFormatTarget
{
	typedef xerces::XMLFormatTarget FormatTargetBase;
	
// Construction
public:
	XMLStreamFormatTarget(shared_ptr<IStream> stream);
	virtual ~XMLStreamFormatTarget();

// xerces::XMLFormatTarget interface
public:
	virtual void writeChars(const XMLByte * const toWrite, const XMLSize_t count, xerces::XMLFormatter * const formatter);

// xerces::XMLFormatTarget overrides
public:
    virtual void flush();

private:
	shared_ptr<IStream> m_stream;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_XML_XMLDETAILS_H
