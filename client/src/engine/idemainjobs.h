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

#ifndef _OS_ENGINE_IDEMAINJOBS_H
#define _OS_ENGINE_IDEMAINJOBS_H

#include "iidemainpage.h"
#include "engine.h"

//////////////////////////////////////////////////////////////////////

#define OS_PAGEJOBS_PARAM_JOB			_W("job")

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlCheckBox;
class HtmlXSLControl;
class XMLExporter;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((ide)(main))

//////////////////////////////////////////////////////////////////////

class EngineExport Jobs : public IMainPage
{
    typedef IMainPage PageBase;

// Construction
public:
    Jobs(shared_ptr<HttpSession> session);
    virtual ~Jobs();

// Operations
private:
	void exportJobs(shared_ptr<XMLExporter> exporter, const Engine::BackgroundJobs &jobs, bool completed);
public:
	static bool exportJob(shared_ptr<XMLExporter> exporter, shared_ptr<IBackgroundJob> job, bool completed);

// Events
private:
	void onClearCompletedJobs();

// IMainPage interface
public:
	virtual String getPageName() const;

// IMainPage overrides
protected:
	virtual void onInit();
	virtual void onPreRender();

private:
	shared_ptr<HtmlCheckBox> m_showAll;	
	shared_ptr<HtmlXSLControl> m_pageTemplate;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((ide)(main))

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_IDEMAINJOBS_H
