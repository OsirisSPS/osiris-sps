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

#ifndef _OS_PYTHON_PYTHONSTATE_H
#define _OS_PYTHON_PYTHONSTATE_H

#include "boost/noncopyable.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "sealed.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class PythonState : Sealed<PythonState>,
					public boost::noncopyable
{
// Internal
private:
	class locked_error_already_set;

// Construction
public:
	PythonState(PyThreadState *state);
	~PythonState();

// Operations
public:
	void enter();
	void leave();

public:
	static void throw_locked_error_already_set(shared_ptr<PythonState> state);

private:
	PyThreadState *m_state;
	bool m_locked;
	bool m_swapped;
	boost::recursive_mutex::scoped_lock m_stateLock;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_PYTHON_PYTHONSTATE_H
