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

#ifndef _OS_ENGINE_IDEPICKERDATETIMESPECIAL_H
#define _OS_ENGINE_IDEPICKERDATETIMESPECIAL_H

#include "idecontrol.h"
#include "searchsearch.h"
#include "ihtmlcontrol.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlComboBox;
class IdePickerDateTime;

//////////////////////////////////////////////////////////////////////

class EngineExport IdePickerDateTimeSpecial : public IPageControl<IHtmlControl>
{
	typedef IPageControl<IHtmlControl> ControlBase;
	
// Construction
public:
	IdePickerDateTimeSpecial();
	virtual ~IdePickerDateTimeSpecial();

// Attributes
public:
	virtual void setValue(const SearchDateTime &type, const DateTime &date);

	virtual SearchDateTime getValueType() const;
	virtual void setValueType(const SearchDateTime &type);

	virtual DateTime getValueSpecific() const;
	virtual void setValueSpecific(const DateTime &date);

// IControl interface
public:
	virtual void onLoad();
	virtual void onPreRender();

protected:
	shared_ptr<HtmlComboBox> m_type;
	shared_ptr<IdePickerDateTime> m_specific;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_IDEPICKERDATETIMESPECIAL_H
