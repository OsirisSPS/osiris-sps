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

#ifndef _OS_PYTHON_PYTHONINTERPRETER_H
#define _OS_PYTHON_PYTHONINTERPRETER_H

#include "base/object.h"
#include "boost/function.hpp"

//////////////////////////////////////////////////////////////////////

#define OS_PYTHON_FILE_EXTENSION			"py"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class PythonInterpreter : public Object
{
// Construction
public:
	PythonInterpreter(PyThreadState *state);
	virtual ~PythonInterpreter();

// Attributes
public:
	inline PyThreadState * getThreadState() const;

// Operations
public:
	bool execute(const String &source, bool fileSource, const boost::function<void(const boost::python::object &)> &defaultCallback = boost::function<void(const boost::python::object &)>(), const boost::function<void(const boost::python::object &)> &errorCallback = boost::function<void(const boost::python::object &)>());
	bool invoke(const boost::function<void(const boost::python::object &)> &callback, const boost::function<void(const boost::python::object &)> &errorCallback = boost::function<void(const boost::python::object &)>());

private:
	PyThreadState *m_state;
	scoped_ptr<boost::python::object, os_delete_t> m_module;
	scoped_ptr<boost::python::object, os_delete_t> m_dictionary;
};

//////////////////////////////////////////////////////////////////////

inline PyThreadState * PythonInterpreter::getThreadState() const { return m_state; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_PYTHON_PYTHONINTERPRETER_H
