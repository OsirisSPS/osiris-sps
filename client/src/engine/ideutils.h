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

#ifndef _IDE_UTILS_H
#define _IDE_UTILS_H

#include "collections.h"
#include "constants.h"
#include "ideide.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlTable;
class IHtmlControl;
class IPage;
class XMLNode;

//////////////////////////////////////////////////////////////////////

namespace ide {

//////////////////////////////////////////////////////////////////////

class EngineExport Utils
{
public:
	static void addInputRow(shared_ptr<HtmlTable> table, shared_ptr<IPage> page, shared_ptr<IHtmlControl> ctrl, const String &key);

	static shared_ptr<XMLNode> addAction(shared_ptr<XMLNode> nodeActions, shared_ptr<IPage> page, const String &suffix, const String &actionName, const String &actionHref, bool selected, bool confirm);
	//static shared_ptr<XMLNode> addAction(shared_ptr<XMLNode> nodeActions, shared_ptr<IPage> page, const String &suffix, const String &actionName, const String &href, shared_ptr<XMLNode> actionContent, bool confirm = false);	
	static String generateConfirmHref(shared_ptr<IPage> page, const String &href, const String &confirm);
};

//////////////////////////////////////////////////////////////////////

} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_UTILS_H
