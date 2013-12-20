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
#include "condition.h"

#include "boost/thread/condition.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/xtime.hpp"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

struct Condition::impl
{
	shared_ptr<boost::condition> condition;
	mutable boost::timed_mutex mutex;
};

//////////////////////////////////////////////////////////////////////

Condition::Condition()
{
	create();
}

Condition::~Condition()
{
	reset();
}

bool Condition::isValid() const
{
	boost::timed_mutex::scoped_lock lock(m_impl->mutex);
	return m_impl->condition != nullptr;
}

void Condition::create()
{
	boost::timed_mutex::scoped_lock lock(m_impl->mutex);

	if(m_impl->condition == nullptr)
		m_impl->condition.reset(OS_NEW_T(boost::condition), os_delete_t());
}
	
void Condition::reset()
{
	boost::timed_mutex::scoped_lock lock(m_impl->mutex);

	if(m_impl->condition != nullptr)
	{
		m_impl->condition->notify_all();
		m_impl->condition.reset();
	}	
}

bool Condition::wait()
{
	boost::timed_mutex::scoped_lock lock(m_impl->mutex);

	if(m_impl->condition == nullptr)
		return false;

	// N.B.: effettua una copia per evitare che una possibile reset distrugga l'oggetto (in questo modo verrebbe distrutto all'uscita della funzione)
	shared_ptr<boost::condition> condition = m_impl->condition;	
	condition->wait(lock);

	return true;
}

bool Condition::wait(uint32 millisec)
{
	boost::timed_mutex::scoped_lock lock(m_impl->mutex);
	
	if(m_impl->condition == nullptr)
		return false;

	// N.B.: effettua una copia per evitare che una possibile reset distrugga l'oggetto (in questo modo verrebbe distrutto all'uscita della funzione)
	shared_ptr<boost::condition> condition = m_impl->condition;
	return condition->timed_wait(lock, utils::xtime_millisec(millisec));
}

void Condition::signal()
{
	shared_ptr<boost::condition> condition;

	{
		boost::timed_mutex::scoped_lock lock(m_impl->mutex);
		if(m_impl->condition == nullptr)
			return;

		// N.B.: effettua una copia per evitare che una possibile reset distrugga l'oggetto (in questo modo verrebbe distrutto all'uscita della funzione)
		condition = m_impl->condition;
	}
	
	OS_ASSERT(condition != nullptr);
	condition->notify_all();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
