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

#ifndef _OS_CORE_DATATREE_H
#define _OS_CORE_DATATREE_H

#include "boost/operators.hpp"
#include "collections.h"
#include "dataitem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;
class CryptKey;

//////////////////////////////////////////////////////////////////////

class CoreExport DataTree : public Collections<String, DataItem>::handler,
							public Object,
						    public boost::equality_comparable<DataTree>
{
	typedef Object BaseClass;

    typedef Collections<String, DataItem>::handler HandlerBase;
	typedef Collections<String, DataItem>::smart_collection DataItems;

public:
	typedef DataItems::iterator iterator;

	#pragma pack(push, 1)
	struct HEADER
	{
		HEADER();
		uint8 version;
		uint8 flags;
		uint32 crc;
	};
	#pragma pack(pop)

// Construction
public:
	DataTree();
	DataTree(const DataTree &v);
	virtual ~DataTree();

private:
	void construct();
	void destroy();

// Attributes
public:
	inline DataTree * getParent() const;

	inline iterator begin() const;
	inline iterator end() const;

	inline DataItem * front() const;
	inline DataItem * back() const;

	inline bool exists(const String &name) const;
	inline bool empty() const;
	inline uint32 size() const;

// Operations
public:
	const DataItem & getV(const String &name) const;
	DataItem getV(const String &name, const DataItem &defaultValue) const;
	DataItem * getVP(const String &name) const;

	const DataTree & getT(const String &name) const;
	DataTree * getTP(const String &name) const;

	DataItem * setV(const String &name, const DataItem &v);
	DataTree * setT(const String &name, const DataTree &v);
	DataTree * setT(const String &name);

	bool remove(const String &name);
	void clear();

	bool read(const Buffer &buff, const CryptKey *key = nullptr);
	bool write(Buffer &buff, bool compress = true, const CryptKey *key = nullptr) const;

	bool load(const String &filename, const CryptKey *key = nullptr);
	bool save(const String &filename, bool compress = true, const CryptKey *key = nullptr) const;

	bool readItems(Buffer &buff);
	bool writeItems(Buffer &buff) const;

protected:
	bool readHeader(const Buffer &buff, HEADER &header);
	bool writeHeader(Buffer &buff, HEADER &header) const;

	DataItem * get(const String &name) const;
	DataItem * set(const String &name, const DataItem &v);

// Collections <String, DataItem>::handler interface
protected:
	virtual void onInsert(const String &name, DataItem *v);
	virtual void onRemove(const String &name, DataItem *v);
	virtual void onClear();

// Operators
public:
	DataTree & operator=(const DataTree &v);
	bool operator ==(const DataTree &v) const;
	
protected:
	DataTree *m_parent;
	DataItems *m_items;

public:
	static const DataTree EMPTY;

private:
	static const uint8 VERSION;
	static const uint8 COMPRESSBIT;
	static const uint8 ENCRYPTBIT;
};

//////////////////////////////////////////////////////////////////////

inline DataTree * DataTree::getParent() const { return m_parent; }

inline DataTree::iterator DataTree::begin() const { return m_items->begin(); }
inline DataTree::iterator DataTree::end() const { return m_items->end(); }

inline DataItem * DataTree::front() const { return m_items->front(); }
inline DataItem * DataTree::back() const { return m_items->back(); }

inline bool DataTree::exists(const String &name) const { return m_items->exists(name); }
inline bool DataTree::empty() const { return m_items->empty(); }
inline uint32 DataTree::size() const { return static_cast<uint32>(m_items->size()); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_DATATREE_H
