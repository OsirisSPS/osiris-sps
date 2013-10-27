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
#include "p2ppacketsqueryresponse.h"

#include "convert.h"
#include "dataentry.h"
#include "datatree.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {
namespace packets {

//////////////////////////////////////////////////////////////////////

const String QueryResponse::ENTRIES = _S("entries");

//////////////////////////////////////////////////////////////////////

QueryResponse::QueryResponse() : PacketBase(ptQueryResponse)
{
	// Inizializza la root delle entries
	getData()->setT(ENTRIES);
}

QueryResponse::~QueryResponse()
{

}

void QueryResponse::getEntries(DataEntry::DataEntryList &entries) const
{
	DataTree *root = getRoot();
	if(root != null)
	{
		// Scandisce l'elenco delle entries disponibili
		for(DataTree::iterator i = root->begin(); i != root->end(); ++i)
		{
			shared_ptr<DataEntry> entry = loadEntry(root, i->first.to_ascii());
			if(entry != null)
				entries.push_back(entry);
		}
	}
}

void QueryResponse::setEntries(const DataEntry::DataEntryList &entries)
{
	DataTree *root = getRoot();
	if(root != null)
	{
		// Resetta le entries attuali
		root->clear();
		// Scandisce l'elenco delle entries specificate
		for(DataEntry::DataEntryList::const_iterator i = entries.begin(); i != entries.end(); ++i)
		{
			// Salva l'entry corrente
			saveEntry(root, *i);
		}
	}
}

DataTree * QueryResponse::getRoot() const
{
	return getData()->getTP(ENTRIES);
}

shared_ptr<DataEntry> QueryResponse::loadEntry(DataTree *root, const ObjectID &id) const
{
	OS_ASSERT(root != null);
	DataItem *value = root->getVP(id.toUTF16());
	if(value == null)
		return null;

	shared_ptr<DataEntry> entry(OS_NEW DataEntry());
	entry->id = id;
	entry->submit_date = DateTime(static_cast<String>(*value));

	return entry;
}

bool QueryResponse::saveEntry(DataTree *root, const shared_ptr<DataEntry> &entry)
{
	OS_ASSERT(root != null);
	OS_ASSERT(entry->id->empty() == false);

	if(entry->id->empty() || root->exists(entry->id.get().toUTF16()))
		return false;

	root->setV(entry->id.get().toUTF16(), entry->submit_date->toString());
	return true;
}

bool QueryResponse::create()
{
	return true;
}

bool QueryResponse::parse()
{
	return getRoot() != null;
}

//////////////////////////////////////////////////////////////////////

} // packets
} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
