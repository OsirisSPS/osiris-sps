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
#include "htmlfilebrowser.h"

#include "constants.h"
#include "filesystem.h"
#include "htmlpage.h"
#include "htmlwriter.h"
#include "httppostparam.h"
#include "httprequest.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlFileBrowser::HtmlFileBrowser()
{

}

HtmlFileBrowser::~HtmlFileBrowser()
{

}

String HtmlFileBrowser::getFileName() const
{
	shared_ptr<HttpPostParam> param = _getPostParam();
	if(param != null)
	{
		// N.B.: alcuni browser restituiscono il path completo mentre altri solo il nome del file
		return FileSystem::instance()->getFileName(param->getFilename());
	}

	return String::EMPTY;
}

String HtmlFileBrowser::getFileType() const
{
	shared_ptr<HttpPostParam> param = _getPostParam();
	if(param != null)
		return param->getContentType();

	return String::EMPTY;
}

const Buffer * HtmlFileBrowser::getFileBuffer() const
{
	shared_ptr<HttpPostParam> param = _getPostParam();
	if(param != null)
		return param->getBuffer();

	return null;
}

shared_ptr<HttpPostParam> HtmlFileBrowser::_getPostParam() const
{
	if(getPostBack() && hasID())
        return getRequest()->getPostParam(getTarget().to_wide());

	return null;
}

void HtmlFileBrowser::renderAttributes(HtmlWriter &writer)
{
	ControlBase::renderAttributes(writer);

	writer.writeAttribute(_S("type"), _S("file"));
}

void HtmlFileBrowser::onLoad()
{
	ControlBase::onLoad();

	getPage()->setEncoding(httpEncodingMultipartData);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
