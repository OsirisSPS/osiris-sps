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

#ifndef _OS_ARCHIVES_ZIPARCHIVE_H
#define _OS_ARCHIVES_ZIPARCHIVE_H

#include "iarchivesarchive.h"
#include "pimpl_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ArchivesZipArchive : public IArchivesArchive
{
// Construction
public:
	ArchivesZipArchive();
	virtual ~ArchivesZipArchive();

// Attributes
public:
	inline pimpl<ArchivesZipArchive> * getImpl() const;

// IArchive interface
public:
	virtual bool open(const String &filename);
	virtual void close();	
	virtual const Entries & getEntries() const;

private:
	pimpl_of<ArchivesZipArchive>::type m_impl;
};

//////////////////////////////////////////////////////////////////////

inline pimpl<ArchivesZipArchive> * ArchivesZipArchive::getImpl() const { return m_impl.get(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ARCHIVES_ZIPARCHIVE_H
