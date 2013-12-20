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
#include "pythoncodecontext.h"

#include "boost/thread/recursive_mutex.hpp"
#include "lock.h"
#include "pythoncommon.h"
#include "pythonengine.h"
#include "pythoninterpreter.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

PythonCodeContext::PythonCodeContext() : m_fileSource(false),
										 m_parseSource(false),
										 m_interpreter(PythonEngine::instance()->createInterpreter())		// N.B.: l'interprete a livello di extension deve rimanere caricato per tutta la durata della plugin altrimenti eventuali oggetti registrati potrebbero essere deallocati
{

}

PythonCodeContext::~PythonCodeContext()
{

}

void PythonCodeContext::executeCallback(const boost::python::object &dictionary, shared_ptr<IExtensionsExtension> extension, bool loaded)
{
	std::string function = loaded ? OS_PYTHON_LOAD_ENTRY_POINT : OS_PYTHON_UNLOAD_ENTRY_POINT;
	if(dictionary.contains(function))
	{
		boost::python::list args;
		args.append(extension); 	
		dictionary[function](args);
	}
}

bool PythonCodeContext::parseFile(const String &filename)
{
	// TODO: "compilare" il file generando il pyc?

	m_source = filename;
	m_fileSource = true;
	m_parseSource = true;

	return true;
}

bool PythonCodeContext::parseString(const String &str)
{
	m_source = str;
	m_fileSource = false;
	m_parseSource = true;

	return true;
}

bool PythonCodeContext::execute(shared_ptr<IExtensionsExtension> extension, bool loaded)
{
	if(m_interpreter == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	if(m_parseSource)
	{
		if(m_interpreter->execute(m_source, m_fileSource) == false)
			return false;

		m_parseSource = false;
	}

	return m_interpreter->invoke(boost::bind(&PythonCodeContext::executeCallback, _1, extension, loaded));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
