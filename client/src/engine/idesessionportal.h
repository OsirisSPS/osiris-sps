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

#ifndef _ENGINE_IDESESSIONPORTAL_H
#define _ENGINE_IDESESSIONPORTAL_H

#include "boost/thread/recursive_mutex.hpp"
#include "datadata.h"
#include "datetime.h"
#include "enable_this_ptr.h"
#include "languageresult.h"
#include "objectsobjects.h"
#include "dbdb.h"
#include "entitiesentities.h"
#include "buffer.h"
#include "ideide.h"
#include "base/object.h"
#include "portalsportals.h"
#include "range.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Portal;

//////////////////////////////////////////////////////////////////////

class EngineExport IdeSessionPortal : public Object,
									  public enable_this_ptr<IdeSessionPortal>
{
// Construction
public:
	IdeSessionPortal();
	virtual ~IdeSessionPortal();

// Attributes
public:	
	inline bool getGuest() const;
	inline void setGuest(bool guest);
	// Restituisce la lista degli oggetti selezionati dall'utente
	inline StringMap & getSelectedObjects();

	// Restituisce se il LoggedUser  valido, cio gi agganciato ad un account o ad un anonimo.
	//inline bool isLogged() const;
		
// Operations
public:

protected:
	bool m_guest;
	StringMap m_selectedObjects;
};

//////////////////////////////////////////////////////////////////////

inline bool IdeSessionPortal::getGuest() const { return m_guest; }
inline void IdeSessionPortal::setGuest(bool guest) { m_guest = guest; }
inline StringMap & IdeSessionPortal::getSelectedObjects() { return m_selectedObjects; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_IDESESSIONPORTAL_H
