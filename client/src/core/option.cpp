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
#include "option.h"

#include "ioptionvalidator.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

Option::Option(const String &name, const DataItem &defaultValue)
{
	m_public = false;
	m_name = name;
	m_value = defaultValue;
	m_defaultValue = defaultValue;

	// Clodo Note: "public" it's used only to know if an option maybe managed by user in the "options" page.
	// I think it's better to create another "about:config"-style page for manual editings of ALL options, remove this "public" flag, and show in "options" page only explicit user-options.
}

Option::Option(const Option &second) : BaseClass(second)
{
	*this = second;
}

Option::~Option()
{

}

const DataItem & Option::getValue() const
{
	return m_value;
}

bool Option::setValue(const DataItem &value, bool allowDefault)
{
	DataItem::ValueType type = getType();

	DataItem castedValue;
	if(type != DataItem::vt_null)
		// N.B.: effettua una copia mantenendo la tipologia di valore
		castedValue = value.cast(getType());
	else
		castedValue = value;

	// Se è stato specificato un validatore a livello di opzione verifica che il valore sia valido
	if(m_validator != null && m_validator->validate(castedValue) == false)
	{
		if(allowDefault)
			toDefault();

		return false;
	}

	m_value = castedValue;

	return true;
}

const DataItem & Option::getDefaultValue() const
{
	return m_defaultValue;
}

void Option::setDefaultValue(const DataItem &defaultValue)
{
	bool needRefresh = m_defaultValue.getType() != defaultValue.getType();

	m_defaultValue = defaultValue;

	if(needRefresh)
		refreshValue();
}

shared_ptr<IOptionValidator> Option::getValidator() const
{
	return m_validator;
}

void Option::setValidator(shared_ptr<IOptionValidator> validator)
{
	bool needRefresh = m_validator != validator;

	m_validator = validator;

	OS_ASSERT(m_validator == null || m_validator->validate(getDefaultValue()));

	if(needRefresh)
		refreshValue();
}

const DataItem & Option::fromString(const String &value)
{
	setValue(value);
	return m_value;
}

String Option::toString() const
{
	return m_value.get<String>();
}

Option & Option::toDefault()
{
	setValue(getDefaultValue());
	return *this;
}

void Option::refreshValue()
{
	setValue(getValue(), true);
}

Option & Option::operator =(const Option &second)
{
	m_name = second.m_name;
	m_defaultValue = second.m_defaultValue;
	m_value = second.m_value;
	m_validator = second.m_validator;

	return *this;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
