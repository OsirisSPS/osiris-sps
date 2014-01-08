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
#include "idemainaccounts.h"

#include "boost/bind.hpp"
#include "conversions.h"
#include "css.h"
#include "dataaccount.h"
#include "ideaccount.h"
#include "ideaccountsmanager.h"
#include "ideblock.h"
#include "idebutton.h"
#include "idesystem.h"
#include "idesession.h"
#include "ideskin.h"
#include "objectsavatar.h"
#include "objectsuser.h"
#include "options.h"
#include "osiriscommon.h"
#include "portalsaccount.h"
#include "portalsportal.h"
#include "portalsportalssystem.h"
#include "htmlevent.h"
#include "htmldiv.h"
#include "htmlfilebrowser.h"
#include "htmlimage.h"
#include "htmllink.h"
#include "htmlliteral.h"
#include "htmlspace.h"
#include "htmltable.h"
#include "htmltablecell.h"
#include "htmltableheader.h"
#include "htmltablerow.h"
#include "htmltext.h"
#include "htmlunit.h"
#include "htmlwriter.h"
#include "mimemanager.h"
#include "httprequest.h"
#include "httpsession.h"
#include "xmlschema.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace main {

//////////////////////////////////////////////////////////////////////

Accounts::Accounts(shared_ptr<HttpSession> session) : PageBase(session),
													  m_accountBrowser(OS_NEW HtmlFileBrowser())
{
	m_accountBrowser->setID(_S("accountBrowser"));
	m_accountBrowser->setCss(_S("os_input_full"));
}

Accounts::~Accounts()
{

}

Accounts::Action Accounts::getAction() const
{
	uint32 action = conversions::from_wstring<uint32>(getRequest()->getUrlParam(OS_URL_PARAM_ACTION));
	switch(action)
	{
	case aaViewAccounts:
	case aaAccountRedirect:
	case aaAccountImport:
	case aaAccountExport:
	case aaAccountRemove:
							return static_cast<Action>(action);

	default:				OS_ASSERTFALSE();
							break;
	}

	return aaViewAccounts;
}

shared_ptr<IdeAccount> Accounts::getAccount() const
{
	String id = getRequest()->getUrlParam(OS_URL_PARAM_ID);
	if(id.empty())
		return nullptr;

	return IdeAccountsManager::instance()->getByID(id);	
}

std::string Accounts::getAccountLoginLink(const String &id) const
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_URL_PARAM_ID, id.to_wide());

	//return PortalsSystem::instance()->getLoginLink(portal, params);
	return PortalsSystem::instance()->getMainLink("login", params);
}

std::string Accounts::getAccountRedirectLink(const String &id) const
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_URL_PARAM_ACTION, conversions::to_wstring(static_cast<uint32>(aaAccountRedirect)));
	params.set(OS_URL_PARAM_ID, id.to_wide());

	//return PortalsSystem::instance()->getAccountsLink(portal, params);
	return PortalsSystem::instance()->getMainLink("accounts2", params);
}

std::string Accounts::getAccountImportLink() const
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_URL_PARAM_ACTION, conversions::to_wstring(static_cast<uint32>(aaAccountImport)));

	//return PortalsSystem::instance()->getAccountsLink(portal, params);
	return PortalsSystem::instance()->getMainLink("accounts2", params);
}

std::string Accounts::getAccountExportLink(const String &id) const
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_URL_PARAM_ACTION, conversions::to_wstring(static_cast<uint32>(aaAccountExport)));
	params.set(OS_URL_PARAM_ID, id.to_wide());

	//return PortalsSystem::instance()->getAccountsLink(portal, params);
	return PortalsSystem::instance()->getMainLink("accounts2", params);
}

std::string Accounts::getAccountRemoveLink(const String &id) const
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_URL_PARAM_ACTION, conversions::to_wstring(static_cast<uint32>(aaAccountRemove)));
	params.set(OS_URL_PARAM_ID, id.to_wide());

	//return PortalsSystem::instance()->getAccountsLink(portal, params);
	return PortalsSystem::instance()->getMainLink("accounts2", params);
}

void Accounts::onViewAccounts()
{
	bool hasAccounts = IdeAccountsManager::instance()->hasAccounts();

	shared_ptr<IdeBlock> block(OS_NEW IdeBlock(getText(_S("main.pages.accounts.title"))));
	//block->setSpecialPage(true);
	getArea(pageAreaContent)->getControls()->add(block);

	shared_ptr<HtmlTable> table(OS_NEW HtmlTable());
	table->setCss(_S("os_table_data"));
	block->getBody()->getControls()->add(table);

	if(hasAccounts)
	{
		//map<String, shared_ptr<ObjectsUser>>::type accounts_users;

		const IdeAccountsManager::AccountsList &accounts = IdeAccountsManager::instance()->getAccounts();
		IdeAccountsManager::AccountsList::const_iterator i;
		// Effettua una prima passata per determinare se almeno un utente ha un avatar (altrimenti la colonna relativa non serve...)
		/*
		for(i = accounts.begin(); i != accounts.end(); ++i)
		{
			// Se disponibile carica l'utente associato all'account
			shared_ptr<ObjectsUser> user = i->second->loadUser(getDatabase());
			if(user != nullptr)
				// Salva l'associazione account - utente
				accounts_users[i->second->getID()] = user;
		}
		*/

		shared_ptr<HtmlTableRow> rowHeader = table->addRow();
		shared_ptr<HtmlTableHeader> header;

		/*
		header = rowHeader->addHeader(getText(_S("main.pages.accounts.avatar")));
		header->setCss(_S("os_cell_center"));
		header->setWidth(HtmlUnit::percent(1));
		*/

		header = rowHeader->addHeader(getText(_S("main.pages.accounts.name")));
		header->setCss(_S("os_cell_left"));
		header->setWidth(HtmlUnit::percent(55));

		header = rowHeader->addHeader(getText(_S("main.pages.accounts.lastAccess")));
		header->setCss(_S("os_cell_center"));
		header->setWidth(HtmlUnit::percent(15));

		header = rowHeader->addHeader(getText(_S("main.pages.accounts.connected")));
		header->setCss(_S("os_cell_center"));
		header->setWidth(HtmlUnit::percent(15));

		header = rowHeader->addHeader(getText(_S("main.pages.accounts.actions")));
		header->setCss(_S("os_cell_center"));
		header->setWidth(HtmlUnit::percent(15));

		for(i = accounts.begin(); i != accounts.end(); ++i)
		{
			shared_ptr<IdeAccount> account = *i;
			//shared_ptr<ObjectsUser> user;

			String id = account->getAccount()->id;

			/*
			map<String, shared_ptr<ObjectsUser>>::type::const_iterator p = accounts_users.find(account->getID());
			if(p != accounts_users.end())
				user = p->second;
			*/

			String accountLink;

			bool hasAutologin = account->getAccount()->hasAutologin();
			if(hasAutologin)
				accountLink = getAccountRedirectLink(id);
			else
				accountLink = getAccountLoginLink(id);

			shared_ptr<HtmlTableRow> row = table->addRow();

			/*
			shared_ptr<HtmlTableCell> cellAvatar = row->addCell();
			cellAvatar->setCss(_S("os_cell_center"));
			shared_ptr<HtmlImage> user_image(OS_NEW HtmlImage());

			if(hasAutologin)
			{
				shared_ptr<ObjectsAvatar> avatar;
				if(user != nullptr)
					avatar = user->getAvatar(getDatabase());

				if(avatar != nullptr)
					user_image->setUrl(portal->getAvatarLink(avatar->id, avatar->submit_date));
				else
					user_image->setUrl(getSkin()->getImageUrl(_S("user_known.png")));
			}
			else
			{
				user_image->setUrl(getSkin()->getImageUrl(_S("user_unknown.png")));
			}

			cellAvatar->getControls()->add(user_image);
			*/

			shared_ptr<HtmlTableCell> cellName = row->addCell();
			cellName->setCss(_S("os_cell_left"));
			cellName->getControls()->add(shared_ptr<HtmlLink>(OS_NEW HtmlLink(account->getName(), accountLink)));

			shared_ptr<HtmlTableCell> cellLastAccess = row->addCell();
			cellLastAccess->setCss(_S("os_cell_center"));
			cellLastAccess->getControls()->add(shared_ptr<HtmlLiteral>(OS_NEW HtmlLiteral(formatDate(account->getLastAccessDate(),DateTime::dpUserFriendly))));

			shared_ptr<HtmlTableCell> cellOnline = row->addCell();
			cellOnline->setCss(_S("os_cell_center"));
			cellOnline->getControls()->add(shared_ptr<HtmlLiteral>(OS_NEW HtmlLiteral(IdeSystem::instance()->isAccountLogged(account->getID()) ? _S("si") : _S("no"))));

			shared_ptr<HtmlTableCell> cellActions = row->addCell();
			cellActions->setCss(_S("os_cell_center"));
			cellActions->getControls()->add(shared_ptr<HtmlLink>(OS_NEW HtmlLink(getText(_S("main.pages.accounts.actions.enter")), accountLink)));
			cellActions->getControls()->add(shared_ptr<IHtmlControl>(OS_NEW HtmlSpace()));
			cellActions->getControls()->add(shared_ptr<HtmlLink>(OS_NEW HtmlLink(getText(_S("main.pages.accounts.actions.remove")), getAccountRemoveLink(id))));
			cellActions->getControls()->add(shared_ptr<IHtmlControl>(OS_NEW HtmlSpace()));
			cellActions->getControls()->add(shared_ptr<HtmlLink>(OS_NEW HtmlLink(getText(_S("main.pages.accounts.actions.export")), getAccountExportLink(id))));
		}
	}

	shared_ptr<HtmlTableCell> cellCommands = table->addRow()->addCell();
	cellCommands->setCss(OS_CSS_FORM_COMMANDS);
	cellCommands->setColSpan(5);

	shared_ptr<HtmlLink> link_register(OS_NEW HtmlLink(getText(_S("main.pages.accounts.actions.create"))));
	link_register->setCss(_S("os_button"));
	link_register->setID(_S("register"));

	link_register->setHref(PortalsSystem::instance()->getMainLink("register"));
	cellCommands->getControls()->add(link_register);

	shared_ptr<HtmlLink> link_import(OS_NEW HtmlLink(getText(_S("main.pages.accounts.actions.import"))));
	link_import->setCss(_S("os_button"));
	link_import->setID(_S("import"));
	link_import->setHref(getAccountImportLink());
	cellCommands->getControls()->add(link_import);
}

void Accounts::onAccountRedirect()
{
	shared_ptr<IdeAccount> account = getAccount();
	if(account == nullptr || account->getAccount()->hasAutologin() == false)
	{
		onViewAccounts();
		return;
	}

	if(loginAccountWithID(account->getID(), account->getRealPassword()))
		redirect(PortalsSystem::instance()->getMainLink("home"));
	else
		showError(getText(_S("main.pages.accounts.errors.cannot_login")));	
}

void Accounts::onAccountImport()
{
	shared_ptr<IdeBlock> block(OS_NEW IdeBlock(getText(_S("main.pages.accounts.import.title"))));
	getArea(pageAreaContent)->getControls()->add(block);

	shared_ptr<HtmlTable> table(OS_NEW HtmlTable());
	table->setCss(_S("os_table_data"));
	block->getBody()->getControls()->add(table);

	shared_ptr<HtmlTableRow> rowFile = table->addRow();

	shared_ptr<HtmlTableCell> left_cell = rowFile->addCell();
	left_cell->setCss(OS_CSS_FORM_LEFT);
	left_cell->getControls()->add(shared_ptr<HtmlLiteral>(OS_NEW HtmlLiteral(getText(_S("main.pages.accounts.actions.import.select")))));

	shared_ptr<HtmlTableCell> right_cell = rowFile->addCell();
	right_cell->setCss(OS_CSS_FORM_RIGHT);
	right_cell->getControls()->add(m_accountBrowser);

	// Comandi

	shared_ptr<HtmlTableCell> cell_commands = table->addRow()->addCell();
	cell_commands->setCss(OS_CSS_FORM_COMMANDS);
	cell_commands->setColSpan(2);

	shared_ptr<IdeButton> cmdImport(OS_NEW IdeButton(getText(_S("main.pages.accounts.actions.import.import"))));
	cmdImport->setID(_S("importAccount"));
	cmdImport->setIsDefault(true);
	cmdImport->getEventClick()->connect(boost::bind(&Accounts::_importAccount, this));

	cell_commands->getControls()->add(cmdImport);
}

void Accounts::onAccountExport()
{
	shared_ptr<IdeAccount> account = getAccount();
	if(account != nullptr)
		m_document = account->getAccount()->exportXML();
}

void Accounts::onAccountRemove()
{
	shared_ptr<IdeAccount> account = getAccount();
	if(account != nullptr)
	{
		IdeAccountsManager::instance()->remove(account->getID());
		//portal->removeAccount(getDatabase(), account->getID());
	}

	onViewAccounts();
}

void Accounts::_importAccount()
{
	/*
	shared_ptr<Portal> portal = getPortalFromUrl();
	if(portal == nullptr)
		return;
	*/

	const Buffer *accountBuffer = m_accountBrowser->getFileBuffer();
	if(accountBuffer == nullptr)
		return;

	shared_ptr<XMLSchema> schema(OS_NEW XMLSchema());
	schema->parseFile(utils::makeFilePath(utils::makeFolderPath(Options::instance()->getSharePath(),OS_SCHEMAS_PATH), OS_SCHEMA_ACCOUNT));

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument(schema));
	if(document->parseBuffer(*accountBuffer) == false)
	{
		showError(getText(_S("main.pages.accounts.actions.import.error.invalidFile")));
		return;
	}

	shared_ptr<DataAccount> dataAccount(OS_NEW DataAccount());
	if(dataAccount->importXML(document) == false)
	{
		showError(getText(_S("main.pages.accounts.actions.import.error.invalidAccount")));
		return;
	}

	shared_ptr<IdeAccount> account = IdeAccountsManager::instance()->createAccount(dataAccount);
	if(account == nullptr)
	{
		showError(getText(_S("main.pages.accounts.actions.import.error.cannotImport")));
		return;
	}

	redirect(PortalsSystem::instance()->getMainLink("/main/accounts"));
}

String Accounts::getPageName() const
{
	return _S("main.pages.accounts");
}

void Accounts::onLoad()
{
	PageBase::onLoad();

	/*
	shared_ptr<Portal> portal = getPortalFromUrl();
	if(portal == nullptr)
	{
		showError(_S("Invalid portal"));
		return;
	}
	*/

	switch(getAction())
	{
	case aaViewAccounts:		onViewAccounts();
								break;

	case aaAccountRedirect:		onAccountRedirect();
								break;

	case aaAccountImport:		onAccountImport();
								break;

	case aaAccountExport:		onAccountExport();
								break;

	case aaAccountRemove:		onAccountRemove();
								break;

	default:					OS_ASSERTFALSE();
								break;
	}
}

bool Accounts::onTransmit(HtmlWriter &writer)
{
	if(m_document != nullptr)
	{
		Buffer buffer;
		if(m_document->writeBuffer(buffer) == false)
			return false;

		String mime_type;
		MimeManager::instance()->getMimeType(_S("xml"), mime_type);

		getSession()->transmitBuffer(buffer, _S("account.xml"), mime_type);
		return true;
	}

	return PageBase::onTransmit(writer);
}

//////////////////////////////////////////////////////////////////////

} // main
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
