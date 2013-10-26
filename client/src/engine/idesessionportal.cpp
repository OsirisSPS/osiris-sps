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
#include "idesessionportal.h"

#include "compatibilitymanager.h"
#include "dataaccount.h"
#include "objectsuser.h"
#include "iportaldatabase.h"
#include "engine.h"
#include "entitiesentitiescache.h"
#include "entitiessnapshotmanager.h"
#include "entitiesstatisticscache.h"
#include "ideaccount.h"
#include "ideaccountsmanager.h"
#include "ideskin.h"
#include "idesystem.h"
#include "iidepage.h"
#include "iideportalpage.h"
#include "isisendpoint.h"
#include "languagemanager.h"
#include "lock.h"
#include "log.h"
#include "options.h"
#include "osiriscommon.h"
#include "platformmanager.h"
#include "portalsaccount.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalsportalssystem.h"
#include "portalsprofilescache.h"
#include "reputationsmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdeSessionPortal::IdeSessionPortal() : m_guest(true)
{	
}

IdeSessionPortal::~IdeSessionPortal()
{

}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
