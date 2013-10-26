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
#include "reputationsscore.h"

#include "osiriscommon.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const ReputationsScore ReputationsScore::EMPTY;
const float ReputationsScore::DEFAULT = 0;
const float ReputationsScore::GOOD = OS_REPUTATION_MAX;
const float ReputationsScore::BAD = OS_REPUTATION_MIN;

//////////////////////////////////////////////////////////////////////

ReputationsScore::ReputationsScore()
{
	reset();
}

ReputationsScore::ReputationsScore(const ReputationsScore &second)
{
	*this = second;
}

ReputationsScore::ReputationsScore(const double &value, const String &description, bool follow)
{
	reset();

	setValue(value);
	setDescription(description);
	setFollow(follow);
}

ReputationsScore::~ReputationsScore()
{

}

double ReputationsScore::getSafeValue() const
{
	return utils::limit <double>(m_value, BAD, GOOD);
}

double ReputationsScore::getRoundedValue() const
{
	// '-1' -> '-1'
	// '+1' -> '+1'
	// '+0.001' -> '+0.1'
	// '-0.001' -> '-0.1'
	// '+0.78' -> '+0.8'
	// '+0.74' -> '+0.7'
	// '+0.98' -> '+0.9'

	if(m_value<=BAD) return BAD;
	if(m_value>=GOOD) return GOOD;
	if(m_value == 0) return 0;

	double v = floor((abs(m_value)+0.05)*10) / 10;
	if(v>=0.9) v = 0.9;
	if(v<=0.1) v = 0.1;
	return (m_value<0) ? -1*v : v;
}

String ReputationsScore::getStringValue() const
{
	return String::format(_S("%1.1f").c_str(), getRoundedValue());
}

void ReputationsScore::reset()
{
	m_type = vtEmpty;
	m_value = DEFAULT;
	m_description.clear();
	m_follow = false;
	m_direct = false;
}

ReputationsScore::operator double () const
{
	return getValue();
}

ReputationsScore & ReputationsScore::operator =(const ReputationsScore &second)
{
	m_type = second.m_type;
	m_value = second.m_value;
	m_description = second.m_description;
	m_follow = second.m_follow;
	m_direct = second.m_direct;

	return *this;
}

bool ReputationsScore::operator ==(const ReputationsScore &second) const
{
	if(m_type != second.m_type)
		return false;

	if(m_value != second.m_value)
		return false;

	if(m_description != second.m_description)
		return false;

	if(m_follow != second.m_follow)
		return false;

	return true;
}

bool ReputationsScore::operator !=(const ReputationsScore &second) const
{
	return (*this == second) == false;
}

ReputationsScore & ReputationsScore::operator +=(const double &second)
{
	setValue(getValue() + second);
	return *this;
}

ReputationsScore & ReputationsScore::operator -=(const double &second)
{
	setValue(getValue() - second);
	return *this;
}

ReputationsScore & ReputationsScore::operator /=(const double &second)
{
	setValue(getValue() / second);
	return *this;
}

ReputationsScore & ReputationsScore::operator *=(const double &second)
{
	setValue(getValue() * second);
	return *this;
}

String ReputationsScore::toString(const double &score)
{
	return String::format(_S("%1.1f").c_str(), score);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
