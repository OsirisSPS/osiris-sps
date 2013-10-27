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
#include "ned.h"

#include "nedmalloc/nedmalloc.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

CoreExport void * ned_allocate(const size_t size)
{
	return nedalloc::nedmalloc(size);
}

CoreExport void ned_deallocate(void *ptr)
{
	// Assicura la compatibilità con free(null) visto che internamente nedmalloc non effettua questo controllo
	if(ptr != null)
		nedalloc::nedfree(ptr);
}

CoreExport void * ned_reallocate(void *ptr, const size_t size)
{
	// Assicura la compatibilità con lo standard secondo cui se la dimensione è 0 il buffer deve essere deallocato ed essere restituito null
	if(size == 0)
	{
		ned_deallocate(ptr);
		return null;
	}

	return nedalloc::nedrealloc(ptr, size);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
