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

#ifndef _IDE_EDIT_IOBJECTEDITOR_H
#define _IDE_EDIT_IOBJECTEDITOR_H

#include "entitiesentity.h"
#include "iidexslrenderer.h"
#include "ideportalcontrol.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IdeOMLEditor;
class IdePositionEditor;
class HtmlCheckBox;

//////////////////////////////////////////////////////////////////////

namespace ide {
namespace edit {

//////////////////////////////////////////////////////////////////////

class EngineExport IObjectEditor : public IXSLRenderer<IPortalPageControl<IHtmlControl> >
{
	typedef IXSLRenderer<IPortalPageControl<IHtmlControl> > ControlBase;

// Enumerated types
public:
	enum RedirectionType
	{
		rtEdit			= 0,
		rtEditParent	= 1,
		rtView			= 2,
		rtViewParent	= 3,
	};

// Construction
public:
	IObjectEditor(PortalObjectType type, shared_ptr<EntitiesEntity> entity = null, shared_ptr<EntitiesEntity> parent = null);
	virtual ~IObjectEditor();

// Attributes
public:
	inline shared_ptr<EntitiesEntity> getEntity() const;
	inline shared_ptr<EntitiesEntity> getParent() const;
	inline shared_ptr<ObjectsIRevisionable> getCurrent() const;

	// Indica se  in corso la crezione di una revisione o di una primaria
	inline bool isRevision() const;

	inline RedirectionType getRedirectionType() const;
	inline void setRedirectionType(RedirectionType redirection);

	const std::wstring & getRedirectionParam(const std::wstring &name) const;
	inline void setRedirectionParam(const std::wstring &name, const std::wstring &value);

#ifdef OS_TODOCIP
	const EntityID & getEntityID() const;
	const EntityID & getParentID() const;
#else
	EntityID getEntityID() const;
	EntityID getParentID() const;
#endif

// Operations
public:
	void onCreateObject();

// Interface
protected:
	virtual shared_ptr<ObjectsIRevisionable> createObject() = 0;

// Overridables
protected:
	virtual String getRedirectLocation(shared_ptr<ObjectsIRevisionable> object) const;
	virtual void renderActions(shared_ptr<XMLNode> nodeActions);

// IControl overrides
protected:
	virtual void onInit();
	virtual void onPreRender();

protected:
	RedirectionType m_redirectionType;
	ordered_map<std::wstring, std::wstring> m_redirectionParams;
	PortalObjectType m_type;				// Type
	shared_ptr<EntitiesEntity> m_entity;			// Istanza di base
	shared_ptr<EntitiesEntity> m_parent;			// Istanza padre
	shared_ptr<IdePositionEditor> m_position;
	shared_ptr<HtmlCheckBox> m_enableComment;
	shared_ptr<IdeOMLEditor> m_comment;

private:
	static const String EVENT_ONCREATEOBJECT;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<EntitiesEntity> IObjectEditor::getEntity() const { return m_entity; }
inline shared_ptr<EntitiesEntity> IObjectEditor::getParent() const { return m_parent; }
inline shared_ptr<ObjectsIRevisionable> IObjectEditor::getCurrent() const { return m_entity != null ? m_entity->getCurrent() : null; }

inline bool IObjectEditor::isRevision() const { return m_entity != null; }

inline IObjectEditor::RedirectionType IObjectEditor::getRedirectionType() const { return m_redirectionType; }
inline void IObjectEditor::setRedirectionType(RedirectionType redirection) { m_redirectionType = redirection; }

inline void IObjectEditor::setRedirectionParam(const std::wstring &name, const std::wstring &value) { m_redirectionParams.set(name, value); }

//////////////////////////////////////////////////////////////////////

} // edit
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_EDIT_IOBJECTEDITOR_H
