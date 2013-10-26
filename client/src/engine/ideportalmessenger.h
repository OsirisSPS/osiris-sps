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

#ifndef _ENGINE_IDEPORTALMESSENGER_H
#define _ENGINE_IDEPORTALMESSENGER_H

#include "iideportalpage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IdeOMLEditor;
class HtmlCheckBox;
class HtmlComboBox;
class HtmlFileBrowser;
class HtmlLiteral;
class HtmlTextBox;
class IdePickerCulture;
class IdePickerDateTime;
class IdePickerSkin;

//////////////////////////////////////////////////////////////////////

namespace ide {
namespace portal {

//////////////////////////////////////////////////////////////////////

class EngineExport Messenger : public IPortalPage
{
	typedef IPortalPage PageBase;

public:
	enum Action
	{
		acUnknown			= 0,
		acEditMessage		= 1,
		acViewInbox			= 2,
		acViewSentItems		= 3,
		acViewTrash			= 4,
		acViewMessage		= 5,		
		acDefault			= acViewInbox,
	};

// Construction
public:
	Messenger(shared_ptr<HttpSession> session);
	virtual ~Messenger();

// Attributes
public:
	Action getAction() const;
	static String getAction(Action action);
	String getUrl(Action action) const;

	static std::string getViewMessageLink(shared_ptr<Portal> portal, const ObjectID &id, bool secure);

private:
	inline shared_ptr<IHtmlControl> _getView() const;

private:
	static String _toString(Action action);
	static Action _fromString(const String &str);

// Operations
private:
	void _createBrowser();

	void _editMessage();
	shared_ptr<ObjectsUser> _getMessageReference();
	void _sendMessage();

	void _showInbox();
	void _showTrash();
	void _showSentItems();
	void _showMessage(const ObjectID &id);

// IPage interface
public:
	virtual String getPageName() const;

	// Accesso MCP richiesto
	virtual bool isMcpModeRequired() const;

	// Accesso Isis consentito
	virtual bool isIsisAllowed() const;

// IPortalPage overrides
protected:
	virtual void onInit();
	virtual void onLoad();
	virtual void onPreRender();

protected:
	shared_ptr<IHtmlControl> m_view;

	shared_ptr<HtmlTextBox> m_messageSubject;
	shared_ptr<IdeOMLEditor> m_messageBody;
		
public:
	static const std::wstring ACTION;
	static const std::wstring SECURE;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IHtmlControl> Messenger::_getView() const { return m_view; }

//////////////////////////////////////////////////////////////////////

} // portal
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_IDEPORTALMESSENGER_H
