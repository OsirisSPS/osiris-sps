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

#ifndef _OS_ARCHIVES_IARCHIVEENTRY_H
#define _OS_ARCHIVES_IARCHIVEENTRY_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IArchivesArchive;
class IStream;

//////////////////////////////////////////////////////////////////////

class ArchivesExport IArchivesArchiveEntry : public Object
{
// Construction
public:
	IArchivesArchiveEntry(shared_ptr<IArchivesArchive> archive);
	virtual ~IArchivesArchiveEntry();

// Attributes
public:
	inline shared_ptr<IArchivesArchive> getArchive() const;

// Interface
public:
	// Indica se l'entry si riferisce ad un file o ad una directory
	virtual bool isFolder() const = 0;
	// Restituisce la directory del file
	virtual const String & getPath() const = 0;
	// Restituisce il nome del file
	virtual const String & getName() const = 0;
	// Restituisce la dimensione compressa del file (0 nel caso di directory)
	virtual uint64 getCompressedSize() const = 0;
	// Restituisce la dimensione decompressa del file (0 nel caso di directory)
	virtual uint64 getUncompressedSize() const = 0;
	// Apre lo stream del file (restituisce sempre nullptr nel caso di directory)
	virtual shared_ptr<IStream> openStream() const = 0;

private:
	weak_ptr<IArchivesArchive> m_archive;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IArchivesArchive> IArchivesArchiveEntry::getArchive() const { return m_archive.lock(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ARCHIVES_IARCHIVEENTRY_H
