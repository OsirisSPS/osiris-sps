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

#ifndef _OMLMANAGER_H
#define _OMLMANAGER_H

//////////////////////////////////////////////////////////////////////

#include "eventsource.h"
#include "ievent.h"
#include "ideide.h"
#include "portalsportals.h"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

#define OSMLCODES_OPTIONS_STOPWITHERROR		false	// Identifica se il processo di parsing deve fermarsi al primo errore
#define OSMLCODES_OPTIONS_NEWENGINE					// Identifica se usare l'engine <=0.10 o quello >=0.11
#define OSMLCODES_OPTIONS_TAGERRORASTEXT	true	// Identifica se dumpare come testo i tag non interpretati, a differenza dell'html

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IOMLCode;
class OMLContext;
class OMLEmoticon;
class OMLItem;
class IPage;

//////////////////////////////////////////////////////////////////////

class EngineExport OMLManager : public StaticSingleton<OMLManager>
{
public:
	typedef map<String, shared_ptr<IOMLCode> >::type Codes;
	typedef list<shared_ptr<OMLEmoticon> >::type Emoticons;
	typedef list<shared_ptr<OMLItem> >::type Items;

// Events
public:
	class EventBeforeParse : public IEvent
	{
		// Attributes
		public:
			const String & getOml() const;
			void setOml(const String &oml);

		protected:
			String m_oml;
	};

	class EventAfterParse : public IEvent
	{
		// Attributes
		public:
			const String & getHtml() const;
			void setHtml(const String &html);

		protected:
			String m_html;
	};

	class EventBeforeReverseParse : public IEvent
	{
		// Attributes
		public:
			const String & getSource() const;
			void setSource(const String &source);
			const std::string & getFormat() const;
			void setFormat(const std::string &format);

		protected:
			String m_source;
			std::string m_format;
	};

	class EventAfterReverseParse : public IEvent
	{
		// Attributes
		public:
			const String & getOml() const;
			void setOml(const String &oml);

		protected:
			String m_oml;
	};

// Construction
public:
	OMLManager();
	virtual ~OMLManager();

// Attributes
public:
	inline const Emoticons & getEmoticons() const;

	inline shared_ptr<EventSource> getEventBeforeParse() const;
	inline shared_ptr<EventSource> getEventAfterParse() const;
	inline shared_ptr<EventSource> getEventBeforeReverseParse() const;
	inline shared_ptr<EventSource> getEventAfterReverseParse() const;

// Operations
public:
	void add(shared_ptr<IOMLCode> code);
	void remove(shared_ptr<IOMLCode> code);

	String parse(const String& text, shared_ptr<IPage> page, bool full, bool secure, bool row, OMLRenderMode mode, const String &refID, const String &fullUrl);
	shared_ptr<OMLContext> parse(const String& text, shared_ptr<IPage> page, bool enableWarning, bool full, bool secure, bool row, OMLRenderMode mode, const String &refID, const String &fullUrl);
	String reverseParse(const String& htmlText, const std::string &format);
	String parseCss(const String& cssText, shared_ptr<IPage> page, bool secure, bool encode);

private:
	void init();

// Helper
public:
	void addReplace(const String& tag, const int32 minParams, const int32 maxParams, const String &replaceHtml, const String &replacePlain, bool allowedInRowMode = false, const String &mapDefaultParamTo = String::EMPTY);
	void addHtmlWrapper(const String& tag, const String& htmlTag, bool allowedInRowMode, const String &parentTag = String::EMPTY, const String &childsTag = String::EMPTY, const String &mapDefaultParamTo = String::EMPTY);

	bool isKnownTag(const String& tag);
	String processItemForHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context);
	String processItemForOsml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context);

private:
	void addEmoticon(const String &code, const String &name);

private:
	Codes m_codes;
	Emoticons m_emoticons;

	shared_ptr<EventSource> m_eventBeforeParse;
	shared_ptr<EventSource> m_eventAfterParse;
	shared_ptr<EventSource> m_eventBeforeReverseParse;
	shared_ptr<EventSource> m_eventAfterReverseParse;
};

//////////////////////////////////////////////////////////////////////

inline const String & OMLManager::EventBeforeParse::getOml() const { return m_oml; }
inline void OMLManager::EventBeforeParse::setOml(const String &oml) { m_oml = oml; }
inline const String & OMLManager::EventAfterParse::getHtml() const { return m_html; }
inline void OMLManager::EventAfterParse::setHtml(const String &html) { m_html = html; }
inline const String & OMLManager::EventBeforeReverseParse::getSource() const { return m_source; }
inline void OMLManager::EventBeforeReverseParse::setSource(const String &source) { m_source = source; }
inline const std::string & OMLManager::EventBeforeReverseParse::getFormat() const { return m_format; }
inline void OMLManager::EventBeforeReverseParse::setFormat(const std::string &format) { m_format = format; }
inline const String & OMLManager::EventAfterReverseParse::getOml() const { return m_oml; }
inline void OMLManager::EventAfterReverseParse::setOml(const String &oml) { m_oml = oml; }

inline const OMLManager::Emoticons & OMLManager::getEmoticons() const { return m_emoticons; }

inline shared_ptr<EventSource> OMLManager::getEventBeforeParse() const { return m_eventBeforeParse; }
inline shared_ptr<EventSource> OMLManager::getEventAfterParse() const { return m_eventAfterParse; }
inline shared_ptr<EventSource> OMLManager::getEventBeforeReverseParse() const { return m_eventBeforeReverseParse; }
inline shared_ptr<EventSource> OMLManager::getEventAfterReverseParse() const { return m_eventAfterReverseParse; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OMLMANAGER_H
