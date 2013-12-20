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
#include "file.h"

#include "boost/date_time/posix_time/posix_time.hpp"
#include "platformmanager.h"
#include <sys/stat.h>

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

File::File()
{
	m_handle = nullptr;
}

File::File(const String &filename, uint32 flags)
{
	m_handle = nullptr;
	open(filename, flags);
}

File::~File()
{
	close();
}

bool File::getStats(boost::posix_time::ptime *timeCreation, boost::posix_time::ptime *timeLastModify, boost::posix_time::ptime *timeLastAccess) const
{
    OS_ASSERT(m_handle != nullptr);
    return PlatformManager::instance()->fileStats(m_handle, timeCreation, timeLastModify, timeLastAccess);
}

int32 File::convert(SeekPosition pos)
{
	switch(pos)
	{
	case seekBegin:		return SEEK_SET;
	case seekCurrent:	return SEEK_CUR;
	case seekEnd:		return SEEK_END;
	}

	OS_ASSERTFALSE();
	return SEEK_SET;
}

String File::convert(uint32 flags)
{
	String mode;

	OS_STATIC_ASSERT((ofRead | ofWrite | ofReadWrite) == 3);
	switch(flags & 3)
	{
	case ofRead:		mode = _S("r");
						break;

	case ofWrite:		mode = (flags & ofNoTruncate) ? _S("a") : _S("w");
						break;

	case ofReadWrite:	mode = (flags & ofNoTruncate) ? _S("a+") : _S("w+");
						break;

	default:			OS_ASSERTFALSE();
						break;
	}

	if(mode.empty() == false)
	{
		if(flags & ofText)
			mode += _S("t");
		else
			mode += _S("b");	// default
	}

	return mode;
}

bool File::open(const String &filename, uint32 flags)
{
	close();

	String mode = convert(flags);
	if(mode.empty())
		return false;

	// Apre il file
	m_handle = PlatformManager::instance()->fileOpen(filename.c_str(), mode.c_str());
	return m_handle != nullptr;
}

bool File::is_open() const
{
	return m_handle != nullptr;
}

bool File::close()
{
	if(m_handle != nullptr)
	{
		bool closed = PlatformManager::instance()->fileClose(m_handle);
		m_handle = nullptr;
		return closed;
	}

	return true;
}

uint32 File::read(void *v, uint32 size) const
{
	if(m_handle == nullptr)
	{
		OS_ASSERTFALSE();
		return 0;
	}

	// Se la dimensione specificata è molto alta potrebbero verificarsi problemi in
	// una lettura secca se ad esempio si accede ad una risorsa di rete, pertanto
	// si effettua una lettura a blocchi se la dimensione supera OS_FILE_READ_CHUNK

	byte *buffer = reinterpret_cast<byte *>(v);

	uint32 total = 0;
	while(total < size)
	{
		uint32 chunk = std::min<uint32>(size - total, OS_FILE_READ_CHUNK);
		uint32 result = PlatformManager::instance()->fileRead(m_handle, buffer + total, chunk);
		total += result;

		if(result != chunk)
			return total;
	}

	return total;
}

uint32 File::write(const void *v, uint32 size)
{
	if(m_handle == nullptr)
	{
		OS_ASSERTFALSE();
		return 0;
	}

	// Se la dimensione specificata è molto alta potrebbero verificarsi problemi in
	// una scrittura secca se ad esempio si accede ad una risorsa di rete, pertanto
	// si effettua una scrittura a blocchi se la dimensione supera OS_FILE_WRITE_CHUNK

	const byte *buffer = reinterpret_cast<const byte *>(v);

	uint32 total = 0;
	while(total < size)
	{
		uint32 chunk = std::min<uint32>(size - total, OS_FILE_WRITE_CHUNK);
		uint32 result = PlatformManager::instance()->fileWrite(m_handle, buffer + total, chunk);
		total += result;

		if(result != chunk)
			return total;
	}

	return total;
}

bool File::seek(uint64 offset, SeekPosition from) const
{
	if(m_handle == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	return PlatformManager::instance()->fileSeek(m_handle, offset, convert(from));
}

uint64 File::position() const
{
	if(m_handle == nullptr)
	{
		OS_ASSERTFALSE();
		return 0;
	}

	return PlatformManager::instance()->filePosition(m_handle);
}

uint64 File::size() const
{
	if(m_handle == nullptr)
	{
		OS_ASSERTFALSE();
		return 0;
	}

	File *pFile = const_cast<File *>(this);

	// Salva la posizione attuale
	uint64 pos = pFile->position();
	// Si posiziona alla fine del file
	pFile->seekToEnd();
	// Salva la dimensione del file
	uint64 size = pFile->position();
	// Ripristina la posizione corrente
	pFile->seek(pos, seekBegin);
	// Restituisce la dimensione del file
	return size;
}

bool File::flush()
{
	if(m_handle == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	return PlatformManager::instance()->fileFlush(m_handle);
}

bool File::eof() const
{
	if(m_handle == nullptr)
	{
		OS_ASSERTFALSE();
		return true;
	}

    return PlatformManager::instance()->fileEof(m_handle);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
