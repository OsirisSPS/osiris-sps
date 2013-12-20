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
#include "xmldetails.h"

#include "buffer.h"
#include "collections.h"
#include "istream.h"
#include "log.h"
#include "platformmanager.h"
#include "thirdpartylibrariesreporter.h"
#include "thirdpartylibrary.h"
#include "xmlattribute.h"
#include "xmlattributes.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static class XMLGlobals
{
// Internal
private:
	class XMLMemoryManager : public XERCES_CPP_NAMESPACE::MemoryManager
	{
	public:
		XMLMemoryManager();
		virtual ~XMLMemoryManager();

	// XERCES_CPP_NAMESPACE::MemoryManager interface
	public:
		virtual void * allocate(XMLSize_t size);
	    virtual void deallocate(void *p);
		virtual XERCES_CPP_NAMESPACE::MemoryManager * getExceptionMemoryManager();
	};

// Construction
public:
	XMLGlobals()
	{
		// Assicura che il gestore della memoria sia inizializzato in quanto le funzioni seguenti effettuano delle allocazioni di memoria (g_xmlGlobals è statica e potrebbe essere inizializzata prima di MemoryTracker)
#ifdef OS_ENABLE_MEMORY_TRACKER
		MemoryTracker::instance();
#endif // OS_ENABLE_MEMORY_TRACKER

		xerces::XMLPlatformUtils::Initialize(XERCES_CPP_NAMESPACE::XMLUni::fgXercescDefaultLocale, nullptr, nullptr, &m_memoryManager);
		xalan::XalanTransformer::initialize(m_memoryManager);

		registerLibraries();
	}

	~XMLGlobals()
	{
		xalan::XalanTransformer::terminate();
		xerces::XMLPlatformUtils::Terminate();
		xalan::XalanTransformer::ICUCleanUp();
	}

// Operations
private:
	void registerLibraries()
	{
		{
			shared_ptr<ThirdPartyLibrary> xalanLibrary = ThirdPartyLibrariesReporter::instance()->addLibrary();
			xalanLibrary->setName("Xalan");
			xalanLibrary->setVersion(XALAN_FULLVERSIONDOT);
			xalanLibrary->setDescription("Xalan is an XSLT processor for transforming XML documents into HTML, text, or other XML document types");
			xalanLibrary->setUrl("http://xml.apache.org/xalan-c/");
			xalanLibrary->setLicense("Apache License");
			xalanLibrary->setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0");
		}

		{
			shared_ptr<ThirdPartyLibrary> xercesLibrary = ThirdPartyLibrariesReporter::instance()->addLibrary();
			xercesLibrary->setName("Xerces");
			xercesLibrary->setVersion(XERCES_FULLVERSIONDOT);
			xercesLibrary->setDescription("Xerces-C++ is a validating XML parser written in a portable subset of C++");
			xercesLibrary->setUrl("http://xerces.apache.org/xerces-c/");
			xercesLibrary->setLicense("Apache License");
			xercesLibrary->setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0");
		}
	}
private:
	XMLMemoryManager m_memoryManager;
}g_xmlGlobals;

//////////////////////////////////////////////////////////////////////

XMLGlobals::XMLMemoryManager::XMLMemoryManager()
{

}
		
XMLGlobals::XMLMemoryManager::~XMLMemoryManager()
{

}

void * XMLGlobals::XMLMemoryManager::allocate(XMLSize_t size)
{
	return OS_ALLOCATE(size);
}

void XMLGlobals::XMLMemoryManager::deallocate(void *p)
{
	OS_DEALLOCATE(p);
}

XERCES_CPP_NAMESPACE::MemoryManager * XMLGlobals::XMLMemoryManager::getExceptionMemoryManager()
{
	return this;
}

//////////////////////////////////////////////////////////////////////

IXMLHandler::impl::impl(IXMLHandler *handler, shared_ptr<XMLSchema> schema) : m_handler(handler),
																			  m_schema(schema),
																			  m_stopParser(false)
{
	OS_ASSERT(handler != nullptr);	
}

IXMLHandler::impl::~impl()
{

}

void IXMLHandler::impl::startDocument()
{
	HandlerBase::startDocument();

	m_handler->onStartDocument();
}

void IXMLHandler::impl::endDocument()
{
	HandlerBase::endDocument();

	m_handler->onEndDocument();
}

void IXMLHandler::impl::startElement(const XMLCh * const uri, const XMLCh * const localname, const XMLCh* const qname, const xerces::Attributes &attrs)
{
	HandlerBase::startElement(uri, localname, qname, attrs);

	StringCollection<String> attributes;
	XMLSize_t count = attrs.getLength();
	for(XMLSize_t i = 0; i < count; i++)
	{
		attributes.set(attrs.getLocalName(i), attrs.getValue(i));
	}

	m_handler->onStartElement(localname, attributes);
}

void IXMLHandler::impl::endElement(const XMLCh * const uri, const XMLCh * const localname, const XMLCh* const qname)
{
	HandlerBase::endElement(uri, localname, qname);

	m_handler->onEndElement(localname);
}

void IXMLHandler::impl::characters(const XMLCh * const chars, const XMLSize_t length)
{
	HandlerBase::characters(chars, length);

	m_handler->onCharacters(chars);
}

void IXMLHandler::impl::warning(const xerces::SAXParseException &e)
{
	m_handler->onWarning(e.getMessage());

	HandlerBase::warning(e);
}

void IXMLHandler::impl::error(const xerces::SAXParseException &e)
{
	m_handler->onError(e.getMessage());

	HandlerBase::error(e);
}

void IXMLHandler::impl::fatalError(const xerces::SAXParseException &e)
{
	m_handler->onError(e.getMessage());

	HandlerBase::fatalError(e);
}

//////////////////////////////////////////////////////////////////////

pimpl<XMLSchema>::pimpl()
{
	
}

pimpl<XMLSchema>::~pimpl()
{
	
}

void pimpl<XMLSchema>::parseFile(const String &filename)
{
	m_source.reset(OS_XERCES_NEW xerces::LocalFileInputSource(filename.c_str()));
}

void pimpl<XMLSchema>::parseString(const String &str)
{
	m_source.reset(OS_XERCES_NEW xerces::MemBufInputSource(reinterpret_cast<const XMLByte *>(str.buffer()), static_cast<XMLSize_t>(str.buffer_size()), "", false));
}

void pimpl<XMLSchema>::parseBuffer(const Buffer &buffer)
{
	m_source.reset(OS_XERCES_NEW xerces::MemBufInputSource(reinterpret_cast<const XMLByte *>(buffer.getData()), static_cast<XMLSize_t>(buffer.getSize()), static_cast<const XMLCh *>(nullptr), false));
}

//////////////////////////////////////////////////////////////////////

XMLStreamInput::XMLStreamInput(shared_ptr<IStream> stream) : m_stream(stream)
{
	OS_ASSERT(stream != nullptr);	
}

XMLStreamInput::~XMLStreamInput()
{

}

inline shared_ptr<IStream> XMLStreamInput::getStream() const
{
	return m_stream;
}

XMLFilePos XMLStreamInput::curPos() const
{
	return static_cast<XMLFilePos>(m_stream->position());
}

XMLSize_t XMLStreamInput::readBytes(XMLByte * const toFill, const XMLSize_t maxToRead)
{
	return m_stream->read(reinterpret_cast<byte *>(toFill), static_cast<uint32>(maxToRead));
}

const XMLCh * XMLStreamInput::getContentType() const
{
	return nullptr;
}

//////////////////////////////////////////////////////////////////////

XMLStreamInputSource::XMLStreamInputSource(shared_ptr<IStream> stream) : m_stream(stream)
{
	OS_ASSERT(stream != nullptr);	
}

XMLStreamInputSource::~XMLStreamInputSource()
{

}

inline shared_ptr<IStream> XMLStreamInputSource::getStream() const
{
	return m_stream;
}

xerces::BinInputStream * XMLStreamInputSource::makeStream() const
{
	return OS_XERCES_NEW XMLStreamInput(m_stream);
}

//////////////////////////////////////////////////////////////////////

XMLStreamFormatTarget::XMLStreamFormatTarget(shared_ptr<IStream> stream) : m_stream(stream)
{
	OS_ASSERT(stream != nullptr);
}

XMLStreamFormatTarget::~XMLStreamFormatTarget()
{

}

void XMLStreamFormatTarget::writeChars(const XMLByte * const toWrite, const XMLSize_t count, xerces::XMLFormatter * const formatter)
{
	uint32 size = static_cast<uint32>(count);
	if(m_stream->write(toWrite, size) != size)
		ThrowXML(xerces::IOException, xerces::XMLExcepts::File_CouldNotWriteToFile);
}

void XMLStreamFormatTarget::flush()
{
	FormatTargetBase::flush();

	m_stream->flush();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

namespace XERCES_CPP_NAMESPACE {

//////////////////////////////////////////////////////////////////////

class XMLPScanTokenScope
{
public:
	XMLPScanTokenScope(SAX2XMLReader *parser, XMLPScanToken &scanToken) : m_parser(parser),
																		  m_scanToken(scanToken)
	{
		OS_ASSERT(parser != nullptr);
	}

	~XMLPScanTokenScope()
	{
		m_parser->parseReset(m_scanToken);	
	}

private:
	SAX2XMLReader *m_parser;
	XMLPScanToken &m_scanToken;
};

//////////////////////////////////////////////////////////////////////

DOMImplementation * getImplementation()
{
	return DOMImplementationRegistry::getDOMImplementation(nullptr);
}

bool parseSource(const InputSource &source, OS_NAMESPACE_NAME::IXMLHandler *handler, InputSource *xsd)
{
	bool result = false;

	OS_NAMESPACE_NAME::scoped_ptr<SAX2XMLReader> parser(XMLReaderFactory::createXMLReader());

	try
	{
		if(handler != nullptr)
		{
			handler->setStopParser(false);
			parser->setContentHandler(handler->getImpl());
			parser->setErrorHandler(handler->getImpl());
		}

		if(xsd != nullptr)
		{
			// Carica lo schema
			Grammar *grammar = parser->loadGrammar(*xsd, Grammar::SchemaGrammarType, true);
			if(grammar == nullptr)
			{
				OS_NAMESPACE_NAME::String error = _S("Invalid xml schema");

				const XMLCh *id = xsd->getSystemId();
				if(id != nullptr)
				{
					error += _S(", id=");
					error += id;
				}

				OS_EXCEPTION(error.to_utf8());
			}
			else
			{
				// Abilita la validazione dello schema
				parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
				// Disabilita la validazione opzionale (solo se esiste la con dichiarazione DOCTYPE dello schema nell'xml)
				parser->setFeature(XMLUni::fgXercesDynamic, false);

				// Questi flag stranamente non sono abilitati di default, sono comunque necessari altrimenti la validazione fallisce...
				parser->setFeature(XMLUni::fgXercesCacheGrammarFromParse, true);
				parser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, true);
			}
		}

		XMLPScanToken scanToken;
		if(parser->parseFirst(source, scanToken))
		{
			XMLPScanTokenScope scanTokenScope(parser.get(), scanToken);

			while(parser->getErrorCount() == 0)
			{
				if(handler != nullptr && handler->getStopParser())
					break;

				if(parser->parseNext(scanToken) == false)
					break;
			}

			result = parser->getErrorCount() == 0;			
		}			
	}
	catch(const XMLException &e)
	{
		OS_LOG_ERROR(e.getMessage());
	}
	catch(const SAXException &e)
	{
		OS_LOG_ERROR(e.getMessage());
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(xml));
	}

	return result;
}

DOMLSSerializer * createWriter()
{
	DOMLSSerializer *writer = getImplementation()->createLSSerializer();
	DOMConfiguration *config = writer->getDomConfig();

	if(config->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
		config->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);

	return writer;
}

DOMLSOutput * createOutputStream(const OS_NAMESPACE_NAME::String &encoding)
{
	DOMLSOutput *stream = getImplementation()->createLSOutput();
	stream->setEncoding(encoding.c_str());
	return stream;
}

bool exportToString(const OS_NAMESPACE_NAME::XMLDocument &document, OS_NAMESPACE_NAME::String &str)
{
	try
	{
		AutoPtr<DOMDocument> doc = getImplementation()->createDocument();
		if(convertDocument(document, doc) == false)
			return false;

		AutoPtr<DOMLSSerializer> writer(createWriter());

		XMLCh *xmlString = writer->writeToString(doc->getDocumentElement());
		str = xmlString;
		XMLString::release(&xmlString);

		return true;
	}
	catch(const XMLException &e)
	{
		OS_LOG_ERROR(e.getMessage());
	}
	catch(const DOMException &e)
	{
		OS_LOG_ERROR(e.getMessage());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(xml));
	}

	return false;
}

bool exportToTarget(const OS_NAMESPACE_NAME::XMLDocument &document, XMLFormatTarget *target, const OS_NAMESPACE_NAME::String &encoding)
{
	try
	{
		AutoPtr<DOMDocument> doc = getImplementation()->createDocument();
		if(convertDocument(document, doc) == false)
			return false;

		AutoPtr<DOMLSSerializer> writer(createWriter());
		AutoPtr<DOMLSOutput> stream(createOutputStream(encoding));
		stream->setByteStream(target);

		writer->write(doc, stream);

		return true;
	}
	catch(const XMLException &e)
	{
		OS_LOG_ERROR(e.getMessage());
	}
	catch(const DOMException &e)
	{
		OS_LOG_ERROR(e.getMessage());
	}
	catch(...)
	{
		OS_LOG_ERROR(OS_ERR_UNKNOWN(xml));
	}

	return false;
}

bool convertDocument(const OS_NAMESPACE_NAME::XMLDocument &document, DOMDocument *doc)
{
	OS_NAMESPACE_NAME::shared_ptr<OS_NAMESPACE_NAME::XMLNode> rootNode = document.getRoot();
	if(rootNode == nullptr)
		return false;

	DOMElement *root_element = doc->createElement(rootNode->getName().c_str());
	if(root_element == nullptr)
		return false;

	doc->appendChild(root_element);
	return convertNode(rootNode, root_element);
}

bool convertNode(OS_NAMESPACE_NAME::shared_ptr<OS_NAMESPACE_NAME::XMLNode> node, DOMElement *element)
{
	if(element == nullptr)
		return false;

	OS_NAMESPACE_NAME::shared_ptr<OS_NAMESPACE_NAME::XMLAttributes> attributes = node->getAttributes();
	for(OS_NAMESPACE_NAME::XMLAttributes::const_iterator a = attributes->begin(); a != attributes->end(); ++a)
	{
		element->setAttribute((*a)->getName().c_str(), (*a)->getValue().c_str());
	}

	if(node->hasData())
	{
		element->appendChild(element->getOwnerDocument()->createTextNode(node->getData().c_str()));
	}

	OS_NAMESPACE_NAME::shared_ptr<OS_NAMESPACE_NAME::XMLNodes> nodes = node->getNodes();
	for(OS_NAMESPACE_NAME::XMLNodes::const_iterator i = nodes->begin(); i != nodes->end(); ++i)
	{
		DOMElement *child_element = element->getOwnerDocument()->createElement((*i)->getName().c_str());
		if(child_element == nullptr)
			return false;

		element->appendChild(child_element);
		if(convertNode(*i, child_element) == false)
			return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////

} // xerces

//////////////////////////////////////////////////////////////////////
