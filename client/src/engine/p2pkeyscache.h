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

#ifndef _OS_P2P_KEYSCACHE_H
#define _OS_P2P_KEYSCACHE_H

#include "base/object.h"
#include "boost/ptr_container/ptr_vector.hpp"
#include "boost/thread/recursive_mutex.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CryptKeyAgreement;

//////////////////////////////////////////////////////////////////////

namespace p2p {

//////////////////////////////////////////////////////////////////////

class EngineExport KeysCache : public Object
{
// Internal
private:
	class Key;
	typedef boost::ptr_vector<Key> Keys;

// Construction
public:
	KeysCache(uint32 cacheSize, uint32 keysSize, uint32 maxKeysUsage);
	virtual ~KeysCache();

// Operations
public:
	shared_ptr<CryptKeyAgreement> peekKey();
	void clear();

private:
	boost::recursive_mutex m_cs;
	Keys m_keys;
	uint32 m_cacheSize;
	uint32 m_keysSize;
	uint32 m_maxKeysUsage;
};

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_P2P_KEYSCACHE_H
