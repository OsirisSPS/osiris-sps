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

#if !defined(BOOST_PP_IS_ITERATING)

//////////////////////////////////////////////////////////////////////

#ifndef _OS_PYTHON_PYTHONOVERRIDE_H
#define _OS_PYTHON_PYTHONOVERRIDE_H

#include "boost/preprocessor/iteration/iterate.hpp"
#include "safe_bool.h"
#include "pythonmethodresult.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class PythonOverride : public safe_bool<PythonOverride>
                       //, public boost::noncopyable
{
// Construction
public:
	PythonOverride(const boost::python::override &override);
	PythonOverride(const PythonOverride &second);
	~PythonOverride();

// Operations
public:
    bool boolean_test() const;

private:
	void reportError() const;

// Operators
public:
	#define BOOST_PYTHON_FAST_ARG_TO_PYTHON_GET(z, n, _)   , boost::python::converter::arg_to_python<A##n>(a##n).get()
	#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_PYTHON_MAX_ARITY, "pythonoverride.h"))
	#include BOOST_PP_ITERATE()

	PythonOverride & operator=(const boost::python::override &override);
	PythonOverride & operator=(const PythonOverride &second);

private:
	boost::python::override m_override;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_PYTHON_PYTHONOVERRIDE_H

//////////////////////////////////////////////////////////////////////

#else // BOOST_PP_IS_ITERATING

//////////////////////////////////////////////////////////////////////

#if BOOST_PP_ITERATION() == 0

//////////////////////////////////////////////////////////////////////

/*
PythonMethodResult operator()()
{
	try
	{
		// N.B.: non restituire qui m_override() altrimenti viene chiamato l'operatore di conversione a PythonMethodResult e non costruito un oggetto PythonMethodResult di cui poi verrà chiamato l'operatore ()
		boost::python::detail::method_result result = m_override();		// Fix gcc mac, non digerisce "return PythonMethodResult(m_override())"
		return PythonMethodResult(result);
	}
	catch(boost::python::error_already_set const &)
	{
		reportError();
		throw;
	}
}
*/

PythonMethodResult operator()()
{
	try
	{
		return PythonMethodResult(PyEval_CallFunction(m_override.ptr(), const_cast<char*>("()")));
	}
	catch(boost::python::error_already_set const &)
	{
		reportError();
		throw;
	}
}

//////////////////////////////////////////////////////////////////////

#else //  BOOST_PP_ITERATION() == 0

//////////////////////////////////////////////////////////////////////

/*
template <BOOST_PP_ENUM_PARAMS_Z(1, BOOST_PP_ITERATION(), typename A)>
PythonMethodResult operator()(BOOST_PP_ENUM_BINARY_PARAMS_Z(1, BOOST_PP_ITERATION(), A, const &a)) const
{
   	try
	{
		// N.B.: non restituire qui m_override() altrimenti viene chiamato l'operatore di conversione a PythonMethodResult e non costruito un oggetto PythonMethodResult di cui poi verrà chiamato l'operatore ()
		boost::python::detail::method_result result = m_override(BOOST_PP_ENUM_PARAMS_Z(1, BOOST_PP_ITERATION(), a));	// Vedi note sopra
		return PythonMethodResult(result);
	}
	catch(boost::python::error_already_set const &)
	{
		reportError();
		throw;
	}
}
*/

template <BOOST_PP_ENUM_PARAMS_Z(1, BOOST_PP_ITERATION(), typename A)>
PythonMethodResult operator()(BOOST_PP_ENUM_BINARY_PARAMS_Z(1, BOOST_PP_ITERATION(), A, const &a)) const
{
   	try
	{		
		return PythonMethodResult(PyEval_CallFunction(m_override.ptr(), const_cast<char*>("(" BOOST_PP_REPEAT_1ST(BOOST_PP_ITERATION(), BOOST_PYTHON_FIXED, "O") ")")BOOST_PP_REPEAT_1ST(BOOST_PP_ITERATION(), BOOST_PYTHON_FAST_ARG_TO_PYTHON_GET, nil)));	
	}
	catch(boost::python::error_already_set const &)
	{
		reportError();
		throw;
	}
}

//////////////////////////////////////////////////////////////////////

#endif //  BOOST_PP_ITERATION() == 0

//////////////////////////////////////////////////////////////////////

#endif // BOOST_PP_IS_ITERATING

//////////////////////////////////////////////////////////////////////
