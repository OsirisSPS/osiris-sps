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
#include "ixmlfunction.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IXMLFunction::IContext::IContext(shared_ptr<IXMLFunction> function) : m_function(function),
																	  m_errors(0)
{

}

IXMLFunction::IContext::~IContext()
{

}

void IXMLFunction::IContext::reportError(const String &message)
{
	m_errors++;

	String str;
	if(m_function != null)
		str = String::format(_S("'%S': ").c_str(), m_function->getName().c_str());

	if(message.empty())
		str += _S("generic error");
	else
		str += message;

	onError(str);
}

//////////////////////////////////////////////////////////////////////

IXMLFunction::IXMLFunction(const String &name, const String &ns, uint32 required_args, uint32 optionals_args) : m_name(name),
																												m_namespace(ns),
																												m_required_args(required_args),
																												m_optionals_args(optionals_args)
{

}

IXMLFunction::~IXMLFunction()
{

}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
