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
#include "idesearchglobalparams.h"

#include "convert.h"
#include "conversions.h"
#include "idepickerbool.h"
#include "idepickerdatetimespecial.h"
#include "ideskin.h"
#include "isearchresults.h"
#include "htmlevent.h"
#include "htmlcombobox.h"
#include "htmlliteral.h"
#include "htmltextbox.h"
#include "searchsearch.h"
#include "searchquery.h"
#include "xmlattributes.h"
#include "xmldocument.h"
#include "xmlmanager.h"
#include "xmlnode.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdeSearchGlobalParams::IdeSearchGlobalParams(shared_ptr<SearchQuery> query) : m_query(query),
																					m_text(OS_NEW HtmlTextBox()),
																					m_textMode(OS_NEW HtmlComboBox()),
																					m_textWhere(OS_NEW HtmlComboBox()),
																					m_orderMode(OS_NEW HtmlComboBox()),
																					m_orderDirection(OS_NEW HtmlComboBox()),
																					m_fromSubmitDate(OS_NEW IdePickerDateTimeSpecial()),
																					m_toSubmitDate(OS_NEW IdePickerDateTimeSpecial()),
																					m_fromInsertDate(OS_NEW IdePickerDateTimeSpecial()),
																					m_toInsertDate(OS_NEW IdePickerDateTimeSpecial()),
																					m_user(OS_NEW HtmlTextBox()),
																					m_parentsList(OS_NEW HtmlTextBox()),
																					m_parentsRecursive(OS_NEW IdePickerBool()),
																					m_treeMode(OS_NEW IdePickerBool()),
																					m_groupMode(OS_NEW IdePickerBool()),
																					m_showDeleted(OS_NEW IdePickerBool()),
																					m_showInfo(OS_NEW IdePickerBool()),
																					m_templateView(OS_NEW HtmlComboBox()),
																					m_templatePager(OS_NEW HtmlComboBox()),
																					m_showFilterAlphabetic(OS_NEW IdePickerBool()),
																					m_maxResults(OS_NEW HtmlTextBox()),
																					m_pagerSize(OS_NEW HtmlTextBox())
{
	m_text->setID(_S("text"));
	m_textMode->setID(_S("textMode"));	
	m_textWhere->setID(_S("textWhere"));	
	m_orderMode->setID(_S("orderMode"));	
	m_orderDirection->setID(_S("orderDirection"));	
	m_fromSubmitDate->setID(_S("fromSubmitDate"));	
	m_toSubmitDate->setID(_S("toSubmitDate"));	
	m_fromInsertDate->setID(_S("fromInsertDate"));	
	m_toInsertDate->setID(_S("toInsertDate"));	
	m_user->setID(_S("user"));	
	m_parentsList->setID(_S("parentsList"));	
	m_parentsRecursive->setID(_S("parentsRecursive"));	
	m_treeMode->setID(_S("treeMode"));	
	m_groupMode->setID(_S("groupMode"));	
	m_showDeleted->setID(_S("showDeleted"));	
	m_showInfo->setID(_S("showInfo"));	
	m_templateView->setID(_S("templateView"));	
	m_templatePager->setID(_S("templatePager"));	
	m_showFilterAlphabetic->setID(_S("showFilterAlphabetic"));
	m_maxResults->setID(_S("maxResults"));	
	m_pagerSize->setID(_S("pagerSize"));
}

IdeSearchGlobalParams::~IdeSearchGlobalParams()
{

}

String IdeSearchGlobalParams::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("search_global_params.xsl"));
}

void IdeSearchGlobalParams::onInit()
{
	ControlBase::onInit();

	OS_ASSERT(m_query != nullptr);

	// Crea il documento per l'xsl
	shared_ptr<XMLNode> nodeRoot = getDocument()->create(_S("globalParams"));	
	
	getTemplate()->addChildParam(m_text);
	getTemplate()->addChildParam(m_textMode);
	getTemplate()->addChildParam(m_textWhere);
	getTemplate()->addChildParam(m_orderMode);
	getTemplate()->addChildParam(m_orderDirection);
	getTemplate()->addChildParam(m_fromSubmitDate);
	getTemplate()->addChildParam(m_toSubmitDate);
	getTemplate()->addChildParam(m_fromInsertDate);
	getTemplate()->addChildParam(m_toInsertDate);
	getTemplate()->addChildParam(m_user);
	getTemplate()->addChildParam(m_parentsList);
	getTemplate()->addChildParam(m_parentsRecursive);
	getTemplate()->addChildParam(m_treeMode);
	getTemplate()->addChildParam(m_groupMode);
	getTemplate()->addChildParam(m_showDeleted);
	getTemplate()->addChildParam(m_showInfo);
	getTemplate()->addChildParam(m_templateView);
	getTemplate()->addChildParam(m_templatePager);
	getTemplate()->addChildParam(m_showFilterAlphabetic);
	getTemplate()->addChildParam(m_maxResults);
	getTemplate()->addChildParam(m_pagerSize);


	m_text->setSize(80);
	m_textMode->addOption(getPage()->getText(_S("search.mode.any_words")), conversions::to_utf16<uint32>(searchTextModeAnyWords));
	m_textMode->addOption(getPage()->getText(_S("search.mode.all_words")), conversions::to_utf16<uint32>(searchTextModeAllWords));
	m_textMode->addOption(getPage()->getText(_S("search.mode.exact_phrase")), conversions::to_utf16<uint32>(searchTextModeExactPhrase));
	
	m_textWhere->addOption(getPage()->getText(_S("search.fields.all")), conversions::to_utf16<uint32>(searchTextWhereAllFields));
	m_textWhere->addOption(getPage()->getText(_S("search.fields.title")), conversions::to_utf16<uint32>(searchTextWhereOnlyTitle));
	m_textWhere->addOption(getPage()->getText(_S("search.fields.custom")), conversions::to_utf16<uint32>(searchTextWhereCustom));

	m_orderMode->addOption(getPage()->getText(_S("search.order_mode.relevance")), conversions::to_utf16<uint32>(searchOrderModeRelevance));
	m_orderMode->addOption(getPage()->getText(_S("search.order_mode.reputation")), conversions::to_utf16<uint32>(searchOrderModeReputation));
	m_orderMode->addOption(getPage()->getText(_S("search.order_mode.submit_date")), conversions::to_utf16<uint32>(searchOrderModeSubmitDate));
	m_orderMode->addOption(getPage()->getText(_S("search.order_mode.insert_date")), conversions::to_utf16<uint32>(searchOrderModeInsertDate));
	m_orderMode->addOption(getPage()->getText(_S("search.order_mode.position")), conversions::to_utf16<uint32>(searchOrderModePosition));
	m_orderMode->addOption(getPage()->getText(_S("search.order_mode.title")), conversions::to_utf16<uint32>(searchOrderModeTitle));
	m_orderMode->addOption(getPage()->getText(_S("search.order_mode.last_object")), conversions::to_utf16<uint32>(searchOrderModeLastObject));
	m_orderMode->addOption(getPage()->getText(_S("search.order_mode.votes_total")), conversions::to_utf16<uint32>(searchOrderModeVotesTotal));
	m_orderMode->addOption(getPage()->getText(_S("search.order_mode.votes_average")), conversions::to_utf16<uint32>(searchOrderModeVotesAverage));
	

	m_orderDirection->addOption(getPage()->getText(_S("search.order_direction.descendent")), conversions::to_utf16<uint32>(searchOrderDirectionDescendent));
	m_orderDirection->addOption(getPage()->getText(_S("search.order_direction.ascendent")), conversions::to_utf16<uint32>(searchOrderDirectionAscendent));
	
	m_user->setStyle(_S("width:95%"));

	m_templateView->addOption(getPage()->getText(_S("search.template.preview")), _S("preview"));
	m_templateView->addOption(getPage()->getText(_S("search.template.table")), _S("table"));
	m_templateView->addOption(getPage()->getText(_S("search.template.blog")), _S("blog"));
	m_templateView->addOption(getPage()->getText(_S("search.template.blog-twocols")), _S("blog-twocols"));
	m_templateView->addOption(getPage()->getText(_S("search.template.row")), _S("row"));
	m_templateView->addOption(getPage()->getText(_S("search.template.row-detail")), _S("row-detail"));
	m_templateView->addOption(getPage()->getText(_S("search.template.row-tech")), _S("row-tech"));

	m_templatePager->addOption(getPage()->getText(_S("search.template.pager.forum")), _S("forum"));
	m_templatePager->addOption(getPage()->getText(_S("search.template.pager.blog")), _S("blog"));

	// Inizializzo il form in base ai valori di m_query
	m_text->setValue(m_query->getText());
	m_textMode->setValue(conversions::to_utf16<uint32>(m_query->getTextMode()));
	m_textWhere->setValue(conversions::to_utf16<uint32>(m_query->getTextWhere()));
	m_orderMode->setValue(conversions::to_utf16<uint32>(m_query->getOrderMode()));
	m_orderDirection->setValue(conversions::to_utf16<uint32>(m_query->getOrderDirection()));
	m_fromSubmitDate->setValue(m_query->getFromSubmitDateType(),m_query->getFromSubmitDateSpecific());
	m_toSubmitDate->setValue(m_query->getToSubmitDateType(),m_query->getToSubmitDateSpecific());
	m_fromInsertDate->setValue(m_query->getFromInsertDateType(),m_query->getFromInsertDateSpecific());
	m_toInsertDate->setValue(m_query->getToInsertDateType(),m_query->getToInsertDateSpecific());
	m_user->setValue(m_query->getByUser());
	m_parentsList->setValue(m_query->getParentsListString());
	m_parentsRecursive->setCheck(m_query->getParentsRecursive());
	m_treeMode->setCheck(m_query->getTreeMode());
	m_groupMode->setCheck(m_query->getGroupMode());
	m_showDeleted->setCheck(m_query->getShowDeleted());
	m_showInfo->setCheck(m_query->getShowInfo());
	m_templateView->setValue(m_query->getTemplateView());
	m_templatePager->setValue(m_query->getTemplatePager());
	m_showFilterAlphabetic->setCheck(m_query->getShowFilterAlphabetic());
	m_maxResults->setValue(conversions::to_utf16<uint32>(m_query->getMaxResults()));
	m_pagerSize->setValue(conversions::to_utf16<uint32>(m_query->getPagerSize()));
}

void IdeSearchGlobalParams::onLoad()
{
	ControlBase::onLoad();	
}

void IdeSearchGlobalParams::onPreRender()
{
	ControlBase::onPreRender();
}

void IdeSearchGlobalParams::fillQuery()
{
	// Valorizzo m_query
	m_query->setText(m_text->getValue());
	m_query->setTextMode((SearchTextMode) conversions::from_utf16<uint32>(m_textMode->getValue()));
	m_query->setTextWhere((SearchTextWhere) conversions::from_utf16<uint32>(m_textWhere->getValue()));
	m_query->setOrderMode((SearchOrderMode) conversions::from_utf16<uint32>(m_orderMode->getValue()));
	m_query->setOrderDirection((SearchOrderDirection) conversions::from_utf16<uint32>(m_orderDirection->getValue()));

	m_query->setFromSubmitDateType(m_fromSubmitDate->getValueType());
	m_query->setFromSubmitDateSpecific(m_fromSubmitDate->getValueSpecific());
	m_query->setToSubmitDateType(m_toSubmitDate->getValueType());
	m_query->setToSubmitDateSpecific(m_toSubmitDate->getValueSpecific());
	m_query->setFromInsertDateType(m_fromInsertDate->getValueType());
	m_query->setFromInsertDateSpecific(m_fromInsertDate->getValueSpecific());
	m_query->setToInsertDateType(m_toInsertDate->getValueType());
	m_query->setToInsertDateSpecific(m_toInsertDate->getValueSpecific());
	m_query->setByUser(m_user->getValue());	
	m_query->setParentsListString(m_parentsList->getValue());
	m_query->setParentsRecursive(m_parentsRecursive->getCheck());
	m_query->setTreeMode(m_treeMode->getCheck());
	m_query->setGroupMode(m_groupMode->getCheck());
	m_query->setShowDeleted(m_showDeleted->getCheck());
	m_query->setShowInfo(m_showInfo->getCheck());
	m_query->setTemplateView(m_templateView->getValue());
	m_query->setTemplatePager(m_templatePager->getValue());
	m_query->setShowFilterAlphabetic(m_showFilterAlphabetic->getCheck());
	m_query->setMaxResults(conversions::from_utf16<uint32>(m_maxResults->getValue()));	
	m_query->setPagerSize(conversions::from_utf16<uint32>(m_pagerSize->getValue()));	
}


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
