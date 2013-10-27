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

#ifndef _IDE_IPAGEABLE_H
#define _IDE_IPAGEABLE_H

#include "ideide.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IdeSkin;
class XMLNode;

//////////////////////////////////////////////////////////////////////

class EngineExport IIdePageable
{
// Construction
public:
	IIdePageable();
	virtual ~IIdePageable();

// Operations
public:
	static bool adjustOffset(const uint32 &total_items, const uint32 &items_to_show, uint32 &offset);

// Overridables
protected:
	void createPager(shared_ptr<IdeSkin> skin, shared_ptr<XMLNode> node, uint32 total_items, uint32 items_to_show, uint32 offset);

// Interface
public:
	virtual String getPageHref(uint32 offset) const = 0;

public:
	static const int32 OFFSET_LAST;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_IPAGEABLE_H
