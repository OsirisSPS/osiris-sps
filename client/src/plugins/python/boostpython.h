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

#ifndef _OSIRIS_PYTHON_BOOSTPYTHON_H
#define _OS_PYTHON_BOOSTPYTHON_H

//////////////////////////////////////////////////////////////////////

#if OS_COMPILER	== OS_COMPILER_MSVC
	#pragma warning(disable : 4244)		// conversion from 'type' to 'type' of greater size	
#endif

//////////////////////////////////////////////////////////////////////

// boost::python di default (sia in debug che in release) viene linkato in automatico alla versione di release
// del python. Per abilitare la versione di debug occorre compilarlo abilitando python-debugging=on (es: bjam toolset=<...> --with-python python-debugging=on)

/*
#ifdef OS_DEBUG
	#define Py_DEBUG
	#define BOOST_DEBUG_PYTHON
#endif // OS_DEBUG
*/

#define BOOST_PYTHON_STATIC_LIB

//////////////////////////////////////////////////////////////////////

#include "boost/python.hpp"
#include "boost/python/def_visitor.hpp"
#include "boost/python/signature.hpp"

//////////////////////////////////////////////////////////////////////

#define BOOST_PYTHON_MODULE_FUNCTION(module)		&init_module_##module
#define BOOST_PYTHON_DECLARE_MODULE(module)			extern void init_module_##module();

#ifdef OS_DEBUG
	namespace { inline const char * get_module_name(void (*)(void), const char *name) { return name; } }
	#define BOOST_PYTHON_MODULE_NAME(module)		get_module_name(BOOST_PYTHON_MODULE_FUNCTION(module), #module)		// Produce un errore di compilazione in caso di modulo non valido
#else
	#define BOOST_PYTHON_MODULE_NAME(module)		#module
#endif

//////////////////////////////////////////////////////////////////////

#if OS_COMPILER	== OS_COMPILER_MSVC
	#pragma warning(default : 4244)
#endif

//////////////////////////////////////////////////////////////////////

#endif // _OS_PYTHON_BOOSTPYTHON_H
