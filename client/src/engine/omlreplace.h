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

#ifndef _IDE_BBCODES_REPLACE_H
#define _IDE_BBCODES_REPLACE_H

#include "iomlcode.h"

#include "ideide.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport OMLReplace : public IOMLCode
{
// Construction
public:
	OMLReplace(const String &tag, int32 minParams, int32 maxParams, const String &replaceHtml, const String &replacePlain, const bool allowedInRowMode = false, const String &mapDefaultParamTo = String::EMPTY);
	virtual ~OMLReplace();

// ICode implementation
public:
	virtual bool allowRowMode() const;
	virtual String processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const;

// Internal:
private:
	int32 m_minParams;
	int32 m_maxParams;
	String m_replaceHtml;
	String m_replacePlain;
	bool m_allowedInRowMode;
	String m_mapDefaultParamTo;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_BBCODES_REPLACE_H
