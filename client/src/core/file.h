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

#ifndef _OS_CORE_FILE_H
#define _OS_CORE_FILE_H

#include "istream.h"

//////////////////////////////////////////////////////////////////////

#define OS_FILE_READ_CHUNK						1024*1024
#define OS_FILE_WRITE_CHUNK						1024*1024

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport File : public IStream
{
	typedef IStream StreamBase;

public:
	enum OpenFlags
	{
		ofRead	     = (uint32) 0x00001,		// Apre il file in sola lettura
		ofWrite	     = (uint32) 0x00002,		// Apre il file in sola scrittura
		ofReadWrite  = (uint32) 0x00003,		// Apre il file in lettura/scrittura
		ofNoTruncate = (uint32) 0x00004,		// Se ofWrite o ofReadWrite sono settati indica di non cancellare il contenuto
		ofBinary	 = (uint32) 0x00008,		// Apre il file come binario (modalità default)
		ofText		 = (uint32) 0x00010,		// Apre il file come test
	};

// Construction
public:
	File();
	File(const String &filename, uint32 flags);
	virtual ~File();

// Attributes
public:
	inline FILE * getHandle() const;

	// Restituisce le statistiche sul file in UTC
	bool getStats(boost::posix_time::ptime *timeCreation, boost::posix_time::ptime *timeLastModify = nullptr, boost::posix_time::ptime *timeLastAccess = nullptr) const;

private:
	static int32 convert(SeekPosition pos);
	static String convert(uint32 flags);

// Operations
public:
	virtual bool open(const String &filename, uint32 flags);

// IStream interface
public:
	virtual bool is_open() const;
	virtual bool close();
	virtual uint32 read(void *v, uint32 size) const;
	virtual uint32 write(const void *v, uint32 size);
	virtual bool seek(uint64 offset, SeekPosition from) const;
	virtual uint64 position() const;
	virtual uint64 size() const;
	virtual bool flush();
	virtual bool eof() const;

protected:
	FILE *m_handle;
};

//////////////////////////////////////////////////////////////////////

inline FILE * File::getHandle() const { return m_handle; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_FILE_H
