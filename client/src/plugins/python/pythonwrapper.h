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

#ifndef _OS_PYTHON_PYTHONWRAPPER_H
#define _OS_PYTHON_PYTHONWRAPPER_H

#include "boost/python/wrapper.hpp"
#include "pythonstate.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
class PythonWrapper : public boost::python::wrapper<T>
{
// Construction
public:
	PythonWrapper();
	virtual ~PythonWrapper();

// Attributes
public:
	inline PyThreadState * getPythonThreadState() const;

// Operations
public:
	void python_pure_virtual_called();

private:
	PyThreadState *m_pythonState;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
PythonWrapper<T>::PythonWrapper() : m_pythonState(PyThreadState_Get())	// TODO: corretto? in teoria qui non serve una lock...
{
	OS_ASSERT(m_pythonState != nullptr);
}

template <typename T>
PythonWrapper<T>::~PythonWrapper()
{

}

template <typename T>
inline PyThreadState * PythonWrapper<T>::getPythonThreadState() const
{
	return m_pythonState;
}

template <typename T>
void PythonWrapper<T>::python_pure_virtual_called()
{
	shared_ptr<PythonState> state(new PythonState(getPythonThreadState()));

	PyErr_SetString(PyExc_RuntimeError, "Pure virtual function called");

	PythonState::throw_locked_error_already_set(state);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_PYTHON_PYTHONWRAPPER_H
