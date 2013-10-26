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

#ifndef _IDBSTATEMENT_H
#define _IDBSTATEMENT_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IDbParameter;

//////////////////////////////////////////////////////////////////////

class DatabaseExport IDbStatement : public Object
{
public:
	typedef deque<shared_ptr<IDbParameter> >::type Parameters;

// Construction
public:
	IDbStatement();
	virtual ~IDbStatement();

// Attributes
public:
	inline shared_ptr<IDbParameter> getParameter(uint32 index) const;
	inline const Parameters & getParameters() const;

// Operations
public:
	shared_ptr<IDbParameter> addParameter();
	void execute();

// Interface
public:
	virtual uint32 neededParameters() = 0;
	virtual void close() = 0;

protected:
	virtual shared_ptr<IDbParameter> createParameter(uint32 index) = 0;
	virtual void run() = 0;

protected:
	Parameters m_parameters;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IDbParameter> IDbStatement::getParameter(uint32 index) const { return m_parameters[index]; }
inline const IDbStatement::Parameters & IDbStatement::getParameters() const { return m_parameters; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDBSTATEMENT_H
