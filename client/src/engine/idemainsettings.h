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

#ifndef _IDE_MAIN_SETTINGS_H
#define _IDE_MAIN_SETTINGS_H

#include "iidemainpage.h"

//////////////////////////////////////////////////////////////////////

#define OS_PAGE_SETTINGS_GROUP_ISIS			_S("isis")

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlTextBox;
class HtmlCheckBox;
class HtmlComboBox;
class IHtmlInput;
class IsisEndpoint;

//////////////////////////////////////////////////////////////////////

namespace ide {
namespace main {

//////////////////////////////////////////////////////////////////////

class EngineExport Settings : public IMainPage
{
	typedef IMainPage PageBase;

// Internal
private:
	enum MessageType
	{
		messageIsisAdded	= 0,
		messageIsisEdited	= 1,
		messageIsisRemoved	= 2,
	};

// Construction
public:
	Settings(shared_ptr<HttpSession> session);
	virtual ~Settings();

// Attributes
public:
	String getEditIsisEndpointLink(const shared_ptr<Portal> &portal, uint32 endpointID) const;

// Operations
private:
	void displayMessage();
	bool initIsisControlValue(shared_ptr<IHtmlInput> control, const String &paramName);

// IPage interface
public:
	virtual String getPageName() const;
	
// IMainPage overrides
protected:
	virtual void onLoad();
	
// Events:
private:
	void onSaveOptions(IEvent *e);
	void onAddIsisEndpoint(IEvent *e);
	void onEditIsisEndpoint(IEvent *e);
	void onRemoveIsisEndpoint(IEvent *e);	

private:
	static const String EVENT_ONSAVEOPTIONS;	
	static const String EVENT_ONADDISISENDPOINT;	
	static const String EVENT_ONEDITISISENDPOINT;		
	static const String EVENT_ONREMOVEISISENDPOINT;	
	static const std::wstring PARAM_MESSAGE;	
	static const std::wstring PARAM_ISISENDPOINT;	
	static const std::wstring PARAM_ISIS;	

protected:
	shared_ptr<HtmlTextBox> m_name;
	shared_ptr<HtmlCheckBox> m_exchangeEnabled;
	shared_ptr<HtmlCheckBox> m_publicDefault;
	shared_ptr<HtmlCheckBox> m_publicEnabled;
	shared_ptr<HtmlCheckBox> m_loginEnabled;
	shared_ptr<HtmlCheckBox> m_registerEnabled;	
	shared_ptr<HtmlTextBox> m_isisName;	
	shared_ptr<HtmlTextBox> m_isisUrl;	
	shared_ptr<HtmlCheckBox> m_isisEnabled;	
	shared_ptr<HtmlTextBox> m_isisPassword;	
	shared_ptr<IsisEndpoint> m_editedIsisEndpoint;
	shared_ptr<HtmlComboBox> m_sync;
};

//////////////////////////////////////////////////////////////////////

} // main
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_MAIN_SETTINGS_H
