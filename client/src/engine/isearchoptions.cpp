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
#include "isearchoptions.h"

#include "idbconnection.h"
#include "searchquery.h"
#include "searchresult.h"
#include "searchresultsdatabase.h"
#include "entitiessnapshotmanager.h"
#include "idesearchparam.h"
#include "dataitem.h"
#include "datatable.h"
#include "idesession.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ISearchOptions::ISearchOptions(PortalObjectType type)
{
	m_type = type;
	m_active = true;
}

ISearchOptions::~ISearchOptions()
{

}

shared_ptr<IdeSearchParam> ISearchOptions::getControl()
{
	return shared_ptr<IdeSearchParam>(new IdeSearchParam(get_this_ptr()));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
