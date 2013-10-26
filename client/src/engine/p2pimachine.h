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

#ifndef _OS_P2P_IMACHINE_H
#define _OS_P2P_IMACHINE_H

#include "buffer.h"
#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {

//////////////////////////////////////////////////////////////////////

class EngineExport IMachine : public Object
{
// Construction
protected:
	IMachine();
	virtual ~IMachine();

// Interface
public:
	inline const Buffer & getID() const;
	inline void setID(const Buffer &id);

	inline const Buffer & getPublicKey() const;
	inline void setPublicKey(const Buffer &publicKey);

// Operations
public:
	virtual bool validate() const;

protected:
	Buffer m_id;				// ID della macchina
	Buffer m_publicKey;			// Chiave pubblica della macchina
};

//////////////////////////////////////////////////////////////////////

inline const Buffer & IMachine::getID() const { return m_id; }
inline void IMachine::setID(const Buffer &id) { m_id = id; }

inline const Buffer & IMachine::getPublicKey() const { return m_publicKey; }
inline void IMachine::setPublicKey(const Buffer &publicKey) { m_publicKey = publicKey; }

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_P2P_IMACHINE_H
