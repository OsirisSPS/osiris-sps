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

#ifndef _OS_PYTHON_PYTHONMETHODRESULT_H
#define _OS_PYTHON_PYTHONMETHODRESULT_H

#include "boost/type.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class PythonMethodResult
{
	friend class PythonOverride;

// Construction
private:
	explicit PythonMethodResult(PyObject *result);

public:
	~PythonMethodResult();

// Operators
public:
	template <typename T>
	inline operator T();

	template <typename T>
    inline operator T *();

// Operations
private:
	void reportError() const;

private:
	mutable boost::python::handle<> m_obj;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
inline PythonMethodResult::operator T()
{
	try
	{
		boost::python::converter::return_from_python<T> converter;
        return converter(m_obj.release());
	}
	catch(boost::python::error_already_set const &)
	{
		reportError();
		throw;
	}
}

template <typename T>
inline PythonMethodResult::operator T *()
{
	try
	{
		boost::python::converter::return_from_python<T *> converter;
        return converter(m_obj.release());
	}
	catch(boost::python::error_already_set const &)
	{
		reportError();
		throw;
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_PYTHON_PYTHONOVERRIDE_H
