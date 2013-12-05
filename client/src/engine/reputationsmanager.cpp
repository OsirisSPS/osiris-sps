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
#include "reputationsmanager.h"

#include "objectsreputation.h"
#include "osiriscommon.h"
#include "iportaldatabase.h"
#include "reputationsfeedback.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ReputationsManager::ReputationsManager()
{
	m_stopWithDirect = false;											// Indica se fermarsi se incontra una diretta (true) o se chiedere cmq agli amici (false)
	m_scoreAutoValutation = ReputationsScore(ReputationsScore::GOOD, String::EMPTY, true);	// Indica come gli utenti si valutano loro stessi
	m_directPercentage = 60;											// In presenza di una reputazione diretta e anche da amici, quanto vale la mia diretta?
	m_friendsOnlyPercentage = 100;										// In presenza di reputazioni solo da amici (senza una mia diretta), quanto vale il loro parere?
	m_thresholdListen = 0.0;											// Agli utenti il cui score diretto  sotto questa soglia non chiedo un parere, ANCHE se hanno il follow a true.
	m_skipUnknownFeedbacks = true;										// Se a true, i punteggi a 'unknown' non vengono inseriti nel feedback
}

ReputationsManager::~ReputationsManager()
{

}

bool ReputationsManager::exists(shared_ptr<IPortalDatabase> database, const ObjectID &hash_from, const ObjectID &hash_to)
{
	if(hash_from == hash_to)
		return false;

	shared_ptr<ObjectsReputation> reputation = database->getReputation(IPortalDatabase::getUserReputationID(hash_from, hash_to));
	if(reputation == null)
		return false;

	return reputation->score != ReputationsScore::DEFAULT;
}

shared_ptr<ObjectsReputation> ReputationsManager::get(shared_ptr<IPortalDatabase> database, const ObjectID &hash_from, const ObjectID &hash_to)
{
	if(hash_from == hash_to)
		return null;

	shared_ptr<ObjectsReputation> reputation = database->getReputation(IPortalDatabase::getUserReputationID(hash_from, hash_to));
	if(reputation == null)
		return null;

	if(reputation->score == ReputationsScore::DEFAULT)
		return null;

	return reputation;
}

LanguageResult ReputationsManager::set(shared_ptr<IPortalDatabase> database, const Buffer &private_key, const ObjectID &hash_from, const ObjectID &hash_to, const double &score, const String &comment, bool follow)
{
	/*
	if(hash_from == hash_to)
		return false;

	ObjectID reputation_id = IPortalDatabase::getUserReputationID(hash_from, hash_to);

	shared_ptr<ObjectsReputation> reputation = database->getReputation(reputation_id);
	if(reputation != null)
		return database->setReputationValue(reputation, score, comment, follow, private_key);

	if(score == ReputationsScore::DEFAULT)
		return true;

	return database->__createReputation(hash_to, score, comment, follow, hash_from.toUTF16(), private_key, reputation_id);
	*/

	if(hash_from == hash_to)
		return LanguageResult("same_user");
		
	shared_ptr<ObjectsReputation> reputation(OS_NEW ObjectsReputation());
	//automatica reputation->id = IPortalDatabase::getUserReputationID(hash_from, hash_to);
	reputation->author = hash_from;
	reputation->reference = hash_to;
	reputation->score = utils::limit <double>(score, OS_REPUTATION_MIN, OS_REPUTATION_MAX);
	reputation->description = comment;
	reputation->follow = follow;

	LanguageResult result = reputation->create(database, private_key);
	return result;
}

/* TOCLEAN
bool ReputationsManager::ensure(shared_ptr<IPortalDatabase> database, const Buffer &private_key, const ObjectID &hash_from, const ObjectID &hash_to, double score, bool follow)
{
	if(exists(database, hash_from, hash_to))
		return true;

	return set(database, private_key, hash_from, hash_to, score, String::EMPTY, follow);
}

void ReputationsManager::remove(shared_ptr<IPortalDatabase> database, const Buffer &private_key, const ObjectID &hash_from, const ObjectID &hash_to)
{
	set(database,private_key,hash_from, hash_to, ReputationsScore::DEFAULT, String::EMPTY, false);
}
*/

ReputationsScore ReputationsManager::compute(shared_ptr<IPortalDatabase> database, const ObjectID &hash_from, const ObjectID &hash_to, const shared_ptr<ReputationsFeedback> &feedback)
{
	StepsList steps;
	return compute(database,hash_from, hash_to, steps, false, feedback);

	/*
	// NOTA BENE: Non cachare la GetReputation sotto, dato che  ricorsiva, e la
	// cache non sarebbe affidabile in presenza di livelli di ricorsivit diversi.
	// v2: cachare e in caso di ricorsivit restituire il valore di default (ininfluente)
	// al posto di quello cachate Se  per richiesto il feedback, la cache non viene usata.

	Score S=null;
	if ( (Fb!=null) || (!m_Cache.Get(HashFrom,HashTo,ref S)) )
	{
		ArrayList Steps=new ArrayList();
		S=GetReputation(HashFrom,HashTo,Steps,false,Fb);
		m_Cache.Set(HashFrom,HashTo,S);
	}

	return S;
	*/
}

bool ReputationsManager::getReputations(shared_ptr<IPortalDatabase> database, const String &author, double thresholdListen, objects_reputations_list &reputations) const
{
	OS_ASSERT(database != null);

	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::REPUTATIONS_TABLE));
	select->where.add(DBTABLES::REPUTATIONS::AUTHOR, Convert::toSQL(author));
	select->where.add(DBTABLES::REPUTATIONS::FOLLOW, Convert::toSQL(true));
	select->where.add(DBTABLES::REPUTATIONS::SCORE, Convert::toSQL(thresholdListen), DbSqlCondition::cfMajor | DbSqlCondition::cfAnd);

	DataTable result;
	if(database->execute(select, result) == false)
		return false;

	for(uint32 i = 0; i < result.rows(); i++)
	{
		shared_ptr<ObjectsReputation> ptr(OS_NEW ObjectsReputation());
		if(ptr->read(result[i]))
            reputations.push_back(ptr);
	}

	return true;
}

ReputationsScore ReputationsManager::compute(shared_ptr<IPortalDatabase> database, const ObjectID &hash_from, const ObjectID &hash_to, StepsList &steps, bool onlyDirect, const shared_ptr<ReputationsFeedback> &feedback)
{
	ReputationsScore result;
	result.reset();
	result.setDescription(_S("")); // Ex "unknown"

	bool exit = false;

	if(onlyDirect == false)
	{
		for(StepsList::const_iterator i = steps.begin(); i != steps.end(); ++i)
		{
			if(hash_from == (*i))
			{
				result.reset();
				result.setDescription(_S("Recursive request"));
				exit = true;
				break;
			}
		}
	}

	steps.push_back(hash_from);

	if(exit == false)
	{
		if(hash_from == hash_to)
		{
			result = m_scoreAutoValutation;
			result.setFollow(true);
			result.setDescription(_S("Auto valutation"));
			exit = true;
		}
	}

	if( (exit == false) && (m_stopWithDirect || onlyDirect) )
	{
		shared_ptr<ObjectsReputation> repDirect = get(database,hash_from, hash_to);
		if( (repDirect != null) && (repDirect->score != ReputationsScore::DEFAULT) )
		{
			result = ReputationsScore(repDirect->score, repDirect->description, false);
			String desc = _S("Direct");
			if(!result.getDescription().empty()) desc += _S(" (") + result.getDescription() + _S(")");
			result.setDescription(desc);
			result.setDirect(true);
			exit = true;
		}
	}

	if(exit == false && onlyDirect == false)
	{
		// Computo la mia diretta
		shared_ptr<ReputationsFeedback> directFeedBack;
		if(feedback != null)
			directFeedBack.reset(OS_NEW ReputationsFeedback());

		result = compute(database,hash_from, hash_to, steps, true, directFeedBack);

		if(feedback != null)
		{
			if( (result.isValid()) || (!m_skipUnknownFeedbacks) )
				feedback->addChild(directFeedBack);
		}
	}

	if(exit == false && onlyDirect == false)
	{
		double friendsScore = 0;
		uint32 friendsAnswers = 0;

		objects_reputations_list reputations;
		if(getReputations(database, hash_from.toUTF16(), m_thresholdListen, reputations))
		{
			for(objects_reputations_list::iterator i = reputations.begin(); i != reputations.end(); ++i)
			{
				shared_ptr<ObjectsReputation> reputation = *i;
				shared_ptr<ReputationsFeedback> childFeedBack;

				if(feedback != null)
					childFeedBack.reset(OS_NEW ReputationsFeedback());

				const ObjectID &hash_sub = reputation->reference;

				ReputationsScore currentScore = compute(database,hash_sub, hash_to, steps, false, childFeedBack);

				if(currentScore.isValid())
				{
					currentScore *= reputation->score;
					friendsAnswers++;
					friendsScore += currentScore.getValue();

					if(currentScore.getFollow())
						result.setFollow(true);
				}

				if(feedback != null)
				{
					if( (currentScore.isValid()) || (!m_skipUnknownFeedbacks) )
						feedback->addChild(childFeedBack);
				}
			}
		}

		if(friendsScore != 0)
		{
			// Tra i pareri degli amici, faccio la media aritmetica
			friendsScore /= static_cast<double>(friendsAnswers);

			if(result.isValid())
			{
				double myScore = (result.getValue() * m_directPercentage) / 100;
				friendsScore = (friendsScore * (100-m_directPercentage) ) /100;

				result.setValue(myScore + friendsScore);
				result.setDescription(result.getDescription()+_S(", also from friends"));
			}
			else
			{
				// Vale solo il parere degli amici
				//result = (friendsScore * m_friendsOnlyPercentage) / 100;
				result.setValue((friendsScore * m_friendsOnlyPercentage) / 100);
				result.setDescription(_S("From friends"));
				exit = true;
			}
		}
	}

	if(feedback != null)
	{
		feedback->setLevel(static_cast<uint32>(steps.size()));
		feedback->setHashFrom(hash_from.toUTF16());
		feedback->setHashTo(hash_to.toUTF16());
		feedback->setScore(result);
	}

	steps.pop_back();

	return result;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
