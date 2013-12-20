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
#include "htmlxslcontrol.h"

#include "boost/bind.hpp"
#include "constants.h"
#include "htmlwriter.h"
#include "htmlcontrols.h"
#include "timerperformance.h"
#include "xmldocument.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlXSLControl::HtmlXSLControl(shared_ptr<XMLStylesheet> stylesheet, const shared_ptr<XMLDocument> document) : m_stylesheet(stylesheet),
																											   m_document(document)
{
	
}

HtmlXSLControl::~HtmlXSLControl()
{

}

bool HtmlXSLControl::hasChildParam(shared_ptr<IHtmlControl> child) const
{
	return getChildParam(child).empty() == false;
}

const String & HtmlXSLControl::getChildParam(shared_ptr<IHtmlControl> child) const
{
	ChildsParams::const_iterator i = m_childsParams.find(child);
	return i != m_childsParams.end() ? i->second : String::EMPTY;
}

void HtmlXSLControl::setChildParam(shared_ptr<IHtmlControl> child, const String &param)
{
    OS_EXCEPT_IF(child == nullptr || getControls()->exists(child) == false, "Invalid child");
	OS_EXCEPT_IF(hasChildParam(child), "Child already assigned to a param");

	// Associa il controllo ad un parametro
	m_childsParams[child] = param;
}

void HtmlXSLControl::addChildParam(shared_ptr<IHtmlControl> child)
{
	if(child == nullptr || child->hasID() == false)
	{
		OS_ASSERTFALSE();
		return;
	}

	addChildParam(child, child->getID());
}

void HtmlXSLControl::addChildParam(shared_ptr<IHtmlControl> child, const String &param)
{
	getControls()->add(child);
	setChildParam(child, param);
}

void HtmlXSLControl::onRender(HtmlWriter &writer)
{
	OS_TIMER_PERFORMANCE(TP, _S("HtmlXSLControl::onRender"));

	if(m_stylesheet == nullptr)
	{
	//	OS_ASSERTFALSE();
		return;
	}

	if(m_document == nullptr || m_document->getRoot() == nullptr)
	{
	//	OS_ASSERTFALSE();
		return;
	}

	HtmlControls::Controls basic_controls;
	HtmlControls::const_iterator i;
	// Scorre tutti i figli del controllo
	for(i = m_controls->begin(); i != m_controls->end(); ++i)
	{
		shared_ptr<IHtmlControl> child = *i;
		// Ottiene la variabile da rimpiazzare per il figlio corrente
		const String &childParam = getChildParam(child);
		// Controlla se non è stata specificata alcuna variabile
		if(childParam.empty())
		{
			// Salva il controllo corrente per una successiva render
			basic_controls.push_back(child);
		}
		else
		{
			HtmlWriter tmp;
			// Effettua il render del figlio in una variabile temporanea
			child->render(getPage(), tmp);
			// Crea nello stylesheet un parametro con la variabile di riferimento al controllo
			m_stylesheet->setStringParam(childParam, tmp.getHtml());
		}
	}

	String html;
	{
		OS_TIMER_PERFORMANCE(TP, _S("HtmlXSLControl::onRender::applyToString"));
		// Applica il template al documento di riferimento
		if(m_stylesheet->applyToString(m_document, html) == false)
			return;
	}

	// Renderizza il controllo
	writer.write(html);
	// Renderizza i figli che non erano associati ad alcuna variabile
	for(i = basic_controls.begin(); i != basic_controls.end(); (*i)->render(getPage(), writer), ++i);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
