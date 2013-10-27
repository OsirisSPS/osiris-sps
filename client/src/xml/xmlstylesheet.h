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

#ifndef _OS_XML_XMLSTYLESHEET_H
#define _OS_XML_XMLSTYLESHEET_H

#include "boost/noncopyable.hpp"
#include "boost/signals.hpp"
#include "collections.h"
#include "enable_this_ptr.h"
#include "base/object.h"
#include "pimpl_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IXMLFunction;
class IXMLResolver;
class XMLDocument;

//////////////////////////////////////////////////////////////////////

class XMLExport XMLStylesheet : public Object,
								public enable_this_ptr<XMLStylesheet>,
								public boost::noncopyable
{
public:
	typedef boost::signal<void (const String &)> error_callback;
	typedef Collections<String, IXMLFunction>::shared_collection Functions;

// Internal
private:
	class Context;
	class Callback;

// Construction
public:
	XMLStylesheet();
	virtual ~XMLStylesheet();

// Attributes
public:
	const Functions & getFunctions() const;
	bool hasFunction(const String &name) const;

	shared_ptr<IXMLResolver> getResolver() const;
	void setResolver(shared_ptr<IXMLResolver> resolver);

	error_callback & getEventError();

	const StringCollection<String> & getParams() const;

	const String & getParam(const String &name) const;
	void setParam(const String &name, const String &value);
	void setBoolParam(const String &name, bool value);
	void setStringParam(const String &name, const String &value);

// Operations
public:
	bool registerFunction(shared_ptr<IXMLFunction> function);
	bool unregisterFunction(shared_ptr<IXMLFunction> function);

	bool applyToFile(shared_ptr<XMLDocument> document, const String &filename);
	bool applyToString(shared_ptr<XMLDocument> document, String &str);
	String applyToString2(shared_ptr<XMLDocument> document);

	String transform(shared_ptr<XMLDocument> document);

	// Carica un xsl da un file
	bool parseFile(const String &filename);
	// Carica un xsl da una stringa
	bool parseString(const String &str);

// Overridables
protected:
	virtual void onRegisterFunctions();
	virtual void onError(const String &description);

protected:
	struct impl;
	pimpl_ptr<impl> m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_XML_XMLSTYLESHEET_H
