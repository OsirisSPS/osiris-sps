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
#include "idepathway.h"

#include "boost/bind.hpp"
#include "convert.h"
#include "entitiesentity.h"
#include "idesession.h"
#include "iportaldatabase.h"
#include "objectsdescriptor.h"
#include "objectsirevisionable.h"
#include "objectssystem.h"
#include "portalsportal.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdePathway::Path::Path(const String &name, const String &href, const String &id, const String &type) : m_name(name),
																									m_href(href),
																									m_id(id.to_ascii())
{

}

IdePathway::Path::~Path()
{

}

void IdePathway::Path::exportXML(shared_ptr<XMLNode> node) const
{
	node->setAttributeString(_S("name"), m_name);
	node->setAttributeString(_S("href"), m_href);
	node->setAttributeString(_S("id"), m_id.toUTF16());
}

//////////////////////////////////////////////////////////////////////

IdePathway::IdePathway()
{

}

IdePathway::~IdePathway()
{

}

void IdePathway::add(path_ptr path)
{
	m_paths.push_back(path);
}

void IdePathway::add(const String &name, const String &href)
{
    add(name, href, ObjectsSystem::instance()->getRootID());
}

void IdePathway::add(const String &name, const String &href, const EntityID &id)
{
    add(path_ptr(OS_NEW Path(name, href, id.toUTF16())));
}

void IdePathway::add(shared_ptr<IPortalDatabase> database, shared_ptr<EntitiesEntity> entity, ObjectsRenderMode mode)
{
	// Se viene rilevata una situazione di ricorsività, cio di un oggetto il padre lui stesso o uno dei figli,  come se il padre non fosse trovato.

	Paths paths;
	while((entity != null) && (entity->getEntityID() != ObjectsSystem::instance()->getRootID()) )
	{
		shared_ptr<ObjectsIRevisionable> object = entity->getCurrent();
		if(object == null)
			break;

		String title = object->getTitle();
		// Verifica che l'oggetto abbia un titolo (la root ad esempio non ne ha uno)
		if(title.empty() == false)
		{
			String href;
			switch(mode)
			{
			case rmView:			href = entity->getViewLink(database->getPortal());
									break;

			case rmEdit:			href = entity->getEditLink(database->getPortal());
									break;

			default:				OS_ASSERTFALSE();
									break;
			}

			path_ptr path(OS_NEW Path(title, href, entity->getEntityID().toUTF16()));
			paths.push_front(path);
		}

		entity = entity->getParent(database);
	}

	for(Paths::const_iterator i = paths.begin(); i != paths.end(); ++i)
	{
		add(*i);
	}
}

void IdePathway::exportXML(shared_ptr<XMLNode> node) const
{
	for(Paths::const_iterator i = m_paths.begin(); i != m_paths.end(); ++i)
	{
		(*i)->exportXML(node->addChild(_S("path")));
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
