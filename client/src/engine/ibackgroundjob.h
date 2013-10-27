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

#ifndef _OS_ENGINE_IBACKGROUNDJOB_H
#define _OS_ENGINE_IBACKGROUNDJOB_H

#include "boost/thread/mutex.hpp"
#include "ijob.h"
#include "jobcallback.h"
#include "noninstantiable.h"

//////////////////////////////////////////////////////////////////////

#define OS_IBACKGROUNDJOB_PERCENTAGE_MIN		0
#define OS_IBACKGROUNDJOB_PERCENTAGE_MAX		1

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport IBackgroundJob : public IJob
{
	typedef IJob JobBase;

public:
	class EngineExport Details : public Object
	{
	// Construction
	public:
		Details(const std::wstring &name);
		virtual ~Details();

	// Attributes
	public:
		inline const std::wstring & getName() const;

		inline const std::wstring & getStatus() const;
		inline void setStatus(std::wstring status);

		inline const boost::optional<double> & getPercentage() const;
		inline void setPercentage(const boost::optional<double> &percentage);

		inline const String & getResult() const;
		inline void setResult(const String &result);

	private:
		std::wstring m_name;
		std::wstring m_status;
		boost::optional<double> m_percentage;
		String m_result;
	};

// Construction
public:
	IBackgroundJob(bool visible, uint32 id, TaskPriority priority = taskPriorityDefault);
	virtual ~IBackgroundJob();

// Attributes
public:
	bool getVisible() const;
	void setVisible(bool visible);

	// Restituisce la data di creazione del job
	boost::posix_time::ptime getCreationTime() const;
	// Restituisce la data di ultimo update del job
	boost::posix_time::ptime getLastProgressTime() const;

// Interface
public:
	virtual shared_ptr<Details> getDetails() const = 0;

// Overridables
protected:
	virtual void onProgress();

private:
	mutable boost::mutex m_backgroundJobCS;
	bool m_visible;
	boost::posix_time::ptime m_creationTime;
	boost::posix_time::ptime m_lastProgressTime;
};

//////////////////////////////////////////////////////////////////////

inline const std::wstring & IBackgroundJob::Details::getName() const
{
	return m_name;
}

inline const std::wstring & IBackgroundJob::Details::getStatus() const
{
	return m_status;
}

inline void IBackgroundJob::Details::setStatus(std::wstring status)
{
	m_status = status;
}

inline const boost::optional<double> & IBackgroundJob::Details::getPercentage() const
{
	return m_percentage;
}

inline void IBackgroundJob::Details::setPercentage(const boost::optional<double> &percentage)
{
	OS_ASSERT(percentage == boost::none || (percentage.get() >= OS_IBACKGROUNDJOB_PERCENTAGE_MIN && percentage.get() <= OS_IBACKGROUNDJOB_PERCENTAGE_MAX));
	m_percentage = percentage;
}

inline const String & IBackgroundJob::Details::getResult() const
{
	return m_result;
}

inline void IBackgroundJob::Details::setResult(const String &result)
{
	m_result = result;
}

//////////////////////////////////////////////////////////////////////

template <typename T = boost::function<void ()> >
class BackgroundJobCallback : public JobCallback<T, IBackgroundJob>
{
	typedef JobCallback<T, IBackgroundJob> JobBase;

public:
	typedef typename JobBase::Callback Callback;

// Construction
public:
	BackgroundJobCallback(const std::wstring &name, bool visible, uint32 id, const Callback &callback, TaskPriority priority = taskPriorityDefault);
	virtual ~BackgroundJobCallback();

// IBackgroundJob interface
public:
	virtual shared_ptr<IBackgroundJob::Details> getDetails() const;

private:
	std::wstring m_name;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
BackgroundJobCallback<T>::BackgroundJobCallback(const std::wstring &name, bool visible, uint32 id, const Callback &callback, TaskPriority priority) : JobBase(visible, callback, id, priority),
																																					  m_name(name)
{

}

template <typename T>
BackgroundJobCallback<T>::~BackgroundJobCallback()
{

}

template <typename T>
shared_ptr<IBackgroundJob::Details> BackgroundJobCallback<T>::getDetails() const
{
	return shared_ptr<IBackgroundJob::Details>(OS_NEW IBackgroundJob::Details(m_name));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_IBACKGROUNDJOB_H
