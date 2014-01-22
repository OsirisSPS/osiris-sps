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

#ifndef _OS_CORE_BUFFER_H
#define _OS_CORE_BUFFER_H

#include "base/object.h"
#include "boost/operators.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IStream;

//////////////////////////////////////////////////////////////////////

class CoreExport Buffer : public boost::equality_comparable<Buffer>,
						  public Object
{
	typedef Object BaseClass;

public:
	enum { npos = uint32(-1) };

//	Construction
public:
	Buffer(uint32 growSize = 1024);
	Buffer(const Buffer &second);
	virtual ~Buffer();

private:
	void construct(uint32 growSize);

//	Attributes
public:
	inline byte * getData() const;
	inline byte * getPosition() const;

	inline byte * getEndPosition() const;
	inline byte * getAllocEndPosition() const;

	inline uint32 getAvailable() const;
	inline bool isReadable() const;

	inline uint32 getOffset() const;

	inline uint32 getSize() const;
	inline uint32 getAllocSize() const;

	inline uint32 getGrowSize() const;
	inline void setGrowSize(uint32 Size);

	inline bool empty() const;

	byte getAt(uint32 position) const;

//	Operations
public:
	uint32 assign(const void *data, uint32 size);
	bool compare(const void *data, uint32 size) const;

	byte * seekToBegin() const;
	byte * seekToEnd() const;
	byte * seekAt(byte *position) const;
	byte * seekAt(uint32 position) const;
	byte * seekOffset(uint32 offset) const;

	// Effettua una lettura senza spostare la posizione corrente
	uint32 peek(void *data, uint32 size) const;
	// Effettua una lettura spostando la posizione corrente
	uint32 read(void *data, uint32 size) const;
	// Effettua una scrittura senza spostare la posizione corrente
	uint32 put(const void *data, uint32 size);
	// Accoda dei dati al buffer (aumentando se necessario la dimensione) senza spostare la posizione corrente
	uint32 append(const void *data, uint32 size);
	// Effettua una scrittura spostando la posizione corrente
	uint32 write(const void *data, uint32 size);
	template <typename T>
	uint32 write(const T &value);

	bool readString(String &str) const;
	bool writeString(const String &str);
	bool writeString(const uchar *str);

	bool readBuffer(Buffer &buff) const;
	bool writeBuffer(const Buffer &buff);

	bool readCount(uint32 &count) const;
	bool writeCount(uint32 count);

	void clear();
	bool grow(uint32 size);
	bool reserve(uint32 size);
	// Reset the buffer position without deallocate the current data
	void reset();

	bool load(const String &filename);
	bool load(shared_ptr<const IStream> stream);
	bool save(const String &filename) const;
	bool save(shared_ptr<IStream> stream) const;

	const byte * find(const byte *value, uint32 length, const byte *pos = nullptr) const;

	bool erase(uint32 pos, uint32 count = npos);
	bool eraseOffset();

	std::string toHex() const;
	bool fromHex(const std::string &str);

	std::string toBase64() const;
	bool fromBase64(const std::string &str);

	void xorValue(const void *data, uint32 size);
		
//	Operators
public:
	Buffer & operator =(const Buffer &second);
	bool operator ==(const Buffer &second) const;

public:
	static const Buffer EMPTY;

protected:
	byte *m_data;
	mutable byte *m_position;
	uint32 m_size;
	uint32 m_allocSize;
	uint32 m_growSize;
};

//////////////////////////////////////////////////////////////////////

inline byte * Buffer::getData() const { return m_data; }
inline byte * Buffer::getPosition() const { return m_position; }

inline byte * Buffer::getEndPosition() const { return getData() + getSize(); }
inline byte * Buffer::getAllocEndPosition() const { return getData() + getAllocSize(); }

inline uint32 Buffer::getAvailable() const { return getSize() - getOffset(); }
inline bool Buffer::isReadable() const { return getAvailable() > 0; }

inline uint32 Buffer::getOffset() const { return static_cast<uint32>(getPosition() - getData()); }

inline uint32 Buffer::getSize() const { return m_size; }
inline uint32 Buffer::getAllocSize() const { return m_allocSize; }

inline uint32 Buffer::getGrowSize() const { return m_growSize; }
inline void Buffer::setGrowSize(uint32 size) { m_growSize = std::max<uint32>(size, 1); }

inline bool Buffer::empty() const { return m_size == 0; }

template <typename T>
uint32 Buffer::write(const T &value)
{
	return write(&value, sizeof(T));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_BUFFER_H
