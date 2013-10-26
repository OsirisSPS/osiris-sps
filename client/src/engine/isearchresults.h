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

#ifndef _OS_ENGINE_ISEARCHRESULTS_H
#define _OS_ENGINE_ISEARCHRESULTS_H

#include "base/object.h"
#include "dbdb.h"
#include "enable_this_ptr.h"
#include "ideide.h"
#include "ids.h"
#include "objectsobjects.h"
#include "portalsportals.h"
#include "searchsearch.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;
class IPortalPage;
class ObjectID;
class Portal;
class SearchQuery;
class SearchResult;
class XMLNode;

//////////////////////////////////////////////////////////////////////

class EngineExport ISearchResults : public Object,
									public enable_this_ptr<ISearchResults>,
									public boost::noncopyable
{
	typedef unordered_map<String, shared_ptr<XMLNode> >::type XMLNodesCache;

// Construction
public:
	ISearchResults();
	virtual ~ISearchResults();

// Attributes
public:
	inline bool getPartial() const;
	inline void setPartial(bool partial);
	inline double getElapsed() const;

// Operations
public:
	// Esporta una ricerca
	//virtual void exportXML(shared_ptr<XMLNode> node, search::query_ptr query, shared_ptr<IPortalPage> page, const String &title);
	virtual void exportXML(shared_ptr<XMLNode> node, shared_ptr<SearchQuery> query, shared_ptr<IPortalPage> page, bool external);
	// Effettua una ricerca
	virtual void search(shared_ptr<IPortalDatabase> database, shared_ptr<Portal> portal, shared_ptr<SearchQuery> query, const StringMap& objectsMap);

private:
	shared_ptr<XMLNode> _exportNode(shared_ptr<XMLNode> nodeRoot, shared_ptr<SearchQuery> query, shared_ptr<IPortalPage> page, const EntityID id, const double score, XMLNodesCache *nodeCache, bool isResult, bool external);

// Interface
public:
	// Resetta la ricerca
	virtual void clear() = 0;

	// Restituisce il risultato richiesto
	virtual shared_ptr<SearchResult> getResult(shared_ptr<IPortalDatabase> database, uint32 index, bool asGroup = false) = 0;
	// Restituisce il numero di risultati caricati
	virtual uint32 getResults() = 0;
	// Restituisce il numero totale di risultati
	virtual uint32 getTotalResults() = 0;

protected:
	virtual void executeQuery(shared_ptr<IPortalDatabase> database, shared_ptr<Portal> portal, shared_ptr<SearchQuery> query, const StringMap& objectsMap) = 0;

protected:
	bool m_partial;	// If all results are extracted (false), or a safe limit is reached (true);
	double m_elapsed;	// Elapsed time for search execution
};

//////////////////////////////////////////////////////////////////////

inline bool ISearchResults::getPartial() const { return m_partial; }
inline void ISearchResults::setPartial(bool partial) { m_partial = partial; }
inline double ISearchResults::getElapsed() const { return m_elapsed; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_ISEARCHRESULTS_H
