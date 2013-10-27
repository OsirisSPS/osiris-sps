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

#ifndef _IDE_VIEW_IOBJECTVIEWER_H
#define _IDE_VIEW_IOBJECTVIEWER_H

#include "iidepageable.h"
#include "iidexslrenderer.h"
#include "ideportalcontrol.h"
#include "entitiesentity.h"


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IdeVoteEditor;
class XMLPortalExporter;

//////////////////////////////////////////////////////////////////////

namespace ide {
namespace view {

//////////////////////////////////////////////////////////////////////

class EngineExport IObjectViewer : public IXSLRenderer<IPortalPageControl<IHtmlControl> >,
								   public IIdePageable
{
	typedef IXSLRenderer<IPortalPageControl<IHtmlControl> > ControlBase;

// Construction
public:
	IObjectViewer(shared_ptr<EntitiesEntity> entity = null);
	virtual ~IObjectViewer();

// Attributes
public:
	// Restituisce l'entit di riferimento
	inline shared_ptr<EntitiesEntity> getEntity() const;	
#ifdef OS_TODOCIP
	const EntityID & getEntityID() const;
#else
	EntityID getEntityID() const;
#endif
	shared_ptr<ObjectsIRevisionable> getObject() const;		

	/*
	// Restituisce la primaria dell'oggetto di riferimento
	shared_ptr<ObjectsIRevisionable> getPrimaryObject() const;	
	// Restituisce la revisione corrente dell'oggetto di riferimento
	shared_ptr<ObjectsIRevisionable> getCurrentObject() const;		

	const ObjectID & getPrimaryID() const;
	*/

// Operations
public:
	virtual void renderVote(shared_ptr<XMLPortalExporter> exporter) const;	
	virtual void renderTags(shared_ptr<XMLPortalExporter> exporter) const;	

// IPageable interface
protected:
	virtual String getPageHref(uint32 offset) const;	

// IControl overrides
protected:
	virtual void onLoad();
	virtual void onPreRender();

// Events
protected:
	virtual void onUpdateVote();

protected:
	shared_ptr<EntitiesEntity> m_entity;
	shared_ptr<IdeVoteEditor> m_voteEditor;
	shared_ptr<ObjectsVote> m_vote;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<EntitiesEntity> IObjectViewer::getEntity() const { return m_entity; }
inline shared_ptr<ObjectsIRevisionable> IObjectViewer::getObject() const { return (m_entity != null) ? m_entity->getCurrent() : null; }

//////////////////////////////////////////////////////////////////////

} // view
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_VIEW_IOBJECTVIEWER_H
