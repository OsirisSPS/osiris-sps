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

#ifndef _OS_CORE_ISTREAMLAYER_H
#define _OS_CORE_ISTREAMLAYER_H

#include "base/object.h"
#include "boost/noncopyable.hpp"
#include "io.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport IStreamLayer : public Object,
								public boost::noncopyable
{
// Construction
public:
	IStreamLayer();
	virtual ~IStreamLayer();

// Attributes
public:
	inline shared_ptr<IStreamLayer> getNext() const;
	inline void setNext(shared_ptr<IStreamLayer> next);

	uint64 available() const;

// Interface
public:
	virtual uint32 read(void *v, uint32 size) const = 0;
	virtual uint32 write(const void *v, uint32 size) = 0;

// Overridables
public:
	virtual bool seek(uint64 offset, SeekPosition from);
	virtual uint64 position() const;
	virtual uint64 size() const;
	virtual bool close();
	virtual bool flush();

protected:
	shared_ptr<IStreamLayer> m_next;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IStreamLayer> IStreamLayer::getNext() const { return m_next; }
inline void IStreamLayer::setNext(shared_ptr<IStreamLayer> next) { m_next = next; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_ISTREAMLAYER_H
