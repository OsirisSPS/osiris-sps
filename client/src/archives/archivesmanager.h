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

#ifndef _OS_ARCHIVES_ARCHIVESMANAGER_H
#define _OS_ARCHIVES_ARCHIVESMANAGER_H

#include "singleton.h"
#include "archivesconfig.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IArchivesArchive;
class IArchivesArchiveFactory;

//////////////////////////////////////////////////////////////////////

class ArchivesExport ArchivesManager : public StaticSingleton<ArchivesManager>
{
	typedef list<shared_ptr<IArchivesArchiveFactory> >::type Factories;

// Construction
protected:
	ArchivesManager();

public:
	virtual ~ArchivesManager();

private:
	void registerFactory(shared_ptr<IArchivesArchiveFactory> factory);
	void unregisterFactory(shared_ptr<IArchivesArchiveFactory> factory);

// Operations
public:
	shared_ptr<IArchivesArchive> open(const String &filename);
	bool extract(const String &filename, const String &path);

private:
	Factories m_factories;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ARCHIVES_ARCHIVESMANAGER_H
