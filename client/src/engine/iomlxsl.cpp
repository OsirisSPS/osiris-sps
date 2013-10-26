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
#include "iomlxsl.h"

#include "omlcontext.h"
#include "omlitem.h"
#include "iidepage.h"
#include "ideskin.h"
#include "idesession.h"
#include "css.h"
#include "htmlxslcontrol.h"
#include "htmlwriter.h"
#include "xmlattribute.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

/*IXsl::IXsl() : m_document(OS_NEW XMLDocument())
{	
	m_root = getDocument()->create(_S("tag"));	
}*/
IOMLXsl::IOMLXsl(const String& tag) : IOMLCode(tag)
{
}

IOMLXsl::~IOMLXsl()
{
}

shared_ptr<XMLDocument> IOMLXsl::initDocument(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{
	shared_ptr<XMLDocument> doc(OS_NEW XMLDocument());
	doc->create(getTag());

	// Creo il nodo e riporto i parametri
	//shared_ptr<XMLNode> nodeParams = getRoot()->addChild(_S("params"));
	for(StringCollection<String>::iterator i3 = i->getParams().begin(); i3 != i->getParams().end(); ++i3)
	{
		doc->getRoot()->setAttributeString(i3->first, *i3->second);
	}

	return doc;
}

String IOMLXsl::processDocument(shared_ptr<XMLDocument> document, shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{
	String path = context->getPage()->getSkin()->getTemplatePath(_S("oml/") + getTag() + _S(".xsl"));

	// Qui sarebbe meglio chiamare la getPage()->loadStylesheet...
	shared_ptr<XMLStylesheet> stylesheet(OS_NEW XMLStylesheet());
	stylesheet->parseFile(path);
	
	String out;
	stylesheet->applyToString(document, out);
	return out;
}

String IOMLXsl::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{		
	return String::EMPTY;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
