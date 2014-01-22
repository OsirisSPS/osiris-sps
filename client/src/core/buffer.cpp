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
#include "buffer.h"

#include "constants.h"
#include "cryptmanager.h"
#include "file.h"
#include "utils.h"
#include <memory.h>

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const Buffer Buffer::EMPTY;

//////////////////////////////////////////////////////////////////////

Buffer::Buffer(uint32 growSize)
{
	construct(growSize);
}

Buffer::Buffer(const Buffer &second) : BaseClass(second)
{
	construct(second.getGrowSize());
	*this = second;
}

Buffer::~Buffer()
{
	clear();
}

void Buffer::construct(uint32 growSize)
{
	m_data = nullptr;
	m_position = nullptr;
	m_size = 0;
	m_allocSize = 0;
	setGrowSize(growSize);
}

byte Buffer::getAt(uint32 position) const
{
	OS_ASSERT(position < getSize());
	return m_data[position];
}

uint32 Buffer::assign(const void *data, uint32 size)
{
	clear();
	return put(data, size);
}

bool Buffer::compare(const void *data, uint32 size) const
{
	if(getSize() != size)
		return false;

	return memcmp(getData(), data, size) == 0;
}

bool Buffer::seekToBegin() const
{
	return seekAt(getData());
}

bool Buffer::seekToEnd() const
{
	return seekAt(getEndPosition());
}

bool Buffer::seekAt(byte *position) const
{
	if(position >= getData() && position <= getEndPosition())
	{
		m_position = const_cast<byte *>(position);
		return true;
	}

	OS_ASSERTFALSE();
	return false;
}

bool Buffer::seekAt(uint32 position) const
{
	return seekAt(getData() + position);
}

bool Buffer::seekOffset(uint32 offset) const
{
	return seekAt(getPosition() + offset);
}

uint32 Buffer::peek(void *data, uint32 size) const
{
	if(size > 0)
	{
		size = std::min<uint32>(size, getAvailable());
		memcpy(data, m_position, size);
	}

	return size;
}

uint32 Buffer::read(void *data, uint32 size) const
{
	size = peek(data, size);
	seekOffset(size);

	return size;
}

uint32 Buffer::put(const void *data, uint32 size)
{
	if(size > 0)
	{
		if(reserve(size) == false)
			return 0;

		memcpy(m_position, data, size);
	}

	return size;
}

uint32 Buffer::append(const void *data, uint32 size)
{
	// Salva l'offset corrente (non come puntatore perchè il buffer potrebbe essere riallocato)
	uint32 offset = getOffset();
	// Si sposta al termine del buffer
	if(seekToEnd() == false)
		return 0;

	// Accoda i dati
	size = put(data, size);
	// Ripristina la posizione precedente
	seekAt(offset);

	return size;
}

uint32 Buffer::write(const void *data, uint32 size)
{
	size = put(data, size);
	seekOffset(size);

	return size;
}

bool Buffer::readString(String &str) const
{
	OS_ASSERT(str.empty());

	uint32 size = 0;
	if(readCount(size) == false)
		return false;

	if(size > 0)
	{
		// Razor: From UCS to UTF8
		/*
		if(size > getAvailable())
			return false;	// Evita di allocare inutilmente un buffer se la dimensione va oltre quella disponibile

		// Crea il buffer considerando il carattere terminatore (visto che non viene serializzato)
		uint32 buffer_size = size + 1*sizeof(uchar);
		scoped_array<byte, os_deallocate_t> buffer(OS_ALLOCATE_T(byte, buffer_size));
		// Inizializza il buffer: si potrebbe settare a 0 anche soltato il carattere terminatore tenendo presente però che occupa 2 bytes nel caso di un wchar
		OS_ZEROMEMORY(buffer.get(), buffer_size);

		// Carica il buffer della stringa
		if(read(buffer.get(), size) != size)
			return false;

		//	Nota: se si passa anche il numero di caratteri tenere presente della dimensione di tchar rispetto a byte
		str.assign(reinterpret_cast<const uchar *>(buffer.get()));
		*/

		if(size > getAvailable())
		{
			OS_ASSERTFALSE();
			return false;	// Evita di allocare inutilmente un buffer se la dimensione va oltre quella disponibile
		}

		scoped_array<achar, os_deallocate_t> buffer(OS_ALLOCATE_T(achar, size));
		if(read(buffer.get(), size) != size)
			return false;

		std::string utf8_str(buffer.get(), static_cast<std::string::size_type>(size));		
		str.from_utf8(utf8_str);
	}
	else
	{
		str.clear();
	}

	return true;
}

bool Buffer::writeString(const String &str)
{
	// Razor: From UCS to UTF8
	/*
	uint32 size = static_cast<uint32>(str.buffer_size());
	if(writeCount(size) == false)
		return false;

	return write(str.buffer(), size) == size;
	*/

	std::string s = str.to_utf8();
	uint32 size = static_cast<uint32>(s.size());
	if(writeCount(size) == false)
		return false;

	return write(s.data(), size) == size;
}

bool Buffer::writeString(const uchar *str)
{
	return writeString(utils::safeString(str));
}

bool Buffer::readBuffer(Buffer &buff) const
{
	OS_ASSERT(buff.empty());
	uint32 size = 0;
	if(readCount(size) == false)
		return false;

	if(size > 0)
	{
		if(size > getAvailable())
			return false;	// Evita di allocare inutilmente un buffer se la dimensione va oltre quella disponibile

        if(buff.grow(size) == false)
			return false;

		if(read(buff.getData(), size) != size)
			return false;
	}
	else
	{
		buff.clear();
	}

	return true;
}

bool Buffer::writeBuffer(const Buffer &buff)
{
	uint32 size = buff.getSize();
	if(writeCount(size) == false)
		return false;

	return write(buff.getData(), size) == size;
}

bool Buffer::readCount(uint32 &count) const
{
	uint16 count16 = 0;
	if(read(&count16, sizeof(uint16)) != sizeof(uint16))
		return false;

	if(count16 != 0xffff)
	{
		count = count16;
		return true;
	}

	uint32 count32 = 0;
	if(read(&count32, sizeof(uint32)) != sizeof(uint32))
		return false;

	count = count32;
	return true;
}

bool Buffer::writeCount(uint32 count)
{
	uint16 count16 = 0xffff;
	if(count < 0xffff)
	{
		count16 = static_cast<uint16>(count);
		return write(&count16, sizeof(uint16)) == sizeof(uint16);
	}

	if(write(&count16, sizeof(uint16)) != sizeof(uint16))
		return false;

	if(write(&count, sizeof(uint32)) != sizeof(uint32))
		return false;

	return true;
}

void Buffer::clear()
{
	OS_DEALLOCATE(m_data);
	m_data = nullptr;

	m_position = nullptr;
	m_size = 0;
	m_allocSize = 0;
}

bool Buffer::grow(uint32 size)
{
	uint32 offset = getOffset();
	if(size > m_allocSize)
	{
		while(size > m_allocSize)
		{
			m_allocSize += m_growSize;
		}

		if(m_data != nullptr)
		{
			m_data = static_cast<byte *>(OS_REALLOCATE(m_data, m_allocSize));
			if(m_data == nullptr)
			{
				OS_ASSERTFALSE();

				clear();
				return false;
			}
		}
		else
		{
			m_data = static_cast<byte *>(OS_ALLOCATE(m_allocSize));
			if(m_data == nullptr)
			{
				OS_ASSERTFALSE();

				clear();
				return false;
			}

			OS_ZEROMEMORY(m_data, m_allocSize);
		}

		m_size = size;
		m_position = m_data + offset;
	}
	else
	{
		if(size > m_size)
			m_size = size;
	}

	return true;
}

bool Buffer::reserve(uint32 size)
{
	return grow(getOffset() + size);
}

void Buffer::reset()
{
	m_position = m_data;
	m_size = 0;
}

bool Buffer::load(const String &filename)
{
	shared_ptr<File> file(OS_NEW File());
	// Apre il file specificato in lettura
	if(file->open(filename, File::ofRead) == false)
		return false;

	// Carica il file
	return load(file);
}

bool Buffer::load(shared_ptr<const IStream> stream)
{
	clear();

	bool done = true;

	// Ottiene la posizione del file
	uint32 pos = static_cast<uint32>(stream->position());
	// Calcola la dimensione del file
	uint32 size = static_cast<uint32>(stream->size());
	// Calcola il numero di bytes da leggere
	uint32 needed = size - pos;
	if(needed > 0)
	{
		// Alloca la dimensione necessaria
		if(grow(needed))
		{
			// Legge i dati dal file
			done = stream->read(m_data, needed) == needed;
			// Inizializza la posizione corrente
			seekToBegin();
		}
	}

	return done;
}

bool Buffer::save(const String &filename) const
{
	shared_ptr<File> file(OS_NEW File());
	// Apre il file specificato in scrittura
	if(file->open(filename, File::ofWrite) == false)
		return false;

	// Salva il file
	return save(file);
}

bool Buffer::save(shared_ptr<IStream> stream) const
{
	if(m_size == 0)
		return true;

	// Salva il file
	return stream->write(m_data, m_size) == m_size;
}

const byte * Buffer::find(const byte *value, uint32 length, const byte *pos) const
{
	OS_ASSERT(value != nullptr && length > 0);

	const byte *startPosition = getData();
	const byte *endPosition = getEndPosition();

	// Se non è stat specificata una posizione parte dall'inizio
	if(pos == nullptr)
		pos = startPosition;

	OS_ASSERT(pos >= startPosition && pos < endPosition);
	// Verifica che la posizione specificata sia valida
	if(pos >= startPosition && pos < endPosition)
	{
		// Effettua la ricerca
		const byte *result = std::search(pos, endPosition, value, value + length);
		OS_ASSERT(result <= endPosition);
		// Controlla se il valore è stato trovato
		if(result != endPosition)
			return result;
	}

	return nullptr;
}

bool Buffer::erase(uint32 pos, uint32 count)
{
	uint32 size = getSize();

	if(pos > size)
		return false;

	if(count > (size - pos))
		count = size - pos;

	if(count > 0)
	{
		// 07/06/2010: modificata la gestione della posizione

		/*
		// Salva l'offset corrente
		uint32 offset = getOffset();

		memmove(m_data + pos, m_data + pos + count, size - pos - count);
		m_size = size - count;

		if(offset > m_size)
			m_position = m_data + m_size;
		*/

		uint32 offset = getOffset();

		memmove(m_data + pos, m_data + pos + count, size - pos - count);
		m_size = size - count;
	
		if(offset > pos)
		{
			if(offset > (pos + count))
				offset -= count;
			else
				offset = pos;

			if(seekAt(m_data + offset) == false)
			{
				OS_ASSERTFALSE();
				return false;
			}
		}
	}

	return true;
}

bool Buffer::eraseOffset()
{
	return erase(0, getOffset());
}

std::string Buffer::toHex() const
{
	std::string str;
	if(CryptManager::instance()->encodeHex(getData(), getSize(), str) == false)
	{
		OS_ASSERTFALSE();
		return constants::empty_string;
	}

	return str;
}

bool Buffer::fromHex(const std::string &str)
{
	return CryptManager::instance()->decodeHex(str, *this);
}

std::string Buffer::toBase64() const
{
	std::string str;
	if(CryptManager::instance()->encodeBase64(getData(), getSize(), str) == false)
	{
		OS_ASSERTFALSE();
		return constants::empty_string;
	}

	return str;
}

bool Buffer::fromBase64(const std::string &str)
{
	return CryptManager::instance()->decodeBase64(str, *this);
}

void Buffer::xorValue(const void *data, uint32 size)
{
	byte *position = getPosition();
	uint32 available = getAvailable();

	for(uint32 i = 0; i < available; i++)
	{
		for(uint32 k = 0; k < size; k++)
		{
			position[i] ^= static_cast<const byte *>(data)[k];
		}
	}
}

Buffer & Buffer::operator =(const Buffer &second)
{
	clear();
	m_growSize = second.m_growSize;

	write(second.getData(), second.getSize());
	seekAt(second.getOffset());

	return *this;
}

bool Buffer::operator ==(const Buffer &second) const
{
	return compare(second.getData(), second.getSize());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
