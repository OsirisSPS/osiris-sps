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
#include "ideportalmessenger.h"

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
#include "idesession.h"
#include "ideskin.h"
#include "idepickerskin.h"
#include "ideblock.h"
#include "ideomleditor.h"
#include "idebutton.h"
#include "idepickerculture.h"
#include "idepickerdatetime.h"
#include "idemailbox.h"
#include "idesystem.h"
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

const std::wstring Messenger::ACTION(L"act");
const std::wstring Messenger::SECURE(L"secure");

//////////////////////////////////////////////////////////////////////

Messenger::Messenger(shared_ptr<HttpSession> session) : PageBase(session),
													m_messageSubject(OS_NEW HtmlTextBox()),
													m_messageBody(OS_NEW IdeOMLEditor())													
{
	m_messageSubject->setID(_S("messageSubject"));
	m_messageSubject->setCss(_S("os_input_full"));
	m_messageBody->setID(_S("messageBody"));

}

Messenger::~Messenger()
{

}

Messenger::Action Messenger::getAction() const
{
	Action action = _fromString(getRequest()->getUrlParam(ACTION));
	return action == acUnknown ? acDefault : action;
}

String Messenger::getAction(Action action)
{
    return _toString(action);
}

String Messenger::getUrl(Action action) const
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(ACTION, _toString(action).to_wide());

	return getPortal()->getLink("account", params);
}

std::string Messenger::getViewMessageLink(shared_ptr<Portal> portal, const ObjectID &id, bool secure)
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(ACTION, _toString(acViewMessage).to_wide());
	params.set(OS_URL_PARAM_ID, id.toWide());
	params.set(SECURE, conversions::to_wstring(secure));

	return portal->getLink("account", params);
}

String Messenger::_toString(Action action)
{
	return conversions::to_utf16(static_cast<int32>(action));
}

Messenger::Action Messenger::_fromString(const String &str)
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
								return static_cast<Action>(action);

	default:					OS_ASSERTFALSE();
								break;
	}

	return acUnknown;
}

void Messenger::_createBrowser()
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


}

void Messenger::_editMessage()
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
	cmdSend->getEventClick()->connect(boost::bind(&Messenger::_sendMessage, this));

	cell_commands->getControls()->add(cmdSend);
}

shared_ptr<ObjectsUser> Messenger::_getMessageReference()
{
	return objects_user_cast(getObject(static_cast<String>(getRequest()->getUrlParam(OS_URL_PARAM_ID)).to_ascii()));
}

void Messenger::_sendMessage()
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

void Messenger::_showInbox()
{
	shared_ptr<IHtmlControl> mailbox(OS_NEW IdeMailBox(messageFolderInbox));
	mailbox->setID(_S("mailbox"));
	_getView()->getControls()->add(mailbox);
}

void Messenger::_showTrash()
{
	shared_ptr<IHtmlControl> mailbox(OS_NEW IdeMailBox(messageFolderTrash));
	mailbox->setID(_S("mailbox"));
	_getView()->getControls()->add(mailbox);
}

void Messenger::_showSentItems()
{
	shared_ptr<IHtmlControl> mailbox(OS_NEW IdeMailBox(messageFolderSentItems));
	mailbox->setID(_S("mailbox"));
	_getView()->getControls()->add(mailbox);
}

void Messenger::_showMessage(const ObjectID &id)
{
	shared_ptr<IdeMailBox> mailbox(OS_NEW IdeMailBox(id));
	mailbox->setID(_S("mailbox"));
	String secure = getRequest()->getUrlParam(SECURE);
	if(secure.empty() == false)
		mailbox->setSecure(conversions::from_utf16<bool>(secure));

	_getView()->getControls()->add(mailbox);
}


String Messenger::getPageName() const
{
	return _S("portal.pages.messenger");
}

bool Messenger::isMcpModeRequired() const
{
	return false;
}

bool Messenger::isIsisAllowed() const
{
	return false;
}

void Messenger::onInit()
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

	default:					OS_ASSERTFALSE();
								break;
	}


	

	
}

void Messenger::onLoad()
{
	PageBase::onLoad();

	if(getSessionAccount()->isPortalGuest(getDatabase()))
	{
		return;
	}

	
}

void Messenger::onPreRender()
{
	PageBase::onPreRender();

	switch(getAction())
	{
	case acEditMessage:
	case acViewInbox:
	case acViewSentItems:
	case acViewTrash:
	case acViewMessage:
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
