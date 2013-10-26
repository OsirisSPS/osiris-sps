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

#ifndef _OS_ENGINE_ISEARCHOPTIONS_H
#define _OS_ENGINE_ISEARCHOPTIONS_H

#include "search.h"

#include "collections.h"
#include "datadata.h"
#include "iportaldatabase.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IdeSearchParam;

//////////////////////////////////////////////////////////////////////

class EngineExport ISearchOptions : public Object,
									public enable_this_ptr<ISearchOptions>
{
// Construction
public:
	ISearchOptions(PortalObjectType type);
	virtual ~ISearchOptions();

// Attributes
public:
	inline PortalObjectType getType() const;

	inline bool getActive() const;
	inline void setActive(bool active);

// Operations
public:
	virtual shared_ptr<IdeSearchParam> getControl();

protected:
	PortalObjectType m_type;
	bool m_active;
};

//////////////////////////////////////////////////////////////////////

inline PortalObjectType ISearchOptions::getType() const { return m_type; }
inline bool ISearchOptions::getActive() const { return m_active; }
inline void ISearchOptions::setActive(bool active) { m_active = active; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_ISEARCHOPTIONS_H
