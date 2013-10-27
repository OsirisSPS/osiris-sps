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
#include "pythonoverride.h"

#include "pythonengine.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

PythonOverride::PythonOverride(const boost::python::override &override) : m_override(override)
{

}

PythonOverride::PythonOverride(const PythonOverride &second) : m_override(second.m_override)
{

}

PythonOverride::~PythonOverride()
{

}

bool PythonOverride::boolean_test() const
{
	return m_override;
}

void PythonOverride::reportError() const
{
	// N.B.: nel caso di un errore in una funzione virtuale è possibile che il metodo non sia stato invocato direttamente da PythonInterpreter (quindi sotto try/catch di boost::python::error_already_set),
	// ma "indirettamente" (ad esempio da un'altra classe C++) e pertanto, in questo caso, non è possibile gestire un'eccezione boost::python::error_already_set (qui pertanto assicura di loggare l'errore).
	// L'unico inconveniente di questa soluzione è che se l'errore viene comunque generato "indirettamente", ma a monte è stato invocato uno script tramite PythonInterpreter, è possibile che venga loggato due volte.

	PythonEngine::instance()->logLastError(false);
}

PythonOverride & PythonOverride::operator=(const boost::python::override &override)
{
    m_override = override;
    return *this;
}

PythonOverride & PythonOverride::operator=(const PythonOverride &second)
{
    m_override = second.m_override;
    return *this;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
