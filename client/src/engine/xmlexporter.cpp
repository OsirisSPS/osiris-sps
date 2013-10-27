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
#include "xmlportalexporter.h"

#include "conversions.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

XMLExporter::XMLExporter(shared_ptr<XMLNode> root) : m_root(root)
{
	OS_ASSERT(root != null);	
}

XMLExporter::XMLExporter(shared_ptr<XMLNode> root, const XMLExporter &second) : m_root(root)
{
	OS_ASSERT(root != null);	
}

XMLExporter::~XMLExporter()
{

}

shared_ptr<XMLNode> XMLExporter::getNode(const String &name, bool ensure) const
{
	shared_ptr<XMLNode> node = m_root->getNode(name);
	if(node == null && ensure)
		return createNode(name);

	return node;
}

shared_ptr<XMLNode> XMLExporter::createAction(const String &name, const String &href, const String &group, bool requireConfirm)
{
	OS_ASSERT(name.empty() == false);
	OS_ASSERT(href.empty() == false);

	shared_ptr<XMLNode> nodeAction = getNodeActions()->addChild(OS_IDE_XML_ACTION);
	nodeAction->setAttributeString(OS_IDE_XML_ACTION_NAME, name);
	nodeAction->setAttributeString(OS_IDE_XML_ACTION_HREF, href);
	if(group.empty() == false)
		nodeAction->setAttributeString(OS_IDE_XML_ACTION_GROUP, group);		

	if(requireConfirm)
		nodeAction->setAttributeBool(OS_IDE_XML_ACTION_CONFIRM, true);

    return nodeAction;
}

shared_ptr<XMLNode> XMLExporter::createLink(const String &name, const String &href, const String &group)
{
	OS_ASSERT(name.empty() == false);
	OS_ASSERT(href.empty() == false);

	shared_ptr<XMLNode> nodeLink = getNodeLinks()->addChild(OS_IDE_XML_LINK);
	nodeLink->setAttributeString(OS_IDE_XML_LINK_NAME, name);
	nodeLink->setAttributeString(OS_IDE_XML_LINK_HREF, href);
	if(group.empty() == false)
		nodeLink->setAttributeString(OS_IDE_XML_ACTION_GROUP, group);		

    return nodeLink;
}

shared_ptr<XMLNode> XMLExporter::createNode(const String &name) const
{
	return m_root->addChild(name);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
