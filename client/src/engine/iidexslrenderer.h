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

#ifndef _OS_ENGINE_IXSLRENDERER_H
#define _OS_ENGINE_IXSLRENDERER_H

#include "htmlcontrols.h"
#include "htmlxslcontrol.h"
#include "xmldocument.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
class IXSLRenderer : public T
{
	typedef T ControlBase;

// Construction
protected:
	IXSLRenderer() { _construct(); }
	template <typename P1>
	IXSLRenderer(const P1 &p1) : ControlBase(p1) { _construct(); }
	template <typename P1, typename P2>
	IXSLRenderer(const P1 &p1, const P2 &p2) : ControlBase(p1, p2) { _construct(); }
	template <typename P1, typename P2, typename P3>
	IXSLRenderer(const P1 &p1, const P2 &p2, const P3 &p3) : ControlBase(p1, p2, p3) { _construct(); }

public:
	virtual ~IXSLRenderer() { }

private:
	void _construct();

// Attributes
public:
	inline shared_ptr<XMLDocument> getDocument() const;
	inline shared_ptr<XMLStylesheet> getStylesheet() const;
	inline shared_ptr<HtmlXSLControl> getTemplate() const;

// Overridables
protected:
	virtual void onInitStylesheet();

// IControl overrides
protected:
	virtual void onInit();

// Interface
public:
	virtual String getTemplatePath() = 0;

protected:
	shared_ptr<XMLDocument> m_document;
	shared_ptr<XMLStylesheet> m_stylesheet;
	shared_ptr<HtmlXSLControl> m_template;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
inline shared_ptr<XMLDocument> IXSLRenderer<T>::getDocument() const { return m_document; }

template <typename T>
inline shared_ptr<XMLStylesheet> IXSLRenderer<T>::getStylesheet() const { return m_stylesheet; }

template <typename T>
inline shared_ptr<HtmlXSLControl> IXSLRenderer<T>::getTemplate() const { return m_template; }

//////////////////////////////////////////////////////////////////////

template <typename T>
void IXSLRenderer<T>::_construct()
{
	m_document.reset(OS_NEW XMLDocument());
	m_template.reset(OS_NEW HtmlXSLControl());

	ControlBase::getControls()->add(m_template); // 0.14 RC6
}

template <typename T>
void IXSLRenderer<T>::onInitStylesheet()
{

}

template <typename T>
void IXSLRenderer<T>::onInit()
{
	ControlBase::onInit();

	//ControlBase::getControls()->add(m_template); // 0.14 RC6

	// Carica lo stylesheet
	m_stylesheet = ControlBase::getPage()->loadStylesheet(getTemplatePath());
	if(m_stylesheet != nullptr)
	{
		onInitStylesheet();

		m_template->setStylesheet(m_stylesheet);
		m_template->setDocument(m_document);
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_IXSLRENDERER_H
