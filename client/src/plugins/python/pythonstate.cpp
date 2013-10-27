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
#include "pythonstate.h"

#include "pythonengine.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class PythonState::locked_error_already_set : public boost::python::error_already_set,
											  public std::exception
{
public:
	locked_error_already_set(shared_ptr<PythonState> state) : m_state(state)
	{
		OS_ASSERT(state != null);
	}

	virtual ~locked_error_already_set() throw()
	{

	}

public:
	virtual const char * what() const throw()
	{
		return "Python error, call PyErr_Fetch to get details";
	}

private:
	shared_ptr<PythonState> m_state;
};

//////////////////////////////////////////////////////////////////////

// N.B.: ogni PyThreadState non deve essere utilizzato in parallelo da più threads

PythonState::PythonState(PyThreadState *state) : m_stateLock(PythonEngine::instance()->getInterpreterMutex(state)),
												 m_state(state),												
												 m_locked(false),
												 m_swapped(false)
{
	enter();
}

PythonState::~PythonState()
{
	leave();
}

void PythonState::enter()
{
	if(m_locked == false)
	{
		// Assicura che il thread abbia ottenuto la lock
		PythonEngine::instance()->ensureLock();
		m_locked = true;
	}

	if(m_swapped == false)
	{
		OS_ASSERT(m_locked);

		if(m_state != _PyThreadState_Current)
		{
			OS_ASSERT(m_state != null);
			PyThreadState_Swap(m_state);			
			m_swapped = true;
		}
	}
}

void PythonState::leave()
{
	if(m_swapped)
	{
		OS_ASSERT(m_locked);

		PyThreadState_Swap(null);
		m_swapped = false;
	}

	if(m_locked)
	{
		m_locked = false;

		// Rilascia la lock sul thread
		PythonEngine::instance()->releaseLock();		
	}
}

void PythonState::throw_locked_error_already_set(shared_ptr<PythonState> state)
{
	throw locked_error_already_set(state);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
