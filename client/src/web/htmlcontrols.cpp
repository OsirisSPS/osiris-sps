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
#include "htmlcontrols.h"

#include "ihtmlcontrol.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlControls::HtmlControls(IHtmlControl &control) : m_control(control)
{

}

HtmlControls::~HtmlControls()
{

}

bool HtmlControls::exists(shared_ptr<IHtmlControl> control) const
{
	return utils::exists(m_controls, control);
}

void HtmlControls::add(shared_ptr<IHtmlControl> control)
{
	_insert(control, true);
}

void HtmlControls::remove(shared_ptr<IHtmlControl> control)
{
	iterator i = std::find(m_controls.begin(), m_controls.end(), control);
	if(i != m_controls.end())
	{
		control->m_parent.reset();
		m_controls.erase(i);
	}
}

void HtmlControls::remove(const String &id)
{
	shared_ptr<IHtmlControl> control = find(id);
	if(control != nullptr)
		remove(control);
}

void HtmlControls::push_back(shared_ptr<IHtmlControl> control)
{
	_insert(control, true);
}

void HtmlControls::push_front(shared_ptr<IHtmlControl> control)
{
	_insert(control, false);
}

void HtmlControls::_insert(shared_ptr<IHtmlControl> control, bool back)
{
	OS_EXCEPT_IF((control == nullptr) || (control->getParent() != nullptr), "Invalid control");
	
	OS_ASSERT(utils::exists(m_controls, control) == false);		// Se il controllo non è ancora stato agganciato è impossibile che figuri nella lista dei figli

	control->m_parent = getControl().get_this_ptr();
	control->m_page = getControl().getPage();

	if(back)
        m_controls.push_back(control);
	else
		m_controls.push_front(control);
}

shared_ptr<IHtmlControl> HtmlControls::find(const String &id) const
{
	for(const_iterator i = m_controls.begin(); i != m_controls.end(); ++i)
	{
		if((*i)->getID() == id)
			return *i;
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
