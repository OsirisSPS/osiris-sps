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

#ifndef _IDE_MAIN_SUBSCRIBE_H
#define _IDE_MAIN_SUBSCRIBE_H

#include "iidemainpage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IDbDriver;
class IDbOptions;
class IHtmlInput;
class HtmlCheckBox;
class HtmlComboBox;
class HtmlTable;
class HtmlTableRow;
class HtmlTextBox;

//////////////////////////////////////////////////////////////////////

namespace ide {
namespace main {

//////////////////////////////////////////////////////////////////////

class EngineExport Subscribe : public IMainPage
{
	typedef IMainPage PageBase;

	typedef unordered_map<String, shared_ptr<HtmlTextBox> >::type DriverOptionsControls;

// Construction
public:
	Subscribe(shared_ptr<HttpSession> session);
	virtual ~Subscribe();

// Attributes
private:
	shared_ptr<HtmlTextBox> getDriverOptionControl(const String &driverName, const String &optionName) const;

// Operations
private:
	bool initControlValue(shared_ptr<IHtmlInput> control, const String &paramName);

// IPage interface
public:
	virtual String getPageName() const;
	
// IMainPage overrides
protected:
	virtual void onInit();
	virtual void onLoad();
	virtual void onPreRender();

// Events
private:
	void onCancel();
	void onSubscribe();	

private:
	shared_ptr<HtmlTextBox> m_portalLink;
	shared_ptr<HtmlCheckBox> m_showPortalParams;
	shared_ptr<HtmlTextBox> m_portalName;
	shared_ptr<HtmlTextBox> m_portalDescription;
	shared_ptr<HtmlTextBox> m_portalID;
	shared_ptr<HtmlTextBox> m_portalPov;	
	shared_ptr<HtmlTextBox> m_portalPassword;			
	shared_ptr<HtmlComboBox> m_databaseDriver;
	shared_ptr<HtmlTable> m_table;
	shared_ptr<HtmlTableRow> m_rowPortalLink;
	shared_ptr<HtmlTableRow> m_rowName;
	shared_ptr<HtmlTableRow> m_rowDescription;
	shared_ptr<HtmlTableRow> m_rowID;
	shared_ptr<HtmlTableRow> m_rowUser;
	shared_ptr<IDbDriver> m_driver;
	shared_ptr<IDbOptions> m_driverOptions;
	mutable DriverOptionsControls m_driverOptionsControls;
};

//////////////////////////////////////////////////////////////////////

} // main
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_MAIN_SUBSCRIBE_H
