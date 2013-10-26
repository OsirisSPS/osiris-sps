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
#include "dynlib.h"

#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DynLib::DynLib(const String &filename)
{
	m_handle = null;

	if(filename.empty() == false)
		load(filename);
}

DynLib::~DynLib()
{
	unload();
}

void * DynLib::findProcedure(const String &procedure) const
{
	OS_EXCEPT_IF(m_handle == null, "Invalid library handle");
	return PlatformManager::instance()->findProcedure(m_handle, procedure);
}

bool DynLib::load(const String &filename)
{
	unload();

	OS_ASSERT(filename.ends_with(OS_SHARED_LIBRARY_EXTENSION) == false);
	m_handle = PlatformManager::instance()->loadLibrary(filename + _S(".") + OS_SHARED_LIBRARY_EXTENSION);
	return m_handle != null;
}

void DynLib::unload()
{
	if(m_handle != null)
	{
		PlatformManager::instance()->unloadLibrary(m_handle);
		m_handle = null;
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
