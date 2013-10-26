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
#include "htmlpage.h"

#include "buffer.h"
#include "conversions.h"
#include "cryptmanager.h"
#include "css.h"
#include "datatree.h"
#include "htmllinktag.h"
#include "htmlmetatag.h"
#include "htmlwriter.h"
#include "httpparser.h"
#include "httppostparam.h"
#include "httprequest.h"
#include "httpsession.h"
#include "timerperformance.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String HtmlPage::ID = _S("page");
const String HtmlPage::VIEW_STATE = _S("__VIEWSTATE");
const String HtmlPage::CTL_ID = _S("osPostCtlId");
const String HtmlPage::CTL_ARG = _S("osPostCtlArg");

//////////////////////////////////////////////////////////////////////

HtmlPage::HtmlPage(shared_ptr<HttpSession> session) : m_session(session),
													  m_viewStates(OS_NEW DataTree()),
													  m_pageMode(httpPageModeFull)
{
	OS_ASSERT(session != null);

	setID(ID);

	setAction(session->getRequest()->getRawUrl());
	setMethod(httpMethodPost);
	setEncoding(httpEncodingNone);

	addMetaTag(HtmlMetaTag::createByName(_S("generator"), _S("Osiris")));	
	// Removed Osiris 0.13 RC2, HTML5 don't support that.
	// addMetaTag(HtmlMetaTag::createByHttpEquiv(_S("Pragma"), _S("no-cache")));
	// addMetaTag(HtmlMetaTag::createByHttpEquiv(_S("Expires"), _S("-1")));
	// addMetaTag(HtmlMetaTag::createByHttpEquiv(_S("Cache-Control"), _S("no-cache")));
	
	// URGENT: da quanto ho capito, "Cache-Control" "no-cache" va nella risposta http. 
	// Ma non dovrebbe servire: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
	// If an HTTP/1.1 cache receives such a response, and the response does not include a Cache-Control header field, it SHOULD consider the response to be non-cacheable
	
	// Added Osiris 0.13 RC2
	addMetaTag(HtmlMetaTag::createByHttpEquiv(_S("Content-type"), _S("text/html;charset=UTF-8")));	
}

HtmlPage::~HtmlPage()
{

}

const DataTree & HtmlPage::getViewStates() const
{
	OS_ASSERT(m_viewStates != null);
	return *m_viewStates;
}

bool HtmlPage::hasCss(const String &url) const
{
	return utils::exists(m_css, url);
}

bool HtmlPage::hasJavascript(const String &url) const
{
	return utils::exists(m_javascripts, url);
}

void HtmlPage::addCss(const String &url)
{
	if(url.empty())
		return;

	if(hasCss(url) == false)
		m_css.push_back(url);
}

void HtmlPage::addMetaTag(shared_ptr<HtmlMetaTag> metatag)
{
	if(metatag != null)
		m_metatags.push_back(metatag);
}

void HtmlPage::addLinkTag(shared_ptr<HtmlLinkTag> linktag)
{
	if(linktag != null)
		m_linktags.push_back(linktag);
}

void HtmlPage::addJavascript(const String &url)
{
	if(url.empty())
	{
		OS_ASSERTFALSE();
		return;
	}

	if(hasJavascript(url) == false)
		m_javascripts.push_back(url);
}

void HtmlPage::reload(const ordered_map<std::wstring, std::wstring> &additionalParams)
{
	redirect(HttpParser::generateLink(getRequest()->getRawUrl(), additionalParams));
}

void HtmlPage::redirect(const std::string &url)
{
	getSession()->redirect(url);
}

bool HtmlPage::transmit()
{
	OS_TIMER_PERFORMANCE(TP, (_S("HtmlPage::transmit")));

	HtmlWriter writer;
	render(get_this_ptr<HtmlPage>(), writer);

	if(getSession()->getProcessed())
		return true;

	return onTransmit(writer);
}

void HtmlPage::reportError(HttpStatusCode status)
{
	getSession()->reportStatusCode(status);	
}

String HtmlPage::getBaseTitle()
{
	return String::EMPTY;
}

String HtmlPage::formatTitle()
{
	String title = m_pageTitle;
	String baseTitle = getBaseTitle();
	if(baseTitle.empty() == false)
	{
		if(title.empty() == false)
			title += _S(" - ");
		title += baseTitle;
	}

	return title;
}

void HtmlPage::renderHeader(HtmlWriter &writer)
{
	// XHTML 1.0 Strict
	// Use this when you want really clean markup, free of presentational clutter. Use this together with Cascading Style Sheets.
	//writer.writeLine(_S("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">"));

	// XHTML 1.0 Transitional
	// Use this when you need to take advantage of HTML's presentational features and when you want to support browsers that don't understand Cascading Style Sheets.
	//writer.writeLine(_S("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">"));

	// HTML 4.01 Strict
	//writer.writeLine(_S("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">"));

	// HTML 4.01 Transitional.
	//writer.writeLine(_S("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">"));

	// HTML5, Osiris 0.13 RC2
	writer.writeLine(_S("<!DOCTYPE HTML>"));
}

void HtmlPage::renderContent(HtmlWriter &writer)
{
	writer.openTag(_S("html"));

	// L'attributo xmlns va specificato se il documento è XHTML (Strict o Transitional)
	//writer.writeAttribute(_S("xmlns"),_S("http://www.w3.org/1999/xhtml"));
	
	// Commented in 0.13 RC2, not need with Html5
	//writer.writeAttribute(_S("xmlns:os"),_S("http://www.osiris-sps.org/xhtml/os")); // For custom tags

	writer.closeTag();

	renderHead(writer);
	renderBody(writer);

	writer.closeTag(_S("html"));
}

void HtmlPage::renderHead(HtmlWriter &writer)
{
	addMetaTag(HtmlMetaTag::createByName(_S("description"), m_pageDescription));

	if(m_favicon.empty() == false)
		addLinkTag(HtmlLinkTag::create(_S("shortcut icon"), m_favicon));
	
	writer.beginTag(_S("head"));
	writer.writeText(_S("title"), formatTitle());
	//writer.writeLine(_S("<meta name=\"description\" content=\"") + m_pageDescription + _S("\">"));

	renderMetatags(writer);
	renderLinktags(writer);

	/*
	if(m_favicon.empty() == false)
		writer.writeLine(_S("<link rel=\"shortcut icon\" href=\"") + m_favicon + _S("\">"));
	*/

	renderCss(writer);
	renderJavascripts(writer);

	writer.writeLine(m_additionalHead);
	writer.endTag();
}

void HtmlPage::renderBody(HtmlWriter &writer)
{
	writer.openTag(_S("body"));
	//writer.writeAttribute(_S("onload"),_S("javascript:Osiris.initialization()"));
	writer.closeTag();

	renderForm(writer);

	writer.closeTag(_S("body"));
}

void HtmlPage::renderForm(HtmlWriter &writer)
{
	writer.openTag(_S("form"));
	//writer.writeAttribute(_S("name"), _S("Form"));	// XHTML_VALIDATOR
	writer.writeAttribute(_S("method"), conversions::to_string(getMethod()));
	writer.writeAttribute(_S("action"), getAction());
	writer.writeAttribute(_S("id"), _S("Form"));
	if(m_encoding != httpEncodingNone)
		writer.writeAttribute(_S("enctype"), conversions::to_string(m_encoding));

	writer.closeTag();
	writer.writeLine();

	writer.writeLine(_S("<input type=\"hidden\" id=\"osPostCtlId\" name=\"osPostCtlId\"><input type=\"hidden\" id=\"osPostCtlArg\" name=\"osPostCtlArg\">"));
	renderViewState(writer);

	writer.writeLine(_S("<div class=\"") + OS_CSS_ROOT + _S("\">"));

	renderChilds(writer);

	writer.writeLine(_S("</div>"));

	writer.closeTag(_S("form"));
	writer.writeLine();
}

void HtmlPage::renderViewState(HtmlWriter &writer)
{
	OS_ASSERT(m_viewStates != null);
	m_viewStates->clear();
	// Salva lo stato dei controlli
	saveViewState(*m_viewStates);
	if(m_viewStates->empty() == false)
	{
		Buffer state;
		if(m_viewStates->write(state))
		{
			writer.openTag(_S("input"));
			writer.writeAttribute(_S("type"), _S("hidden"));
			writer.writeAttribute(_S("name"), VIEW_STATE);
			writer.writeAttribute(_S("value"), state.toHex());
			writer.selfCloseTag();
			writer.writeLine();
		}
	}
}

void HtmlPage::renderCss(HtmlWriter &writer)
{
	for(StringList::const_iterator i = m_css.begin(); i != m_css.end(); ++i)
	{
		writer.openTag(_S("link"));
		writer.writeAttribute(_S("href"), *i);
		writer.writeAttribute(_S("type"), _S("text/css"));
		writer.writeAttribute(_S("rel"), _S("stylesheet/less"));
		writer.closeTag();
		//writer.closeTag(_S("link"), false);  // Da decommentare per XHTML
		writer.writeLine();
	}
}

void HtmlPage::renderMetatags(HtmlWriter &writer)
{
	for(MetaTagsList::const_iterator i = m_metatags.begin(); i != m_metatags.end(); ++i)
	{
		(*i)->render(writer);
		writer.writeLine();
	}
}

void HtmlPage::renderLinktags(HtmlWriter &writer)
{
	for(LinkTagsList::const_iterator i = m_linktags.begin(); i != m_linktags.end(); ++i)
	{
		(*i)->render(writer);
		writer.writeLine();
	}
}

void HtmlPage::renderJavascripts(HtmlWriter &writer)
{
	for(StringList::const_iterator i = m_javascripts.begin(); i != m_javascripts.end(); ++i)
	{
		writer.openTag(_S("script"));
		writer.writeAttribute(_S("type"), _S("text/javascript"));
		writer.writeAttribute(_S("src"), *i);
		writer.closeTag();
		writer.closeTag(_S("script"), false);
		writer.writeLine();
	}
}

void HtmlPage::loadViewState()
{
	shared_ptr<HttpPostParam> viewState = getRequest()->getPostParam(VIEW_STATE.to_wide());
	if(viewState != null)
	{
		Buffer decoded;
		if(decoded.fromHex(utils::wtoa(viewState->getString())))
		{
			decoded.seekToBegin();
			OS_ASSERT(m_viewStates != null);
			OS_EXCEPT_IF(m_viewStates->read(decoded) == false, "Invalid view state");
		}
	}
}

void HtmlPage::raiseEvents()
{
	// Carica il target dell'evento
	shared_ptr<HttpPostParam> eventTarget = getRequest()->getPostParam(CTL_ID.to_wide());
	if(eventTarget != null)
	{
		// Ricerca il target dell'evento
		shared_ptr<IHtmlControl> control = findTarget(eventTarget->getString());
		if(control != null)
		{
			// Carica il parametro dell'evento
			shared_ptr<HttpPostParam> eventArg = getRequest()->getPostParam(CTL_ARG.to_wide());
			if(eventArg != null)
			{
				// Lancia l'evento
				control->raiseEvent(eventArg->getString());
			}
		}
	}
}

shared_ptr<HttpSession> HtmlPage::getSession() const
{
	return m_session;
}

void HtmlPage::onInit()
{
	// Il caricamento della viewstate va fatto prima di richiamare l'onInit della base perchè
	// la pagina di per sè potrebbe salvare dei dati e richiamando successivamente l'evento onInit
	// della base seguirebbe il giro di caricamento corretto
	loadViewState();

	ControlBase::onInit();
}

void HtmlPage::onPreRender()
{
	ControlBase::onPreRender();

	// Assicura che la pagina sia in stato di load
	ensureStatus(get_this_ptr<HtmlPage>(), IHtmlControl::csLoad);
	// Carica gli eventi
	raiseEvents();
}

void HtmlPage::onRender(HtmlWriter &writer)
{
	if(getPageMode() == httpPageModeAjax)
	{
		renderChilds(writer);	
	}
	else
	{
		renderHeader(writer);
		renderContent(writer);
	}
}

bool HtmlPage::onTransmit(HtmlWriter &writer)
{
	getSession()->transmitHtml(writer.getHtml().to_utf8(), httpStatusOK);
	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
