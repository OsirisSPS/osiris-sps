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
#include "archivesmanager.h"

#include "file.h"
#include "filesystem.h"
#include "iarchivesarchive.h"
#include "iarchivesarchiveentry.h"
#include "utils.h"
#include "archivesziparchivefactory.h"

//////////////////////////////////////////////////////////////////////

#define OS_ARCHIVESMANAGER_BUFFER_SIZE		1024

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ArchivesManager::ArchivesManager()
{
	registerFactory(shared_ptr<IArchivesArchiveFactory>(OS_NEW ArchivesZipArchiveFactory()));
}

ArchivesManager::~ArchivesManager()
{

}

void ArchivesManager::registerFactory(shared_ptr<IArchivesArchiveFactory> factory)
{
	if(factory == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	m_factories.push_back(factory);
}

void ArchivesManager::unregisterFactory(shared_ptr<IArchivesArchiveFactory> factory)
{
	Factories::iterator i = std::find(m_factories.begin(), m_factories.end(), factory);
	if(i != m_factories.end())
		m_factories.erase(i);
}

shared_ptr<IArchivesArchive> ArchivesManager::open(const String &filename)
{
	for(Factories::const_iterator i = m_factories.begin(); i != m_factories.end(); ++i)
	{
		shared_ptr<IArchivesArchive> archive = (*i)->openArchive(filename);
		if(archive != nullptr)
			return archive;
	}

	return nullptr;
}

bool ArchivesManager::extract(const String &filename, const String &path)
{
	if(FileSystem::instance()->ensureDirectory(path) == false)
		return false;

	shared_ptr<IArchivesArchive> archive = open(filename);
	if(archive == nullptr)
		return false;
	
	shared_array<byte> buffer;
	try
	{
		buffer.reset(OS_NEW_ARRAY_T(byte, OS_ARCHIVESMANAGER_BUFFER_SIZE), os_delete_array_t(OS_ARCHIVESMANAGER_BUFFER_SIZE));
	}
	catch(std::exception &)
	{
		return false;
	}

	const IArchivesArchive::Entries &entries = archive->getEntries();
	for(IArchivesArchive::Entries::const_iterator i = entries.begin(); i != entries.end(); ++i)
	{
		shared_ptr<IArchivesArchiveEntry> entry = *i;

		OS_ASSERT(entry->getName().empty() == false);

		String entryPath = entry->getPath();
		bool ensure = entryPath.empty() == false;
		entryPath = utils::makeFolderPath(path, entryPath);

		if(ensure)
		{
			if(FileSystem::instance()->ensureDirectory(entryPath) == false)
				return false;
		}

		if(entry->isFolder())
		{
			if(FileSystem::instance()->ensureDirectory(utils::makeFolderPath(entryPath, entry->getName())) == false)
				return false;
		}
		else
		{
			shared_ptr<IStream> entryInputStream = entry->openStream();
			if(entryInputStream == nullptr)
				return false;			

			shared_ptr<File> entryOutputStream(OS_NEW File());
			if(entryOutputStream->open(utils::makeFilePath(entryPath, entry->getName()), File::ofWrite) == false)
				return false;

			for(;;)
			{
				uint32 result = entryInputStream->read(buffer.get(), OS_ARCHIVESMANAGER_BUFFER_SIZE);
				if(result == 0)
					break;

				if(entryOutputStream->write(buffer.get(), result) != result)
					return false;
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
