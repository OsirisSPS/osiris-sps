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

#ifndef _OS_HTML_HTMLXSLCONTROL_H
#define _OS_HTML_HTMLXSLCONTROL_H

#include "ihtmlcontrol.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLDocument;
class XMLStylesheet;

//////////////////////////////////////////////////////////////////////

class WebExport HtmlXSLControl : public IHtmlControl
{
	typedef IHtmlControl ControlBase;

	typedef map<shared_ptr<IHtmlControl>, String>::type ChildsParams;

// Construction
public:
	HtmlXSLControl(shared_ptr<XMLStylesheet> stylesheet = nullptr, shared_ptr<XMLDocument> document = nullptr);
	virtual ~HtmlXSLControl();

// Attributes
public:
	inline shared_ptr<XMLDocument> getDocument() const;
	inline void setDocument(shared_ptr<XMLDocument> document);

	inline shared_ptr<XMLStylesheet> getStylesheet() const;
	inline void setStylesheet(shared_ptr<XMLStylesheet> stylesheet);

	// Indica se un controllo figlio è associato ad una variabile
	bool hasChildParam(shared_ptr<IHtmlControl> child) const;
	// Restituisce il nome della variabile associata ad un controllo
	const String & getChildParam(shared_ptr<IHtmlControl> child) const;
	// Crea un'associazione tra un controllo è una variabile
	void setChildParam(shared_ptr<IHtmlControl> child, const String &param);

// Operations
public:
	void addChildParam(shared_ptr<IHtmlControl> child);
	void addChildParam(shared_ptr<IHtmlControl> child, const String &param);

// IHtmlControl overrides
public:
	virtual void onRender(HtmlWriter &writer);

protected:
	shared_ptr<XMLDocument> m_document;			// XML sorgente
	shared_ptr<XMLStylesheet> m_stylesheet;		// Template da applicare all'xml
	ChildsParams m_childsParams;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<XMLDocument> HtmlXSLControl::getDocument() const { return m_document; }
inline void HtmlXSLControl::setDocument(shared_ptr<XMLDocument> document) { m_document = document; }

inline shared_ptr<XMLStylesheet> HtmlXSLControl::getStylesheet() const { return m_stylesheet; }
inline void HtmlXSLControl::setStylesheet(shared_ptr<XMLStylesheet> stylesheet) { m_stylesheet = stylesheet; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLXSLCONTROL_H
