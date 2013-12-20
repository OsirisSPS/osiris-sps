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
#include "p2pkeyscache.h"

#include "base/object.h"
#include "cryptkeyagreement.h"
#include "lock.h"
#include "randomnumbergenerator.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {

//////////////////////////////////////////////////////////////////////

class KeysCache::Key : public Object
{
// Construction
public:
	Key();
	virtual ~Key();

// Attributes
public:
	inline shared_ptr<CryptKeyAgreement> getKey() const;

	inline uint32 getUsageCount() const;
	inline void setUsageCount(uint32 usageCount);

// Operations
public:
	bool init(uint32 keySize);

private:
	shared_ptr<CryptKeyAgreement> m_key;
	uint32 m_usageCount;
};

//////////////////////////////////////////////////////////////////////

KeysCache::Key::Key() : m_key(OS_NEW CryptKeyAgreement()),
						m_usageCount(0)
{

}

KeysCache::Key::~Key()
{

}

inline shared_ptr<CryptKeyAgreement> KeysCache::Key::getKey() const
{
	return m_key;
}

inline uint32 KeysCache::Key::getUsageCount() const
{
	return m_usageCount;
}

inline void KeysCache::Key::setUsageCount(uint32 usageCount)
{
	m_usageCount = usageCount;
}

bool KeysCache::Key::init(uint32 keySize)
{
	return m_key->init(keySize);
}

//////////////////////////////////////////////////////////////////////

KeysCache::KeysCache(uint32 cacheSize, uint32 keysSize, uint32 maxKeysUsage)
{
	m_cacheSize = std::max<uint32>(cacheSize, 1);
	OS_ASSERT(keysSize > 0);
	m_keysSize = keysSize;
	m_maxKeysUsage = std::max<uint32>(maxKeysUsage, 1);
}

KeysCache::~KeysCache()
{
	clear();
}

shared_ptr<CryptKeyAgreement> KeysCache::peekKey()
{
	OS_LOCK(m_cs);
	
	shared_ptr<CryptKeyAgreement> key = nullptr;
	if(m_keys.size() < m_cacheSize)
	{
		std::auto_ptr<Key> cachedKey(OS_NEW Key());
		if(cachedKey->init(m_keysSize))
			m_keys.push_back(cachedKey.release());
	}

	if(m_keys.empty())
		return nullptr;

	Keys::iterator i = m_keys.begin() + RandomNumberGenerator::instance()->generate<uint32>(0, static_cast<uint32>(m_keys.size() - 1));
	OS_ASSERT(i != m_keys.end());
	
	key = i->getKey();
	if((i->getUsageCount() + 1) >= m_maxKeysUsage)
		m_keys.erase(i);
	else
		i->setUsageCount(i->getUsageCount() + 1);

	return key;
}

void KeysCache::clear()
{
	OS_LOCK(m_cs);
	m_keys.clear();
}

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
