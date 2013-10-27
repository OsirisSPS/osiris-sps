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
#include "ideportalaccount.h"

#include "algorithms.h"
#include "boost/bind.hpp"
#include "buffer.h"
#include "convert.h"
#include "conversions.h"
#include "dataaccount.h"
#include "datalocalmessage.h"
#include "objectsavatar.h"
#include "objectsmessage.h"
#include "objectsreputation.h"
#include "objectstag.h"
#include "objectsuser.h"
#include "iportaldatabase.h"
#include "dbdatabasessystem.h"
#include "entitiessnapshotmanager.h"
#include "htmlunit.h"
#include "ideaccount.h"
#include "ideskin.h"
#include "idepickerskin.h"
#include "ideblock.h"
#include "ideomleditor.h"
#include "idebutton.h"
#include "idepickerculture.h"
#include "idepickerdatetime.h"
#include "idemailbox.h"
#include "idesystem.h"
#include "idesession.h"
#include "ideutils.h"
#include "locked.h"
#include "portalsaccount.h"
#include "portalsmessenger.h"
#include "portalsportal.h"
#include "portalsportalssystem.h"
#include "htmlevent.h"
#include "htmlattributes.h"
#include "css.h"
#include "httprequest.h"
#include "httppostparam.h"
#include "htmlcheckbox.h"
#include "htmlcombobox.h"
#include "htmlfilebrowser.h"
#include "htmlimage.h"
#include "htmllink.h"
#include "htmlliteral.h"
#include "htmloption.h"
#include "htmloptiongroup.h"
#include "htmlpanel.h"
#include "htmltable.h"
#include "htmltablecell.h"
#include "htmltableheader.h"
#include "htmltablerow.h"
#include "htmltext.h"
#include "htmltextarea.h"
#include "htmltextbox.h"
#include "uniqueid.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace portal {

//////////////////////////////////////////////////////////////////////

const std::wstring Account::ACTION(L"act");
const std::wstring Account::SECURE(L"secure");

//////////////////////////////////////////////////////////////////////

Account::Account(shared_ptr<HttpSession> session) : PageBase(session),
													m_messageSubject(OS_NEW HtmlTextBox()),
													m_messageBody(OS_NEW IdeOMLEditor()),
													m_accountName(OS_NEW HtmlTextBox()),
													m_accountPassword(OS_NEW HtmlTextBox()),
													m_accountViewMode(OS_NEW HtmlComboBox()),
													m_skinPicker(OS_NEW IdePickerSkin()),
													m_username(OS_NEW HtmlTextBox()),
													m_description(OS_NEW HtmlTextBox()),
													m_authorsThreshold(OS_NEW HtmlComboBox()),
													m_editorsThreshold(OS_NEW HtmlComboBox()),
													m_birthdate(OS_NEW IdePickerDateTime()),
													m_gender(OS_NEW HtmlComboBox()),
													m_location(OS_NEW HtmlTextBox()),													
													m_email(OS_NEW HtmlTextBox()),
													m_website(OS_NEW HtmlTextBox()),
													m_showAvatar(OS_NEW HtmlCheckBox()),
													m_markEditor(OS_NEW IdeOMLEditor()),
													m_showMark(OS_NEW HtmlCheckBox()),
													m_avatarBrowser(OS_NEW HtmlFileBrowser()),
													m_markPreview(OS_NEW HtmlLiteral()),
													m_paramsPanel(OS_NEW HtmlPanel()),
													m_miscEditor(OS_NEW IdeOMLEditor()),
													m_paramName(OS_NEW HtmlTextBox()),
													m_paramValue(OS_NEW HtmlTextBox()),
													m_accountTimeOffset(OS_NEW HtmlComboBox()),
													m_accountTimeDST(OS_NEW HtmlComboBox()),
													m_accountCulturePicker(OS_NEW IdePickerCulture())
{
	m_messageSubject->setID(_S("messageSubject"));
	m_messageSubject->setCss(_S("os_input_full"));
	m_messageBody->setID(_S("messageBody"));

	m_accountName->setID(_S("accountName"));
	m_accountName->setCss(_S("os_input_full"));
	m_accountPassword->setID(_S("accountPassword"));
	m_accountPassword->setPassword(true);
	m_accountPassword->setCss(_S("os_input_full"));
	m_accountViewMode->setID(_S("viewMode"));

	m_skinPicker->setID(_S("skin"));
	m_skinPicker->setShowSystem(true);

	m_username->setID(_S("username"));
	m_username->setCss(_S("os_input_full"));
	m_description->setID(_S("description"));
	m_description->setCss(_S("os_input_full"));
	m_authorsThreshold->setID(_S("authorsThreshold"));
	m_editorsThreshold->setID(_S("editorsThreshold"));
	m_birthdate->setID(_S("birthdate"));
	m_birthdate->setShowTime(false);
	m_gender->setID(_S("gender"));
	m_location->setID(_S("location"));
	m_location->setCss(_S("os_input_full"));	
	m_email->setID(_S("email"));
	m_email->setCss(_S("os_input_full"));
	m_website->setID(_S("website"));
	m_website->setCss(_S("os_input_full"));
	m_showAvatar->setID(_S("showAvatar"));
	m_showMark->setID(_S("showMark"));
	m_miscEditor->setID(_S("miscEditor"));
	m_avatarBrowser->setID(_S("avatar"));
	m_markEditor->setID(_S("markEditor"));

	m_accountTimeOffset->setID(_S("timeOffset"));
	m_accountTimeDST->setID(_S("dstOffset"));
	m_accountCulturePicker->setID(_S("language"));
	m_accountCulturePicker->setShowSystem(true);
}

Account::~Account()
{

}

Account::Action Account::getAction() const
{
	Action action = _fromString(getRequest()->getUrlParam(ACTION));
	return action == acUnknown ? acDefault : action;
}

String Account::getAction(Action action)
{
    return _toString(action);
}

String Account::getUrl(Action action) const
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(ACTION, _toString(action).to_wide());

	return getPortal()->getLink("account", params);
}

std::string Account::getViewMessageLink(shared_ptr<Portal> portal, const ObjectID &id, bool secure)
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(ACTION, _toString(acViewMessage).to_wide());
	params.set(OS_URL_PARAM_ID, id.toWide());
	params.set(SECURE, conversions::to_wstring(secure));

	return portal->getLink("account", params);
}

String Account::_toString(Action action)
{
	return conversions::to_utf16(static_cast<int32>(action));
}

Account::Action Account::_fromString(const String &str)
{
	uint32 action = conversions::from_utf16<uint32>(str);
	switch(action)
	{
	case acUnknown:
	case acEditMessage:
	case acViewInbox:
	case acViewSentItems:
	case acViewTrash:
	case acViewMessage:
	case acEditAccount:
	case acEditSkin:
	case acEditLanguage:
	case acEditProfile:
	case acEditMark:
	case acEditAvatar:
								return static_cast<Action>(action);

	default:					OS_ASSERTFALSE();
								break;
	}

	return acUnknown;
}

void Account::_createBrowser()
{
	shared_ptr<HtmlTable> table_main(OS_NEW HtmlTable());
	table_main->setWidth(HtmlUnit::percent(100));
	getArea(pageAreaContent)->getControls()->add(table_main);
	shared_ptr<HtmlTableRow> row_main = table_main->addRow();

	shared_ptr<HtmlTableCell> cell_main_left = row_main->addCell();
	cell_main_left->setWidth(HtmlUnit::pixels(150));
	m_view = row_main->addCell();
	m_view->setCss(_S("os_actions_left_page"));	

	shared_ptr<HtmlDiv> actionsDiv(OS_NEW HtmlDiv());
	actionsDiv->setCss(_S("os_actions_left_container"));
	cell_main_left->getControls()->add(actionsDiv);


	Action act = getAction();

	

	

	shared_ptr<HtmlDiv> groupUser(OS_NEW HtmlDiv());
	groupUser->setCss(_S("os_actions_group"));
	groupUser->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("portal.pages.account.user.title")))));
	actionsDiv->getControls()->add(groupUser);

	shared_ptr<HtmlLink> editProfile(OS_NEW HtmlLink(getText(_S("portal.pages.account.user.modify")), getUrl(acEditProfile)));
	editProfile->setCss((act == acEditProfile) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	actionsDiv->getControls()->add(editProfile);

	shared_ptr<HtmlLink> editMark(OS_NEW HtmlLink(getText(_S("portal.pages.account.user.signature")), getUrl(acEditMark)));
	editMark->setCss((act == acEditMark) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	actionsDiv->getControls()->add(editMark);

	shared_ptr<HtmlLink> editAvatar(OS_NEW HtmlLink(getText(_S("portal.pages.account.user.avatar")), getUrl(acEditAvatar)));
	editAvatar->setCss((act == acEditAvatar) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	actionsDiv->getControls()->add(editAvatar);



	shared_ptr<HtmlDiv> groupAccount(OS_NEW HtmlDiv());
	groupAccount->setCss(_S("os_actions_group"));
	groupAccount->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("portal.pages.account.account.title")))));
	actionsDiv->getControls()->add(groupAccount);

	shared_ptr<HtmlLink> editSkin(OS_NEW HtmlLink(getText(_S("portal.pages.account.account.skin")), getUrl(acEditSkin)));
	editSkin->setCss((act == acEditSkin) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	actionsDiv->getControls()->add(editSkin);

	shared_ptr<HtmlLink> editLanguage(OS_NEW HtmlLink(getText(_S("portal.pages.account.account.culture")), getUrl(acEditLanguage)));
	editLanguage->setCss((act == acEditLanguage) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	actionsDiv->getControls()->add(editLanguage);

	shared_ptr<HtmlLink> editAccount(OS_NEW HtmlLink(getText(_S("portal.pages.account.account.modify")), getUrl(acEditAccount)));
	editAccount->setCss((act == acEditAccount) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	actionsDiv->getControls()->add(editAccount);


	shared_ptr<HtmlDiv> groupMailbox(OS_NEW HtmlDiv());
	groupMailbox->setCss(_S("os_actions_group"));
	groupMailbox->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("mailbox.name")))));
	actionsDiv->getControls()->add(groupMailbox);

	shared_ptr<HtmlLink> viewInbox(OS_NEW HtmlLink(getText(_S("mailbox.inbox.name")), getUrl(acViewInbox)));
	viewInbox->setCss((act == acViewInbox) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	actionsDiv->getControls()->add(viewInbox);

	shared_ptr<HtmlLink> viewSentItems(OS_NEW HtmlLink(getText(_S("mailbox.sent_items.name")), getUrl(acViewSentItems)));
	viewSentItems->setCss((act == acViewSentItems) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	actionsDiv->getControls()->add(viewSentItems);

	shared_ptr<HtmlLink> viewTrash(OS_NEW HtmlLink(getText(_S("mailbox.trash.name")), getUrl(acViewTrash)));
	viewTrash->setCss((act == acViewTrash) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	actionsDiv->getControls()->add(viewTrash);


	/*
	shared_ptr<IdeBlock> block_menu(OS_NEW IdeBlock(_S("Menu")));
	block_menu->setType(blockTypeSmall);
	cell_main_left->getControls()->add(block_menu);

	shared_ptr<HtmlTable> table_menu(OS_NEW HtmlTable());
	block_menu->getBody()->getControls()->add(table_menu);

	Action act = getAction();

	shared_ptr<HtmlTableCell> cell_messenger = table_menu->addRow()->addHeader();
	cell_messenger->setCss(_S("os_actions_group"));
	cell_messenger->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("mailbox.name")))));

	shared_ptr<HtmlTableCell> cell_showInbox = table_menu->addRow()->addCell();
	cell_showInbox->setCss((act == acViewInbox) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	cell_showInbox->getControls()->add(shared_ptr<HtmlLink>(OS_NEW HtmlLink(getText(_S("mailbox.inbox.name")), getUrl(acViewInbox))));

	shared_ptr<HtmlTableCell> cell_showSentItems = table_menu->addRow()->addCell();
	cell_showSentItems->setCss((act == acViewSentItems) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	cell_showSentItems->getControls()->add(shared_ptr<HtmlLink>(OS_NEW HtmlLink(getText(_S("mailbox.sent_items.name")), getUrl(acViewSentItems))));

	shared_ptr<HtmlTableCell> cell_showTrash = table_menu->addRow()->addCell();
	cell_showTrash->setCss((act == acViewTrash) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	cell_showTrash->getControls()->add(shared_ptr<HtmlLink>(OS_NEW HtmlLink(getText(_S("mailbox.trash.name")), getUrl(acViewTrash))));

	shared_ptr<HtmlTableCell> cell_account = table_menu->addRow()->addHeader();
	cell_account->setCss(_S("os_actions_group"));
	cell_account->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("portal.pages.account.account.title")))));

	shared_ptr<HtmlTableCell> cell_editAccount = table_menu->addRow()->addCell();
	cell_editAccount->setCss((act == acEditAccount) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	cell_editAccount->getControls()->add(shared_ptr<HtmlLink>(OS_NEW HtmlLink(getText(_S("portal.pages.account.account.modify")), getUrl(acEditAccount))));

	shared_ptr<HtmlTableCell> cell_editSkin = table_menu->addRow()->addCell();
	cell_editSkin->setCss((act == acEditSkin) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	cell_editSkin->getControls()->add(shared_ptr<HtmlLink>(OS_NEW HtmlLink(getText(_S("portal.pages.account.account.skin")), getUrl(acEditSkin))));

	shared_ptr<HtmlTableCell> cell_editCultures = table_menu->addRow()->addCell();
	cell_editCultures->setCss((act == acEditLanguage) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	cell_editCultures->getControls()->add(shared_ptr<HtmlLink>(OS_NEW HtmlLink(getText(_S("portal.pages.account.account.culture")), getUrl(acEditLanguage))));

	shared_ptr<HtmlTableCell> cell_profile = table_menu->addRow()->addHeader();
	cell_profile->setCss(_S("os_actions_group"));
	cell_profile->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("portal.pages.account.user.title")))));

	shared_ptr<HtmlTableCell> cell_editProfile = table_menu->addRow()->addCell();
	cell_editProfile->setCss((act == acEditProfile) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	cell_editProfile->getControls()->add(shared_ptr<HtmlLink>(OS_NEW HtmlLink(getText(_S("portal.pages.account.user.modify")), getUrl(acEditProfile))));

	shared_ptr<HtmlTableCell> cell_editMark = table_menu->addRow()->addCell();
	cell_editMark->setCss((act == acEditMark) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	cell_editMark->getControls()->add(shared_ptr<HtmlLink>(OS_NEW HtmlLink(getText(_S("portal.pages.account.user.signature")), getUrl(acEditMark))));

	shared_ptr<HtmlTableCell> cell_editAvatar = table_menu->addRow()->addCell();
	cell_editAvatar->setCss((act == acEditAvatar) ? _S("os_actions_left_selected") : _S("os_actions_left"));
	cell_editAvatar->getControls()->add(shared_ptr<HtmlLink>(OS_NEW HtmlLink(getText(_S("portal.pages.account.user.avatar")), getUrl(acEditAvatar))));
	*/
}

void Account::_editMessage()
{
	shared_ptr<IdeBlock> block(OS_NEW IdeBlock(getText(_S("mailbox.new_message.title"))));
	_getView()->getControls()->add(block);

	shared_ptr<HtmlTable> table(OS_NEW HtmlTable());
	table->setCss(_S("os_table_data"));
	block->getBody()->getControls()->add(table);

	shared_ptr<ObjectsUser> message_reference = _getMessageReference();
	if(message_reference == null)
	{
		redirect(getUrl(acDefault).to_ascii());
		return;
	}

	// Label messaggio private

	shared_ptr<HtmlTableCell> label_message = table->addRow()->addCell();
	label_message->setCss(OS_CSS_SUBTITLE);
	label_message->setColSpan(2);
	label_message->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("mailbox.new_message.subtitle")) + message_reference->name)));

	// Oggetto del messaggio

	shared_ptr<HtmlTableRow> row_subject = table->addRow();

	shared_ptr<HtmlTableCell> subject_left = row_subject->addCell();
	subject_left->setCss(OS_CSS_FORM_LEFT);
	subject_left->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("mailbox.new_message.subject")))));

	shared_ptr<HtmlTableCell> subject_right = row_subject->addCell();
	subject_right->setCss(OS_CSS_FORM_RIGHT);
	if(getPostBack() == false)
	{
		String paramQuote;
		//paramQuote.from_utf8(getRequest()->getUrlParam(OS_URL_PARAM_QUOTE));
		paramQuote = getRequest()->getUrlParam(OS_URL_PARAM_QUOTE); // 0.15
		m_messageSubject->setValue(paramQuote);
	}

	subject_right->getControls()->add(m_messageSubject);

	// Contenuto del messaggio

	shared_ptr<HtmlTableRow> row_body = table->addRow();

	shared_ptr<HtmlTableCell> body_left = row_body->addCell();
	body_left->setCss(OS_CSS_FORM_LEFT);
	body_left->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("mailbox.new_message.content")))));

	shared_ptr<HtmlTableCell> body_right = row_body->addCell();
	body_right->setCss(OS_CSS_FORM_RIGHT);
	body_right->getControls()->add(m_messageBody);

	// Comandi

	shared_ptr<HtmlTableCell> cell_commands = table->addRow()->addCell();
	cell_commands->setCss(OS_CSS_FORM_COMMANDS);
	cell_commands->setColSpan(2);

	shared_ptr<IdeButton> cmdSend(OS_NEW IdeButton(getText(_S("mailbox.new_message.send"))));
	cmdSend->setID(_S("sendMessage"));
	cmdSend->setIsDefault(true);
	cmdSend->getEventClick()->connect(boost::bind(&Account::_sendMessage, this));

	cell_commands->getControls()->add(cmdSend);
}

shared_ptr<ObjectsUser> Account::_getMessageReference()
{
	return objects_user_cast(getObject(static_cast<String>(getRequest()->getUrlParam(OS_URL_PARAM_ID)).to_ascii()));
}

void Account::_sendMessage()
{
	shared_ptr<ObjectsUser> message_reference = _getMessageReference();
	if(message_reference == null)
		return;

	shared_ptr<ObjectsMessage> message(OS_NEW ObjectsMessage());
	message->subject = m_messageSubject->getValue();
	message->body = m_messageBody->getValue();

	if(message->body.empty())
	{
		showError(getText(_S("mailbox.new_message.error.invalid")));
		return;
	}

	ObjectID id;
	if(getPortal()->getMessenger()->createPrivateMessage(message, message_reference, getDatabase(), getSessionAccount(), id, true))
		redirect(getUrl(acViewSentItems).to_ascii());
	else
		showError(getText(_S("messenger.send_message_error")));
}

void Account::_showInbox()
{
	shared_ptr<IHtmlControl> mailbox(OS_NEW IdeMailBox(messageFolderInbox));
	mailbox->setID(_S("mailbox"));
	_getView()->getControls()->add(mailbox);
}

void Account::_showTrash()
{
	shared_ptr<IHtmlControl> mailbox(OS_NEW IdeMailBox(messageFolderTrash));
	mailbox->setID(_S("mailbox"));
	_getView()->getControls()->add(mailbox);
}

void Account::_showSentItems()
{
	shared_ptr<IHtmlControl> mailbox(OS_NEW IdeMailBox(messageFolderSentItems));
	mailbox->setID(_S("mailbox"));
	_getView()->getControls()->add(mailbox);
}

void Account::_showMessage(const ObjectID &id)
{
	shared_ptr<IdeMailBox> mailbox(OS_NEW IdeMailBox(id));
	mailbox->setID(_S("mailbox"));
	String secure = getRequest()->getUrlParam(SECURE);
	if(secure.empty() == false)
		mailbox->setSecure(conversions::from_utf16<bool>(secure));

	_getView()->getControls()->add(mailbox);
}

void Account::_editAccount()
{
	shared_ptr<IdeBlock> block(OS_NEW IdeBlock(getText(_S("portal.pages.account.account.modify.title"))));
	_getView()->getControls()->add(block);

	shared_ptr<IdeAccount> account = getSessionAccount()->getAccount();

	shared_ptr<HtmlTable> table(OS_NEW HtmlTable());
	table->setCss(_S("os_table_data"));
	block->getBody()->getControls()->add(table);

	// Label informazioni

	shared_ptr<HtmlTableCell> label_info = table->addRow()->addCell();
	label_info->setCss(OS_CSS_SUBTITLE);
	label_info->setColSpan(2);
	label_info->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("portal.pages.account.account.modify.subtitle")))));

	// Modalit di visualizzazione
	m_accountViewMode->addOption(getText(_S("account.view_mode.normal")), Convert::toString(dataViewModeNormal));
	m_accountViewMode->addOption(getText(_S("account.view_mode.editor")), Convert::toString(dataViewModeEditor));
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_accountViewMode, _S("account.view_mode"));

	// Name
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_accountName, _S("account.username"));

	// Password
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_accountPassword, _S("account.password"));

	// Comandi
	shared_ptr<HtmlTableCell> cell_commands = table->addRow()->addCell();
	cell_commands->setCss(OS_CSS_FORM_COMMANDS);
	cell_commands->setColSpan(2);

	shared_ptr<IdeButton> cmdSave(OS_NEW IdeButton(getText(_S("portal.pages.account.account.modify.action.confirm"))));
	cmdSave->setID(_S("editAccount"));
	cmdSave->setIsDefault(true);
	cmdSave->getEventClick()->connect(boost::bind(&Account::_updateAccount, this));

	cell_commands->getControls()->add(cmdSave);
}

void Account::_initAccount()
{
	shared_ptr<IdeAccount> account = getSessionAccount()->getAccount();
	OS_ASSERT(account != null);

	if(getPostBack() == false)
	{
		m_accountViewMode->setValue(Convert::toString(account->getViewMode()));
		m_accountName->setValue(account->getName());
		m_accountPassword->setValue(account->getRealPassword());
	}
}

void Account::_updateAccount()
{
	shared_ptr<IdeAccount> account = getSessionAccount()->getAccount();

	// Aggiorna la modalit di visualizzazione
	account->setViewMode(Convert::toAccountViewMode(m_accountViewMode->getValue()));

	// Aggiorna lo username
	account->setName(m_accountName->getValue());

	// Aggiorna la password
	if(account->setRealPassword(m_accountPassword->getValue()) == false)
	{
		showError(getText(_S("portal.pages.account.account.modify.error.invalid_password")));
		return;
	}

	if(account->updateAccount(getDatabase()))
		showMessage(getText(_S("portal.pages.account.account.modify.error.success")));
	else
		showError(getText(_S("portal.pages.account.account.modify.error.generic")));
}

void Account::_editSkin()
{
	shared_ptr<IdeBlock> block(OS_NEW IdeBlock(getText(_S("portal.pages.account.account.skin.title"))));
	_getView()->getControls()->add(block);

	shared_ptr<HtmlTable> table(OS_NEW HtmlTable());
	table->setCss(_S("os_table_data"));
	block->getBody()->getControls()->add(table);

	// Enumera le skins disponibili
	Locked<const IdeSystem::Skins>::unique skins = IdeSystem::instance()->getAvailableSkins();
	// Verifica che ci sia almeno una skin
	if(skins->empty() == false)
	{
		// Skin

		shared_ptr<HtmlTableRow> row_skin = table->addRow();

		shared_ptr<HtmlTableCell> skin_left = row_skin->addCell();
		skin_left->setCss(OS_CSS_FORM_LEFT);
		skin_left->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("portal.pages.account.account.skin.label.skin")))));

		shared_ptr<HtmlTableCell> skin_right = row_skin->addCell();
		skin_right->setCss(OS_CSS_FORM_RIGHT);
		/*
		shared_ptr<HtmlOptionGroup> skinsGroup = m_skin->addOptionGroup(getText(_S("portal.pages.account.account.skin.main_group")));
		m_availableSkins.push_back(skinsGroup->addOption(getText(_S("portal.pages.account.account.skin.default")), UniqueID::EMPTY.toUTF16()));
		for(IdeSystem::Skins::const_iterator i = skins->begin(); i != skins->end(); ++i)
		{
			shared_ptr<IdeSkin> skin = i->second;
			if(skin != null)
				m_availableSkins.push_back(skinsGroup->addOption(skin->getName(), skin->getID().toUTF16()));
		}
		*/
		skin_right->getControls()->add(m_skinPicker);
	}

	// Comandi

	shared_ptr<HtmlTableCell> cell_commands = table->addRow()->addCell();
	cell_commands->setCss(OS_CSS_FORM_COMMANDS);
	cell_commands->setColSpan(2);

	shared_ptr<IdeButton> cmdSave(OS_NEW IdeButton(getText(_S("portal.pages.account.account.skin.action.confirm"))));
	cmdSave->setID(_S("editSkin"));
	cmdSave->setIsDefault(true);
	cmdSave->getEventClick()->connect(boost::bind(&Account::_updateSkin, this));

	cell_commands->getControls()->add(cmdSave);
}

void Account::_initSkin()
{
	shared_ptr<IdeAccount> account = getSessionAccount()->getAccount();
	OS_ASSERT(account != null);

	if(getPostBack() == false)
		m_skinPicker->setValue(account->getSkinID().toUTF16());
	/*
	if(m_skin != null)
	{
		// Scorre tutte le skin caricate
		for(list<shared_ptr<IHtmlControl> >::type::iterator i = m_availableSkins.begin(); i != m_availableSkins.end(); ++i)
		{
			shared_ptr<HtmlOption> skin_option = boost::dynamic_pointer_cast<HtmlOption>(*i);
			if(skin_option != null)
			{
				// Controlla se la skin corrente coincide con quella dell'account
				if(skin_option->getValue() == account->getSkinID())
				{
					// Seleziona la skin
					skin_option->setSelected(true);
					break;
				}
			}
		}
	}
	*/
}

void Account::_updateSkin()
{	
	shared_ptr<IdeAccount> account = getSessionAccount()->getAccount();

	account->setSkin(m_skinPicker->getValue().to_ascii());

	if(account->updateAccount(getDatabase()))
		// Ricarica la pagina per applicare la skin
		reload();
	else
		showError(getText(_S("portal.pages.account.account.skin.error.generic")));
}

void Account::_initLanguage()
{
	shared_ptr<IdeAccount> account = getSessionAccount()->getAccount();
	OS_ASSERT(account != null);

	if(getPostBack() == false)
	{
		m_accountCulturePicker->setValue(account->getAccount()->getLanguage());		
		m_accountTimeOffset->setValue(conversions::to_utf16<int32>(account->getAccount()->getTimeOffset()));
		m_accountTimeDST->setValue(conversions::to_utf16<int32>(account->getAccount()->getTimeDST()));	
	}
}

void Account::_editLanguage()
{
	shared_ptr<IdeBlock> block(OS_NEW IdeBlock(getText(_S("portal.pages.account.account.culture.title"))));
	_getView()->getControls()->add(block);

	shared_ptr<HtmlTable> table(OS_NEW HtmlTable());
	table->setCss(_S("os_table_data"));
	block->getBody()->getControls()->add(table);

	// Time Offset		
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 10000000)), "10000000");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), -43200)), "-43200");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), -39600)), "-39600");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), -36000)), "-36000");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), -34200)), "-34200");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), -32400)), "-32400");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), -28800)), "-28800");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), -25200)), "-25200");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), -21600)), "-21600");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), -18000)), "-18000");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), -16200)), "-16200");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), -14400)), "-14400");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), -12600)), "-12600");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), -10800)), "-10800");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), -7200)), "-7200");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), -3600)), "-3600");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 0)), "0");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 3600)), "3600");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 7200)), "7200");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 10800)), "10800");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 12600)), "12600");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 14400)), "14400");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 16200)), "16200");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 18000)), "18000");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 19800)), "19800");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 20700)), "20700");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 21600)), "21600");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 23400)), "23400");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 25200)), "25200");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 28800)), "28800");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 31500)), "31500");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 32400)), "32400");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 34200)), "34200");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 36000)), "36000");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 37800)), "37800");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 39600)), "39600");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 41400)), "41400");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 43200)), "43200");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 45900)), "45900");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 46800)), "46800");
	m_accountTimeOffset->addOption(getText(String::format(_S("timeTable_GMT_%d").c_str(), 50400)), "50400");	
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_accountTimeOffset, _S("user.time_offset"));

	// DST Offset
	m_accountTimeDST->addOption(getText(String::format(_S("timeTable_DST_%d").c_str(), 10000000)), "10000000");
	m_accountTimeDST->addOption(getText(String::format(_S("timeTable_DST_%d").c_str(), -3600)), "-3600");
	m_accountTimeDST->addOption(getText(String::format(_S("timeTable_DST_%d").c_str(), 0)), "0");
	m_accountTimeDST->addOption(getText(String::format(_S("timeTable_DST_%d").c_str(), 3600)), "3600");
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_accountTimeDST, _S("user.time_dst"));
	
	//cultures->getEventSelect()->connect(boost::bind(&Account::_updateLanguage, this, _1));
	shared_ptr<HtmlTableCell> cellCulture = table->addRow()->addCell();
	cellCulture->setColSpan(2);
	cellCulture->getControls()->add(m_accountCulturePicker);

	// Comandi
	shared_ptr<HtmlTableCell> cell_commands = table->addRow()->addCell();
	cell_commands->setCss(OS_CSS_FORM_COMMANDS);
	cell_commands->setColSpan(2);

	shared_ptr<IdeButton> cmdSave(OS_NEW IdeButton(getText(_S("portal.pages.account.account.modify.action.confirm"))));
	cmdSave->setID(_S("editLanguage"));
	cmdSave->setIsDefault(true);
	cmdSave->getEventClick()->connect(boost::bind(&Account::_updateLanguage, this));

	cell_commands->getControls()->add(cmdSave);
}

/*
void Account::_updateLanguage(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == null)
		return;

	shared_ptr<IdeAccount> account = getLoggedUser()->getAccount();
	account->setLanguage(htmlEvent->get(0));
	account->updateAccount(getDatabase());

	reload();
}
*/

void Account::_updateLanguage()
{
	shared_ptr<IdeAccount> account = getSessionAccount()->getAccount();	
	
	account->getAccount()->setTimeOffset(conversions::from_utf16<int32>(m_accountTimeOffset->getValue()));
	account->getAccount()->setTimeDST(conversions::from_utf16<int32>(m_accountTimeDST->getValue()));
	account->getAccount()->setLanguage(m_accountCulturePicker->getValue());
	
	account->updateAccount(getDatabase());

	reload();
}

void Account::_editProfile()
{
	shared_ptr<IdeBlock> block(OS_NEW IdeBlock(getText(_S("portal.pages.account.user.modify.title"))));
	_getView()->getControls()->add(block);

	shared_ptr<HtmlTable> table(OS_NEW HtmlTable());
	table->setCss(_S("os_table_data"));
	block->getBody()->getControls()->add(table);

	// Label informazioni

	shared_ptr<HtmlTableCell> label_info = table->addRow()->addCell();
	label_info->setCss(OS_CSS_SUBTITLE);
	label_info->setColSpan(2);
	label_info->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("portal.pages.account.user.modify.subtitle")))));

	// Username
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_username, _S("user.name"));

	// Descrizione
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_description, _S("user.description"));

	// Soglia minima autori
	m_authorsThreshold->addOption(getText(_S("reputation.threshold.inherit")), Convert::toString(rtInherit));
	m_authorsThreshold->addOption(getText(_S("reputation.threshold.all")), Convert::toString(rtAll));
	m_authorsThreshold->addOption(getText(_S("reputation.threshold.negative")), Convert::toString(rtNegative));
	m_authorsThreshold->addOption(getText(_S("reputation.threshold.not_negative")), Convert::toString(rtNotNegative));
	m_authorsThreshold->addOption(getText(_S("reputation.threshold.positive")), Convert::toString(rtPositive));
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_authorsThreshold, _S("user.authors_threshold"));

	// Soglia minima editori
	m_editorsThreshold->addOption(getText(_S("reputation.threshold.inherit")), Convert::toString(rtInherit));
	m_editorsThreshold->addOption(getText(_S("reputation.threshold.all")), Convert::toString(rtAll));
	m_editorsThreshold->addOption(getText(_S("reputation.threshold.negative")), Convert::toString(rtNegative));
	m_editorsThreshold->addOption(getText(_S("reputation.threshold.not_negative")), Convert::toString(rtNotNegative));
	m_editorsThreshold->addOption(getText(_S("reputation.threshold.positive")), Convert::toString(rtPositive));
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_editorsThreshold, _S("user.editors_threshold"));

	// Data di nascita
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_birthdate, _S("user.birth_date"));

	// Gender
	m_gender->addOption(getText(_S("users.gender_unspecified")), conversions::to_utf16<uint32>(ugUnspecified));
	m_gender->addOption(getText(_S("users.gender_male")), conversions::to_utf16<uint32>(ugMale));
	m_gender->addOption(getText(_S("users.gender_female")), conversions::to_utf16<uint32>(ugFemale));
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_gender, _S("user.gender"));

	// Location
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_location, _S("user.location"));

	// EMail
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_email, _S("user.email"));

	// WebSite
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_website, _S("user.website"));

	// Mostra avatar
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_showAvatar, _S("user.show_avatar"));

	// Mostra mark
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_showMark, _S("user.show_mark"));

	// Misc
	Utils::addInputRow(table, get_this_ptr<IPage>(), m_miscEditor, _S("user.misc"));

	// Comandi
	shared_ptr<HtmlTableCell> cell_commands = table->addRow()->addCell();
	cell_commands->setCss(OS_CSS_FORM_COMMANDS);
	cell_commands->setColSpan(2);

	shared_ptr<IdeButton> cmdSave(OS_NEW IdeButton(getText(_S("portal.pages.account.user.modify.action.confirm"))));
	cmdSave->setID(_S("editProfile"));
	cmdSave->setIsDefault(true);
	cmdSave->getEventClick()->connect(boost::bind(&Account::_updateProfile, this));

	cell_commands->getControls()->add(cmdSave);
}

void Account::_initProfile()
{
	shared_ptr<ObjectsUser> user = getSessionAccount()->getUser(getDatabase());
	OS_ASSERT(user != null);

	if(getPostBack() == false)
	{
		m_username->setValue(user->name);
		m_description->setValue(user->description);
		if(user->authors_threshold.isNull() == false)
			m_authorsThreshold->setValue(conversions::to_utf16<uint32>(user->authors_threshold));
		if(user->editors_threshold.isNull() == false)
			m_editorsThreshold->setValue(conversions::to_utf16<uint32>(user->editors_threshold));

		if(user->birth_date->isValid())
			m_birthdate->setValue(user->birth_date);
		m_gender->setValue(conversions::to_utf16<uint32>(user->gender));
		m_location->setValue(user->location);	
		m_email->setValue(user->email);
		m_website->setValue(user->website);
		m_showAvatar->setCheck(user->show_avatar);
		m_showMark->setCheck(user->show_mark);
		m_miscEditor->setValue(user->misc);
	}
}

void Account::_updateProfile()
{
	shared_ptr<ObjectsUser> user = getSessionAccount()->getUser(getDatabase());
	OS_ASSERT(user != null);

	if(Portal::validateUsername(m_username->getValue()) == false)
	{
		showError(getText(_S("portal.pages.account.user.modify.error.generic")));
		return;
	}

	user->name = m_username->getValue();

	user->description = algorithms::trim_copy(m_description->getValue());

	DbValue<uint32> newAuthorsThreshold;
	if(m_authorsThreshold->getValue().empty() == false)
		newAuthorsThreshold = Convert::toReputationThreshold(m_authorsThreshold->getValue());
	DbValue<uint32> newEditorsThreshold;
	if(m_editorsThreshold->getValue().empty() == false)
		newEditorsThreshold = Convert::toReputationThreshold(m_editorsThreshold->getValue());

	if( (user->authors_threshold != newAuthorsThreshold) || (user->editors_threshold != newEditorsThreshold) )
	{
		user->authors_threshold = newAuthorsThreshold;
		user->editors_threshold = newEditorsThreshold;
		//getPortal()->getSnapshotManager()->onAccountChanged(getDatabase(), getLoggedUser()->getAccount()->getID(), user);
		getPortal()->getSnapshotManager()->onChangeReputationLevel(getDatabase());
	}
	user->birth_date = m_birthdate->getValue();
	user->gender = conversions::from_utf16<uint32>(m_gender->getValue());
	user->location = algorithms::trim_copy(m_location->getValue());	
	user->email = algorithms::trim_copy(m_email->getValue());
	user->website = algorithms::trim_copy(m_website->getValue());
	user->show_mark = m_showMark->getCheck();
	user->show_avatar = m_showAvatar->getCheck();
	user->misc = algorithms::trim_copy(m_miscEditor->getValue());

	LanguageResult result = getDatabase()->_signAndUpdate(user, getSessionAccount()->getPrivateKey(), true);
	if(result.empty())
		showMessage(getText(_S("portal.pages.account.user.modify.error.success")));
	else
		showError(getText(result));		
}

void Account::_editAvatar()
{
	shared_ptr<IdeBlock> block(OS_NEW IdeBlock(getText(_S("portal.pages.account.user.avatar.title"))));
	_getView()->getControls()->add(block);

	shared_ptr<HtmlTable> table(OS_NEW HtmlTable());
	table->setCss(_S("os_table_data"));
	block->getBody()->getControls()->add(table);

	// Label avatar corrente

	shared_ptr<HtmlTableCell> label_current = table->addRow()->addCell();
	label_current->setCss(OS_CSS_SUBTITLE);
	label_current->setColSpan(2);
	label_current->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("portal.pages.account.user.avatar.subtitle_actual")))));

	// Avatar corrente

	shared_ptr<HtmlTableCell> cell_current = table->addRow()->addCell();
	cell_current->setColSpan(2);
	m_avatarPanel = cell_current;

	// Label nuovo avatar

	shared_ptr<HtmlTableCell> label_edit = table->addRow()->addCell();
	label_edit->setCss(OS_CSS_SUBTITLE);
	label_edit->setColSpan(2);
	label_edit->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("portal.pages.account.user.avatar.subtitle_modify")))));

	// Nuovo avatar

	shared_ptr<HtmlTableRow> row_edit = table->addRow();

	shared_ptr<HtmlTableCell> edit_left = row_edit->addCell();
	edit_left->setCss(OS_CSS_FORM_LEFT);
	edit_left->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("portal.pages.account.user.avatar.label.select")))));

	shared_ptr<HtmlTableCell> edit_right = row_edit->addCell();
	edit_right->setCss(OS_CSS_FORM_RIGHT);
	edit_right->getControls()->add(m_avatarBrowser);

	// Comandi

	shared_ptr<HtmlTableCell> cell_commands = table->addRow()->addCell();
	cell_commands->setCss(OS_CSS_FORM_COMMANDS);
	cell_commands->setColSpan(2);

	shared_ptr<IdeButton> cmdSave(OS_NEW IdeButton(getText(_S("portal.pages.account.user.avatar.action.confirm"))));
	cmdSave->setID(_S("editAvatar"));
	cmdSave->setIsDefault(true);
	cmdSave->getEventClick()->connect(boost::bind(&Account::_updateAvatar, this));

	cell_commands->getControls()->add(cmdSave);
}

void Account::_initAvatar()
{
	if(m_avatarPanel == null)
		return;

	if(m_avatarInfo != null)
	{
		m_avatarPanel->getControls()->remove(m_avatarInfo);
		m_avatarInfo.reset();
	}

	shared_ptr<ObjectsAvatar> avatar = getSessionAccount()->getUser(getDatabase())->getAvatar(getDatabase());
	if(avatar != null)
		m_avatarInfo.reset(OS_NEW HtmlImage(getPortal()->getAvatarLink(avatar->id, avatar->submit_date)));
	else
		m_avatarInfo.reset(OS_NEW HtmlText(getText(_S("portal.pages.account.user.avatar.label.none"))));

	m_avatarPanel->getControls()->add(m_avatarInfo);
}

void Account::_updateAvatar()
{
	const Buffer *pFileBuffer = m_avatarBrowser->getFileBuffer();
	if(pFileBuffer != null)
	{
		if(ObjectsAvatar::validateFormat(getPortal(), *pFileBuffer))
		{
			LanguageResult result = getDatabase()->setUserAvatar(getSessionAccount()->getUser(getDatabase()), getSessionAccount()->getPrivateKey(), m_avatarBrowser->getFileName(), m_avatarBrowser->getFileType(), *pFileBuffer);
			if(result.empty() == false)
				showError(getText(result));			
		}
		else
		{
			showError(getText(_S("portal.pages.account.user.avatar.error.invalid")));
		}
	}
}

void Account::_editMark()
{
	shared_ptr<IdeBlock> block(OS_NEW IdeBlock(getText(_S("portal.pages.account.user.signature.title"))));
	_getView()->getControls()->add(block);

	shared_ptr<HtmlTable> table(OS_NEW HtmlTable());
	table->setCss(_S("os_table_data"));
	block->getBody()->getControls()->add(table);

	String mark = getSessionAccount()->getUser(getDatabase())->mark;
	// Verifica che l'utente abbia una firma
	if(mark.empty() == false)
	{
		// Label firma attuale

		shared_ptr<HtmlTableCell> label_current = table->addRow()->addCell();
		label_current->setCss(OS_CSS_SUBTITLE);
		label_current->setColSpan(2);
		label_current->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("portal.pages.account.user.signature.subtitle_actual")))));

		// Firma attuale

		shared_ptr<HtmlTableCell> cell_mark = table->addRow()->addCell();
		cell_mark->setColSpan(2);
		cell_mark->getControls()->add(m_markPreview);
	}

	// Label nuova firma

	shared_ptr<HtmlTableCell> label_edit = table->addRow()->addCell();
	label_edit->setCss(OS_CSS_SUBTITLE);
	label_edit->setColSpan(2);
	label_edit->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("portal.pages.account.user.signature.subtitle_modify")))));

	// Nuova firma

	shared_ptr<HtmlTableRow> row_edit = table->addRow();

	shared_ptr<HtmlTableCell> edit_right = row_edit->addCell();
	edit_right->setCss(OS_CSS_FORM_RIGHT);
	edit_right->setColSpan(2);
	m_markEditor->setValue(getSessionAccount()->getUser(getDatabase())->mark);
	edit_right->getControls()->add(m_markEditor);

	// Comandi

	shared_ptr<HtmlTableCell> cell_commands = table->addRow()->addCell();
	cell_commands->setCss(OS_CSS_FORM_COMMANDS);
	cell_commands->setColSpan(2);

	shared_ptr<IdeButton> cmdSave(OS_NEW IdeButton(getText(_S("portal.pages.account.user.signature.action.confirm"))));
	cmdSave->setID(_S("editMark"));
	cmdSave->setIsDefault(true);
	cmdSave->getEventClick()->connect(boost::bind(&Account::_updateMark, this));

	cell_commands->getControls()->add(cmdSave);
}

void Account::_initMark()
{
	m_markPreview->setHtml(parseOml(getSessionAccount()->getUser(getDatabase())->mark));
}

void Account::_updateMark()
{
	shared_ptr<ObjectsUser> user = getSessionAccount()->getUser(getDatabase());
	OS_ASSERT(user != null);

	user->mark = m_markEditor->getValue();

	LanguageResult result = getDatabase()->_signAndUpdate(user, getSessionAccount()->getPrivateKey(), true);
	if(result.empty())
		reload();
	else
		showError(getText(result));
}

String Account::getPageName() const
{
	return _S("portal.pages.account");
}

bool Account::isMcpModeRequired() const
{
	return false;
}

bool Account::isIsisAllowed() const
{
	return false;
}

void Account::onInit()
{
	PageBase::onInit();

	if(getSessionAccount()->isPortalGuest(getDatabase()))
	{
		showMessage(getText(_S("portals.errors.loggeduser_required")));
		return;
	}

	_createBrowser();
	OS_ASSERT(m_view != null);

	switch(getAction())
	{
	case acEditMessage:			// Mostra l'editor dei messaggi privati
								_editMessage();
								break;

	case acViewInbox:			// Mostra la posta in arrivo
								_showInbox();
								break;

	case acViewSentItems:		// Mostra la posta inviata
								_showSentItems();
								break;

	case acViewTrash:			// Mostra la posta eliminata
								_showTrash();
								break;

	case acViewMessage:			// Mostra il messaggio
								_showMessage(static_cast<String>(getRequest()->getUrlParam(OS_URL_PARAM_ID)).to_ascii());
								break;

	case acEditAccount:			// Edita l'account
								_editAccount();
								break;

	case acEditSkin:			// Edita la skin
								_editSkin();
								break;

	case acEditLanguage:		// Seleziona la cultura
								_editLanguage();
								break;

	case acEditProfile:			// Edita il profilo
								_editProfile();
								break;

	case acEditMark:			// Edita la firma
								_editMark();
								break;

	case acEditAvatar:			// Edita l'avatar
								_editAvatar();
								break;

	default:					OS_ASSERTFALSE();
								break;
	}


	

	
}

void Account::onLoad()
{
	PageBase::onLoad();

	if(getSessionAccount()->isPortalGuest(getDatabase()))
		return;	
	
}

void Account::onPreRender()
{
	PageBase::onPreRender();

	if(getSessionAccount()->isPortalGuest(getDatabase()))
		return;	

	switch(getAction())
	{
	case acEditMessage:
	case acViewInbox:
	case acViewSentItems:
	case acViewTrash:
	case acViewMessage:
								break;

	case acEditAccount:			// Inizializza le impostazioni dell'account
								_initAccount();
								break;

	case acEditProfile:			// Inizializza le impostazioni del profilo
								_initProfile();
								break;

	case acEditMark:			// Inizializza le impostazioni della firma
								_initMark();
								break;

	case acEditAvatar:			// Inizializza le impostazioni dell'avatar
								_initAvatar();
								break;

	case acEditSkin:			// Inizializza le impostazioni della skin
								_initSkin();
								break;

	case acEditLanguage:		// Inizializza le impostazioni della lingua
								_initLanguage();
								break;

	default:					OS_ASSERTFALSE();
								break;
	}
}

//////////////////////////////////////////////////////////////////////

} // portal
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
