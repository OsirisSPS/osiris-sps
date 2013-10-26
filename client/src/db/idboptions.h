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

#ifndef _IDBOPTIONS_H
#define _IDBOPTIONS_H

#include "base/object.h"
#include "ordered_map.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DatabaseExport IDbOptions : public Object
{
// Construction
public:
	IDbOptions();
	virtual ~IDbOptions();

// Attributes
public:
	inline size_t size() const;

	// Restituisce il valore di un parametro
	const String & getParam(const String &name) const;
	// Setta un parametro
	void setParam(const String &name, const String &value);
	// Indica se il parametro specificato è stato settato
	bool hasParam(const String &name) const;

// Interface
public:
	virtual ordered_map<String, String> & getParams() const = 0;
};

//////////////////////////////////////////////////////////////////////

inline size_t IDbOptions::size() const { return getParams().size(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDBOPTIONS_H
