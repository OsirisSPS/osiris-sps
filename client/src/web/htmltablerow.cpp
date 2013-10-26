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
#include "htmltablerow.h"

#include "htmlwriter.h"
#include "htmlcontrols.h"
#include "htmltablecell.h"
#include "htmltableheader.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlTableRow::HtmlTableRow()
{

}

HtmlTableRow::~HtmlTableRow()
{

}

shared_ptr<HtmlTableCell> HtmlTableRow::addCell()
{
	shared_ptr<HtmlTableCell> cell(OS_NEW HtmlTableCell());
	getControls()->add(cell);
	return cell;
}

shared_ptr<HtmlTableHeader> HtmlTableRow::addHeader(const String &caption)
{
	shared_ptr<HtmlTableHeader> header(OS_NEW HtmlTableHeader(caption));
	getControls()->add(header);
	return header;
}

void HtmlTableRow::onRender(HtmlWriter &writer)
{
	writer.openTag(_S("tr"));
	renderAttributes(writer);
	writer.closeTag();
	writer.writeLine();
	renderChilds(writer);
	writer.closeTag(_S("tr"));
	writer.writeLine();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
