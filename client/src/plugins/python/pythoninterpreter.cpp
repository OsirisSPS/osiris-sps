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
#include "pythoninterpreter.h"

#include "boost/scoped_array.hpp"
#include "filesystem.h"
#include "pythonengine.h"
#include "pythonstate.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

PythonInterpreter::PythonInterpreter(PyThreadState *state) : m_state(state)
{
	OS_ASSERT(m_state != null);
}

PythonInterpreter::~PythonInterpreter()
{
	PythonState scope(m_state);
	
	m_dictionary.reset();
	m_module.reset();

	PyErr_Clear();
	PyGC_Collect();		// TODO: corretto? se non viene chiamato PyGC_Collect, Py_EndInterpreter viene richiamato prima della distruzione di oggetti creati nel contesto di riferimento

	Py_EndInterpreter(m_state);	
	m_state = null;
}

bool PythonInterpreter::execute(const String &source, bool fileSource, const boost::function<void(const boost::python::object &)> &defaultCallback, const boost::function<void(const boost::python::object &)> &errorCallback)
{
	// Consente l'esecuzione di più scripts in parallelo a meno che non ci sia un interprete in fase di inizializzazione (vedere le note sotto PythonEngine::allocateInterpreter)
	boost::shared_lock<boost::shared_mutex> evalLock(PythonEngine::instance()->getEvalCS());

	PythonState scope(m_state);				// Deve essere in uno scope superiore al tracciamento di eventuali errori		

	m_module.reset(OS_NEW_T(boost::python::object));
	m_dictionary.reset(OS_NEW_T(boost::python::object));

	try
	{
		*m_module = boost::python::import(OS_PYTHON_MAIN_MODULE_NAME);
		*m_dictionary = m_module->attr(OS_PYTHON_MODULE_DICTIONARY);
	
		std::string utf8_source = source.to_utf8();		

		if(fileSource)
		{			
			size_t length = utf8_source.length();
			scoped_array<char, os_deallocate_t> buffer(OS_ALLOCATE_T(char, length + 1));
			char *argv = buffer.get();
			strcpy(argv, utf8_source.c_str());
			argv[length] = 0;
			
			PySys_SetArgv(1, &argv);

			// boost::python::exec_file non lo setta in automatico, __name__ viene invece settato
			(*m_dictionary)["__file__"] = utf8_source;

			boost::python::exec_file(utf8_source.c_str(), *m_dictionary, *m_dictionary);	
		}
		else
		{
			static char *empty_argv[1] = {""};
			PySys_SetArgv(1, empty_argv);		// Resetta gli argomenti eventualmenti settati in precedenza su un file

			boost::python::exec(utf8_source.c_str(), *m_dictionary, *m_dictionary);	
		}

		// N.B.: il callback (se settato) deve essere invocato all'interno dello scope di PythonState
		
		if(defaultCallback)
			defaultCallback(*m_dictionary);
	}
	catch(const boost::python::error_already_set &)
	{
		if(errorCallback)
			errorCallback(*m_dictionary);
		else
			PythonEngine::instance()->logLastError(true);

		return false;
	}
	
	return true;
}

bool PythonInterpreter::invoke(const boost::function<void(const boost::python::object &)> &callback, const boost::function<void(const boost::python::object &)> &errorCallback)
{
	OS_ASSERT(callback);

	// Consente l'esecuzione di più scripts in parallelo a meno che non ci sia un interprete in fase di inizializzazione (vedere le note sotto PythonEngine::allocateInterpreter)
	boost::shared_lock<boost::shared_mutex> evalLock(PythonEngine::instance()->getEvalCS());

	PythonState scope(m_state);				// Deve essere in uno scope superiore al tracciamento di eventuali errori		

	if(m_dictionary == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	try
	{
		callback(*m_dictionary);
	}
	catch(const boost::python::error_already_set &)
	{
		if(errorCallback)
			errorCallback(*m_dictionary);
		else
			PythonEngine::instance()->logLastError(true);

		return false;
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
