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

#ifndef _IDE_BBCODES_URL_H
#define _IDE_BBCODES_URL_H

#include "iomlcode.h"

#include "ideide.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport OMLUrl : public IOMLCode
{
// Construction
public:
	OMLUrl(const String& tag);
	virtual ~OMLUrl();

// Operations
public:
	void generateBodyFromUrl(shared_ptr<OMLContext> context, const String& url, String& preBody, String& body, String& postBody) const;

private:

	String specialUrlDecode(const String &url) const;

// ICode implementation
public:
	virtual bool allowRowMode() const;
	virtual String processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_BBCODES_URL_H
