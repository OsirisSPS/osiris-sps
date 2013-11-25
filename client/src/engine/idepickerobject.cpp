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
#include "idepickerobject.h"

#include "conversions.h"
#include "ideskin.h"
#include "htmlattributes.h"
#include "htmlcontrols.h"
#include "htmlimage.h"
#include "htmltext.h"
#include "htmltextbox.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdePickerObject::IdePickerObject() : m_input(OS_NEW HtmlTextBox())
{
	m_input->setID(_S("object"));	
}

IdePickerObject::~IdePickerObject()
{

}

void IdePickerObject::onLoad()
{
	m_input->setSize(40);
	m_input->setMaxLength(40);	
	getControls()->add(m_input);
}

ObjectID IdePickerObject::getValue() const
{	
	ObjectID id(m_input->getValue().to_ascii());
	return id;
}

void IdePickerObject::setValue(const ObjectID &object)
{
	m_input->setValue(object.toWide());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
