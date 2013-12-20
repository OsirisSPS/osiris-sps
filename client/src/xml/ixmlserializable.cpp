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
#include "ixmlserializable.h"

#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IXMLSerializable::IXMLSerializable()
{

}

IXMLSerializable::~IXMLSerializable()
{

}

shared_ptr<XMLDocument> IXMLSerializable::exportDocument() const
{
	shared_ptr<XMLNode> rootNode = exportNode();
	if(rootNode == nullptr)
		return nullptr;

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	document->setRoot(rootNode);
	return document;
}

bool IXMLSerializable::importDocument(shared_ptr<XMLDocument> document)
{
	if(document == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	return importNode(document->getRoot());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
