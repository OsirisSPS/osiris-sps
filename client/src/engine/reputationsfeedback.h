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

#ifndef _REPUTATIONS_FEEDBACK_H
#define _REPUTATIONS_FEEDBACK_H

#include "base/object.h"
#include "reputationsreputations.h"
#include "reputationsscore.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ReputationsFeedback : public Object
{
public:
	typedef list<shared_ptr<ReputationsFeedback> >::type ReputationsFeedbackList;
	typedef ReputationsFeedbackList::iterator iterator;
	typedef ReputationsFeedbackList::const_iterator const_iterator;

// Construction
public:
	ReputationsFeedback(const String &hashFrom = String::EMPTY, const String &hashTo = String::EMPTY, const ReputationsScore &score = ReputationsScore::EMPTY, const int32 &level = 0);
	virtual ~ReputationsFeedback();

// Attributes
public:
	inline const String & getHashFrom() const;
	inline void setHashFrom(const String &hash);

	inline const String & getHashTo() const;
	inline void setHashTo(const String &hash);

	inline const ReputationsScore & getScore() const;
	inline void setScore(const ReputationsScore &score);

	inline const int32 & getLevel() const;
	inline void setLevel(const int32 &level);

	inline iterator first_child();
	inline const_iterator first_child() const;

	inline iterator last_child();
	inline const_iterator last_child() const;

// Operations
public:
	void addChild(shared_ptr<ReputationsFeedback> feedback);

protected:
	String m_hashFrom;
	String m_hashTo;
	ReputationsScore m_score;
	int32 m_level;

	ReputationsFeedbackList m_childs;
};

//////////////////////////////////////////////////////////////////////

inline const String & ReputationsFeedback::getHashFrom() const { return m_hashFrom; }
inline void ReputationsFeedback::setHashFrom(const String &hash) { m_hashFrom = hash; }

inline const String & ReputationsFeedback::getHashTo() const { return m_hashTo; }
inline void ReputationsFeedback::setHashTo(const String &hash) { m_hashTo = hash; }

inline const ReputationsScore & ReputationsFeedback::getScore() const { return m_score; }
inline void ReputationsFeedback::setScore(const ReputationsScore &score) { m_score = score; }

inline const int32 & ReputationsFeedback::getLevel() const { return m_level; }
inline void ReputationsFeedback::setLevel(const int32 &level) { m_level = level; }

inline ReputationsFeedback::iterator ReputationsFeedback::first_child() { return m_childs.begin(); }
inline ReputationsFeedback::const_iterator ReputationsFeedback::first_child() const { return m_childs.begin(); }

inline ReputationsFeedback::iterator ReputationsFeedback::last_child() { return m_childs.end(); }
inline ReputationsFeedback::const_iterator ReputationsFeedback::last_child() const { return m_childs.end(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _REPUTATIONS_FEEDBACK_H
