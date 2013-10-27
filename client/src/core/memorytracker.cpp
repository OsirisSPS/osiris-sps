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
#include "memorytracker.h"

//////////////////////////////////////////////////////////////////////

#ifdef OS_ENABLE_MEMORY_TRACKER

//////////////////////////////////////////////////////////////////////

#include <iostream>
#include "boost/thread/recursive_mutex.hpp"
#include "platformmanager.h"
#include <sstream>
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class MemoryTracker::impl
{
// Internal
private:
	class Scope : public MemoryTracker::IScope
	{
	// Construction
	public:
		Scope(MemoryTracker::impl *impl, boost::recursive_mutex &cs);
		virtual ~Scope();

	// IScope interface
	public:
		virtual void trackAllocation(void *ptr, size_t size, const char *file = 0, size_t line = 0, const char *function = 0);
		virtual void trackDeallocation(void *ptr, const char *file = 0, size_t line = 0, const char *function = 0);
		virtual void trackReallocation(void *prev, void *ptr, size_t size, const char *file = 0, size_t line = 0, const char *function = 0);

	private:
		MemoryTracker::impl *m_impl;
		scoped_ptr<boost::recursive_mutex::scoped_lock> m_lock;
	};

	class MemoryBlock
	{
	// Construction
	public:
		MemoryBlock(size_t size, uint64 allocation, const std::string &file, size_t line, const std::string &function);
		~MemoryBlock();

	// Attributes
	public:
		inline size_t getSize() const;
		inline void setSize(size_t size);

		inline uint64 getAllocation() const;
		inline void setAllocation(uint64 allocation);

		inline const std::string & getFile() const;
		inline void setFile(const std::string &file);

		inline size_t getLine() const;
		inline void setLine(size_t line);

		inline const std::string & getFunction() const;
		inline void setFunction(const std::string &function);

		inline bool getReallocated() const;
		inline void setReallocated(bool reallocated);

	private:
		size_t m_size;
		uint64 m_allocation;
		std::string m_file;
		size_t m_line;
		std::string m_function;		
		bool m_reallocated;
	};

	typedef boost::unordered_map<void *, shared_ptr<MemoryBlock> > MemoryBlocks;	// Usa la versione standard per evitare di tracciare anche gli allocamenti di questa mappa

// Construction
public:
	impl();
	virtual ~impl();

// Attributes
public:
	uint64 getBreakAllocation() const;
	void setBreakAllocation(uint64 breakAllocation);

// Operations
public:
	shared_ptr<MemoryTracker::IScope> lock();

	void trackAllocation(void *ptr, size_t size, const char *file, size_t line, const char *function);
	void trackDeallocation(void *ptr, const char *file, size_t line, const char *function);
	void trackReallocation(void *prev, void *ptr, size_t size, const char *file, size_t line, const char *function);

private:
	void reportMemoryLeaks();

private:
	mutable boost::recursive_mutex m_cs;
	MemoryBlocks m_blocks;
	size_t m_memory;
	uint64 m_allocations;
	uint64 m_breakAllocation;
	bool m_finalizing;
};

//////////////////////////////////////////////////////////////////////

MemoryTracker::impl::Scope::Scope(MemoryTracker::impl *impl, boost::recursive_mutex &cs) : m_impl(impl),
																						   m_lock(new boost::recursive_mutex::scoped_lock(cs))
{
	OS_ASSERT(m_impl != null);
}

MemoryTracker::impl::Scope::~Scope()
{

}

void MemoryTracker::impl::Scope::trackAllocation(void *ptr, size_t size, const char *file, size_t line, const char *function)
{
	m_impl->trackAllocation(ptr, size, file, line, function);
}

void MemoryTracker::impl::Scope::trackDeallocation(void *ptr, const char *file, size_t line, const char *function)
{
	m_impl->trackDeallocation(ptr, file, line, function);
}

void MemoryTracker::impl::Scope::trackReallocation(void *prev, void *ptr, size_t size, const char *file, size_t line, const char *function)
{
	m_impl->trackReallocation(prev, ptr, size, file, line, function);
}

//////////////////////////////////////////////////////////////////////

MemoryTracker::impl::MemoryBlock::MemoryBlock(size_t size, uint64 allocation, const std::string &file, size_t line, const std::string &function) : m_size(size),
																																				   m_allocation(allocation),
																																				   m_file(file),
																																				   m_line(line),
																																				   m_function(function),																																
																																				   m_reallocated(false)
{

}

MemoryTracker::impl::MemoryBlock::~MemoryBlock()
{

}

inline size_t MemoryTracker::impl::MemoryBlock::getSize() const
{
	return m_size;
}

inline void MemoryTracker::impl::MemoryBlock::setSize(size_t size)
{
	m_size = size;
}

inline uint64 MemoryTracker::impl::MemoryBlock::getAllocation() const
{
	return m_allocation;
}

inline void MemoryTracker::impl::MemoryBlock::setAllocation(uint64 allocation)
{
	m_allocation = allocation;
}

inline const std::string & MemoryTracker::impl::MemoryBlock::getFile() const
{
	return m_file;
}

inline void MemoryTracker::impl::MemoryBlock::setFile(const std::string &file)
{
	m_file = file;
}

inline size_t MemoryTracker::impl::MemoryBlock::getLine() const
{
	return m_line;
}

inline void MemoryTracker::impl::MemoryBlock::setLine(size_t line)
{
	m_line = line;
}

inline const std::string & MemoryTracker::impl::MemoryBlock::getFunction() const
{
	return m_function;
}

inline void MemoryTracker::impl::MemoryBlock::setFunction(const std::string &function)
{
	m_function = function;
}

inline bool MemoryTracker::impl::MemoryBlock::getReallocated() const
{
	return m_reallocated;
}

inline void MemoryTracker::impl::MemoryBlock::setReallocated(bool reallocated)
{
	m_reallocated = reallocated;
}

//////////////////////////////////////////////////////////////////////

MemoryTracker::impl::impl() : m_memory(0),
							  m_finalizing(false),
							  m_allocations(0),
							  m_breakAllocation(0)
{

}

MemoryTracker::impl::~impl()
{
	reportMemoryLeaks();
}

uint64 MemoryTracker::impl::getBreakAllocation() const
{
	boost::recursive_mutex::scoped_lock lock(m_cs);
	return m_breakAllocation;
}

void MemoryTracker::impl::setBreakAllocation(uint64 breakAllocation)
{
	boost::recursive_mutex::scoped_lock lock(m_cs);
	m_breakAllocation = breakAllocation;
}

shared_ptr<MemoryTracker::IScope> MemoryTracker::impl::lock()
{
	return shared_ptr<MemoryTracker::IScope>(new Scope(this, m_cs));
}

void MemoryTracker::impl::trackAllocation(void *ptr, size_t size, const char *file, size_t line, const char *function)
{
	// trackAllocation è chiamata solo da Scope::trackAllocation in cui il mutex è già loccato

	OS_ASSERT(m_finalizing == false);

	if(ptr == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	m_memory += size;

	m_allocations++;
	if(m_allocations == m_breakAllocation)
		PlatformManager::instance()->debugBreak();

	// Se si arriva in questa circostanza vuol dire che si sta tentando di effettuare un'allocazione allo stesso indirizzo di memoria
	OS_ASSERT(m_blocks.find(ptr) == m_blocks.end());

	m_blocks[ptr] = shared_ptr<MemoryBlock>(new MemoryBlock(size, m_allocations, utils::safe_string<std::string>(file), line, utils::safe_string<std::string>(function)));
}

void MemoryTracker::impl::trackDeallocation(void *ptr, const char *file, size_t line, const char *function)
{
	// trackDeallocation è chiamata solo da Scope::trackDeallocation in cui il mutex è già loccato

	OS_ASSERT(m_finalizing == false);

	if(ptr == null)
		return;		// free/delete null sono operazioni valide, quindi vanno gestite come casi speciali

	MemoryBlocks::iterator i = m_blocks.find(ptr);
	// Se arriva in questa circostanza vuol dire che si sta tentando di allocare un blocco di memoria già precedentemente deallocato o di cui non è stata tracciata l'allocazione (ad esempio è stato allocato con una malloc standard e poi deallocato tramite allocatore)
	OS_ASSERT(i != m_blocks.end());

	m_memory -= i->second->getSize();

	m_blocks.erase(i);
}

void MemoryTracker::impl::trackReallocation(void *prev, void *ptr, size_t size, const char *file, size_t line, const char *function)
{
	// trackReallocation è chiamata solo da Scope::trackReallocation in cui il mutex è già loccato

	OS_ASSERT(m_finalizing == false);

	if(size == 0)
	{
		trackDeallocation(prev, file, line, function);
		return;
	}

	MemoryBlocks::iterator i = m_blocks.find(prev);
	if(i != m_blocks.end())
	{
		shared_ptr<MemoryBlock> block = i->second;
		
		size_t actualSize = block->getSize();
		// Aggiorna la memoria totale in base alla nuova dimensione allocata
		if(size > actualSize)
			m_memory += size - actualSize;
		else
			m_memory -= actualSize - size;

		// Aggiorna i dati sul blocco di memoria
		block->setSize(size);
		block->setFile(utils::safe_string<std::string>(file));
		block->setLine(line);
		block->setFunction(utils::safe_string<std::string>(function));		
		block->setReallocated(true);
		// Controlla se l'indirizzo di memoria è stato spostato
		if(prev != ptr)
		{
			// Cancella il blocco nella vecchia posizione e lo sposta in quella nuova
			m_blocks.erase(i);
			m_blocks[ptr] = block;
		}
	}
	else
	{
		OS_ASSERT(prev == null);	// realloc(null, x) è un'operazione valida, negli altri casi l'indirizzo dovrebbe già essere stato salvato in precedenza
		trackAllocation(ptr, size, file, line, function);
	}
}

void MemoryTracker::impl::reportMemoryLeaks()
{
	boost::recursive_mutex::scoped_lock reportLeaksLock(m_cs);

	m_finalizing = true;

	if(m_blocks.empty())
	{
		OS_ASSERT(m_memory == 0);
		return;
	}

	std::stringstream stream;
	stream << "Detected " << m_blocks.size() << " memory leak(s) (" << m_memory << " bytes):" << std::endl;
	OS_TRACE(stream.str());

	size_t memory = 0;
	for(MemoryBlocks::const_iterator i = m_blocks.begin(); i != m_blocks.end(); ++i)
	{
		shared_ptr<MemoryBlock> block = i->second;

		size_t size = block->getSize();
		const std::string &file = block->getFile();
		size_t line = block->getLine();
		const std::string &function = block->getFunction();
		
		std::stringstream leak;
		leak << "\t" << size << " byte(s) at address 0x";
		leak << i->first;
		leak << ", allocation " << block->getAllocation();
		if(block->getReallocated())
			leak << " (reallocated memory block)";

		leak << std::endl;

		OS_TRACE_CUSTOM(leak.str(), file.c_str(), static_cast<uint32>(line), function.c_str());

		memory += size;
	}
	OS_ASSERT(memory == m_memory);
}

//////////////////////////////////////////////////////////////////////

MemoryTracker::IScope::IScope()
{

}

MemoryTracker::IScope::~IScope()
{

}

//////////////////////////////////////////////////////////////////////

MemoryTracker::MemoryTracker() : m_impl(new impl())
{

}

MemoryTracker::~MemoryTracker()
{

}

uint64 MemoryTracker::getBreakAllocation() const
{
	return m_impl->getBreakAllocation();
}

void MemoryTracker::setBreakAllocation(uint64 breakAllocation)
{
	return m_impl->setBreakAllocation(breakAllocation);
}

shared_ptr<MemoryTracker::IScope> MemoryTracker::lock()
{
	return m_impl->lock();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // OS_ENABLE_MEMORY_TRACKER

//////////////////////////////////////////////////////////////////////
