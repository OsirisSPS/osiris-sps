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

#ifndef _OS_CORE_IJOB_H
#define _OS_CORE_IJOB_H

#include "base/object.h"
#include "boost/noncopyable.hpp"
#include "boost/optional.hpp"
#include "enable_this_ptr.h"
#include "pimpl_ptr.h"
#include "threads.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport IJob : public Object,
						public boost::noncopyable,
						public enable_this_ptr<IJob>
{
	typedef Object BaseClass;

// Enumerated types
public:
	enum JobStatus
	{
		jobIncomplete	= 0,		// L'esecuzione non è ancora completa
		jobComplete		= 1,		// L'esecuzione è completa
	};

// Construction
public:
	IJob(uint32 id, TaskPriority priority = taskPriorityDefault);
	virtual ~IJob();

// Attributes
public:
	uint32 getID() const;

	TaskPriority getPriority() const;
	void setPriority(TaskPriority priority);

    bool getRunnable(uint32 *timeout = null) const;

	// Se settato restituisce il delay del job altrimenti boost::none
	boost::posix_time::ptime getDelay() const;

	JobStatus getStatus() const;

	bool ready() const;
	bool running() const;
	bool stopped() const;

// Operations
public:
	// Applica un delay (espresso in millisecondi) per il job nel caso in cui sia incompleto
	void applyDelay(uint32 delay);
	// Resetta il delay del job se applicato in precedenza
	void resetDelay();

    static boost::posix_time::ptime clock();

// Overridables
public:
	virtual JobStatus start();
	virtual void stop();

// Interface
protected:
	virtual JobStatus run() = 0;

protected:
	pimpl_of<IJob>::type m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_IJOB_H
