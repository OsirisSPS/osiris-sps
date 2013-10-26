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
#include "ibackgroundjob.h"

#include "platformmanager.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IBackgroundJob::Details::Details(const std::wstring &name) : m_name(name)
{

}

IBackgroundJob::Details::~Details()
{

}

//////////////////////////////////////////////////////////////////////

IBackgroundJob::IBackgroundJob(bool visible, uint32 id, TaskPriority priority) : JobBase(id, priority),
																				 m_visible(visible),
																				 m_creationTime(utils::utc_time()),
																				 m_lastProgressTime(m_creationTime)
{
	OS_ASSERT(utils::ptime_is_valid(m_creationTime) && (m_creationTime == m_lastProgressTime));	
}

IBackgroundJob::~IBackgroundJob()
{

}

bool IBackgroundJob::getVisible() const
{
	boost::mutex::scoped_lock lock(m_backgroundJobCS);
	return m_visible;
}

void IBackgroundJob::setVisible(bool visible)
{
	boost::mutex::scoped_lock lock(m_backgroundJobCS);
	m_visible = visible;	
}

boost::posix_time::ptime IBackgroundJob::getCreationTime() const
{
	boost::mutex::scoped_lock lock(m_backgroundJobCS);	// In teoria la lock non serve qui...
	return m_creationTime;
}

boost::posix_time::ptime IBackgroundJob::getLastProgressTime() const
{
	boost::mutex::scoped_lock lock(m_backgroundJobCS);
	return m_lastProgressTime;
}

void IBackgroundJob::onProgress()
{
	{
		boost::mutex::scoped_lock lock(m_backgroundJobCS);
		m_lastProgressTime = utils::utc_time();
	}

	// VERYURGENT: da verificare se applicabile in generale (attualmente serve a non occupare il 100% per i job di esportazione/importazione)
	PlatformManager::instance()->yield();	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
