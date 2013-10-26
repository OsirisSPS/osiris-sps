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
#include "hash.h"

#include "buffer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<Hash>
{
// Construction
public:
	pimpl(const Buffer &buffer = Buffer::EMPTY, HashType type = hashTypeUnknown);
	~pimpl();

// Attributes
public:
	inline const Buffer & getBuffer() const;
	inline HashType getType() const;

	inline bool empty() const;
	inline bool isValid() const;

// Operations
public:
	void assign(const Buffer &buffer, HashType type);
	void clear();

	std::string toHex() const;
	bool fromHex(const std::string &str, HashType type);

	std::string toBase64() const;
	bool fromBase64(const std::string &str, HashType type);

private:
	Buffer m_buffer;
	HashType m_type;
};

//////////////////////////////////////////////////////////////////////

pimpl<Hash>::pimpl(const Buffer &buffer, HashType type) : m_buffer(buffer),
														  m_type(type)
{

}

pimpl<Hash>::~pimpl()
{

}

inline const Buffer & pimpl<Hash>::getBuffer() const 
{ 
	return m_buffer; 
}

inline HashType pimpl<Hash>::getType() const 
{ 
	return m_type; 
}

inline bool pimpl<Hash>::empty() const
{
	return m_buffer.empty();
}

inline bool pimpl<Hash>::isValid() const
{
	return m_type != hashTypeUnknown && empty() == false;
}

void pimpl<Hash>::assign(const Buffer &buffer, HashType type)
{
	m_buffer = buffer;
	m_type = type;
}

void pimpl<Hash>::clear()
{
	m_buffer.clear();
	m_type = hashTypeUnknown;
}

std::string pimpl<Hash>::toHex() const
{
	return m_buffer.toHex();
}

bool pimpl<Hash>::fromHex(const std::string &str, HashType type)
{
	Buffer buffer;
	if(buffer.fromHex(str) == false)
		return false;

	assign(buffer, type);
	return true;
}

std::string pimpl<Hash>::toBase64() const
{
	return m_buffer.toBase64();
}

bool pimpl<Hash>::fromBase64(const std::string &str, HashType type)
{
	Buffer buffer;
	if(buffer.fromBase64(str) == false)
		return false;

	assign(buffer, type);
	return true;
}

//////////////////////////////////////////////////////////////////////

const Hash Hash::EMPTY;

//////////////////////////////////////////////////////////////////////

Hash::Hash()
{

}

Hash::Hash(const Buffer &buffer, HashType type) : m_impl(buffer, type)
{

}

Hash::Hash(const Hash &second) : BaseClass(second)
{
	*this = second;
}

Hash::~Hash()
{

}

const Buffer & Hash::getBuffer() const
{
	return m_impl->getBuffer();
}

HashType Hash::getType() const
{
	return m_impl->getType();
}

bool Hash::empty() const
{
	return m_impl->empty();
}

bool Hash::isValid() const
{
	return m_impl->isValid();
}

void Hash::assign(const Buffer &buffer, HashType type)
{
	m_impl->assign(buffer, type);
}

void Hash::clear()
{
	m_impl->clear();
}

std::string Hash::toHex() const
{
	return m_impl->toHex();
}

bool Hash::fromHex(const std::string &str, HashType type)
{
	return m_impl->fromHex(str, type);
}

std::string Hash::toBase64() const
{
	return m_impl->toBase64();
}

bool Hash::fromBase64(const std::string &str, HashType type)
{
	return m_impl->fromBase64(str, type);
}

Hash & Hash::operator =(const Hash &second)
{
	m_impl->assign(second.getBuffer(), second.getType());
	return *this;
}

bool Hash::operator ==(const Hash &second) const
{
	if(getType() != second.getType())
		return false;

	if(getBuffer() != second.getBuffer())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
