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

#ifndef _IDE_MAIN_OPTIONS_H
#define _IDE_MAIN_OPTIONS_H

#include "iidemainpage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IdeBlock;
class DataItem;
class HtmlTextBox;
class HtmlCheckBox;
class IHtmlInput;
class IsisEndpoint;
class IdePickerCulture;
class IdePickerSkin;


//////////////////////////////////////////////////////////////////////

namespace ide {

//////////////////////////////////////////////////////////////////////

namespace main {

//////////////////////////////////////////////////////////////////////

class EngineExport Options : public IMainPage
{
	typedef IMainPage PageBase;

	typedef unordered_map<String, boost::function<void()> >::type Setters;

// Construction
public:
	Options(shared_ptr<HttpSession> session);
	virtual ~Options();

// Attributes
public:
	String getControlIDFromOptionName(String optionName) const;

// Operations
public:
	shared_ptr<IHtmlControl> createControl(const String &optionName, shared_ptr<IHtmlControl> optionControl);
	shared_ptr<IHtmlControl> createControl(const String &optionName, shared_ptr<IHtmlControl> optionControl, const boost::function<void()> &optionReader, const boost::function<void()> &optionWriter);
	shared_ptr<IHtmlControl> createControlBool(const String &optionName);
	shared_ptr<IHtmlControl> createControlEnum(const String &optionName, const StringCollection<String> &values);
	shared_ptr<IHtmlControl> createControlNumber(const String &optionName);
	shared_ptr<IHtmlControl> createControlString(const String &optionName);
	shared_ptr<IHtmlControl> createPage(shared_ptr<IHtmlControl> tab, const String &name, uint32 &pageIndex);
	shared_ptr<IHtmlControl> createPageGroup(shared_ptr<IHtmlControl> tabPage, const String &name);
	//shared_ptr<IHtmlControl> createSeparator();

// Events
private:
	void onSave();

// IPage interface
public:
	virtual String getPageName() const;

// IMainPage overrides
protected:
	virtual void onInit();
	virtual void onLoad();

private:
	Setters m_setters;
	shared_ptr<IdeBlock> m_block;
	shared_ptr<IdePickerSkin> m_skinPicker;
	shared_ptr<IdePickerCulture> m_culturePicker1;
	shared_ptr<IdePickerCulture> m_culturePicker2;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((ide)(main))

//////////////////////////////////////////////////////////////////////

#endif // _IDE_MAIN_OPTIONS_H
