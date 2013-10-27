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

#ifndef _IDE_PORTAL_ACCOUNT_H
#define _IDE_PORTAL_ACCOUNT_H

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

class EngineExport Account : public IPortalPage
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
		acEditAccount		= 6,
		acEditSkin			= 7,
		acEditLanguage		= 8,
		acEditProfile		= 9,
		acEditMark			= 10,
		acEditAvatar		= 11,
		acDefault			= acEditProfile,
	};

// Construction
public:
	Account(shared_ptr<HttpSession> session);
	virtual ~Account();

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

	void _editAccount();
	void _initAccount();
	void _updateAccount();

	void _editSkin();
	void _initSkin();
	void _updateSkin();

	void _editLanguage();
	void _initLanguage();
	void _updateLanguage();
	//void _updateLanguage(IEvent *e);

	void _editProfile();
	void _initProfile();
	void _updateProfile();

	void _editAvatar();
	void _initAvatar();
	void _updateAvatar();

	void _editMark();
	void _initMark();
	void _updateMark();

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

	shared_ptr<HtmlTextBox> m_accountName;
	shared_ptr<HtmlTextBox> m_accountPassword;
	shared_ptr<HtmlComboBox> m_accountViewMode;
	shared_ptr<IdePickerSkin> m_skinPicker;
	/*
	shared_ptr<HtmlComboBox> m_skin;
	list<shared_ptr<IHtmlControl> >::type m_availableSkins;
	*/

	shared_ptr<HtmlTextBox> m_username;
	shared_ptr<HtmlTextBox> m_description;
	shared_ptr<HtmlComboBox> m_authorsThreshold;
	shared_ptr<HtmlComboBox> m_editorsThreshold;
	shared_ptr<IdePickerDateTime> m_birthdate;
	shared_ptr<HtmlComboBox> m_gender;
	shared_ptr<HtmlTextBox> m_location;	
	shared_ptr<HtmlTextBox> m_email;
	shared_ptr<HtmlTextBox> m_website;
	shared_ptr<HtmlCheckBox> m_showAvatar;
	shared_ptr<HtmlCheckBox> m_showMark;
	shared_ptr<IdeOMLEditor> m_miscEditor;

	shared_ptr<HtmlLiteral> m_markPreview;
	shared_ptr<IdeOMLEditor> m_markEditor;

	shared_ptr<IHtmlControl> m_avatarPanel;
	shared_ptr<IHtmlControl> m_avatarInfo;
	shared_ptr<HtmlFileBrowser> m_avatarBrowser;
	shared_ptr<IHtmlControl> m_paramsPanel;

	shared_ptr<HtmlTextBox> m_paramName;
	shared_ptr<HtmlTextBox> m_paramValue;

	shared_ptr<HtmlComboBox> m_accountTimeOffset;
	shared_ptr<HtmlComboBox> m_accountTimeDST;
	shared_ptr<IdePickerCulture> m_accountCulturePicker;

public:
	static const std::wstring ACTION;
	static const std::wstring SECURE;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IHtmlControl> Account::_getView() const { return m_view; }

//////////////////////////////////////////////////////////////////////

} // portal
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_PORTAL_ACCOUNT_H
