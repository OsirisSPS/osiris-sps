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
#include "idboptions.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IDbOptions::IDbOptions()
{

}

IDbOptions::~IDbOptions()
{

}

const String & IDbOptions::getParam(const String &name) const
{
	// Non usare params.get per evitare di inserire nuovi parametri non registrati

	const ordered_map<String, String> &params = getParams();
	ordered_map<String, String>::const_iterator i = params.find(name);
	if(i != params.end())
		return i->second;

	return String::EMPTY;
}

void IDbOptions::setParam(const String &name, const String &value)
{
	getParams().set(name, value);
}

bool IDbOptions::hasParam(const String &name) const
{
	const ordered_map<String, String> &params = getParams();
	return params.find(name) != params.end();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
