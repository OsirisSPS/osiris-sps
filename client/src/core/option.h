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

#ifndef _OS_CORE_OPTION_H
#define _OS_CORE_OPTION_H

#include "base/object.h"
#include "dataitem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IOptionValidator;

//////////////////////////////////////////////////////////////////////

class CoreExport Option : public Object
{
    typedef Object BaseClass;

// Construction
public:
	Option(const String &name = String::EMPTY, const DataItem &defaultValue = DataItem::EMPTY);
	Option(const Option &second);
	virtual ~Option();

// Attributes
public:
	inline const String & getName() const;
	inline DataItem::ValueType getType() const;

	inline bool isDefault() const;
	inline bool isNull() const;

	const DataItem & getValue() const;
	bool setValue(const DataItem &value, bool allowDefault = true);

	const DataItem & getDefaultValue() const;
	void setDefaultValue(const DataItem &defaultValue);

	shared_ptr<IOptionValidator> getValidator() const;
	void setValidator(shared_ptr<IOptionValidator> validator);

	inline bool getPublic() const;
	inline void setPublic(bool value);

// Operations
public:
	const DataItem & fromString(const String &value);
	String toString() const;

	Option & toDefault();

private:
	void refreshValue();

// Operators
public:
	Option & operator =(const Option &second);

private:
	String m_name;
	DataItem m_defaultValue;
	DataItem m_value;
	shared_ptr<IOptionValidator> m_validator;
	bool m_public;
};

//////////////////////////////////////////////////////////////////////

inline const String & Option::getName() const { return m_name; }
inline DataItem::ValueType Option::getType() const { return m_defaultValue.getType(); }

inline bool Option::isDefault() const { return m_value == m_defaultValue; }
inline bool Option::isNull() const { return m_value.isNull(); }

inline bool Option::getPublic() const { return m_public; }
inline void Option::setPublic(bool value) { m_public = value; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_OPTION_H
