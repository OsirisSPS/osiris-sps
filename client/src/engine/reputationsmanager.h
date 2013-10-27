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

#ifndef _REPUTATIONS_MANAGER_H
#define _REPUTATIONS_MANAGER_H

#include "base/object.h"
#include "dbdb.h"
#include "ids.h"
#include "languageresult.h"
#include "objectsobjects.h"
#include "reputationsreputations.h"
#include "reputationsscore.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;
class IPortalDatabase;


//////////////////////////////////////////////////////////////////////

class EngineExport ReputationsManager : public Object
{
	typedef list<ObjectID>::type StepsList;

// Construction
public:
	ReputationsManager();
	virtual ~ReputationsManager();

// Attributes
public:
	bool exists(shared_ptr<IPortalDatabase> database, const ObjectID &hash_from, const ObjectID &hash_to);

	shared_ptr<ObjectsReputation> get(shared_ptr<IPortalDatabase> database, const ObjectID &hash_from, const ObjectID &hash_to);
	LanguageResult set(shared_ptr<IPortalDatabase> database, const Buffer &private_key, const ObjectID &hash_from, const ObjectID &hash_to, const double &score, const String &comment, bool follow);

	// TOCLEAN
	//bool ensure(shared_ptr<IPortalDatabase> database, const Buffer &private_key, const ObjectID &hash_from, const ObjectID &hash_to, double score, bool follow);

// Operations
public:
	ReputationsScore compute(shared_ptr<IPortalDatabase> database, const ObjectID &hash_from, const ObjectID &hash_to, const shared_ptr<ReputationsFeedback> &feedback = null);
	// TOCLEAN
	//void remove(shared_ptr<IPortalDatabase> database, const Buffer &private_key, const ObjectID &hash_from, const ObjectID &hash_to);

private:
	ReputationsScore compute(shared_ptr<IPortalDatabase> database, const ObjectID &hash_from, const ObjectID &hash_to, StepsList &steps, bool onlyDirect, const shared_ptr<ReputationsFeedback> &feedback);
	bool getReputations(shared_ptr<IPortalDatabase> database, const String &author, double thresholdListen, objects_reputations_list &reputations) const;

protected:
	ReputationsScore m_scoreAutoValutation;
	bool m_stopWithDirect;
	double m_directPercentage;
	double m_friendsOnlyPercentage;
	double m_thresholdListen;
	bool m_skipUnknownFeedbacks;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _REPUTATIONS_MANAGER_H
