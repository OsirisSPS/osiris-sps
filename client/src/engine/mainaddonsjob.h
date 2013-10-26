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

#ifndef _OS_ENGINE_MAINADDONSJOB_H
#define _OS_ENGINE_MAINADDONSJOB_H

#include "ibackgroundjob.h"
#include "pimpl_ptr.h"
#include "ids.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

class EngineExport MainAddonsJob : public IBackgroundJob
{
	typedef IBackgroundJob JobBase;

// Construction
public:
	MainAddonsJob(uint32 id, const String &action, const std::string &extensionID, const String &downloadUrl);
	virtual ~MainAddonsJob();

// Attributes
public:	
	inline String getAction() const;
	inline void setAction(const String &action);	

	inline ExtensionID getExtensionID() const;
	inline void setExtensionID(const ExtensionID &id);

	inline String getUrl() const;
	inline void setUrl(const String &url);	

	inline double getProgressPercentage() const;
	inline void setProgressPercentage(double v);

// IJob overrides
public:
	virtual void stop();

// IJob interface
public:
	virtual JobStatus run();

// IBackgroundJob interface
public:
	virtual shared_ptr<Details> getDetails() const;

protected:
	//pimpl_of<MainAddonsJob>::type m_impl;
	ExtensionID m_extensionID;
	String m_action;
	String m_url;
	double m_progressPercentage;

	String m_name;
};

//////////////////////////////////////////////////////////////////////

inline String MainAddonsJob::getAction() const { return m_action; }
inline void MainAddonsJob::setAction(const String &action) { m_action = action; }

inline ExtensionID MainAddonsJob::getExtensionID() const { return m_extensionID; }
inline void MainAddonsJob::setExtensionID(const ExtensionID &id) { m_extensionID = id; }

inline String MainAddonsJob::getUrl() const { return m_url; }
inline void MainAddonsJob::setUrl(const String &url) { m_url = url; }

inline double MainAddonsJob::getProgressPercentage() const
{
	return m_progressPercentage;
}

inline void MainAddonsJob::setProgressPercentage(double v)
{
	m_progressPercentage = v;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_MAINADDONSJOB_H
