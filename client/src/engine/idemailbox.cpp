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
#include "idemailbox.h"

#include "boost/bind.hpp"
#include "conversions.h"
#include "eventssource.h"
#include "omlcontext.h"
#include "iportaldatabase.h"
#include "datalocalmessage.h"
#include "idesession.h"
#include "ideskin.h"
#include "objectsmessage.h"
#include "objectsuser.h"
#include "portalsmessenger.h"
#include "portalsportal.h"
#include "htmlevent.h"
#include "htmlattributes.h"
#include "css.h"
#include "htmlcontrols.h"
#include "htmldiv.h"
#include "htmlimage.h"
#include "htmllink.h"
#include "htmlliteral.h"
#include "htmltable.h"
#include "htmltablecell.h"
#include "htmltableheader.h"
#include "htmltablerow.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String IdeMailBox::PARAM_SECURE = _S("secure");
const String IdeMailBox::EVENT_ONDELETEMESSAGE = _S("onDeleteMessage");
const String IdeMailBox::EVENT_ONRESTOREMESSAGE = _S("onRestoreMessage");
const String IdeMailBox::EVENT_ONREADALLMESSAGES = _S("onReadAllMessages");
const String IdeMailBox::EVENT_ONDELETEALLMESSAGES = _S("onDeleteAllMessages");
const String IdeMailBox::EVENT_ONCHANGEOFFSET = _S("onChangeOffset");

//////////////////////////////////////////////////////////////////////

IdeMailBox::IdeMailBox(MessageFolder folder)
{
	_construct(folder, ObjectID::EMPTY);
}

IdeMailBox::IdeMailBox(const ObjectID &message)
{
	_construct(messageFolderInbox, message);
}

IdeMailBox::~IdeMailBox()
{

}

void IdeMailBox::_construct(MessageFolder folder, const ObjectID &message)
{
	m_folder = folder;
	m_message = message;
	m_offset = 0;
	m_secure = true;

	getEvents()->get(EVENT_ONDELETEMESSAGE)->connect(boost::bind(&IdeMailBox::onDeleteMessage, this, _1));
	getEvents()->get(EVENT_ONRESTOREMESSAGE)->connect(boost::bind(&IdeMailBox::onRestoreMessage, this, _1));
	getEvents()->get(EVENT_ONREADALLMESSAGES)->connect(boost::bind(&IdeMailBox::onReadAllMessages, this, _1));
	getEvents()->get(EVENT_ONDELETEALLMESSAGES)->connect(boost::bind(&IdeMailBox::onDeleteAllMessages, this, _1));
	getEvents()->get(EVENT_ONCHANGEOFFSET)->connect(boost::bind(&IdeMailBox::onChangeOffset, this, _1));
}

void IdeMailBox::renderFolder(MessageFolder folder, shared_ptr<XMLNode> node)
{
	getPortal()->getMessenger()->markNewMessagesAsUnread(getPage()->getDatabase(), getSessionAccount());

	node->setAttributeString(_S("name"), DataLocalMessage::toString(folder));

	// Calcola il numero totale di messaggi nel folder
	uint32 total_messages = getPortal()->getMessenger()->getMessagesCount(getPage()->getDatabase(), getSessionAccount(), folder);
	// Calcola il numero di messaggi da mostrare
	uint32 messages_to_show = getSkin()->getPagerItems();
	// Crea il pager dei messaggi se necessario
	createPager(getSkin(), node, total_messages, messages_to_show, getOffset());

	shared_ptr<XMLNode> node_messages;

	DataLocalMessage::DataLocalMessageList messages;

	RangeUint32 range(m_offset, messages_to_show);
	// Estrae i messaggi nel folder corrente
	if(getPortal()->getMessenger()->getMessages(getPage()->getDatabase(), getSessionAccount(), messages, folder, range))
	{
		for(DataLocalMessage::DataLocalMessageList::const_iterator i = messages.begin(); i != messages.end(); ++i)
		{
			shared_ptr<DataLocalMessage> local_message = *i;
			shared_ptr<ObjectsMessage> message = local_message->decode(getSessionAccount()->getPrivateKey());
			if(message != null)
			{
				if(node_messages == null)
					node_messages = node->addChild(_S("messages"));

				shared_ptr<XMLPortalExporter> messageExporter(OS_NEW XMLPortalExporter(node_messages->addChild(_S("message")), getPage(), XMLPortalExporter::emLite));
				local_message->exportXML(messageExporter, message->subject, String::EMPTY, false);
			}
		}
	}

	shared_ptr<XMLNode> node_actions = node->addChild(_S("actions"));
	switch(folder)
	{
	case messageFolderInbox:		{
										shared_ptr<XMLNode> actionReadAll = node_actions->addChild(_S("action"));
										actionReadAll->setAttributeString(_S("name"), _S("readAllMessages"));
										actionReadAll->setAttributeString(_S("href"), getEventCommand(EVENT_ONREADALLMESSAGES, conversions::to_utf16(folder)));
									}

									break;

	case messageFolderTrash:		{
										shared_ptr<XMLNode> actionDeleteAll = node_actions->addChild(_S("action"));
										actionDeleteAll->setAttributeString(_S("name"), _S("deleteAllMessages"));
										actionDeleteAll->setAttributeString(_S("href"), getEventCommand(EVENT_ONDELETEALLMESSAGES, conversions::to_utf16(folder)));
									}

									break;

	case messageFolderSentItems:
									break;

	default:						OS_ASSERTFALSE();
									break;
	}
}

void IdeMailBox::renderMessage(const ObjectID &id, shared_ptr<XMLNode> node)
{
	shared_ptr<ObjectsMessage> message;

	shared_ptr<DataLocalMessage> local_message = getPortal()->getMessenger()->getMessage(getPage()->getDatabase(), getSessionAccount(), id, true);
	if(local_message != null)
	{
		// Decodifica il messaggio
		message = local_message->decode(getSessionAccount()->getPrivateKey());
	}

	if(message == null)
		return;

	shared_ptr<OMLContext> messageContext = getPage()->parseOmlEx(message->body, false, true, getSecure(), false, omlRenderModeOsiris, String::EMPTY, String::EMPTY);
	if(messageContext == null)
		return;

	shared_ptr<XMLPortalExporter> messageExporter(OS_NEW XMLPortalExporter(node, getPage(), XMLPortalExporter::emFull));
	local_message->exportXML(messageExporter, message->subject, messageContext->getOutput(), messageContext->getSecureCheck());

	shared_ptr<XMLNode> node_actions = node->addChild(_S("actions"));

	shared_ptr<ObjectsUser> author = objects_user_cast(getPage()->getObject(local_message->author));
	if(author != null)
	{
		shared_ptr<XMLNode> action_reply = node_actions->addChild(_S("action"));
		action_reply->setAttributeString(_S("name"), _S("reply"));

		ordered_map<std::wstring, std::wstring> params;
		params.set(OS_URL_PARAM_QUOTE, (_W("re: ") + message->subject).to_wide());
		action_reply->setAttributeString(_S("href"), getPortal()->getSendMessageLink(author->id, params));
	}

	if(local_message->folder == static_cast<uint32>(messageFolderTrash))
	{
		shared_ptr<XMLNode> action_delete = node_actions->addChild(_S("action"));
		action_delete->setAttributeString(_S("name"), _S("restore"));
		action_delete->setAttributeString(_S("href"), getEventCommand(EVENT_ONRESTOREMESSAGE, id.toUTF16()));
	}
	else
	{
		shared_ptr<XMLNode> action_delete = node_actions->addChild(_S("action"));
		action_delete->setAttributeString(_S("name"), _S("delete"));
		action_delete->setAttributeString(_S("href"), getEventCommand(EVENT_ONDELETEMESSAGE, id.toUTF16()));
	}

	if(getSecure())
	{
		bool unsafe = messageContext->getSecureCheck() == false;
		if(unsafe == false)
		{
			shared_ptr<ObjectsUser> messageAuthor = objects_user_cast(getPage()->getObject(local_message->author));
			if(messageAuthor != null)
			{
				shared_ptr<OMLContext> markContext = getPage()->parseOmlEx(messageAuthor->mark, false, true, getSecure(), false, omlRenderModeOsiris, String::EMPTY, String::EMPTY);
				if(markContext != null)
					unsafe = markContext->getSecureCheck() == false;
			}
		}

		if(unsafe)
		{
			shared_ptr<XMLNode> action_delete = node_actions->addChild(_S("action"));
			action_delete->setAttributeString(_S("name"), _S("show"));
			action_delete->setAttributeString(_S("href"), getPortal()->getPrivateMessageLink(id, false));
		}
	}
}

void IdeMailBox::_renderFolder(MessageFolder folder)
{
	m_message.clear();
	m_folder = folder;
}

void IdeMailBox::onDeleteMessage(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == null)
		return;

	ObjectID id = htmlEvent->get(0).to_ascii();
	if(id.empty() == false)
	{
		// Elimina il messaggio
		getPortal()->getMessenger()->moveMessageToTrash(getPage()->getDatabase(), getSessionAccount(), id);
		// Mostra la directory di riferimento
		_renderFolder(m_folder);
	}
}

void IdeMailBox::onRestoreMessage(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == null)
		return;

	ObjectID id = htmlEvent->get(0).to_ascii();
	if(id.empty() == false)
	{
		// Elimina il messaggio
		getPortal()->getMessenger()->moveMessageToInbox(getPage()->getDatabase(), getSessionAccount(), id);
		// Mostra la posta in arrivo
		_renderFolder(messageFolderInbox);
	}
}

void IdeMailBox::onReadAllMessages(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == null)
		return;

	MessageFolder folder = conversions::from_utf16<MessageFolder>(htmlEvent->get(0));
	if(folder != messageFolderNone)
	{
		// Marca tutti i messaggi del folder specificato come letti
		getPortal()->getMessenger()->changeMessagesStatus(getPage()->getDatabase(), getSessionAccount(), folder, messageStatusRead);
		// Effettua il render del folder
		_renderFolder(folder);
	}
}

void IdeMailBox::onDeleteAllMessages(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == null)
		return;

	MessageFolder folder = conversions::from_utf16<MessageFolder>(htmlEvent->get(0));
	if(folder != messageFolderNone)
	{
		switch(folder)
		{
		case messageFolderInbox:
		case messageFolderSentItems:
									// Sposta tutti i messaggi dalla posta in arrivo al cestino
									getPortal()->getMessenger()->moveFolderMessages(getPage()->getDatabase(), getSessionAccount(), messageFolderInbox, messageFolderTrash);
									break;

		case messageFolderTrash:			// Elimina tutti i messaggi nel cestino
									getPortal()->getMessenger()->deleteFolderMessages(getPage()->getDatabase(), getSessionAccount(), messageFolderTrash);
									break;

		default:					OS_ASSERTFALSE();
									break;
		}

		// Effettua il render del folder
		_renderFolder(folder);
	}
}

void IdeMailBox::onChangeOffset(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == null)
		return;

	m_offset = conversions::from_utf16<uint32>(htmlEvent->get(0));
}

String IdeMailBox::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("mailbox.xsl"));
}

void IdeMailBox::onPreRender()
{
	ControlBase::onPreRender();

	getStylesheet()->setBoolParam(PARAM_SECURE, getSecure());

	if(m_message.empty())
	{
		shared_ptr<XMLNode> folder_node = getDocument()->create(_S("folder"));
		switch(m_folder)
		{
		case messageFolderInbox:		renderFolder(messageFolderInbox, folder_node);
										break;

		case messageFolderTrash:		renderFolder(messageFolderTrash, folder_node);
										break;

		case messageFolderSentItems:	renderFolder(messageFolderSentItems, folder_node);
										break;

		default:						OS_ASSERTFALSE();
										break;
		}
	}
	else
	{
		renderMessage(m_message, getDocument()->create(_S("message")));
	}
}

String IdeMailBox::getPageHref(uint32 offset) const
{
	return getEventCommand(EVENT_ONCHANGEOFFSET, conversions::to_utf16(offset));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
