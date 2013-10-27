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
#include "ideportalusers.h"

#include "boost/bind.hpp"
#include "constants.h"
#include "conversions.h"
#include "convert.h"
#include "iportaldatabase.h"
#include "dbsqlselect.h"
#include "dbtables.h"
#include "objectsdescriptor.h"
#include "objectssystem.h"
#include "objectsuser.h"
#include "entitiessnapshotmanager.h"
#include "idebutton.h"
#include "idesession.h"
#include "ideskin.h"
#include "buffer.h"
#include "portalsaccount.h"
#include "portalsportal.h"
#include "portalsportalssystem.h"
#include "portalsprofile.h"
#include "htmlevent.h"
#include "css.h"
#include "htmlcombobox.h"
#include "htmldiv.h"
#include "htmlspan.h"
#include "htmltext.h"
#include "htmltextbox.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace portal {

//////////////////////////////////////////////////////////////////////

const String Users::EVENT_ONCHANGEOFFSET = _S("onChangeOffset");
const String Users::EVENT_ONFILTERALPHABETIC = _S("onFilterAlphabetic");

//////////////////////////////////////////////////////////////////////

Users::Users(shared_ptr<HttpSession> session) : PageBase(session),
												m_nickMode(OS_NEW HtmlComboBox()),
												m_nickName(OS_NEW HtmlTextBox()),
												m_orderMode(OS_NEW HtmlComboBox()),
												m_orderDirection(OS_NEW HtmlComboBox()),
												m_templateType(OS_NEW HtmlComboBox()),
												m_pagerSize(OS_NEW HtmlComboBox()),
												m_offset(0)
{
	getEvents()->get(EVENT_ONCHANGEOFFSET)->connect(boost::bind(&Users::onChangeOffset, this, _1));
	getEvents()->get(EVENT_ONFILTERALPHABETIC)->connect(boost::bind(&Users::onFilterAlphabetic, this, _1));
}

Users::~Users()
{

}

String Users::getPageName() const
{
	return _S("portal.pages.users");
}

String Users::getPageHref(uint32 offset) const
{
	return getEventCommand(EVENT_ONCHANGEOFFSET, conversions::to_utf16(offset));
}

void Users::onLoad()
{
	PageBase::onLoad();

	//getPage()->addJavascript(getLoggedUser()->getCurrentSkin()->getResourceUrl(_S("js/users.js")));

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	shared_ptr<XMLNode> nodeRoot = document->create(OS_PORTAL_OBJECT_USER_GROUPNAME);

	String path = getSkin()->getTemplatePath(_S("users.xsl"));
	m_pageTemplate.reset(OS_NEW HtmlXSLControl(loadStylesheet(path), document));

	// ----------- Options --------------
	//shared_ptr<HtmlDiv> options(OS_NEW HtmlDiv());
	m_nickMode->setID(_S("nickMode"));
	m_nickMode->addOption(getText(_S("portal.pages.users.search.starts_with")), conversions::to_utf16<uint32>(nmStartsWith));
	m_nickMode->addOption(getText(_S("portal.pages.users.search.exact")), conversions::to_utf16<uint32>(nmExact));
	m_nickMode->addOption(getText(_S("portal.pages.users.search.contains")), conversions::to_utf16<uint32>(nmContains));
	m_nickMode->setValue(conversions::to_utf16<uint32>(nmContains));
	//options->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("portal.pages.users.search.nick_mode")))));
	//options->getControls()->add(m_nickMode);

	m_nickName->setID(_S("nickName"));
	//options->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("portal.pages.users.search.nick_name")))));
	//options->getControls()->add(m_nickName);

	m_orderMode->setID(_S("orderMode"));
	m_orderMode->addOption(getText(_S("portal.pages.users.order.reputation_and_name")), conversions::to_utf16<uint32>(omReputationAndName));
	m_orderMode->addOption(getText(_S("portal.pages.users.order.join_date")), conversions::to_utf16<uint32>(omJoinDate));
	m_orderMode->addOption(getText(_S("portal.pages.users.order.name")), conversions::to_utf16<uint32>(omName));
	m_orderMode->addOption(getText(_S("portal.pages.users.order.objects")), conversions::to_utf16<uint32>(omObjects));
	m_orderMode->addOption(getText(_S("portal.pages.users.order.revisions")), conversions::to_utf16<uint32>(omRevisions));
	m_orderMode->setValue(conversions::to_utf16<uint32>(omReputationAndName));

	m_orderDirection->setID(_S("orderDirection"));
	m_orderDirection->addOption(getText("search.order_direction.descendent"),conversions::to_utf16<uint32>(searchOrderDirectionDescendent));
	m_orderDirection->addOption(getText("search.order_direction.ascendent"),conversions::to_utf16<uint32>(searchOrderDirectionAscendent));
	m_orderDirection->setValue(conversions::to_utf16<uint32>(searchOrderDirectionDescendent));

	m_templateType->setID(_S("template"));
	m_templateType->addOption(getText(_S("portal.pages.users.template.box")), conversions::to_utf16<uint32>(utBox));
	m_templateType->addOption(getText(_S("portal.pages.users.template.table")), conversions::to_utf16<uint32>(utTable));
	m_templateType->setValue(conversions::to_utf16<uint32>(utBox));

	m_pagerSize->setID(_S("pagerSize"));
	m_pagerSize->addOption(getText(_S("portal.pages.users.pager_size.default")), conversions::to_utf16<uint32>(0));
	m_pagerSize->addOption(getText(_S("portal.pages.users.pager_size.50")), conversions::to_utf16<uint32>(50));
	m_pagerSize->addOption(getText(_S("portal.pages.users.pager_size.200")), conversions::to_utf16<uint32>(200));
	m_pagerSize->addOption(getText(_S("portal.pages.users.pager_size.1000")), conversions::to_utf16<uint32>(1000));
	//m_pagerSize->addOption(getText(_S("portal.pages.users.pager_size.all")), conversions::to_utf16<int32>(-1));
	m_pagerSize->setValue(conversions::to_utf16<uint32>(0));

	//options->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText(_S("portal.pages.users.search.order_mode")))));
	//options->getControls()->add(m_orderMode);

	m_submit.reset(OS_NEW IdeButton(getText(_S("portal.pages.users.search"))));
	m_submit->setID(_S("submit"));
	m_submit->setIsDefault(true);
	m_submit->getEventClick()->connect(boost::bind(&Users::onSearch, this));
	//options->getControls()->add(submit);

	//m_templateType->addChildParam(options, _S("options"));

	if(getPostBack() == false)
	{
		// Defaults
		//m_orderDirection->setValue(conversions::to_utf16<uint32>(search::odDescendent));
	}

	m_pageTemplate->addChildParam(m_nickMode, _S("nickMode"));
	m_pageTemplate->addChildParam(m_nickName, _S("nickName"));
	m_pageTemplate->addChildParam(m_orderMode, _S("orderMode"));
	m_pageTemplate->addChildParam(m_orderDirection, _S("orderDirection"));
	m_pageTemplate->addChildParam(m_templateType, _S("template"));
	m_pageTemplate->addChildParam(m_pagerSize, _S("pagerSize"));
	m_pageTemplate->addChildParam(m_submit, _S("submit"));

	// Carica il template di riferimento
	getArea(pageAreaContent)->getControls()->add(m_pageTemplate);
}

void Users::onPreRender()
{
	PageBase::onPreRender();

	Template templateName = static_cast<Template>(conversions::from_utf16<uint32>(m_templateType->getValue()));
	int32 pagerSize = static_cast<int32>(conversions::from_utf16<uint32>(m_pagerSize->getValue()));

	shared_ptr<XMLNode> nodeRoot = m_pageTemplate->getDocument()->getRoot();
	nodeRoot->setAttributeString(_S("template"), (templateName == utTable) ? _S("table") : _S("box"));

	String hrefFilterAlphabetic = getEventCommand(EVENT_ONFILTERALPHABETIC, _S("Letter"));
	nodeRoot->setAttributeString(_S("href_filter_alphabetic"), hrefFilterAlphabetic);

	String currentAlphabetic = m_nickName->getValue();
	if(currentAlphabetic.length() != 1)
		currentAlphabetic = _S("*");
	else
		currentAlphabetic = currentAlphabetic.to_upper();

	nodeRoot->setAttributeString(_S("current_filter_alphabetic"), currentAlphabetic);

	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect());
	// Select sulla snapshot_users e users
	select->tables.add(DbSqlTable(DBTABLES::SNAPSHOT_USERS_TABLE));
	select->tables.add(DbSqlTable(DBTABLES::USERS_TABLE));
	// Join tra le due tabelle
	select->where.add(DbSqlField(DBTABLES::SNAPSHOT_USERS::ID, DBTABLES::SNAPSHOT_USERS_TABLE), DbSqlField(DBTABLES::USERS::ID, DBTABLES::USERS_TABLE));
	// VERYURGENT, selezione in base alla soglia

	String nickName = m_nickName->getValue();
	if(nickName.empty() == false)
	{
		NickMode nickMode = static_cast<NickMode>(conversions::from_utf16<uint32>(m_nickMode->getValue()));
		switch(nickMode)
		{
		case nmStartsWith:		select->where.add(DbSqlField(DBTABLES::USERS::NAME), Convert::toSQL(nickName + _S("%")), DbSqlCondition::cfLike | DbSqlCondition::cfAnd);
								break;

		case nmExact:			select->where.add(DbSqlField(DBTABLES::USERS::NAME), Convert::toSQL(nickName), DbSqlCondition::cfEqual | DbSqlCondition::cfAnd);
								break;

		case nmContains:		select->where.add(DbSqlField(DBTABLES::USERS::NAME), Convert::toSQL(_S("%") + nickName + _S("%")), DbSqlCondition::cfLike | DbSqlCondition::cfAnd);
								break;

		default:				OS_ASSERTFALSE();
								break;
		}
	}

	OrderMode orderMode = static_cast<OrderMode>(conversions::from_utf16<uint32>(m_orderMode->getValue()));
	SearchOrderDirection orderDirection = static_cast<SearchOrderDirection>(conversions::from_utf16<uint32>(m_orderDirection->getValue()));
	switch(orderMode)
	{
	case omJoinDate:			select->orderBy.add(DbSqlField(DBTABLES::USERS::JOIN_DATE, DBTABLES::USERS_TABLE), (orderDirection == searchOrderDirectionAscendent) ? DbSqlField::foAsc : DbSqlField::foDesc);
								break;

	case omReputationAndName:	select->orderBy.add(DbSqlField(DBTABLES::SNAPSHOT_USERS::SCORE, DBTABLES::SNAPSHOT_USERS_TABLE), (orderDirection == searchOrderDirectionAscendent) ? DbSqlField::foAsc : DbSqlField::foDesc);
								select->orderBy.add(DbSqlField(DBTABLES::USERS::NAME, DBTABLES::USERS_TABLE), DbSqlField::foAsc);
								break;

	case omName:				select->orderBy.add(DbSqlField(DBTABLES::USERS::NAME, DBTABLES::USERS_TABLE), (orderDirection == searchOrderDirectionAscendent) ? DbSqlField::foAsc : DbSqlField::foDesc);
								break;

	case omObjects:				select->tables.add(DbSqlTable(DBTABLES::USERS_STATS_TABLE));
								// Crea la join con la tabella delle statistiche degli utenti
								select->where.add(DbSqlField(DBTABLES::SNAPSHOT_USERS::ID, DBTABLES::SNAPSHOT_USERS_TABLE), DbSqlField(DBTABLES::USERS_STATS::REFERENCE, DBTABLES::USERS_STATS_TABLE));
								// Aggiunge l'ordinamento per numero di oggetti
								select->orderBy.add(DbSqlField(DBTABLES::USERS_STATS::OBJECTS, DBTABLES::USERS_STATS_TABLE), (orderDirection == searchOrderDirectionAscendent) ? DbSqlField::foAsc : DbSqlField::foDesc);

								break;

	case omRevisions:			select->tables.add(DbSqlTable(DBTABLES::USERS_STATS_TABLE));
								// Crea la join con la tabella delle statistiche degli utenti
								select->where.add(DbSqlField(DBTABLES::SNAPSHOT_USERS::ID, DBTABLES::SNAPSHOT_USERS_TABLE), DbSqlField(DBTABLES::USERS_STATS::REFERENCE, DBTABLES::USERS_STATS_TABLE));
								// Aggiunge l'ordinamento per numero di modifiche
								select->orderBy.add(DbSqlField(DBTABLES::USERS_STATS::REVISIONS, DBTABLES::USERS_STATS_TABLE), (orderDirection == searchOrderDirectionAscendent) ? DbSqlField::foAsc : DbSqlField::foDesc);

								break;

	default:					OS_ASSERTFALSE();
								break;
	}

	select->count = true;

	DataTable result;
	// Calcola il numero di utenti
	if(getDatabase()->execute(select, result))
	{
		if(result.rows() > 0)
		{
			shared_ptr<XMLNode> nodeRoot = m_pageTemplate->getDocument()->getRoot();

			// Estrae il numero totale di utente
			uint32 total_users = *result[0][0];
			// Calcola il numero di utenti da visualizzare
			uint32 users_to_show = pagerSize;
			if(users_to_show == constants::nsize)
				users_to_show = total_users;
			else if(users_to_show == 0)
				users_to_show = getSkin()->getPagerItems();

			// Crea il pager degli utenti se necessario
			createPager(getSkin(), nodeRoot, total_users, users_to_show, getOffset());

			select->count = false;
			// Seleziona l'id dell'utente
			select->fields.add(DbSqlField(DBTABLES::ID, DBTABLES::USERS_TABLE));
			// Imposta l'offset
			select->limit.setRange(getOffset(), users_to_show);

			result.clear();
			// Estra gli utenti nell'offset corrente
			if(getDatabase()->execute(select, result))
			{
				for(uint32 r = 0; r < result.rows(); r++)
				{
					ObjectID id = static_cast<String>(*result.get(r, DBTABLES::ID)).to_ascii();

					shared_ptr<ObjectsUser> user = objects_user_cast(getObject(id));
					if(user)
					{
						shared_ptr<XMLPortalExporter> userExporter(OS_NEW XMLPortalExporter(nodeRoot->addChild(OS_PORTAL_OBJECT_USER_TYPENAME), get_this_ptr<IPortalPage>(), XMLPortalExporter::emFull));
						user->exportXML(userExporter);
					}
				}
			}
		}
	}
}

void Users::onChangeOffset(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == null)
		return;

	m_offset = conversions::from_utf16<uint32>(htmlEvent->get(0));
}

void Users::onFilterAlphabetic(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == null)
		return;

	

	String letter = htmlEvent->get(0);
	if(letter == _S("*"))
		m_nickName->setValue(String::EMPTY);
	else
	{
		m_nickMode->setValue(conversions::to_utf16<uint32>(nmStartsWith));
		m_nickName->setValue(htmlEvent->get(0));
	}
}

void Users::onSearch()
{

}

//////////////////////////////////////////////////////////////////////

} // portal
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
