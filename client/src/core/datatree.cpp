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
#include "datatree.h"

#include "boost/static_assert.hpp"
#include "buffer.h"
#include "cryptmanager.h"
#include "cryptkey.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const DataTree DataTree::EMPTY;

const uint8 DataTree::VERSION = 1;
const uint8 DataTree::COMPRESSBIT = 1 << 0;
const uint8 DataTree::ENCRYPTBIT = 1 << 1;

//////////////////////////////////////////////////////////////////////

BOOST_STATIC_ASSERT(sizeof(DataTree::HEADER) == 6);		// uint8 + uint8 + uint32

//////////////////////////////////////////////////////////////////////

DataTree::HEADER::HEADER()
{
	OS_ZEROMEMORY(this, sizeof(HEADER));
}

//////////////////////////////////////////////////////////////////////

DataTree::DataTree()
{
	construct();
}

DataTree::DataTree(const DataTree &v) : BaseClass(v)
{
	construct();
	*this = v;
}

DataTree::~DataTree()
{
	destroy();
}

void DataTree::construct()
{
	m_parent = nullptr;
	m_items = OS_NEW_T(DataItems)(this);
}

void DataTree::destroy()
{
	clear();

	OS_DELETE_T(m_items);
	m_items = nullptr;
}

const DataItem & DataTree::getV(const String &name) const
{
	DataItem *item = getVP(name);
	return item != nullptr ? *item : DataItem::EMPTY;
}

DataItem DataTree::getV(const String &name, const DataItem &defaultValue) const
{
	DataItem *item = getVP(name);
	if(item == nullptr)
		return defaultValue;

	return *item;
}

DataItem * DataTree::getVP(const String &name) const
{
	return get(name);
}

const DataTree & DataTree::getT(const String &name) const
{
	DataTree *tree = getTP(name);
	return tree ? *tree : DataTree::EMPTY;
}

DataTree * DataTree::getTP(const String &name) const
{
	DataItem *item = get(name);
	if(item != nullptr && item->isDataTree())
		return *item;

	return nullptr;
}

DataItem * DataTree::setV(const String &name, const DataItem &v)
{
	return set(name, v);
}

DataTree * DataTree::setT(const String &name, const DataTree &v)
{
	return *set(name, v);
}

DataTree * DataTree::setT(const String &name)
{
	return *set(name, DataTree::EMPTY);
}

bool DataTree::remove(const String &name)
{
	return m_items->remove(name);
}

void DataTree::clear()
{
	m_items->clear();
}

bool DataTree::read(const Buffer &buff, const CryptKey *key)
{
	OS_ASSERT(buff.getPosition() == buff.getData());

	clear();

	HEADER header;
	if(readHeader(buff, header) == false)
		return false;
	if(header.version > VERSION)
		return false;

	Buffer originalBuffer;
	if(buff.readBuffer(originalBuffer) == false)
		return false;

	if(header.crc != CryptManager::instance()->CRC32(originalBuffer.getData(), originalBuffer.getSize()))
		return false;

	Buffer *pInput = &originalBuffer;

	Buffer decryptedBuffer;
	if((header.flags & ENCRYPTBIT) == ENCRYPTBIT)
	{
		if(key == nullptr)
			return false;

		pInput->seekToBegin();

		if(CryptManager::instance()->aesDecrypt(*key, pInput->getData(), pInput->getSize(), decryptedBuffer) == false)
			return false;

		pInput = &decryptedBuffer;
	}

	Buffer decompressedBuffer;
	if((header.flags & COMPRESSBIT) == COMPRESSBIT)
	{
		pInput->seekToBegin();

		if(CryptManager::instance()->decompress(compressorTypeZlib, pInput->getData(), pInput->getSize(), decompressedBuffer) == false)
			return false;

		pInput = &decompressedBuffer;
	}

	pInput->seekToBegin();
	if(readItems(*pInput) == false)
		return false;

	return true;
}

bool DataTree::write(Buffer &buff, bool compress, const CryptKey *key) const
{
	OS_ASSERT(buff.empty());
	buff.clear();

	HEADER header;
	header.version = VERSION;

	Buffer originalBuffer;
	if(writeItems(originalBuffer) == false)
		return false;

	Buffer *pOutput = &originalBuffer;

	Buffer compressedBuffer;
	if(compress)
	{
		pOutput->seekToBegin();

		if(CryptManager::instance()->compress(compressorTypeZlib, pOutput->getData(), pOutput->getSize(), compressedBuffer, compressionLevelHigh) == false)
			return false;

		if(compressedBuffer.getSize() < pOutput->getSize())
		{
			header.flags |= COMPRESSBIT;
			pOutput = &compressedBuffer;
		}
	}

	Buffer encryptedBuffer;
	if(key != nullptr)
	{
		pOutput->seekToBegin();

		if(CryptManager::instance()->aesEncrypt(*key, pOutput->getData(), pOutput->getSize(), encryptedBuffer) == false)
			return false;

		header.flags |= ENCRYPTBIT;
		pOutput = &encryptedBuffer;
	}

	// Calcola il crc del buffer
	header.crc = CryptManager::instance()->CRC32(pOutput->getData(), pOutput->getSize());

	if(writeHeader(buff, header) == false)
		return false;

	return buff.writeBuffer(*pOutput);
}

bool DataTree::load(const String &filename, const CryptKey *key)
{
	Buffer buffer;
	if(buffer.load(filename) == false)
		return false;

	return read(buffer, key);
}

bool DataTree::save(const String &filename, bool compress, const CryptKey *key) const
{
	Buffer buffer;
	if(write(buffer, compress, key) == false)
		return false;

	return buffer.save(filename);
}

bool DataTree::readItems(Buffer &buff)
{
	uint32 count = 0;
	if(buff.readCount(count) == false)
		return false;

	for(uint32 i = 0; i < count; i++)
	{
		String name;
		if(buff.readString(name) == false)
			return false;

		std::auto_ptr<DataItem> item(OS_NEW DataItem());
		if(item->read(buff) == false)
			return false;
		
        m_items->push_back(name, item.release());
	}

	return true;
}

bool DataTree::writeItems(Buffer &buff) const
{
	uint32 count = size();
	if(buff.writeCount(count) == false)
		return false;

	for(DataItems::iterator i = begin(); i != end(); ++i)
	{
		if(buff.writeString(i->first) == false)
			return false;

		if(i->second->write(buff) == false)
			return false;
	}

	return true;
}

bool DataTree::readHeader(const Buffer &buff, HEADER &header)
{
	return buff.read(&header, sizeof(HEADER)) == sizeof(HEADER);
}

bool DataTree::writeHeader(Buffer &buff, HEADER &header) const
{
	return buff.write(&header, sizeof(HEADER)) == sizeof(HEADER);
}

DataItem * DataTree::get(const String &name) const
{
	return m_items->get(name);
}

DataItem * DataTree::set(const String &name, const DataItem &v)
{
	OS_ASSERT(name.empty() == false);
	if(name.empty())
		return nullptr;

	DataItem *item = getVP(name);
	if(item != nullptr)
	{
		*item = v;
	}
	else
	{
		item = OS_NEW DataItem(v);
		m_items->push_back(name, item);
	}

	return item;
}

void DataTree::onInsert(const String &name, DataItem *v)
{
	OS_ASSERT(v != nullptr);
	if(v->isDataTree())
	{
		DataTree *tree = *v;
		tree->m_parent = this;
	}
}

void DataTree::onRemove(const String &name, DataItem *v)
{
	// Questo passaggio è necessario in quanto la DataItem è derivata da RefCounted: l'elemento potrebbe venire
	// rimosso, ma non cancellato fisicamente, pertanto bisogna resettarne il riferimento all'elemento padre.
	// Se si modificasse la base DataItem tale operazione non sarebbe necessaria

	OS_ASSERT(v != nullptr);
	if(v->isDataTree())
	{
		DataTree *tree = *v;
		tree->m_parent = nullptr;
	}
}

void DataTree::onClear()
{
	// N.B.: stesse note di DataTree::onRemove

	for(DataItems::iterator i = m_items->begin(); i != m_items->end(); ++i)
	{
		DataItem *item = i->second;
		if(item->isDataTree())
		{
			DataTree *tree = *item;
			tree->m_parent = nullptr;
		}
	}
}

DataTree & DataTree::operator=(const DataTree &v)
{
	clear();

	for(DataItems::iterator i = v.m_items->begin(); i != v.m_items->end(); set(i->first, *i->second), ++i);

	return *this;
}

bool DataTree::operator ==(const DataTree &v) const
{
	if(size() != v.size())
		return false;

	for(DataItems::iterator i = begin(); i != end(); ++i)
	{
		DataItem *item = v.getVP(i->first);
		if(item == nullptr)
			return false;

		if(getV(i->first) != *item)
			return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
