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
#include "idepickerselect.h"

#include "boost/bind.hpp"
#include "ideskin.h"
#include "entitiesentity.h"
#include "entitiesentitychilds.h"
#include "eventssource.h"
#include "htmlliteral.h"
#include "htmlparser.h"
#include "htmlscript.h"
#include "idesystem.h"
#include "idesession.h"
#include "ideskin.h"
#include "locked.h"
#include "objectsiobject.h"
#include "objectsirevisionable.h"

#include "htmlevent.h"
#include "htmlcontrols.h"
#include "htmltext.h"
#include "htmltextbox.h"
#include "htmlxslcontrol.h"
#include "objectssystem.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

/*
const String IdeSkinPicker::EVENT_OPEN = _S("onOpen");
const String IdeObjectPicker::EVENT_SELECT = _S("onSelect");
*/

#define OS_PICKERSELECT_EMPTYKEY _S("123123")

//////////////////////////////////////////////////////////////////////

IdePickerSelect::IdePickerSelect() : m_input(OS_NEW HtmlTextBox())
{
	/*
	getEvents()->get(EVENT_OPEN)->connect(boost::bind(&IdeObjectPicker::onOpen, this, _1));
	getEvents()->get(EVENT_SELECT)->connect(boost::bind(&IdeObjectPicker::onSelect, this, _1));
	*/

	m_input->setID(_S("keys"));
	m_input->setHidden(true);	
}

IdePickerSelect::~IdePickerSelect()
{

}

String IdePickerSelect::getValue() const
{
	String val = m_input->getValue(); 
	if(val == OS_PICKERSELECT_EMPTYKEY)
		return String::EMPTY;
	else
		return val;
}

void IdePickerSelect::setValue(const String &val)
{
	if(val == String::EMPTY)
		m_input->setValue(OS_PICKERSELECT_EMPTYKEY);
	else
		m_input->setValue(val);
}

String IdePickerSelect::getKey(const String &key)
{
	if(key == String::EMPTY)
		return OS_PICKERSELECT_EMPTYKEY;
	else
		return key;
}

String IdePickerSelect::getKeyHref(const String &key)
{
	return _S("javascript:Osiris.pickerSelect('") + getTarget() + _S("','") + getKey(key) + _S("','") + m_input->getTarget() + _S("');");
}

void IdePickerSelect::onInit()
{
	ControlBase::onInit();

	getControls()->add(m_input);		

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	m_nodeRoot = document->create(_S("root"));
	
	String path = getSkin()->getTemplatePath(getTemplateName() + _S(".xsl"));
	shared_ptr<HtmlXSLControl> templ(OS_NEW HtmlXSLControl(getPage()->loadStylesheet(path), document));
	getControls()->add(templ);	
}

void IdePickerSelect::onPreRender()
{
	ControlBase::onPreRender();

	getControls()->add(shared_ptr<IHtmlControl>(OS_NEW HtmlScript(_S("Osiris.pickerSelect('") + getTarget() + _S("','") + m_input->getValue() + _S("','") + m_input->getTarget() + _S("');"))));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
