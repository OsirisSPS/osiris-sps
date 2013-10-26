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

#ifndef _OS_XML_IXMLFUNCTION_H
#define _OS_XML_IXMLFUNCTION_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DataItem;

//////////////////////////////////////////////////////////////////////

class XMLExport IXMLFunction : public Object
{
public:
	class XMLExport IContext
	{
	// Construction
	public:
		IContext(shared_ptr<IXMLFunction> function);
		virtual ~IContext();

	// Attributes
	public:
		inline const uint32 & getErrors() const;

	// Operations
	public:
		void reportError(const String &message = String::EMPTY);

	// Interface
	protected:
		virtual void onError(const String &error) const = 0;

	protected:
		uint32 m_errors;
		shared_ptr<IXMLFunction> m_function;
	};

	typedef vector<DataItem>::type Arguments;		// indice-valore dell'argomento

// Construction
public:
	IXMLFunction(const String &name, const String &ns, uint32 required_args = 0, uint32 optionals_args = 0);
	virtual ~IXMLFunction();

// Attributes
public:
	// Restituisce il namespace della funzione
	inline const String & getNamespace() const;
	// Restituisce il nome della funzione
	inline const String & getName() const;

	// Restituisce il numero minimo di parametri richiesti dalla funzione
	inline uint32 requiredArgs() const;
	// Restituisce il numero totale di parametri delle funzione (alcuni possono essere opzionali)
	inline uint32 totalArgs() const;

// Interface
public:
	// Esegue la funzione
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result) = 0;

protected:
	const String m_namespace;		// Namespace della funzione
	const String m_name;			// Nome della funzione
	const uint32 m_required_args;	// Numero di parametri minimo della funzione
	const uint32 m_optionals_args;	// Numero di parametri opzionali della funzione
};

//////////////////////////////////////////////////////////////////////

inline const uint32 & IXMLFunction::IContext::getErrors() const { return m_errors; }

//////////////////////////////////////////////////////////////////////

inline const String & IXMLFunction::getNamespace() const { return m_namespace; }
inline const String & IXMLFunction::getName() const { return m_name; }

inline uint32 IXMLFunction::requiredArgs() const { return m_required_args; }
inline uint32 IXMLFunction::totalArgs() const { return m_required_args + m_optionals_args; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_XML_IXMLFUNCTION_H
