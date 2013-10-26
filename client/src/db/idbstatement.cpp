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
#include "idbstatement.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IDbStatement::IDbStatement()
{

}

IDbStatement::~IDbStatement()
{

}

shared_ptr<IDbParameter> IDbStatement::addParameter()
{
	uint32 index = static_cast<uint32>(m_parameters.size());
	OS_EXCEPT_IF(index >= neededParameters(), "Invalid statement parameters");

	shared_ptr<IDbParameter> parameter = createParameter(index);
	if(parameter != null)
		m_parameters.push_back(parameter);

	return parameter;
}

void IDbStatement::execute()
{
	// Verifica che il numero di parametri passato corrisponda a quello richiesto
	OS_EXCEPT_IF(static_cast<uint32>(m_parameters.size()) != neededParameters(), "Invalid statement parameters count");
	// Esegue lo statement
	run();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
