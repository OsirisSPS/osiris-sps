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
#include "ideide.h"

#include "omlmanager.h"
#include "ideomleditor.h"
#include "idesystem.h"
#include "htmlparser.h"

/////////////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {

//////////////////////////////////////////////////////////////////////

String formatString(const String &str, uint32 flags)
{
	if(flags == ffNone)
		return str;

	String formatted = str;

	if(flags & ffSingleLine)
	{
		formatted.remove(_S("\r"));
		formatted.remove(_S("\n"));
	}

	if(flags & ffTrim)
	{
		formatted.trim();
	}

	// Effettuare questo controllo sempre per ultimo
	if(flags & ffNoBlank)
	{
		if(formatted.empty())
            formatted = OS_EMPTY_NAME;
	}

	return formatted;
}

String encodeString(const String &str, uint32 flags)
{
	String encoded = str;
	HtmlParser::instance()->encode(encoded);
	return formatString(encoded, flags);
}

String formatObjectTitle(const String &title)
{
	return encodeString(title, ffSingleLine | ffTrim | ffNoBlank);
}

//////////////////////////////////////////////////////////////////////

} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
