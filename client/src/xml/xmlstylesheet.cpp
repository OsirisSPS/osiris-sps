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
#include "xmlstylesheet.h"

#include "dataitem.h"
#include "filesystem.h"
#include "ixmlfunction.h"
#include "ixmlresolver.h"
#include "log.h"
#include "platformmanager.h"
#include <sstream>
#include "xmldetails.h"
#include "xmldocument.h"
#include "xmlmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

// Questa funzione serve per risolvere la mancanza di escaping nelle stringhe XPath,
// mancanza (che forse verrà risolta in XPath 2) che non permette l'uso dei quote singoli e doppi contemporaneamente nella stessa stringa.
// http://groups.google.it/group/comp.text.xml/browse_thread/thread/4967ba4ce7dc94ce/527a1312d774b83d?lnk=st&q=xpath+string+quotes&rnum=9&hl=it#527a1312d774b83d
// In:  alfa 'beta' gamma \"delta\" \"test\" 'yota' \"reo\" 'peo' ' \" meo
// Out: concat("alfa 'beta' gamma ",'"delta" "test" ',"'yota' ",'"reo" ',"'peo' ' ",'" meo')
static String xpathStringToExpresson(const String &str)
{
	String out;
	String::size_type posParse = 0;

	uint32 index=0;
	for(;;)
	{
		if(index != 0)
			out += _S(",");

		String::size_type posSingleQuote = str.find(_S("'"), posParse);
		String::size_type posDoubleQuote = str.find(_S("\""), posParse);
		if(posSingleQuote == String::npos)
		{
			out += _S("'") + str.substr(posParse) + _S("'");
			if(index == 0)
				return out;		// Se non ci sono ' restituisce 'str'

			break;
		}
		else if(posDoubleQuote == String::npos)
		{
			out += _S("\"") + str.substr(posParse) + _S("\"");
			if(index == 0)
				return out;		// Se non ci sono " restituisce "str"

			break;
		}
		else
		{
			if(posSingleQuote < posDoubleQuote)
			{
				out += _S("\"") + str.substr(posParse, posDoubleQuote-posParse) + _S("\"");
				posParse = posDoubleQuote;
			}
			else
			{
				out += _S("'") + str.substr(posParse, posSingleQuote-posParse) + _S("'");
				posParse = posSingleQuote;
			}
		}

		index++;
	}

	return _S("concat(") + out + _S(")");
}

//////////////////////////////////////////////////////////////////////

class XMLStylesheet::Context : public IXMLFunction::IContext
{
	typedef IXMLFunction::IContext ContextBase;

// Construction
public:
	Context(shared_ptr<IXMLFunction> function, xalan::XPathExecutionContext &context, xalan::XalanNode *node, const xalan::LocatorType *locator);
	virtual ~Context();

// Attributes
public:
	inline xalan::XPathExecutionContext & getImpl() const;
	inline const xalan::XalanNode * getNode() const;
	inline const xalan::LocatorType * getLocator() const;

// IContext interface
public:
	virtual void onError(const String &error) const;

private:
	xalan::XPathExecutionContext &m_impl;
	xalan::XalanNode *m_node;
	const xalan::LocatorType *m_locator;
};

//////////////////////////////////////////////////////////////////////

inline xalan::XPathExecutionContext & XMLStylesheet::Context::getImpl() const { return m_impl; }
inline const xalan::XalanNode * XMLStylesheet::Context::getNode() const { return m_node; }
inline const xalan::LocatorType * XMLStylesheet::Context::getLocator() const { return m_locator; }

//////////////////////////////////////////////////////////////////////

XMLStylesheet::Context::Context(shared_ptr<IXMLFunction> function, xalan::XPathExecutionContext &context, xalan::XalanNode *node, const xalan::LocatorType *locator) : ContextBase(function),
																																									   m_impl(context),
																																									   m_node(node),
																																									   m_locator(locator)
{

}

XMLStylesheet::Context::~Context()
{

}

void XMLStylesheet::Context::onError(const String &error) const
{
	xalan::XalanDOMString str(m_impl.getMemoryManager());
	str.assign(error.c_str());
	// TODO: nella revisione corrente se m_locator->getSystemId() == null (ad esempio quando l'xsl sorgente è una stringa) causa un'assert nel costruttore di XSLException
	if(m_locator->getSystemId() != null)
	{
		m_impl.problem(xalan::XPathExecutionContext::eXSLTProcessor, xalan::XPathExecutionContext::eError, str, m_locator, m_node);
	}
	else
	{
		str.append(String::format(_S(" (line: %d, column: %d)").c_str(), static_cast<uint32>(m_locator->getLineNumber()), static_cast<uint32>(m_locator->getColumnNumber())).c_str());	
		m_impl.problem(xalan::XPathExecutionContext::eXSLTProcessor, xalan::XPathExecutionContext::eError, str, m_node);
	}
}

//////////////////////////////////////////////////////////////////////

class XMLStylesheet::Callback : public xalan::Function
{
    typedef xalan::Function FunctionBase;

// Construction
public:
	Callback(shared_ptr<IXMLFunction> function, shared_ptr<XMLStylesheet> stylesheet);
	Callback(const Callback &second);
	virtual ~Callback();

private:
	void _construct(shared_ptr<IXMLFunction> function, shared_ptr<XMLStylesheet> stylesheet);

// Attributes
public:
	inline shared_ptr<IXMLFunction> getFunction() const;
	inline shared_ptr<XMLStylesheet> getStylesheet() const;

// Function interface
public:
	// Esegue la funzione
    virtual xalan::XObjectPtr execute(xalan::XPathExecutionContext &executionContext, xalan::XalanNode *node, const XObjectArgVectorType &arguments, const xalan::LocatorType *locator) const;
	// Clona la funzione
	virtual xalan::Function * clone(xalan::MemoryManagerType &theManager) const;

protected:
	// Callback richiamato se alla funzione non viene passato il numero corretto di parametri
	virtual const xalan::XalanDOMString & getError(xalan::XalanDOMString &theResult) const;

// Operations
private:
	void _extractNodeValueRecoursive(xalan::XalanNode *node, String &str) const;
	bool _fromXalan(xalan::XPathExecutionContext &executionContext, const XObjectArgVectorType &input, IXMLFunction::Arguments &output) const;
	bool _fromXalan(xalan::XPathExecutionContext &executionContext, const xalan::XObjectPtr &input, DataItem &output) const;
	bool _toXalan(xalan::XPathExecutionContext &context, const DataItem &input, xalan::XObjectPtr &output) const;

private:
	weak_ptr<XMLStylesheet> m_stylesheet;
	shared_ptr<IXMLFunction> m_function;

private:
	static xalan::XObjectPtr nullValue;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IXMLFunction> XMLStylesheet::Callback::getFunction() const { return m_function; }
inline shared_ptr<XMLStylesheet> XMLStylesheet::Callback::getStylesheet() const { return m_stylesheet.lock(); }

//////////////////////////////////////////////////////////////////////

xalan::XObjectPtr XMLStylesheet::Callback::nullValue(null);

//////////////////////////////////////////////////////////////////////

XMLStylesheet::Callback::Callback(shared_ptr<IXMLFunction> function, shared_ptr<XMLStylesheet> stylesheet)
{
	_construct(function, stylesheet);
}

XMLStylesheet::Callback::Callback(const Callback &second) : FunctionBase(second)
{
	_construct(second.getFunction(), second.getStylesheet());
}

XMLStylesheet::Callback::~Callback()
{

}

void XMLStylesheet::Callback::_construct(shared_ptr<IXMLFunction> function, shared_ptr<XMLStylesheet> stylesheet)
{
	OS_ASSERT(function != null);
	m_function = function;

	OS_ASSERT(stylesheet != null);
	m_stylesheet = stylesheet;
}

xalan::XObjectPtr XMLStylesheet::Callback::execute(xalan::XPathExecutionContext &executionContext, xalan::XalanNode *node, const XObjectArgVectorType &arguments, const xalan::LocatorType *locator) const
{
	Context context(m_function, executionContext, node, locator);

	uint32 args_count = static_cast<uint32>(arguments.size());
	// Controlla se il numero di parametri passato non è accettabile
	if(args_count < m_function->requiredArgs() || args_count > m_function->totalArgs())
	{
		context.reportError(String::format(_S("function does not take %d parameter(s)").c_str(), args_count));
		return nullValue;
	}

	IXMLFunction::Arguments args;
	// Converte gli argomenti passati alla funzione
	if(_fromXalan(executionContext, arguments, args) == false)
	{
		context.reportError(_S("unconvertible value(s)"));
		return nullValue;
	}

	DataItem rval;
	// Esegue la funzione
	if(m_function->execute(context, args, rval) == false)
	{
		if(context.getErrors() == 0)
			// La funzione ha fallito ma non è stato segnalato alcun errore, pertanto ne riporta uno generico
			context.reportError();

		return nullValue;
	}

	xalan::XObjectPtr result;
    if(_toXalan(executionContext, rval, result) == false)
	{
		context.reportError(_S("unconvertible value(s)"));
		return nullValue;
	}

	return result;
}

xalan::Function * XMLStylesheet::Callback::clone(xalan::MemoryManagerType &theManager) const
{
	return xalan::XalanCopyConstruct(theManager, *this);
}

const xalan::XalanDOMString & XMLStylesheet::Callback::getError(xalan::XalanDOMString &theResult) const
{
	OS_ASSERTFALSE();	// Questa funzione non dovrebbe mai essere richiamata (la base non viene mai richiamata nell'override dell'execute)

	// Riporta un errore generico sulla funzione
	theResult.assign(String::format(_S("'%S' generic error").c_str(), m_function->getName().c_str()).c_str());
	return theResult;
}

void XMLStylesheet::Callback::_extractNodeValueRecoursive(xalan::XalanNode *node, String &str) const
{
	if(node == null)
		return;

	// Estra il valore del nodo
	str += static_cast<String>(node->getNodeValue().c_str());

	// Scorre i figli del nodo
	for(xalan::XalanNode *child = node->getFirstChild(); child != null; child = child->getNextSibling())
	{
		// Estra il valore dei figli del nodo
		_extractNodeValueRecoursive(child, str);
	}
}

bool XMLStylesheet::Callback::_fromXalan(xalan::XPathExecutionContext &executionContext, const XObjectArgVectorType &input, IXMLFunction::Arguments &output) const
{
	OS_ASSERT(output.empty());

	for(size_t i = 0; i != input.size(); i++)
	{
		DataItem value;
		if(_fromXalan(executionContext, input[i], value) == false)
			return false;

		output.push_back(value);
	}

	return true;
}

bool XMLStylesheet::Callback::_fromXalan(xalan::XPathExecutionContext &executionContext, const xalan::XObjectPtr &input, DataItem &output) const
{
	switch(input->getType())
	{
	case xalan::XObject::eTypeNull:						output = DataItem::EMPTY;
														return true;

	case xalan::XObject::eTypeBoolean:					output = input->boolean(executionContext);
														return true;

	case xalan::XObject::eTypeNumber:					output = input->num();
														return true;

	case xalan::XObject::eTypeString:					output = static_cast<String>(input->str().c_str());
														return true;

	case xalan::XObject::eTypeNodeSet:					{
															const xalan::NodeRefListBase &list = input->nodeset();
															size_t length = list.getLength();

															String value;
															for(size_t i = 0; i < length; i++)
															{
																_extractNodeValueRecoursive(list.item(static_cast<xalan::NodeRefListBase::size_type>(i)), value);
															}

															output = value;
															return true;
														}

														return false;

	case xalan::XObject::eTypeResultTreeFrag:			{
															String value;
															for(xalan::XalanNode *node = input->rtree().getFirstChild(); node != null; node = node->getNextSibling())
															{
																_extractNodeValueRecoursive(node, value);
															}

															output = value;
															return true;
														}

														return false;

	case xalan::XObject::eTypeUnknown:
	case xalan::XObject::eTypeUserDefined:
	case xalan::XObject::eTypeStringReference:
	case xalan::XObject::eTypeStringAdapter:
	case xalan::XObject::eTypeStringCached:
	case xalan::XObject::eTypeXTokenNumberAdapter:
	case xalan::XObject::eTypeXTokenStringAdapter:
	case xalan::XObject::eTypeNodeSetNodeProxy:
	case xalan::XObject::eUnknown:						// Tipi non convertibili
														break;

	default:											// Tipologia non riconosciuta
														OS_ASSERTFALSE();
														break;
	}

	return false;
}

bool XMLStylesheet::Callback::_toXalan(xalan::XPathExecutionContext &context, const DataItem &input, xalan::XObjectPtr &output) const
{
	switch(input.getType())
	{
	case DataItem::vt_null:			output = nullValue;
									return true;

	case DataItem::vt_bool:			output = context.getXObjectFactory().createBoolean(static_cast<bool>(input));
									return true;

	case DataItem::vt_byte:			output = context.getXObjectFactory().createNumber(static_cast<byte>(input));
									return true;

	case DataItem::vt_int32:		output = context.getXObjectFactory().createNumber(static_cast<int32>(input));
									return true;

	case DataItem::vt_uint32:		output = context.getXObjectFactory().createNumber(static_cast<uint32>(input));
									return true;

	case DataItem::vt_double:		output = context.getXObjectFactory().createNumber(static_cast<double>(input));
									return true;

	case DataItem::vt_string:		output = context.getXObjectFactory().createString((static_cast<String>(input)).c_str());
									return true;

	case DataItem::vt_int64:
	case DataItem::vt_uint64:
	case DataItem::vt_buffer:
	case DataItem::vt_datatree:
									// Tipi non convertibili
									break;

	default:						// Tipologia non riconosciuta
									OS_ASSERTFALSE();
									break;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////

struct XMLStylesheet::impl
{
	class Resolver : public xerces::EntityResolver,
					 public AllocWrapper<Resolver>
	{
	public:
		Resolver(shared_ptr<XMLStylesheet> stylesheet) : m_stylesheet(stylesheet)
		{

		}

		virtual ~Resolver()
		{

		}

		shared_ptr<XMLStylesheet> getStylesheet() const
		{
			return m_stylesheet.lock();
		}

		xerces::InputSource * resolveEntity(const XMLCh *const publicId, const XMLCh *const systemId)
		{
			String path = systemId;

			shared_ptr<XMLStylesheet> stylesheet = getStylesheet();
			if(stylesheet != null)
			{
				shared_ptr<IXMLResolver> entityResolver = stylesheet->getResolver();
				if(entityResolver != null)
				{
					String resolved;
					if(entityResolver->resolve(path, resolved))
					{
						path = resolved;
					}
				}
			}

			return OS_XERCES_NEW xalan::XSLTInputSource(path.c_str());
		}

	private:
		weak_ptr<XMLStylesheet> m_stylesheet;
		String m_path;
	};

	impl() : compiledStylesheet(null)
	{

	}

	~impl()
	{
		reset();
	}

	String getLastError()
	{
		if(transformer != null)
            return transformer->getLastError();

        return String::EMPTY;
	}	

	bool parseFile(shared_ptr<XMLStylesheet> stylesheet, const String &filename)
	{
		xalan::XSLTInputSource source(filename.c_str());
		return doParse(stylesheet, source);
	}

	bool parseString(shared_ptr<XMLStylesheet> stylesheet, const String &str)
	{
		// TODO: errato, da correggere
		std::string s = str.to_utf8();
        std::istringstream stream(s);
		return doParse(stylesheet, stream);
	}

	template <typename T>
	bool doParse(shared_ptr<XMLStylesheet> stylesheet, T &source)
	{
		OS_ASSERT(stylesheet != null);
		reset();

		OS_ASSERT(transformer == null);
		transformer.reset(OS_NEW_T(xalan::XalanTransformer));		// N.B.: XalanTransformer stranamente non è derivato da XMemory e quindi va allocato tramite OS_NEW_T

		OS_ASSERT(entityResolver.get() == null);
		entityResolver.reset(OS_NEW Resolver(stylesheet));
		transformer->setEntityResolver(entityResolver.get());

		try
		{
			if(transformer->compileStylesheet(source, compiledStylesheet) != 0)
			{
				stylesheet->onError(getLastError());
				return false;
			}
		}
		catch(std::exception &e)
		{
			stylesheet->onError(e.what());
			return false;
		}

		return true;
	}

	void reset()
	{
		if(transformer != null)
		{
			if(compiledStylesheet != null)
			{
				// Questa parte si potrebbe anche omettere visto che dovrebbe essere richiamata in automatico nel distruttore di XalanTransformer
				transformer->destroyStylesheet(compiledStylesheet);
				compiledStylesheet = null;
			}

			transformer.reset();
		}

		entityResolver.reset();
	}

	bool transform(shared_ptr<XMLStylesheet> stylesheet, shared_ptr<XMLDocument> document, const xalan::XSLTResultTarget &target)
	{
		if(compiledStylesheet == null)
			return false;

		xerces::AutoPtr<xerces::DOMDocument> doc = xerces::getImplementation()->createDocument();
		if(xerces::convertDocument(*document, doc) == false)
			return false;

		xalan::XercesParserLiaison parserLiaison;
		xalan::XercesDOMSupport DOMSupport(parserLiaison);
		xalan::XercesDOMWrapperParsedSource source(doc, parserLiaison, DOMSupport);

		stylesheet->onRegisterFunctions();
		// Inizializza lo stylesheet
		_initialize(stylesheet);

		try
		{
			// Esegue la trasformazione
			if(transformer->transform(source, compiledStylesheet, target) != 0)
			{
				stylesheet->onError(getLastError());
				return false;
			}
		}
		catch(std::exception &e)
		{
			stylesheet->onError(e.what());
			return false;
		}

		return true;
	}

	void _initialize(shared_ptr<XMLStylesheet> stylesheet)
	{
		OS_ASSERT(stylesheet != null);
		OS_ASSERT(transformer != null);

		const Functions &externalFunctions = stylesheet->getFunctions();
		// Registra le funzioni di callback
		for(Functions::iterator i = externalFunctions.begin(); i != externalFunctions.end(); ++i)
		{
			shared_ptr<IXMLFunction> function = i->second;
			// Registra la funzione corrente
			transformer->installExternalFunction(xalan::XalanDOMString(function->getNamespace().c_str()), xalan::XalanDOMString(function->getName().c_str()), Callback(function, stylesheet));
		}

		const StringCollection<String> &stylesheetParams = stylesheet->getParams();
		// Setta i parametri
		for(StringCollection<String>::iterator i = stylesheetParams.begin(); i != stylesheetParams.end(); ++i)
		{
			// Setta il parametro corrente
			transformer->setStylesheetParam(xalan::XalanDOMString(i->first.c_str()), xalan::XalanDOMString(i->second->c_str()));
		}
	}

	StringCollection<String> params;		// Parametri dello stylesheet
	Functions functions;					// Funzioni esterne registrate
	error_callback eventError;				// Evento per la segnalazione degli errori
	shared_ptr<IXMLResolver> resolver;		// Risolve i path dei files importati/inclusi

	scoped_ptr<xalan::XalanTransformer, os_delete_t> transformer;
	const xalan::XalanCompiledStylesheet *compiledStylesheet;
	scoped_ptr<Resolver> entityResolver;
};

//////////////////////////////////////////////////////////////////////

XMLStylesheet::XMLStylesheet()
{

}

XMLStylesheet::~XMLStylesheet()
{

}

const XMLStylesheet::Functions & XMLStylesheet::getFunctions() const
{
	return m_impl->functions;
}

bool XMLStylesheet::hasFunction(const String &name) const
{
	return m_impl->functions.exists(name);
}

shared_ptr<IXMLResolver> XMLStylesheet::getResolver() const
{
	return m_impl->resolver;
}

void XMLStylesheet::setResolver(shared_ptr<IXMLResolver> resolver)
{
	m_impl->resolver = resolver;
}

XMLStylesheet::error_callback & XMLStylesheet::getEventError()
{
	return m_impl->eventError;
}

const StringCollection<String> & XMLStylesheet::getParams() const
{
	return m_impl->params;
}

const String & XMLStylesheet::getParam(const String &name) const
{
	return m_impl->params.get(name);
}

void XMLStylesheet::setParam(const String &name, const String &value)
{
	m_impl->params.set(name, value);
}

void XMLStylesheet::setBoolParam(const String &name, bool value)
{
	m_impl->params.set(name, value ? _S("true()") : _S("false()"));
}

void XMLStylesheet::setStringParam(const String &name, const String &value)
{
	// Finchè lo xalan accetta come parametri un'espressione xpath è necessario
	// convertire la stringa in espressione, quando verrà modificato l'SDK si
	// potrà settare il parametro ad un XString

	m_impl->params.set(name, xpathStringToExpresson(value));
}

bool XMLStylesheet::registerFunction(shared_ptr<IXMLFunction> function)
{
	OS_ASSERT(function != null);
	OS_ASSERT(function->getName().empty() == false);

	if(function == null || function->getName().empty() || hasFunction(function->getName()))
		return false;

	return m_impl->functions.push_back(function->getName(), function);
}

bool XMLStylesheet::unregisterFunction(shared_ptr<IXMLFunction> function)
{
	OS_ASSERT(function != null);
	return m_impl->functions.remove(function->getName());
}

bool XMLStylesheet::applyToFile(shared_ptr<XMLDocument> document, const String &filename)
{
	xalan::XSLTResultTarget target(filename.c_str());
	return m_impl->transform(get_this_ptr(), document, target);
}

bool XMLStylesheet::applyToString(shared_ptr<XMLDocument> document, String &str)
{
	std::stringstream stream;
	xalan::XSLTResultTarget target(&stream);
	if(m_impl->transform(get_this_ptr(), document, target) == false)
		return false;

	// TODO: errato, da correggere
//	str = stream.str();
	str.from_utf8(stream.str());
	return true;
}

String XMLStylesheet::applyToString2(shared_ptr<XMLDocument> document)
{
	String out;
	if(applyToString(document, out))
		return out;
	else
		return String::EMPTY;
}

String XMLStylesheet::transform(shared_ptr<XMLDocument> document)
{
	String str;
	applyToString(document, str);
	return str;
}

void XMLStylesheet::onRegisterFunctions()
{
	// Nessuna funzione nello Stylesheet di base
}

void XMLStylesheet::onError(const String &description)
{
	if(description.empty() == false)
	{
		// Lancia l'evento di errore
		m_impl->eventError(description);
		// Riporta l'errore
		OS_LOG_ERROR(description);
	}
}

bool XMLStylesheet::parseFile(const String &filename)
{
	return m_impl->parseFile(get_this_ptr(), filename);
}

bool XMLStylesheet::parseString(const String &str)
{
	return m_impl->parseString(get_this_ptr(), str);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
