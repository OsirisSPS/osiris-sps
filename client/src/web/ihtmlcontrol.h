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

#ifndef _OS_HTML_IHTMLCONTROL_H
#define _OS_HTML_IHTMLCONTROL_H

#include "base/object.h"
#include "boost/signals/trackable.hpp"
#include "boost/noncopyable.hpp"
#include "enable_this_ptr.h"
#include "eventssource.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DataTree;
class HtmlAttributes;
class HtmlControls;
class HtmlEvent;
class HtmlPage;
class HtmlUnit;
class HtmlWriter;
class HttpRequest;
class HttpResponse;
class HttpSession;
class IEvent;

//////////////////////////////////////////////////////////////////////

class WebExport IHtmlControl : public Object,
							   public enable_this_ptr<IHtmlControl>,
							   public boost::signals::trackable,
							   public boost::noncopyable
{
	friend class HtmlControls;

public:
	typedef EventsSource<String, IEvent> HtmlEvents;

// Internal
protected:
	enum Status
	{
        csNone			= 0,
		csInit			= 1,
		csLoad			= 2,
		csPreRender		= 3,
		csRender		= 4,
	};

// Construction
protected:
	IHtmlControl();
public:
	virtual ~IHtmlControl();

// Attributes
public:
	inline const String & getID() const;
	inline void setID(const String &id);
	inline const String & getUID() const;
	inline void setUID(const String &id);
	inline bool hasID() const;

	inline const String & getCss() const;
	inline void setCss(const String &css);
	inline bool hasCss() const;

	inline const String & getStyle() const;
	inline void setStyle(const String &style);
	inline bool hasStyle() const;

	inline shared_ptr<HtmlPage> getPage() const;
	inline shared_ptr<IHtmlControl> getParent() const;
	inline shared_ptr<HtmlAttributes> getAttributes() const;

	inline shared_ptr<HtmlControls> getControls() const;
	inline shared_ptr<HtmlEvents> getEvents() const;

	shared_ptr<EventSource> getEventInit() const;
	shared_ptr<EventSource> getEventLoad() const;
	shared_ptr<EventSource> getEventPreRender() const;

	bool hasChilds() const;

	const DataTree & getViewState() const;

	bool getPostBack() const;

	virtual shared_ptr<HttpSession> getSession() const;

	shared_ptr<HttpRequest> getRequest() const;
	shared_ptr<HttpResponse> getResponse() const;

	String getTarget() const;
		
	String getEventCommand(const String &eventName, const HtmlEvent *e = nullptr) const;
	String getEventCommand(const String &eventName, const String &e) const;

	HtmlUnit getWidth() const;
	void setWidth(const HtmlUnit &width);

	HtmlUnit getHeight() const;
	void setHeight(const HtmlUnit &height);

	bool getVisible() const;
	void setVisible(bool visible);

protected:
	String encodeEvent(const String &eventName, const HtmlEvent *e = nullptr) const;
	bool decodeEvent(const String &command, String &eventName, HtmlEvent &e) const;

// Operations
public:
	shared_ptr<IHtmlControl> findTarget(const String &target);

public:
	void render(shared_ptr<HtmlPage> page, HtmlWriter &writer);

	virtual void renderAttributes(HtmlWriter &writer);
	virtual void renderChilds(HtmlWriter &writer);

	void raiseEvent(const String &command);

protected:
	void saveViewState(DataTree &states);
	bool ensureStatus(shared_ptr<HtmlPage> page, const Status &status);

protected:
	virtual void onInit();
	virtual void onLoad();
	virtual void onPreRender();
	virtual void onRender(HtmlWriter &writer);

	virtual void onLoadViewState(const DataTree &state);
	virtual void onSaveViewState(DataTree &state);

	virtual void onEvent(const String &name, IEvent *e = nullptr);

protected:
	weak_ptr<HtmlPage> m_page;					// Pagina di riferimento
	weak_ptr<IHtmlControl>	m_parent;			// Controllo padre
	shared_ptr<HtmlAttributes> m_attributes;	// Attributi del controllo
	shared_ptr<HtmlControls> m_controls;		// Figli del controllo
	String m_uid;								// Unique id. If empty, ID is composed.
	String m_id;								// Id del controllo
	String m_css;								// Classe del controllo
	String m_style;								// Stile del controllo
	Status m_status;							// Stato del controllo
	bool m_visible;								// Visibilità del controllo
	shared_ptr<HtmlEvents> m_events;			// Eventi del controllo

public:
	static const String EVENT_ONINIT;
	static const String EVENT_ONLOAD;
	static const String EVENT_ONPRERENDER;

protected:
	static const String TARGET_DELIMITER;
};

//////////////////////////////////////////////////////////////////////

inline const String & IHtmlControl::getUID() const { return m_uid; }
inline void IHtmlControl::setUID(const String &id) { m_uid = id; }

inline const String & IHtmlControl::getID() const { return m_id; }
inline void IHtmlControl::setID(const String &id) { OS_EXCEPT_IF(id.find(TARGET_DELIMITER) != String::npos, "Invalid control id"); m_id = id; }
inline bool IHtmlControl::hasID() const { return (m_id.empty() == false) || (m_uid.empty() == false); }

inline const String & IHtmlControl::getCss() const { return m_css; }
inline void IHtmlControl::setCss(const String &css) { m_css = css; }
inline bool IHtmlControl::hasCss() const { return m_css.empty() == false; }

inline const String & IHtmlControl::getStyle() const { return m_style; }
inline void IHtmlControl::setStyle(const String &style) { m_style = style; }
inline bool IHtmlControl::hasStyle() const { return m_style.empty() == false; }

inline shared_ptr<HtmlPage> IHtmlControl::getPage() const { return m_page.lock(); }
inline shared_ptr<IHtmlControl> IHtmlControl::getParent() const { return m_parent.lock(); }
inline shared_ptr<HtmlAttributes> IHtmlControl::getAttributes() const { return m_attributes; }
inline shared_ptr<HtmlControls> IHtmlControl::getControls() const { return m_controls; }
inline shared_ptr<IHtmlControl::HtmlEvents> IHtmlControl::getEvents() const { return m_events; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_IHTMLCONTROL_H
