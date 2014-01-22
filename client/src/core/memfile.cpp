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
#include "memfile.h"

#include "buffer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

MemFile::MemFile(shared_ptr<Buffer> buffer) : m_buffer(buffer)
{	
	open();
}

MemFile::~MemFile()
{
	close();
}

bool MemFile::open()
{
	if(m_buffer == nullptr)
		m_buffer.reset(OS_NEW Buffer());

	return true;
}

bool MemFile::is_open() const
{
	return m_buffer != nullptr;
}

bool MemFile::close()
{
	m_buffer.reset();
	return true;
}

uint32 MemFile::read(void *v, uint32 size) const
{
	if(m_buffer == nullptr)
		return 0;

	return m_buffer->read(v, size);
}

uint32 MemFile::write(const void *v, uint32 size)
{
	if(m_buffer == nullptr)
		return 0;

	return m_buffer->write(v, size);
}

bool MemFile::seek(uint64 offset, SeekPosition from) const
{
	if(m_buffer == nullptr)
		return false;

	// Il buffer opera su un uint32 pertanto la conversione non causa problemi
	uint32 roundedOffsed = static_cast<uint32>(offset);
	uint32 pos = 0;

	switch(from)
	{
	case seekBegin:		pos = std::min<uint32>(roundedOffsed, m_buffer->getSize());

						break;

	case seekCurrent:	pos = std::min<uint32>(m_buffer->getOffset() + roundedOffsed, m_buffer->getSize());

						break;

	case seekEnd:		if(roundedOffsed < m_buffer->getSize())
							pos = m_buffer->getSize() - roundedOffsed;
						else
							pos = 0;

						break;

	default:			OS_ASSERTFALSE();
						return false;
	}

	return m_buffer->seekAt(pos);
}

uint64 MemFile::position() const
{
	if(m_buffer == nullptr)
		return 0;

	return m_buffer->getOffset();
}

uint64 MemFile::size() const
{
	if(m_buffer == nullptr)
		return 0;

	return m_buffer->getSize();
}

bool MemFile::flush()
{
	return m_buffer != nullptr;
}

bool MemFile::eof() const
{
	if(m_buffer == nullptr)
		return true;

	return m_buffer->getPosition() == m_buffer->getEndPosition();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
