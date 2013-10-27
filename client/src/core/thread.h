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

#ifndef _OS_CORE_THREAD_H
#define _OS_CORE_THREAD_H

#include "base/object.h"
#include "irunnable.h"
#include "pimpl_ptr.h"
#include "threads.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport Thread : public Object,
						  public IRunnable,
						  public boost::noncopyable
{
	typedef Object BaseClass;

// Construction
public:
	Thread();
	virtual ~Thread();

// Attributes
public:
	// Indica se il thread è pronto per l'esecuzione
	bool ready() const;
	// Indica se il thread è in esecuzione
	bool running() const;
	// Indica se il thread è stato fermato
	bool stopped() const;

	// Restituisce l'id del thread
	boost::any getID() const;

	// Restituisce la priorità del thread
	TaskPriority getPriority() const;
	// Modifica la priorità del thread
	void setPriority(TaskPriority priority);

// Operations
public:
	// Attende la terminazione del thread
	void join();

	void sleep(uint32 milliseconds);
	void yield();

private:
	void runCallback();
	
// IRunnable interface
public:
	virtual bool start();
	virtual void stop();

protected:
	virtual bool onEnter();
	virtual void onLeave();

// Interface
protected:
	virtual void run() = 0;

private:
	pimpl_of<Thread>::type m_impl;	
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_THREAD_H
