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
#include "p2ppacketsobjectsrequest.h"

#include "convert.h"
#include "datatree.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {
namespace packets {

//////////////////////////////////////////////////////////////////////

const String ObjectsRequest::OBJECTS = _S("objects");

//////////////////////////////////////////////////////////////////////

ObjectsRequest::ObjectsRequest(const StringList *objects) : PacketBase(ptObjectsRequest)
{
	// Crea la root degli oggetti
	getData()->setT(OBJECTS);
	// Controlla se sono stati specificati degli oggetti
	if(objects != null)
	{
		// Carica gli oggetti da richiedere
		for(StringList::const_iterator i = objects->begin(); i != objects->end(); request(*i), ++i);
	}
}

ObjectsRequest::~ObjectsRequest()
{

}

void ObjectsRequest::getObjects(StringList &objects)
{
	DataTree *root = getRoot();
	if(root != null)
	{
		// Scandisce tutti gli oggetti richiesti
		for(DataTree::iterator i = root->begin(); i != root->end(); ++i)
		{
			// Salva l'oggetto corrente
			objects.push_back(i->first);
		}
	}
}

DataTree * ObjectsRequest::getRoot() const
{
	return getData()->getTP(OBJECTS);
}

void ObjectsRequest::request(const String &id)
{
	// Verifica che l'id specificato sia valido
	if(id.empty() == false)
		// Salva l'id e la tipologia di oggetto
		getRoot()->setV(id, true);
}

bool ObjectsRequest::create()
{
	return true;
}

bool ObjectsRequest::parse()
{
	// Valida il pacchetto se  presente la root delle richieste
	return getRoot() != null;
}

//////////////////////////////////////////////////////////////////////

} // packets
} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
