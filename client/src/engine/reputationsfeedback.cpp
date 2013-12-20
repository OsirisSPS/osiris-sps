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
#include "reputationsfeedback.h"

#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ReputationsFeedback::ReputationsFeedback(const String &hashFrom, const String &hashTo, const ReputationsScore &score, const int32 &level)
{
	m_hashFrom = hashFrom;
	m_hashTo = hashTo;
	m_score = score;
	m_level = level;
}

ReputationsFeedback::~ReputationsFeedback()
{
	
}

void ReputationsFeedback::addChild(shared_ptr<ReputationsFeedback> feedback) 
{
	OS_ASSERT(feedback != nullptr);

	if(feedback != nullptr && utils::exists(m_childs, feedback) == false)
		m_childs.push_back(feedback); 
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
