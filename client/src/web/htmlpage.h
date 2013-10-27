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

#ifndef _OS_HTML_HTMLPAGE_H
#define _OS_HTML_HTMLPAGE_H

#include "collections.h"
#include "ihtmlcontrol.h"
#include "http.h"
#include "ordered_map.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlLinkTag;
class HtmlMetaTag;

//////////////////////////////////////////////////////////////////////

class WebExport HtmlPage : public IHtmlControl
{
	typedef IHtmlControl ControlBase;

	typedef list<shared_ptr<HtmlMetaTag> >::type MetaTagsList;
	typedef list<shared_ptr<HtmlLinkTag> >::type LinkTagsList;

// Construction
public:
	HtmlPage(shared_ptr<HttpSession> session);
	virtual ~HtmlPage();

// Attributes
public:
	inline bool getAjax() const; // Shortcut to getPageMode
	inline void setAjax(bool ajax); // Shortcut to setPageMode
	inline HttpPageMode getPageMode() const;
	inline void setPageMode(HttpPageMode mode);

	inline const String & getAction() const;
	inline void setAction(const String &action);

	inline HttpMethod getMethod() const;
	inline void setMethod(HttpMethod method);

	inline HttpEncoding getEncoding() const;
	inline void setEncoding(HttpEncoding encoding);

	inline const String & getPageTitle() const;
	inline void setPageTitle(const String &title);

	inline const String & getPageDescription() const;
	inline void setPageDescription(const String &description);

	inline const String & getFavicon() const;
	inline void setFavicon(const String &favicon);	

	const DataTree & getViewStates() const;

	bool hasCss(const String &url) const;
	bool hasJavascript(const String &url) const;

	inline const String & getAdditionalHead() const;
	inline void setAdditionalHead(const String &additionalHead);	
	
// Operations
public:
	void addCss(const String &url);
	void addLinkTag(shared_ptr<HtmlLinkTag> linktag);
	void addMetaTag(shared_ptr<HtmlMetaTag> metatag);
	void addJavascript(const String &url);

	void reload(const ordered_map<std::wstring, std::wstring> &additionalParams = (ordered_map<std::wstring, std::wstring>()));

	virtual void redirect(const std::string &url);	
	virtual bool transmit();

	virtual void reportError(HttpStatusCode status);

// Overridables
protected:
	virtual String getBaseTitle();
	virtual String formatTitle();

protected:
	virtual void renderHeader(HtmlWriter &writer);
	virtual void renderContent(HtmlWriter &writer);
	virtual void renderHead(HtmlWriter &writer);
	virtual void renderBody(HtmlWriter &writer);
	virtual void renderForm(HtmlWriter &writer);
	virtual void renderViewState(HtmlWriter &writer);

	virtual void renderCss(HtmlWriter &writer);
	virtual void renderLinktags(HtmlWriter &writer);
	virtual void renderMetatags(HtmlWriter &writer);
	virtual void renderJavascripts(HtmlWriter &writer);

	virtual void loadViewState();
	virtual void raiseEvents();

// IHtmlControl overrides
public:
	virtual shared_ptr<HttpSession> getSession() const;

protected:
	virtual void onInit();
	virtual void onPreRender();
	virtual void onRender(HtmlWriter &writer);
	virtual bool onTransmit(HtmlWriter &writer);

protected:
	HttpPageMode m_pageMode;
	
	String m_pageTitle;
	String m_pageDescription;
	String m_action;
	String m_favicon;
	HttpMethod m_method;
	HttpEncoding m_encoding;

	MetaTagsList m_metatags;
	LinkTagsList m_linktags;
	StringList m_css;
	StringList m_javascripts;
	String m_additionalHead;

	scoped_ptr<DataTree> m_viewStates;

	shared_ptr<HttpSession> m_session;

protected:
	static const String ID;
	static const String VIEW_STATE;
	static const String CTL_ID;
	static const String CTL_ARG;
};

//////////////////////////////////////////////////////////////////////

//inline bool HtmlPage::getAjax() const { return m_ajax; }
//inline void HtmlPage::setAjax(bool ajax) { m_ajax = ajax; }

inline bool HtmlPage::getAjax() const { return m_pageMode == httpPageModeAjax; }
inline void HtmlPage::setAjax(bool ajax) { m_pageMode = ajax ? httpPageModeAjax : httpPageModeFull; }

inline HttpPageMode HtmlPage::getPageMode() const { return m_pageMode; }
inline void HtmlPage::setPageMode(HttpPageMode mode) { m_pageMode = mode; }

inline const String & HtmlPage::getAction() const { return m_action; }
inline void HtmlPage::setAction(const String &action) { m_action = action; }

inline HttpMethod HtmlPage::getMethod() const { return m_method; }
inline void HtmlPage::setMethod(HttpMethod method) { m_method = method; }

inline HttpEncoding HtmlPage::getEncoding() const { return m_encoding; }
inline void HtmlPage::setEncoding(HttpEncoding encoding) { m_encoding = encoding; }

inline const String & HtmlPage::getPageTitle() const { return m_pageTitle; }
inline void HtmlPage::setPageTitle(const String &title) { m_pageTitle = title; }

inline const String & HtmlPage::getPageDescription() const { return m_pageDescription; }
inline void HtmlPage::setPageDescription(const String &title) { m_pageDescription = title; }

inline const String & HtmlPage::getFavicon() const { return m_favicon; }
inline void HtmlPage::setFavicon(const String &favicon) { m_favicon = favicon; }

inline const String & HtmlPage::getAdditionalHead() const { return m_additionalHead; }
inline void HtmlPage::setAdditionalHead(const String &additionalHead) { m_additionalHead = additionalHead; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLPAGE_H
