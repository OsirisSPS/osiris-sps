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
#include "idepositioneditor.h"

#include "convert.h"
#include "conversions.h"
#include "entitiesentity.h"
#include "entitiesentitychilds.h"
#include "objectsirevisionable.h"
#include "portalsportal.h"
#include "htmlcontrols.h"
#include "htmlcombobox.h"
#include "htmloption.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdePositionEditor::IdePositionEditor() : m_positions(OS_NEW HtmlComboBox())
{
	m_positions->setID(_S("positions"));
}

IdePositionEditor::~IdePositionEditor()
{

}

double IdePositionEditor::getPosition() const
{
	return conversions::from_utf16<double>(m_positions->getValue());
}

void IdePositionEditor::setPosition(shared_ptr<EntitiesEntity> parent, const double position)
{
	m_parent = parent;
	m_currentPosition = position;
}

void IdePositionEditor::onInit()
{
	ControlBase::onInit();

	getControls()->add(m_positions);

	if(getFull())
	{
		shared_ptr<EntitiesEntities> childs = m_parent->getChilds(getDatabase(), portalObjectTypeUnknown, RangeUint32(0, 0), EntitiesEntity::coPositionAsc);

		double prevPosition = 0;
		shared_ptr<EntitiesEntity> prevEntity = nullptr;
		for(EntitiesEntities::iterator i = childs->begin(); i != childs->end(); ++i)
		{
			shared_ptr<EntitiesEntity> childEntity = getPortal()->getEntity(getDatabase(), *i);
			OS_ASSERT(childEntity != nullptr);

			shared_ptr<ObjectsIRevisionable> current = childEntity->getCurrent();
			String title = getPage()->parseOml(current->getTitle(), false, false, true);

			String text;
			if(prevEntity == nullptr)
			{
				text = getPage()->getText(_S("positionEditor_before")) + _S(" '") + title + _S("'");
			}
			else
			{
				String prevTitle = getPage()->parseOml(prevEntity->getCurrent()->getTitle(), false, false, true);
				text = getPage()->getText(_S("positionEditor_between")) + _S(" '") + prevTitle + _S("' ") + getPage()->getText(_S("positionEditor_between_and")) + _S(" '") + title + _S("'");
			}
			double newPosition = (current->position + prevPosition)/2;

			m_positions->addOption(text, conversions::to_utf16(newPosition));

			prevPosition = current->position;
			prevEntity = childEntity;
		}
	}

	m_positions->addOption(getPage()->getText(_S("positionEditor_bottom")), conversions::to_utf16(double(0)));

	if(m_currentPosition!=0)
		m_positions->addOption(getPage()->getText(_S("positionEditor_current")), conversions::to_utf16(m_currentPosition));

	m_positions->setValue(conversions::to_utf16(m_currentPosition));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
