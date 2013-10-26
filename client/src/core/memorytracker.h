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

#ifndef _OS_CORE_MEMORYTRACKER_H
#define _OS_CORE_MEMORYTRACKER_H

#ifdef OS_DEBUG
	// #define OS_ENABLE_MEMORY_TRACKER 
#endif // OS_DEBUG

//////////////////////////////////////////////////////////////////////

#ifdef OS_ENABLE_MEMORY_TRACKER

//////////////////////////////////////////////////////////////////////

#include "singleton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport MemoryTracker : public StaticSingleton<MemoryTracker, false>		// Usa un'inizializzazione statica per assicurare di non riportare falsi positivi
{
public:
	class CoreExport IScope
	{
	// Construction
	protected:
		IScope();

	public:
		virtual ~IScope();

	// Operations
	public:
		virtual void trackAllocation(void *ptr, size_t size, const char *file = 0, size_t line = 0, const char *function = 0) = 0;
		virtual void trackDeallocation(void *ptr, const char *file = 0, size_t line = 0, const char *function = 0) = 0;
		virtual void trackReallocation(void *prev, void *ptr, size_t size, const char *file = 0, size_t line = 0, const char *function = 0) = 0;
	};

// Construction
protected:
	MemoryTracker();

public:
	virtual ~MemoryTracker();

// Attributes
public:
	uint64 getBreakAllocation() const;
	void setBreakAllocation(uint64 breakAllocation);

// Operations
public:
	shared_ptr<IScope> lock();

private:
	class impl;
	scoped_ptr<impl> m_impl;		// N.B.: non è possibile usare qui un pimpl_ptr perchè internamente usa le macro di allocazione/deallocazione OS_NEW_T/OS_DELETE_T
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // OS_ENABLE_MEMORY_TRACKER

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_MEMORYTRACKER_H
