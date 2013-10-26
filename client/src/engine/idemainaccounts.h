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

#ifndef _ENGINE_IDEMAINACCOUNTS_H
#define _ENGINE_IDEMAINACCOUNTS_H

#include "iidemainpage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlFileBrowser;
class IdeAccount;
class XMLDocument;

//////////////////////////////////////////////////////////////////////

namespace ide {
namespace main {

//////////////////////////////////////////////////////////////////////

class EngineExport Accounts : public IMainPage
{
	typedef IMainPage PageBase;

// Enumerated types
public:
	enum Action
	{
		aaViewAccounts		= 0,
		aaAccountRedirect	= 1,
		aaAccountImport		= 2,
		aaAccountExport		= 3,
		aaAccountRemove		= 4,
	};

// Construction
public:
	Accounts(shared_ptr<HttpSession> session);
	virtual ~Accounts();

// Attributes
public:
	Action getAction() const;
	shared_ptr<IdeAccount> getAccount() const;

	std::string getAccountLoginLink(const String &id) const;
	std::string getAccountRedirectLink(const String &id) const;
	std::string getAccountImportLink() const;
	std::string getAccountExportLink(const String &id) const;
	std::string getAccountRemoveLink(const String &id) const;

// Operations
private:
	void onViewAccounts();
	void onAccountRedirect();
	void onAccountImport();
	void onAccountExport();
	void onAccountRemove();

	void _importAccount();

// IPage interface
public:
	virtual String getPageName() const;

// IMainPage overrides
protected:
	virtual void onLoad();
	virtual bool onTransmit(HtmlWriter &writer);

private:
	shared_ptr<HtmlFileBrowser> m_accountBrowser;
	shared_ptr<XMLDocument> m_document;		// Documento xml da restituire
};

//////////////////////////////////////////////////////////////////////

} // main
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_IDEMAINACCOUNTS_H
