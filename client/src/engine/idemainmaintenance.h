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

#ifndef _IDE_MAIN_MAINTENANCE_H
#define _IDE_MAIN_MAINTENANCE_H

#include "iidemainpage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlTextBox;

//////////////////////////////////////////////////////////////////////

namespace ide {
namespace main {

//////////////////////////////////////////////////////////////////////

class EngineExport Maintenance : public IMainPage
{
	typedef IMainPage PageBase;

// Construction
public:
	Maintenance(shared_ptr<HttpSession> session);
	virtual ~Maintenance();

// IPage interface
public:
	virtual String getPageName() const;
	
// IMainPage overrides
protected:
	virtual void onLoad();

// Events:
private:
	void onStabilityRestart();
	void onStabilityRebuild();
	void onLuceneRebuild();
	void onLuceneOptimize();
	void onDatabaseVacuum();
	void onDatabaseRebuildIndexes();
	void onDatabaseAnalyze();

private:
	static const String EVENT_ONSTABILITYRESTART;
	static const String EVENT_ONSTABILITYREBUILD;
	static const String EVENT_ONLUCENEREBUILD;	
	static const String EVENT_ONLUCENEOPTIMIZE;	
	static const String EVENT_ONDATABASEVACUUM;	
	static const String EVENT_ONDATABASEREBUILDINDEXES;	
	static const String EVENT_ONDATABASEANALYZE;
};

//////////////////////////////////////////////////////////////////////

} // main
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_MAIN_MAINTENANCE_H
