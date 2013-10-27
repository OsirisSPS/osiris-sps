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

#ifndef _OS_FIREFOX_OSIRISIMPL_H
#define _OS_FIREFOX_OSIRISIMPL_H

#include "boost/shared_ptr.hpp"
#include "iosiris.h"

//////////////////////////////////////////////////////////////////////

#define OS_FIREFOX_CONTRACTID 	"@osiris-sps.org/osiris;1"
#define OS_FIREFOX_CLASSNAME	"Osiris"
#define OS_FIREFOX_CID			{ 0XBF09DA54, 0XA667, 0X486B, { 0XBE, 0X59, 0XA9, 0X72, 0X39, 0X13, 0X58, 0XEA } }

//////////////////////////////////////////////////////////////////////

class IOsirisContext
{
// Construction
public:
	IOsirisContext();
	virtual ~IOsirisContext();

// Interface
public:
	virtual NS_IMETHODIMP parse(const nsAString &link, nsAString &retval) = 0;
	virtual NS_IMETHODIMP getHomepage(nsAString &nsString) = 0;	
	virtual NS_IMETHODIMP getStarted(PRBool *aStarted) = 0;
};

//////////////////////////////////////////////////////////////////////

class OsirisImpl : public IOsiris
{
public:
	NS_DECL_ISUPPORTS
	NS_DECL_IOSIRIS

//	Construction
public:
	OsirisImpl();
	virtual ~OsirisImpl();

private:
	boost::shared_ptr<IOsirisContext> m_context;
};

//////////////////////////////////////////////////////////////////////

#endif // _OS_FIREFOX_OSIRISIMPL_H
