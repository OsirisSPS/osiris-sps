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

#ifndef _REPUTATIONS_SCORE_H
#define _REPUTATIONS_SCORE_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ReputationsScore
{
// Enumerated types
public:
    enum ValueType
    {
        vtEmpty	= 0,
		vtValid	= 1,
    };

// Construction
public:
	ReputationsScore();
	ReputationsScore(const ReputationsScore &second);
	//Score(const double &value);
	ReputationsScore(const double &value, const String &description, bool follow);
	virtual ~ReputationsScore();

// Attributes
public:
	inline bool isEmpty() const;
	inline bool isValid() const;

// Operations
public:
	inline double getValue() const;
	inline void setValue(const double &value);

	inline const String & getDescription() const;
	inline void setDescription(const String &description);

	inline bool getFollow() const;
	inline void setFollow(bool follow);

	inline bool getDirect() const;
	inline void setDirect(bool direct);

	double getSafeValue() const;
	double getRoundedValue() const;
	String getStringValue() const;

	void reset();

//	Operators
public:
	ReputationsScore & operator =(const ReputationsScore &second);
	operator double () const;

	bool operator ==(const ReputationsScore &second) const;
	bool operator !=(const ReputationsScore &second) const;

	ReputationsScore & operator +=(const double &second);
	ReputationsScore & operator -=(const double &second);
	ReputationsScore & operator /=(const double &second);
	ReputationsScore & operator *=(const double &second);

//  Static
public:
	static String toString(const double &score);

protected:
	ValueType m_type;
	double m_value;
	String m_description;
	bool m_follow;
	bool m_direct;

public:
	static const ReputationsScore EMPTY;
	static const float DEFAULT;
	static const float GOOD;
	static const float BAD;
};

//////////////////////////////////////////////////////////////////////

inline bool ReputationsScore::isEmpty() const { return m_type == vtEmpty; }
inline bool ReputationsScore::isValid() const { return m_type == vtValid; }

inline double ReputationsScore::getValue() const { return isValid() ? m_value : DEFAULT; }
inline void ReputationsScore::setValue(const double &value) { m_type = vtValid; m_value = value; }

inline const String & ReputationsScore::getDescription() const { return m_description; }
inline void ReputationsScore::setDescription(const String &description) { m_description = description; }

inline bool ReputationsScore::getFollow() const { return m_follow; }
inline void ReputationsScore::setFollow(bool follow) { m_follow = follow; }

inline bool ReputationsScore::getDirect() const { return m_direct; }
inline void ReputationsScore::setDirect(bool direct) { m_direct = direct; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _REPUTATIONS_SCORE_H
