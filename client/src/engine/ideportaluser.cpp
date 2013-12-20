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
#include "ideportaluser.h"

#include "boost/bind.hpp"
#include "convert.h"
#include "conversions.h"
#include "entitiessnapshotmanager.h"
#include "objectsdescriptor.h"
#include "objectssystem.h"
#include "objectsreputation.h"
#include "objectsuser.h"
#include "idebutton.h"
#include "idepages.h"
#include "idepathway.h"
#include "idesession.h"
#include "ideskin.h"
#include "idetablequery.h"
#include "ideutils.h"
#include "buffer.h"
#include "portalsaccount.h"
#include "portalsportal.h"
#include "portalsprofile.h"
#include "reputationsmanager.h"
#include "reputationsfeedback.h"
#include "htmlevent.h"
#include "css.h"
#include "httprequest.h"
#include "httpparser.h"
#include "htmlbutton.h"
#include "htmlcheckbox.h"
#include "htmlcombobox.h"
#include "htmldiv.h"
#include "htmlfilebrowser.h"
#include "htmllink.h"
#include "htmlpanel.h"
#include "htmlspan.h"
#include "htmltable.h"
#include "htmltext.h"
#include "htmltextbox.h"
#include "htmlxslcontrol.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace portal {

//////////////////////////////////////////////////////////////////////

const std::wstring User::ID(L"id");
const std::wstring User::ACTION(L"act");

//////////////////////////////////////////////////////////////////////

User::User(shared_ptr<HttpSession> session) : PageBase(session)
{

}

User::~User()
{

}

void User::onPathway2() const
{
	if(m_user == nullptr)
		return;

	// Users
	getPathway()->add(getText(_S("portal.pages.users.title")), getPortal()->getLink("users"));		

	// This user
	getPathway()->add(m_user->name, getPagePathway());		
}

String User::getUrlID() const
{
	return getRequest()->getUrlParam(ID);
}

User::Action User::getUrlAction() const
{
	return static_cast<User::Action>(conversions::from_utf16<uint32>(getRequest()->getUrlParam(ACTION)));
}

String User::getUrl(const String &id, Action action) const
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(ID, id.to_wide());
	params.set(ACTION, conversions::to_wstring(static_cast<int32>(action)));

	return getPortal()->getLink("user", params);
}

void User::createReputationsFeedback(const shared_ptr<XMLNode> &node, const shared_ptr<ReputationsFeedback> &feedback)
{
	shared_ptr<XMLNode> score = node->addChild(_S("score"));
	score->setAttributeString(_S("value"), feedback->getScore().getStringValue());
	score->setAttributeString(_S("comment"), feedback->getScore().getDescription());
	score->setAttributeString(_S("type"), feedback->getScore().getDirect() ? _S("direct") : _S("indirect"));

	shared_ptr<XMLPortalExporter> fromExporter(OS_NEW XMLPortalExporter(node->addChild(_S("from")), get_this_ptr<IPortalPage>(), XMLPortalExporter::emLite));
	shared_ptr<ObjectsUser> userFrom = objects_user_cast(getObject(feedback->getHashFrom().to_ascii()));
	if(userFrom != nullptr)
		userFrom->exportXML(fromExporter);

	shared_ptr<XMLPortalExporter> toExporter(OS_NEW XMLPortalExporter(node->addChild(_S("to")), get_this_ptr<IPortalPage>(), XMLPortalExporter::emLite));
	shared_ptr<ObjectsUser> userTo = objects_user_cast(getObject(feedback->getHashTo().to_ascii()));
	if(userTo != nullptr)
		userTo->exportXML(toExporter);

	for(ReputationsFeedback::const_iterator i = feedback->first_child(); i != feedback->last_child(); ++i)
	{
		shared_ptr<ReputationsFeedback> sub_feedback = *i;

		shared_ptr<XMLNode> sub_reputation = node->addChild(_S("reputation"));

		createReputationsFeedback(sub_reputation,sub_feedback);
	}
}

String User::getPageName() const
{
	return _S("portal.pages.user");
}

void User::onInit()
{
	PageBase::onInit();

	// Ottiene l'id dell'utente richiesto
	ObjectID id = getUrlID().to_ascii();

	if(id.empty() == false)
		m_user = objects_user_cast(getObject(id));

	if(m_user == nullptr)
	{
		showError(_S("Utente non valido"));
		return;
	}
}

void User::onLoad()
{
	PageBase::onLoad();

	if(m_user == nullptr)
		return;

	ObjectID id = getUrlID().to_ascii();
	
	// Titolo
	setPageTitle(m_user->name + _S(" - ") + getText(_S("portal.pages.user.title")));	

	//entities::snapshot_profile_ptr profile = getLoggedUser()->getSnapshotProfile();

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	shared_ptr<XMLNode> nodeRoot = document->create(_S("user"));

	String path = getSkin()->getTemplatePath(_S("user.xsl"));
	m_pageTemplate.reset(OS_NEW HtmlXSLControl(loadStylesheet(path), document));

	switch(getUrlAction())
	{
	case acInfo:
		{
			nodeRoot->setAttributeString(_S("subpage"), _S("info"));
		} break;
	/*case acInvite:
		{
			nodeRoot->setAttributeString(_S("subpage"), _S("invite"));
		} break;*/
	case acReputations:
		{
			nodeRoot->setAttributeString(_S("subpage"), _S("reputations"));
			if(getSessionAccount()->isPortalGuest(getDatabase()) == false)
			{
				/* --- ScoreControl -------------------------------------------------------- */
				shared_ptr<HtmlDiv> reputationDiv(OS_NEW HtmlDiv());
				shared_ptr<HtmlText> reputationValueLabel(OS_NEW HtmlText(getText(_S("reputation.score"))));
				reputationValueLabel->setCss(_S("os_label"));
				reputationDiv->getControls()->add(reputationValueLabel);
				m_reputationValue.reset(OS_NEW HtmlComboBox());
				m_reputationValue->addOption(getText(_S("reputation.score.+1.0")), _S("1.0"));
				m_reputationValue->addOption(getText(_S("reputation.score.+0.5")), _S("0.5"));
				m_reputationValue->addOption(getText(_S("reputation.score.+0.0")), _S("0.0"));
				m_reputationValue->addOption(getText(_S("reputation.score.-0.5")), _S("-0.5"));
				m_reputationValue->addOption(getText(_S("reputation.score.-1.0")), _S("-1.0"));
				m_reputationValue->setID(_S("reputationValue"));
				reputationDiv->getControls()->add(m_reputationValue);

				shared_ptr<HtmlText> reputationCommentLabel(OS_NEW HtmlText(getText(_S("reputation.comment"))));
				reputationCommentLabel->setCss(_S("os_label"));
				reputationDiv->getControls()->add(reputationCommentLabel);
				m_reputationComment.reset(OS_NEW HtmlTextBox());
				m_reputationComment->setID(_S("reputationComment"));
				reputationDiv->getControls()->add(m_reputationComment);

				shared_ptr<HtmlText> reputationFollowLabel(OS_NEW HtmlText(getText(_S("reputation.follow"))));
				reputationFollowLabel->setCss(_S("os_label"));
				reputationDiv->getControls()->add(reputationFollowLabel);
				m_reputationFollow.reset(OS_NEW HtmlCheckBox());
				m_reputationFollow->setID(_S("reputationFollow"));
				reputationDiv->getControls()->add(m_reputationFollow);

				shared_ptr<IdeButton> reputationSubmit(OS_NEW IdeButton(getText(_S("portal.pages.user.update_reputation"))));
				reputationSubmit->setID(_S("reputationSubmit"));
				reputationSubmit->setCss(_S("os_command"));
				reputationSubmit->getEventClick()->connect(boost::bind(&User::onChangeReputation, this));
				reputationDiv->getControls()->add(reputationSubmit);

				if(getPage()->getPostBack() == false)
				{
					shared_ptr<ObjectsReputation> rep = getPortal()->getReputationManager()->get(getDatabase(), getSessionAccount()->getUserID(), id);
					if( (rep != nullptr) && (rep->score != ReputationsScore::DEFAULT) )
					{
						m_reputationValue->setValue(ReputationsScore::toString(rep->score));
						m_reputationComment->setValue(rep->description);
						m_reputationFollow->setCheck(rep->follow);
					}
					else
					{
						// Default
						m_reputationValue->setValue(ReputationsScore::toString(ReputationsScore::DEFAULT));
						m_reputationComment->setValue(String::EMPTY);
						m_reputationFollow->setCheck(false);
					}
				}
				m_pageTemplate->addChildParam(reputationDiv, _S("score"));
			}
		} break;
	case acSearch:
		{
			nodeRoot->setAttributeString(_S("subpage"), _S("search"));

			// Ricerche su di lui
			{
				ordered_map<std::wstring, std::wstring> params;
				params.set(_W("user"), id.toWide());
				
				String href = getPortal()->getLink(OS_IDE_PAGE_SEARCH, params);
				nodeRoot->setAttributeString(_S("href_search1"), href);
			}

			// Dove compare il suo nick
			{
				ordered_map<std::wstring, std::wstring> params;
				params.set(_W("run"), _W("1"));
				params.set(_W("text"), m_user->name->to_wide());
				String rss = getPortal()->getName() + _S(" - ") + m_user->name + _S(" - ") + getText(_S("portal.pages.user.actions.search2.title"));
				params.set(_W("rss"), rss.to_wide());

				String href = getPortal()->getLink(OS_IDE_PAGE_SEARCH, params);
				nodeRoot->setAttributeString(_S("href_search2"), href);
			}

			// Ultimi oggetti ricevuti su di lui
			{
				ordered_map<std::wstring, std::wstring> params;
				params.set(_W("run"), _W("1"));
				params.set(_W("user"), id.toWide());
				params.set(_W("orderMode"), _W("3"));
				String rss = getPortal()->getName() + _S(" - ") + m_user->name + _S(" - ") + getText(_S("portal.pages.user.actions.search3.title"));
				params.set(_W("rss"), rss.to_wide());

				String href = getPortal()->getLink(OS_IDE_PAGE_SEARCH, params);
				nodeRoot->setAttributeString("href_search3", href);
			}

			// Ultimi oggetti creati da lui
			{
				ordered_map<std::wstring, std::wstring> params;
				params.set(_W("run"), _W("1"));
				params.set(_W("user"), id.toWide());
				params.set(_W("orderMode"), _W("2"));
				String rss = getPortal()->getName() + _S(" - ") + m_user->name + _S(" - ") + getText(_S("portal.pages.user.actions.search4.title"));
				params.set(_W("rss"), rss.to_wide());

				String href = getPortal()->getLink(OS_IDE_PAGE_SEARCH, params);
				nodeRoot->setAttributeString("href_search4", href);
			}

			// Ultime discussioni (o oggetti commentabili) iniziate da lui
			{
				ordered_map<std::wstring, std::wstring> params;
				params.set(_W("run"), _W("1"));
				params.set(_W("user"), id.toWide());
				params.set(_W("orderMode"), _W("5"));
				params.set(_W("templateView"), _W("table"));
				String rss = getPortal()->getName() + _S(" - ") + m_user->name + _S(" - ") + getText(_S("portal.pages.user.actions.search5.title"));
				params.set(_W("rss"), rss.to_wide());

				String href = getPortal()->getLink(OS_IDE_PAGE_SEARCH, params);
				nodeRoot->setAttributeString("href_search5", href);
			}

			// Ultime discussioni (o oggetti commentabili) in cui ha partecipato
			{
				ordered_map<std::wstring, std::wstring> params;
				params.set(_W("run"), _W("1"));
				params.set(_W("user"), id.toWide());
				params.set(_W("orderMode"), _W("5"));
				params.set(_W("groupMode"), _W("1"));
				params.set(_W("templateView"), _W("table"));
				String rss = getPortal()->getName() + _S(" - ") + m_user->name + _S(" - ") + getText(_S("portal.pages.user.actions.search6.title"));
				params.set(_W("rss"), rss.to_wide());

				String href = getPortal()->getLink(OS_IDE_PAGE_SEARCH, params);
				nodeRoot->setAttributeString("href_search6", href);
			}
		} break;
	}

	Action action = getUrlAction();

	/* --- Actions -------------------------------------------------------- */
	shared_ptr<XMLNode> nodeActions = nodeRoot->addChild(_S("actions"));

	Utils::addAction(nodeActions, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_USER_TYPENAME, _S("view_profile"), getUrl(id.toUTF16(), acInfo), (action==acInfo), false);	
	/*Utils::addAction(nodeActions, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_USER_TYPENAME, _S("view_invite"), getUrl(id.toUTF16(), acInvite), (action==acInvite), false);*/
	Utils::addAction(nodeActions, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_USER_TYPENAME, _S("view_reputations"), getUrl(id.toUTF16(), acReputations), (action==acReputations), false);	
	Utils::addAction(nodeActions, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_USER_TYPENAME, _S("view_search"), getUrl(id.toUTF16(), acSearch), (action==acSearch), false);
	if(getSessionAccount()->isPortalGuest(getDatabase()) == false)
	{
		//Utils::addAction(nodeActions, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_USER_TYPENAME, _S("send_private_message"), getPortal()->getSendMessageLink(id), false, false);
	}
//	Utils::addAction(nodeActions, get_this_ptr<IPage>(), OS_PORTAL_OBJECT_USER_TYPENAME, _S("view_objects"), getUrl(id, Action::acItems));

	// Carica il template di riferimento
	getArea(pageAreaContent)->getControls()->add(m_pageTemplate);

	return;
}

void User::onPreRender()
{
	PageBase::onPreRender();

	if(m_user == nullptr)
		return;
	
	shared_ptr<XMLNode> nodeRoot = m_pageTemplate->getDocument()->getRoot();

	// User detail

	shared_ptr<XMLPortalExporter> userExporter(OS_NEW XMLPortalExporter(nodeRoot, get_this_ptr<IPortalPage>(), XMLPortalExporter::emFull));
	m_user->exportXML(userExporter);

	portals::profile_ptr userProfile = getPortal()->getProfile(getDatabase(), m_user->id);

	Action action = getUrlAction();
	switch(action)
	{
	/*case acInvite:
		{
			String subscribeLink = getPortal()->generateInviteLink(false);
			nodeRoot->setAttributeString(_S("subscribe_href"), subscribeLink);

			String isisSubscribeLink = getPortal()->generateInviteLink(true);
			nodeRoot->setAttributeString(_S("isis_subscribe_href"), isisSubscribeLink);

			String exportLink = getPortal()->generateExportLink();
			nodeRoot->setAttributeString(_S("export_href"), exportLink);
		} break;*/
	case acReputations:
		{
			/* --- Reputations - Forward -------------------------------------------------------- */
			{
				shared_ptr<ReputationsFeedback> feedback(OS_NEW ReputationsFeedback());
				String fromID = getPortal()->getPovUserID().toUTF16();
				String toID = m_user->id->toUTF16();
				ReputationsScore score = getPortal()->getReputationManager()->compute(getDatabase(), fromID.to_ascii(), toID.to_ascii(), feedback);

				shared_ptr<XMLNode> nodeReputations = nodeRoot->addChild(_S("reputation_forward"));
				shared_ptr<XMLNode> nodeReputation = nodeReputations->addChild(_S("reputation"));
				createReputationsFeedback(nodeReputation, feedback);
			}

			/* --- Reputations - Backward -------------------------------------------------------- */
			/* // Razor - RAZORVERYURGENT, togliere frasi translations.
			if(!getPortal()->getPortalID().isMonarchic())
			{
				shared_ptr<ReputationsFeedback> feedback(OS_NEW ReputationsFeedback());
				String fromID = m_user->id->toUTF16();
				String toID = profile->getUserID().toUTF16();
				ReputationsScore score = getPortal()->getReputationManager()->compute(getDatabase(), fromID.to_ascii(), toID.to_ascii(), feedback);

				shared_ptr<XMLNode> nodeReputations = nodeRoot->addChild(_S("reputation_backward"));
				shared_ptr<XMLNode> nodeReputation = nodeReputations->addChild(_S("reputation"));
				createReputationsFeedback(nodeReputation, feedback);
			}
			*/

			/* --- Reputazioni dirette verso questo utente --- */
			{
				
				ObjectID userId = m_user->id;
				String sql=String::format(_S("select r.author,r.score,r.description,r.submit_date from os_reputations r, os_snapshot_users u where r.score!=0 and u.visible=1 and r.author=u.id and r.reference='%S' order by u.score desc,r.score desc").c_str(), userId.toUTF16().c_str());
				shared_ptr<IdeTableQuery> table_comments(OS_NEW IdeTableQuery());
				table_comments->setSql(sql);
				table_comments->setColumnType(0,IdeTableQuery::ctUserID);
				table_comments->setColumnType(1,IdeTableQuery::ctScore);
				table_comments->setColumnType(2,IdeTableQuery::ctString);
				table_comments->setColumnType(3,IdeTableQuery::ctLongDateTime);
				table_comments->setColumnTitle(0, getText(_S("user.informations.reputations.from_user")));
				table_comments->setColumnTitle(1, getText(_S("user.informations.reputations.score")));
				table_comments->setColumnTitle(2, getText(_S("user.informations.reputations.description")));
				table_comments->setColumnTitle(3, getText(_S("user.informations.reputations.date")));

				m_pageTemplate->addChildParam(table_comments,_S("directReputation_toUser"));
			}

			/* --- Reputazioni date dall'utente --- */
			{
				
				ObjectID userId = m_user->id;
				String sql=String::format(_S("select r.reference,r.score,r.description,r.submit_date from os_reputations r, os_snapshot_users u where r.score!=0 and u.visible=1 and r.author='%S' and r.reference=u.id order by u.score desc,r.score desc").c_str(), userId.toUTF16().c_str());
				shared_ptr<IdeTableQuery> table_comments(OS_NEW IdeTableQuery());
				table_comments->setSql(sql);
				table_comments->setColumnType(0,IdeTableQuery::ctUserID);
				table_comments->setColumnType(1,IdeTableQuery::ctScore);
				table_comments->setColumnType(2,IdeTableQuery::ctString);
				table_comments->setColumnType(3,IdeTableQuery::ctLongDateTime);
				table_comments->setColumnTitle(0, getText(_S("user.informations.reputations.to_user")));
				table_comments->setColumnTitle(1, getText(_S("user.informations.reputations.score")));
				table_comments->setColumnTitle(2, getText(_S("user.informations.reputations.description")));
				table_comments->setColumnTitle(3, getText(_S("user.informations.reputations.date")));

				m_pageTemplate->addChildParam(table_comments,_S("directReputation_fromUser"));
			}

		} break;
	}
}

void User::onChangeReputation()
{
	shared_ptr<IdeSession> ideSession = getSessionAccount();
	if(getSessionAccount()->isPortalGuest(getDatabase()) == false)
	{
		//reputations::Score score(Convert::toDouble(m_scoreValue->getValue()), m_scoreComment->getValue());
		getPortal()->getReputationManager()->set(getDatabase(), ideSession->getPrivateKey(), ideSession->getUser(getDatabase())->id, m_user->id, conversions::from_utf16<double>(m_reputationValue->getValue()), m_reputationComment->getValue(), m_reputationFollow->getCheck());
	}
}

//////////////////////////////////////////////////////////////////////

} // portal
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
